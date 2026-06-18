import os
from PIL import Image

def crop_sparkles():
    img_path = 'resources/imagens/sprites/shields-base.png'
    img = Image.open(img_path)
    
    os.makedirs('scratch/inspecao/sparkles', exist_ok=True)
    
    # We want to crop 4 frames of size 48x48 at Y = 714 (or Y = 712)
    # Let's test different X coordinates.
    # Looking at the previous row 9:
    # x=30, 56, 112, 176 (wait, if there are 4 frames, they might be:
    # Frame 1: around x = 24
    # Frame 2: around x = 80
    # Frame 3: around x = 136
    # Frame 4: around x = 192
    
    # Let's save a row crop of x from 10 to 260, y from 710 to 760
    # with a 48x48 grid to check what aligns best
    frames_info = [
        ("sparkle_0", 24, 712, 48, 48),
        ("sparkle_1", 80, 712, 48, 48),
        ("sparkle_2", 136, 712, 48, 48),
        ("sparkle_3", 192, 712, 48, 48),
    ]
    
    for name, x, y, w, h in frames_info:
        crop = img.crop((x, y, x + w, y + h))
        crop.save(f'scratch/inspecao/sparkles/{name}_x{x}_y{y}.png')
        print(f"Saved {name} at x={x}, y={y}")

if __name__ == '__main__':
    crop_sparkles()
