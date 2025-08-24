using System.Diagnostics;
using System.Reflection;
using Wacton.Unicolour.Icc;

const string profilesRelativePath = "../../../../profiles/";
const string converterRelativePath = "../../../../../cmake-build-debug/EntryPoint.exe";

var exeLocation = Assembly.GetEntryAssembly()!.Location;
var profilesPath = Path.GetFullPath(Path.Combine(exeLocation, profilesRelativePath));
var converterPath = Path.GetFullPath(Path.Combine(exeLocation, converterRelativePath));
var outputPath = Path.GetFullPath(Path.Combine(exeLocation, "../output/"));

Directory.CreateDirectory(outputPath);

var converterDateTime = new FileInfo(converterPath).LastWriteTime;
Console.WriteLine($"Converter built {converterDateTime:f}");

var files = Directory.GetFiles(profilesPath, "*.icc");
foreach (var file in files)
{
    var profile = new Profile(file);
    try
    {
        ProcessProfile(profile);
    }
    catch (Exception e)
    {
        Console.WriteLine($"Could not process profile: {Path.GetFileName(file)}{Environment.NewLine}{e}");
    }
}

return;

void ProcessProfile(Profile profile)
{
    var isLabPcs = profile.Header.Pcs switch
    {
        "Lab " => true,
        "XYZ " => false,
        _ => throw new NotSupportedException($"PCS {profile.Header.Pcs}")
    };

    var deviceChannels = profile.Header.DataColourSpace switch
    {
        "GRAY" => 1,
        "RGB " => 3,
        "CMYK" => 4,
        "7CLR" => 7,
        _ => throw new NotSupportedException($"Device space {profile.Header.DataColourSpace}")
    };
    
    var deviceToPcsInputs = GenerateNormalisedInputs(deviceChannels);
    GenerateData(profile, deviceToPcsInputs, deviceToPcs: true);
    
    var pcsToDeviceInputs = isLabPcs ? GenerateLabInputs() : GenerateNormalisedInputs(3);
    GenerateData(profile, pcsToDeviceInputs, deviceToPcs: false);
}

List<string> GenerateNormalisedInputs(int channels)
{
    // standard range
    var valuesPerChannel = channels <= 4 ? 6 : 3;
    var vectors = GenerateVectorsOfBaseN(channels, valuesPerChannel);
    var inputs = vectors
        .Select(vector => vector.Select(x => $"{x / ((double)valuesPerChannel - 1)}"))
        .Select(vector => string.Join(",", vector))
        .ToList();

    // each device channel out of range
    for (var i = 0; i < channels; i++)
    {
        var lowerBound = new double[channels].Select(_ => 0.5).ToArray();
        lowerBound[i] = -0.1;
        inputs.Add(string.Join(",", lowerBound));
        
        var upperBound = new double[channels].Select(_ => 0.5).ToArray();
        upperBound[i] = 1.1;
        inputs.Add(string.Join(",", upperBound));
    }

    return inputs;
}

List<string> GenerateLabInputs()
{
    int[] lValues = [0, 20, 40, 60, 80, 100];
    int[] aValues = [-127, -100, -75, -50, -25, 0, 25, 50, 75, 100, 128];
    int[] bValues = [-127, -100, -75, -50, -25, 0, 25, 50, 75, 100, 128];

    var inputs = new List<string>();
    
    // standard range
    foreach (var l in lValues)
    foreach (var a in aValues)
    foreach (var b in bValues)
    {
        inputs.Add($"{l},{a},{b}");
    }
    
    // each device channel out of range
    inputs.AddRange(
    [
        "-1,0,0",
        "101,0,0",
        "50,-130,0",
        "50,130,0",
        "50,0,-130",
        "50,0,130"
    ]);

    return inputs;
}

void GenerateData(Profile profile, List<string> inputRows, bool deviceToPcs)
{
    const string inputCsvRelativePath = "./input.csv";
    var inputCsvPath = Path.GetFullPath(inputCsvRelativePath);
    File.WriteAllLines(inputCsvPath, inputRows);

    for (var intent = 0; intent <= 3; intent++)
    {
        var outputCsvFilename = $"{profile.Name}_{(deviceToPcs ? "ToPcs" : "ToDevice")}_ICC-{intent}.csv";
        var outputCsvPath = Path.Combine(outputPath, outputCsvFilename);

        var iccFile = Path.Combine(profilesPath, $"{profile.Name}.icc");
        var arguments = $"-profile=\"{iccFile}\" -deviceToPcs={(deviceToPcs ? "1" : "0")} -render_intent={intent} -input_file=\"{inputCsvPath}\" -output_to=\"{outputCsvPath}\"";
        
        var process = new Process
        {
            StartInfo = new ProcessStartInfo
            {
                FileName = $"\"{converterPath}\"",
                Arguments = arguments,
                UseShellExecute = false,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                CreateNoWindow = true
            }
        };

        process.Start();
        process.WaitForExit();
    }
}

// copied from Wacton.Unicolour.Icc
static List<int[]> GenerateVectorsOfBaseN(int n, int @base)
{
    var totalVectors = (int)Math.Pow(@base, n);
            
    var vectors = new List<int[]>();
    for (var i = 0; i < totalVectors; i++)
    {
        var vector = new int[n];
                
        var dimensionIndex = n - 1;
        var value = i;
        while (value > 0)
        {
            var remainder = value % @base;
            vector[dimensionIndex] = remainder;
            value /= @base;
            dimensionIndex--;
        }

        vectors.Add(vector);
    }

    return vectors;
}