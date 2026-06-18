import os
from PIL import Image

def check():
    folder = 'scratch/inspecao/sparkles'
    files = sorted(os.listdir(folder))
    for f in files:
        if not f.endswith('.png'):
            continue
        path = os.path.join(folder, f)
        img = Image.open(path)
        w, h = img.size
        pixels = list(img.getdata())
        
        white_count = 0
        bg_count = 0
        for p in pixels:
            r, g, b = p[0], p[1], p[2]
            # Check green bg
            if g > r and g > b and g > 30 and r < 120 and b < 120:
                bg_count += 1
            elif r > 240 and g > 240 and b > 240:
                white_count += 1
                
        active_count = w * h - bg_count
        print(f"File: {f} -> size: {w}x{h}, active: {active_count}, white: {white_count} ({white_count/active_count:.1%} of active)")

if __name__ == '__main__':
    check()
