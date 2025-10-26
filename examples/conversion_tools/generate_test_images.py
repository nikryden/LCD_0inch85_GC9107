#!/usr/bin/env python3
"""
Test Image Generator for ESP32 LCD Display System
Creates PNG test images with transparency and converts them to RGB565 RAW format

Requirements: pip install pillow numpy
"""

import os
from PIL import Image, ImageDraw
import numpy as np
import struct

# Configuration
IMAGE_SIZE = 128
IMAGES_DIR = "images"
CONVERTED_DIR = "convertedimages"
TRANSPARENCY_COLOR = (0, 255, 0)  # Bright green for transparency

def ensure_directories():
    """Create required directories if they don't exist"""
    os.makedirs(IMAGES_DIR, exist_ok=True)
    os.makedirs(CONVERTED_DIR, exist_ok=True)

def create_transparency_test_image():
    """Create a test image with different colors and transparency areas"""
    print("Creating transparency test image...")
    
    # Create 128x128 RGBA image
    img = Image.new('RGBA', (IMAGE_SIZE, IMAGE_SIZE), (255, 255, 255, 0))
    draw = ImageDraw.Draw(img)
    
    # Background sections with different colors (opaque)
    colors = [
        (255, 0, 0, 255),    # Red
        (0, 0, 255, 255),    # Blue  
        (255, 255, 0, 255),  # Yellow
        (255, 0, 255, 255),  # Magenta
    ]
    
    # Draw colored quadrants
    for i, color in enumerate(colors):
        x = (i % 2) * 64
        y = (i // 2) * 64
        draw.rectangle([x, y, x + 63, y + 63], fill=color)
    
    # Add some geometric shapes with transparency
    # Transparent circle in center
    draw.ellipse([32, 32, 96, 96], fill=(0, 0, 0, 0))
    
    # Add text labels
    try:
        # Draw text on colored areas
        draw.text((10, 10), "RED", fill=(255, 255, 255, 255))
        draw.text((74, 10), "BLUE", fill=(255, 255, 255, 255))
        draw.text((10, 74), "YELLOW", fill=(0, 0, 0, 255))
        draw.text((74, 74), "MAGENTA", fill=(255, 255, 255, 255))
        draw.text((48, 48), "TRANSP", fill=(255, 255, 255, 255))
    except:
        print("Note: Text rendering may not work without font support")
    
    # Add transparent stripes
    for i in range(0, IMAGE_SIZE, 16):
        if i % 32 == 0:
            draw.rectangle([i, 0, i + 8, IMAGE_SIZE], fill=(0, 0, 0, 0))
    
    # Save PNG
    png_path = os.path.join(IMAGES_DIR, "transparency_test.png")
    img.save(png_path)
    print(f"✅ Created: {png_path}")
    
    return img

def create_gradient_test_image():
    """Create a test image with gradient colors and transparency"""
    print("Creating gradient test image...")
    
    # Create 128x128 RGBA image
    img = Image.new('RGBA', (IMAGE_SIZE, IMAGE_SIZE), (0, 0, 0, 0))
    
    # Create gradient arrays
    pixels = np.zeros((IMAGE_SIZE, IMAGE_SIZE, 4), dtype=np.uint8)
    
    for y in range(IMAGE_SIZE):
        for x in range(IMAGE_SIZE):
            # Create radial gradient from center
            center_x, center_y = IMAGE_SIZE // 2, IMAGE_SIZE // 2
            distance = np.sqrt((x - center_x)**2 + (y - center_y)**2)
            max_distance = np.sqrt(center_x**2 + center_y**2)
            
            # Normalize distance (0-1)
            normalized_distance = min(distance / max_distance, 1.0)
            
            # Create rainbow gradient based on angle and distance
            angle = np.arctan2(y - center_y, x - center_x)
            hue = (angle + np.pi) / (2 * np.pi)  # 0-1
            
            # Convert HSV to RGB
            import colorsys
            r, g, b = colorsys.hsv_to_rgb(hue, 1.0, 1.0 - normalized_distance * 0.5)
            
            # Create transparency pattern
            # Make some areas transparent based on position
            alpha = 255
            if (x + y) % 32 < 8:  # Diagonal stripes
                alpha = 0
            elif distance < 20:  # Center circle
                alpha = 0
            elif normalized_distance > 0.8:  # Outer ring
                alpha = int(255 * (1.0 - normalized_distance) * 5)  # Fade out
            
            pixels[y, x] = [int(r * 255), int(g * 255), int(b * 255), alpha]
    
    # Convert to PIL image
    img = Image.fromarray(pixels, 'RGBA')
    
    # Add some opaque geometric shapes for contrast
    draw = ImageDraw.Draw(img)
    
    # Add corner markers (opaque)
    corners = [(0, 0), (IMAGE_SIZE-20, 0), (0, IMAGE_SIZE-20), (IMAGE_SIZE-20, IMAGE_SIZE-20)]
    for i, (x, y) in enumerate(corners):
        color = [(255, 0, 0, 255), (0, 255, 0, 255), (0, 0, 255, 255), (255, 255, 255, 255)][i]
        draw.rectangle([x, y, x + 19, y + 19], fill=color)
    
    # Save PNG
    png_path = os.path.join(IMAGES_DIR, "gradient_test.png")
    img.save(png_path)
    print(f"✅ Created: {png_path}")
    
    return img

def rgb888_to_rgb565(r, g, b):
    """Convert RGB888 to RGB565 format"""
    r = (r >> 3) & 0x1F  # 5 bits
    g = (g >> 2) & 0x3F  # 6 bits  
    b = (b >> 3) & 0x1F  # 5 bits
    return (r << 11) | (g << 5) | b

def convert_png_to_raw(png_path, raw_path):
    """Convert PNG image to RGB565 RAW format with transparency support"""
    print(f"Converting {png_path} to {raw_path}...")
    
    # Open PNG and convert to RGBA
    img = Image.open(png_path).convert('RGBA')
    
    # Resize to exact dimensions if needed
    if img.size != (IMAGE_SIZE, IMAGE_SIZE):
        img = img.resize((IMAGE_SIZE, IMAGE_SIZE), Image.Resampling.LANCZOS)
    
    # Convert to numpy array for processing
    pixels = np.array(img)
    
    # Prepare raw data
    raw_data = []
    transparent_count = 0
    
    for y in range(IMAGE_SIZE):
        for x in range(IMAGE_SIZE):
            r, g, b, a = pixels[y, x]
            
            # Handle transparency
            if a < 128:  # Transparent pixel
                # Use bright green (0x07E0) as transparency key
                rgb565 = 0x07E0
                transparent_count += 1
            else:
                # Convert to RGB565
                rgb565 = rgb888_to_rgb565(r, g, b)
            
            # Pack as big-endian 16-bit value
            raw_data.append(struct.pack('>H', rgb565))
    
    # Write raw file
    with open(raw_path, 'wb') as f:
        f.write(b''.join(raw_data))
    
    # Verify file size
    file_size = os.path.getsize(raw_path)
    expected_size = IMAGE_SIZE * IMAGE_SIZE * 2  # 2 bytes per pixel
    
    print(f"✅ Converted: {raw_path}")
    print(f"   File size: {file_size} bytes (expected: {expected_size})")
    print(f"   Transparent pixels: {transparent_count}")
    
    if file_size != expected_size:
        print(f"⚠️  Warning: File size mismatch!")
    
    return transparent_count

def main():
    """Main function to generate all test images"""
    print("🎨 ESP32 LCD Test Image Generator")
    print("=" * 40)
    
    # Ensure directories exist
    ensure_directories()
    
    # Create test images
    transparency_img = create_transparency_test_image()
    gradient_img = create_gradient_test_image()
    
    print("\n🔄 Converting PNG images to RAW format...")
    
    # Convert to RAW format
    transparency_raw = os.path.join(CONVERTED_DIR, "transparency_test.raw")
    gradient_raw = os.path.join(CONVERTED_DIR, "gradient_test.raw")
    
    trans_count1 = convert_png_to_raw(
        os.path.join(IMAGES_DIR, "transparency_test.png"),
        transparency_raw
    )
    
    trans_count2 = convert_png_to_raw(
        os.path.join(IMAGES_DIR, "gradient_test.png"),
        gradient_raw
    )
    
    print("\n✅ Test Image Generation Complete!")
    print("=" * 40)
    print(f"📁 PNG Images: {IMAGES_DIR}/")
    print(f"   - transparency_test.png")
    print(f"   - gradient_test.png")
    print(f"📁 RAW Images: {CONVERTED_DIR}/")
    print(f"   - transparency_test.raw ({trans_count1} transparent pixels)")
    print(f"   - gradient_test.raw ({trans_count2} transparent pixels)")
    print("\n🎯 Usage:")
    print("1. Copy .raw files to your ESP32 SD card")
    print("2. Upload Arduino code to ESP32")
    print("3. Images will display with transparency support")
    print("4. Transparent areas will show white background")

if __name__ == "__main__":
    main()