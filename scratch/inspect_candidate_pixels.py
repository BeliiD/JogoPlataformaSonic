import os
from PIL import Image

def analyze_pixels(filepath):
    if not os.path.exists(filepath):
        print(f"{filepath} not found!")
        return
    img = Image.open(filepath)
    w, h = img.size
    pixels = list(img.getdata())
    
    # We want to count:
    # 1. White pixels (R > 240, G > 240, B > 240)
    # 2. Blue pixels (B > 180 and B > R * 1.2)
    # 3. Yellow/Green pixels (G > 180 and R > 150 and B < 150)
    # 4. Background pixels (typical green bg)
    
    cnt_white = 0
    cnt_blue = 0
    cnt_yellow = 0
    cnt_bg = 0
    cnt_total = 0
    
    for p in pixels:
        r, g, b = p[0], p[1], p[2]
        if g > r and g > b and g > 30 and r < 120 and b < 120:
            cnt_bg += 1
            continue
        cnt_total += 1
        if r > 240 and g > 240 and b > 240:
            cnt_white += 1
        elif b > 160 and b > r * 1.1 and b > g * 1.1:
            cnt_blue += 1
        elif r > 160 and g > 160 and b < 150:
            cnt_yellow += 1
            
    print(f"\nImage: {os.path.basename(filepath)} ({w}x{h}, active pixels: {cnt_total})")
    print(f"  White (Sparkles?): {cnt_white} ({cnt_white/cnt_total:.1%} of active)")
    print(f"  Blue (Shield?): {cnt_blue} ({cnt_blue/cnt_total:.1%} of active)")
    print(f"  Yellow/Green (Shield highlights?): {cnt_yellow} ({cnt_yellow/cnt_total:.1%} of active)")

if __name__ == '__main__':
    analyze_pixels('scratch/inspecao/shield_candidate_y472.png')
    analyze_pixels('scratch/inspecao/candidate_y714.png')
    analyze_pixels('scratch/inspecao/candidate_y820.png')
    analyze_pixels('scratch/inspecao/candidate_top.png')
