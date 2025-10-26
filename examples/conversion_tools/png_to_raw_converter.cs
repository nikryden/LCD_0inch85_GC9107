using System;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;

/// <summary>
/// PNG to Raw Converter for GC9107 LCD with Bit Inversion
/// Converts PNG images to 128x128 RGB565 raw format
/// Compatible with bit-inverting LCD displays
/// 
/// Usage: dotnet run PngToRawConverter.cs input.png [output.raw]
/// </summary>
public class PngToRawConverter
{
    public static void Main(string[] args)
    {
        Console.WriteLine("🖼️  PNG to Raw Converter for GC9107 LCD");
        Console.WriteLine("=" + new string('=', 45));
        
        if (args.Length < 1)
        {
            Console.WriteLine("Usage: dotnet run PngToRawConverter.cs <input.png> [output.raw]");
            Console.WriteLine("Example: dotnet run PngToRawConverter.cs davinci_icon.png davinci_icon.raw");
            return;
        }
        
        string inputFile = args[0];
        string outputFile = args.Length > 1 ? args[1] : Path.ChangeExtension(inputFile, ".raw");
        
        try
        {
            ConvertPngToRaw(inputFile, outputFile);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"❌ Error: {ex.Message}");
        }
    }
    
    /// <summary>
    /// Converts PNG image to RGB565 raw format for GC9107 LCD
    /// </summary>
    /// <param name="inputPath">Input PNG file path</param>
    /// <param name="outputPath">Output raw file path</param>
    public static void ConvertPngToRaw(string inputPath, string outputPath)
    {
        if (!File.Exists(inputPath))
        {
            throw new FileNotFoundException($"Input file not found: {inputPath}");
        }
        
        Console.WriteLine($"📂 Input:  {inputPath}");
        Console.WriteLine($"📁 Output: {outputPath}");
        
        using (var originalImage = new Bitmap(inputPath))
        {
            Console.WriteLine($"📐 Original size: {originalImage.Width}x{originalImage.Height}");
            Console.WriteLine($"🎨 Original format: {originalImage.PixelFormat}");
            
            // Resize to 128x128 for LCD
            using (var resizedImage = new Bitmap(128, 128))
            {
                using (var graphics = Graphics.FromImage(resizedImage))
                {
                    // Use high-quality scaling
                    graphics.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;
                    graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
                    graphics.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.HighQuality;
                    graphics.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighQuality;
                    
                    graphics.DrawImage(originalImage, 0, 0, 128, 128);
                }
                
                Console.WriteLine("🔄 Resized to 128x128");
                
                // Convert to RGB565 raw data
                var rawData = new byte[128 * 128 * 2]; // 2 bytes per pixel (RGB565)
                int dataIndex = 0;
                
                for (int y = 0; y < 128; y++)
                {
                    for (int x = 0; x < 128; x++)
                    {
                        Color pixel = resizedImage.GetPixel(x, y);
                        ushort rgb565;
                        
                        // Check for transparency
                        if (pixel.A < 128)  // Alpha threshold for transparency (0-255)
                        {
                            // Use transparency key color (bright green RGB565: 0x07E0)
                            rgb565 = 0x07E0;  // This matches TRANSPARENT_COLOR in Arduino
                        }
                        else
                        {
                            // Convert 8-bit RGB to RGB565
                            int r5 = (pixel.R >> 3) & 0x1F; // 8-bit to 5-bit
                            int g6 = (pixel.G >> 2) & 0x3F; // 8-bit to 6-bit  
                            int b5 = (pixel.B >> 3) & 0x1F; // 8-bit to 5-bit
                            
                            // Combine into RGB565 (16-bit)
                            rgb565 = (ushort)((r5 << 11) | (g6 << 5) | b5);
                        }
                        
                        // IMPORTANT: Store as standard RGB565 - do NOT apply bit inversion here!
                        // The Arduino code handles bit inversion in transformColorForLCD()
                        
                        // Store as big-endian (high byte first) to match LCD driver
                        rawData[dataIndex++] = (byte)(rgb565 >> 8);   // High byte
                        rawData[dataIndex++] = (byte)(rgb565 & 0xFF); // Low byte
                    }
                    
                    // Progress indicator
                    if (y % 16 == 0)
                    {
                        Console.Write($"\r🔄 Converting... {(y * 100 / 128):D3}%");
                    }
                }
                
                Console.WriteLine($"\r🔄 Converting... 100%");
                
                // Write raw data to file
                File.WriteAllBytes(outputPath, rawData);
                
                Console.WriteLine($"✅ Conversion complete!");
                Console.WriteLine($"📊 Output size: {rawData.Length} bytes");
                Console.WriteLine($"🎨 Format: 128x128 RGB565, big-endian");
                Console.WriteLine($"🔧 Compatible with bit-inverting GC9107 LCD");
                Console.WriteLine($"📤 Ready to upload to SD card!");
            }
        }
    }
    
    /// <summary>
    /// Creates a test pattern for verification
    /// </summary>
    public static void CreateTestPattern(string outputPath = "test_pattern.raw")
    {
        Console.WriteLine("🧪 Creating test pattern...");
        
        var rawData = new byte[128 * 128 * 2];
        int dataIndex = 0;
        
        // Create RGB test pattern
        ushort[] testColors = {
            0xF800, // Red
            0x07E0, // Green
            0x001F, // Blue
            0xFFE0, // Yellow
            0xF81F, // Magenta
            0x07FF, // Cyan
            0xFFFF, // White
            0x0000  // Black
        };
        
        for (int y = 0; y < 128; y++)
        {
            for (int x = 0; x < 128; x++)
            {
                // Create 4x2 grid of colors
                int colorIndex = (y / 32) * 4 + (x / 32);
                if (colorIndex >= testColors.Length) colorIndex = testColors.Length - 1;
                
                ushort color = testColors[colorIndex];
                
                // Store as big-endian
                rawData[dataIndex++] = (byte)(color >> 8);
                rawData[dataIndex++] = (byte)(color & 0xFF);
            }
        }
        
        File.WriteAllBytes(outputPath, rawData);
        Console.WriteLine($"✅ Test pattern created: {outputPath}");
    }
}

/* 
COMPILATION AND USAGE:

1. Save this file as PngToRawConverter.cs

2. Compile and run with .NET 9:
   dotnet run --langversion preview PngToRawConverter.cs input.png output.raw

3. Or compile to executable:
   dotnet publish -c Release -r win-x64 --self-contained -p:PublishSingleFile=true

FEATURES:
- High-quality image scaling (bicubic interpolation)
- Proper RGB565 conversion
- Big-endian format for GC9107 LCD
- Compatible with bit-inverting LCD (no pre-inversion applied)
- Progress indication during conversion
- Error handling and validation

NOTES:
- Output is standard RGB565 format
- Arduino code applies bit inversion automatically
- Images are resized to 128x128 pixels
- 2 bytes per pixel (32,768 bytes total)
*/