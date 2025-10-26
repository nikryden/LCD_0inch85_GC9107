#!/usr/bin/env python3
"""
PNG to Raw Converter for GC9107 LCD with Bit Inversion
===============================================

Based on our testing and findings:
- GC9107 LCD uses complete bit inversion (~color)
- RGB565 format with big-endian byte order
- 128x128 pixel resolution
- Standard RGB color channels work correctly

Usage:
    python png_to_raw_converter.py input.png [output.raw]
    python png_to_raw_converter.py davinci_icon.png davinci_icon.raw

Features:
- High-quality image scaling
- Proper RGB565 conversion  
- Compatible with bit-inverting LCD
- Progress indication
- Error handling
"""

from PIL import Image, ImageDraw, ImageFont
import struct
import os
import sys
import argparse

class GC9107Converter:
    """PNG to Raw converter for GC9107 LCD with bit inversion support"""
    
    LCD_WIDTH = 128
    LCD_HEIGHT = 128
    BYTES_PER_PIXEL = 2
    
    def __init__(self):
        self.verbose = False
    
    def log(self, message):
        """Print message if verbose mode is enabled"""
        if self.verbose:
            print(message)
    
    def rgb888_to_rgb565(self, r, g, b):
        """Convert 24-bit RGB to 16-bit RGB565"""
        # Convert 8-bit values to RGB565 bit counts
        r5 = (r >> 3) & 0x1F  # 8-bit to 5-bit (divide by 8)
        g6 = (g >> 2) & 0x3F  # 8-bit to 6-bit (divide by 4)
        b5 = (b >> 3) & 0x1F  # 8-bit to 5-bit (divide by 8)
        
        # Combine into 16-bit RGB565 value
        rgb565 = (r5 << 11) | (g6 << 5) | b5
        return rgb565
    
    def convert_png_to_raw(self, input_path, output_path, verbose=False):
        """
        Convert PNG image to raw RGB565 format for GC9107 LCD
        
        Args:
            input_path (str): Path to input PNG file
            output_path (str): Path to output raw file
            verbose (bool): Enable verbose logging
            
        Returns:
            bool: True if conversion successful, False otherwise
        """
        self.verbose = verbose
        
        if not os.path.exists(input_path):
            print(f"❌ Error: Input file '{input_path}' not found!")
            return False
        
        try:
            print(f"🖼️  PNG to Raw Converter for GC9107 LCD")
            print(f"=" * 50)
            print(f"📂 Input:  {input_path}")
            print(f"📁 Output: {output_path}")
            
            # Open and process the image
            with Image.open(input_path) as img:
                print(f"📐 Original: {img.size[0]}x{img.size[1]}, mode: {img.mode}")
                
                # Handle transparency support
                has_transparency = False
                if img.mode in ('RGBA', 'LA') or 'transparency' in img.info:
                    has_transparency = True
                    print(f"🔍 Transparency detected in image")
                    # Keep RGBA mode to preserve alpha channel
                    if img.mode != 'RGBA':
                        img = img.convert('RGBA')
                        print(f"🔄 Converted to RGBA mode for transparency")
                else:
                    # Convert to RGB if no transparency
                    if img.mode != 'RGB':
                        img = img.convert('RGB')
                        print(f"🔄 Converted to RGB mode")
                
                # Resize to LCD dimensions with high-quality scaling
                target_size = (self.LCD_WIDTH, self.LCD_HEIGHT)
                if img.size != target_size:
                    # Use Lanczos resampling for best quality
                    img = img.resize(target_size, Image.Resampling.LANCZOS)
                    print(f"📏 Resized to {target_size[0]}x{target_size[1]}")
                
                # Convert to raw RGB565 data
                raw_data = bytearray()
                total_pixels = self.LCD_WIDTH * self.LCD_HEIGHT
                
                print(f"🔄 Converting to RGB565...")
                
                for y in range(self.LCD_HEIGHT):
                    for x in range(self.LCD_WIDTH):
                        pixel = img.getpixel((x, y))
                        
                        if has_transparency:
                            # Handle RGBA pixel
                            r, g, b, a = pixel
                            
                            # Check for transparency
                            if a < 128:  # Threshold for transparency (0-255)
                                # Use transparency key color (bright green RGB565: 0x07E0)
                                rgb565 = 0x07E0  # This matches TRANSPARENT_COLOR in Arduino
                            else:
                                # Convert to RGB565
                                rgb565 = self.rgb888_to_rgb565(r, g, b)
                        else:
                            # Handle RGB pixel
                            r, g, b = pixel
                            rgb565 = self.rgb888_to_rgb565(r, g, b)
                        
                        # CRITICAL: Store as standard RGB565 format
                        # Do NOT apply bit inversion here!
                        # The Arduino transformColorForLCD() function handles inversion
                        
                        # Pack as big-endian (high byte first) to match LCD driver
                        raw_data.extend(struct.pack('>H', rgb565))
                    
                    # Progress indicator
                    if y % 16 == 0 or y == self.LCD_HEIGHT - 1:
                        progress = int((y + 1) * 100 / self.LCD_HEIGHT)
                        print(f"\r🔄 Converting... {progress:3d}%", end='', flush=True)
                
                print()  # New line after progress
                
                # Write raw data to file
                with open(output_path, 'wb') as f:
                    f.write(raw_data)
                
                # Success message
                print(f"✅ Conversion successful!")
                print(f"📊 Output size: {len(raw_data):,} bytes")
                print(f"🎨 Format: {self.LCD_WIDTH}x{self.LCD_HEIGHT} RGB565, big-endian")
                print(f"🔧 Compatible with bit-inverting GC9107 LCD")
                print(f"📤 Ready to upload to SD card!")
                
                return True
                
        except Exception as e:
            print(f"❌ Error during conversion: {e}")
            return False
    
    def create_test_pattern(self, output_path="test_pattern.raw"):
        """Create a test pattern for verification"""
        print(f"🧪 Creating test pattern: {output_path}")
        
        # Create test image
        img = Image.new('RGB', (self.LCD_WIDTH, self.LCD_HEIGHT), 'black')
        draw = ImageDraw.Draw(img)
        
        # Define test colors (RGB values)
        colors = [
            (255, 0, 0),    # Red
            (0, 255, 0),    # Green  
            (0, 0, 255),    # Blue
            (255, 255, 0),  # Yellow
            (255, 0, 255),  # Magenta
            (0, 255, 255),  # Cyan
            (255, 255, 255),# White
            (0, 0, 0)       # Black
        ]
        
        color_names = ["RED", "GREEN", "BLUE", "YELLOW", "MAGENTA", "CYAN", "WHITE", "BLACK"]
        
        # Create 4x2 grid of colored rectangles
        rect_width = self.LCD_WIDTH // 4
        rect_height = self.LCD_HEIGHT // 2
        
        for i, (color, name) in enumerate(zip(colors, color_names)):
            col = i % 4
            row = i // 4
            
            x1 = col * rect_width
            y1 = row * rect_height
            x2 = x1 + rect_width - 1
            y2 = y1 + rect_height - 1
            
            draw.rectangle([x1, y1, x2, y2], fill=color)
            
            self.log(f"  {name}: ({x1},{y1}) to ({x2},{y2}) = RGB{color}")
        
        # Convert the test image
        return self.convert_png_to_raw_from_image(img, output_path)
    
    def convert_png_to_raw_from_image(self, img, output_path):
        """Convert PIL Image object to raw format"""
        try:
            raw_data = bytearray()
            
            for y in range(self.LCD_HEIGHT):
                for x in range(self.LCD_WIDTH):
                    r, g, b = img.getpixel((x, y))
                    rgb565 = self.rgb888_to_rgb565(r, g, b)
                    raw_data.extend(struct.pack('>H', rgb565))
            
            with open(output_path, 'wb') as f:
                f.write(raw_data)
            
            print(f"✅ Test pattern created: {output_path}")
            return True
            
        except Exception as e:
            print(f"❌ Error creating test pattern: {e}")
            return False

