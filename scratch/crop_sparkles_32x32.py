import os
from PIL import Image

def crop_sparkles():
    img_path = 'resources/imagens/sprites/shields-base.png'
    img = Image.open(img_path)
    
    os.makedirs('scratch/inspecao/sparkles32', exist_ok=True)
    
    # Let's crop 4 frames of size 32x32 at x=24, 56, 88, 120, y=712
    frames_info = [
        ("sparkle_0", 24, 712, 32, 32),
        ("sparkle_1", 56, 712, 32, 32),
        ("sparkle_2", 88, 712, 32, 32),
        ("sparkle_3", 120, 712, 32, 32),
    ]
    
    for name, x, y, w, h in frames_info:
        crop = img.crop((x, y, x + w, y + h))
        crop.save(f'scratch/inspecao/sparkles32/{name}_x{x}_y{y}.png')
        print(f"Saved {name} at x={x}, y={y}")

if __name__ == '__main__':
    crop_sparkles()
