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
    if (!file.Contains("Fogra39")) continue;
    var profile = new Profile(file);

    var deviceChannels = profile.Header.DataColourSpace switch
    {
        "RGB" => 3,
        "CMYK" => 4,
        "7Clr" => 7
    };

    var pcsChannels = 3; // always to/from LAB
    
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

    // ICC file needs to be next to EntryPoint.exe - needs improvement
    var copiedIccFileLocation = Path.Combine(Path.GetDirectoryName(converterPath)!, Path.GetFileName(file));
    File.Copy(file, copiedIccFileLocation, overwrite: true);
    
    for (var intent = 0; intent <= 3; intent++)
    {
        var outputCsvFilename = $"./{Path.GetFileNameWithoutExtension(file)}_ToPcs_ICC-{intent}.csv";
        var outputCsvPath = Path.GetFullPath(outputCsvFilename);

        // e.g. -profile=Coated_Fogra39L_VIGC_300.icc -input_file=input.csv -deviceToPcs=0 -render_intent=1 -output_to=output.csv
        var arguments = $"-profile={Path.GetFileName(copiedIccFileLocation)} -deviceToPcs=0 -render_intent={intent} -input_file=\"{inputCsvPath}\" -output_to=\"{outputCsvPath}\"";
        
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

return;

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