def main():
    """Main function with command line interface"""
    parser = argparse.ArgumentParser(
        description='Convert PNG images to raw RGB565 format for GC9107 LCD',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s image.png                    # Creates image.raw
  %(prog)s image.png output.raw         # Specify output name
  %(prog)s --test                       # Create test pattern
  %(prog)s image.png -v                 # Verbose output

Notes:
  - Images are automatically resized to 128x128 pixels
  - Output format is RGB565 big-endian (32,768 bytes)
  - Compatible with bit-inverting GC9107 LCD displays
  - Arduino code should handle bit inversion automatically
        """
    )
    
    parser.add_argument('input', nargs='?', help='Input PNG file')
    parser.add_argument('output', nargs='?', help='Output raw file (optional)')
    parser.add_argument('-v', '--verbose', action='store_true', help='Verbose output')
    parser.add_argument('--test', action='store_true', help='Create test pattern')
    parser.add_argument('--version', action='version', version='GC9107 PNG Converter v1.0')
    
    args = parser.parse_args()
    
    converter = GC9107Converter()
    
    # Handle test pattern creation
    if args.test:
        return converter.create_test_pattern()
    
    # Validate input
    if not args.input:
        parser.print_help()
        return False
    
    # Determine output filename
    if args.output:
        output_path = args.output
    else:
        base_name = os.path.splitext(args.input)[0]
        output_path = f"{base_name}.raw"
    
    # Convert the image
    success = converter.convert_png_to_raw(args.input, output_path, args.verbose)
    
    if success:
        print(f"\n🎯 Conversion complete! Upload '{output_path}' to your SD card.")
        return True
    else:
        print(f"\n❌ Conversion failed!")
        return False

if __name__ == "__main__":
    try:
        success = main()
        sys.exit(0 if success else 1)
    except KeyboardInterrupt:
        print(f"\n⏹️  Conversion cancelled by user")
        sys.exit(1)
    except Exception as e:
        print(f"\n💥 Unexpected error: {e}")
        sys.exit(1)