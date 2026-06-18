import os
from PIL import Image

def check():
    crop_dir = 'scratch/crops_detectados'
    files_to_check = [
        'crop_13_x306_y200_w11_h11.png',
        'crop_27_x306_y224_w11_h11.png',
        'crop_18_x408_y202_w15_h15.png',
        'crop_2_x360_y198_w15_h21.png',
        'crop_3_x384_y198_w15_h23.png'
    ]
    
    for f in files_to_check:
        path = os.path.join(crop_dir, f)
        if not os.path.exists(path):
            print(f"File not found: {f}")
            continue
        img = Image.open(path)
        w, h = img.size
        pixels = list(img.getdata())
        
        # calculate main colors (excluding bg)
        colors = {}
        bg_count = 0
        total_pixels = 0
        
        for p in pixels:
            r, g, b = p[0], p[1], p[2]
            # check background green
            if g > r and g > b and g > 30 and r < 120 and b < 120:
                bg_count += 1
                continue
            total_pixels += 1
            color_key = (r, g, b)
            colors[color_key] = colors.get(color_key, 0) + 1
            
        sorted_colors = sorted(colors.items(), key=lambda x: x[1], reverse=True)
        print(f"\nFile: {f} (size: {w}x{h}, visible pixels: {total_pixels}/{w*h})")
        print("Top 5 colors:")
        for color, count in sorted_colors[:5]:
            pct = count / total_pixels if total_pixels > 0 else 0
            print(f"  RGB: {color} -> count: {count} ({pct:.1%})")

if __name__ == '__main__':
    check()
