using System.Diagnostics;
using System.Reflection;
using Wacton.Unicolour.Icc;

const string profilesRelativePath = "../../../../profiles/";
const string converterRelativePath = "../../../../../cmake-build-debug/EntryPoint.exe";

var exeLocation = Assembly.GetEntryAssembly()!.Location;
var profilesPath = Path.GetFullPath(Path.Combine(exeLocation, profilesRelativePath));
var converterPath = Path.GetFullPath(Path.Combine(exeLocation, converterRelativePath));

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
    var pcsChannels = profile.Header.Pcs switch
    {
        "Lab " => 3,
        _ => throw new NotSupportedException($"PCS {profile.Header.Pcs}")
    };

    var deviceChannels = profile.Header.DataColourSpace switch
    {
        "RGB" => 3,
        "CMYK" => 4,
        "7Clr" => 7,
        _ => throw new NotSupportedException($"Device space {profile.Header.DataColourSpace}")
    };

    // standard range
    var deviceToPcsValuesPerChannel = deviceChannels <= 4 ? 6 : 3;
    var deviceToPcsVectors = GenerateVectorsOfBaseN(deviceChannels, deviceToPcsValuesPerChannel);
    var deviceToPcsRows = deviceToPcsVectors
        .Select(vector => vector.Select(x => $"{x / ((double)deviceToPcsValuesPerChannel - 1)}"))
        .Select(vector => string.Join(",", vector))
        .ToList();

    // each device channel out of range
    for (var i = 0; i < deviceChannels; i++)
    {
        var lowerBound = new double[deviceChannels].Select(_ => 0.5).ToArray();
        lowerBound[i] = -0.1;
        deviceToPcsRows.Add(string.Join(",", lowerBound));
        
        var upperBound = new double[deviceChannels].Select(_ => 0.5).ToArray();
        upperBound[i] = 1.1;
        deviceToPcsRows.Add(string.Join(",", upperBound));
    }

    const string inputCsvRelativePath = "./input.csv";
    var inputCsvPath = Path.GetFullPath(inputCsvRelativePath);
    
    File.WriteAllLines(inputCsvPath, deviceToPcsRows);

    // TODO: generate PCS-to-device data
    var deviceToPcs = true;
    for (var intent = 0; intent <= 3; intent++)
    {
        var profilePath = profile.FileInfo.FullName;
        var outputCsvFilename = $"./{Path.GetFileNameWithoutExtension(profilePath)}_ToPcs_ICC-{intent}.csv";
        var outputCsvPath = Path.GetFullPath(outputCsvFilename);

        var arguments = $"-profile=\"{profilePath}\" -deviceToPcs={(deviceToPcs ? "1" : "0")} -render_intent={intent} -input_file=\"{inputCsvPath}\" -output_to=\"{outputCsvPath}\"";
        
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