import os
from PIL import Image

def check():
    folder = 'scratch/inspecao/shields48'
    files = sorted(os.listdir(folder))
    for f in files:
        if not f.endswith('.png'):
            continue
        path = os.path.join(folder, f)
        img = Image.open(path)
        w, h = img.size
        pixels = list(img.getdata())
        
        blue_count = 0
        white_count = 0
        bg_count = 0
        for p in pixels:
            r, g, b = p[0], p[1], p[2]
            if g > r and g > b and g > 30 and r < 120 and b < 120:
                bg_count += 1
            elif r > 240 and g > 240 and b > 240:
                white_count += 1
            elif b > 140 and b > r * 1.1 and b > g * 1.1:
                blue_count += 1
                
        active_count = w * h - bg_count
        print(f"File: {f} -> size: {w}x{h}, active: {active_count}, blue: {blue_count} ({blue_count/active_count:.1%} of active), white: {white_count} ({white_count/active_count:.1%})")

if __name__ == '__main__':
    check()
