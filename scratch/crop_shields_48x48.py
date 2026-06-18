import os
from PIL import Image

def crop_shields():
    img_path = 'resources/imagens/sprites/shields-base.png'
    img = Image.open(img_path)
    
    os.makedirs('scratch/inspecao/shields48', exist_ok=True)
    
    # We want to crop 3 frames of size 48x48 at x=24, 80, 136, y=472
    frames_info = [
        ("shield_0", 24, 472, 48, 48),
        ("shield_1", 80, 472, 48, 48),
        ("shield_2", 136, 472, 48, 48),
    ]
    
    for name, x, y, w, h in frames_info:
        crop = img.crop((x, y, x + w, y + h))
        crop.save(f'scratch/inspecao/shields48/{name}_x{x}_y{y}.png')
        print(f"Saved {name} at x={x}, y={y}")

if __name__ == '__main__':
    crop_shields()
