from PIL import Image

img = Image.open("resources/imagens/sprites/shields-base.png")

# Coordinates of the 4 frames:
# Frame 0: X=24, Y=713, W=32, H=48
# Frame 1: X=56, Y=713, W=48, H=48
# Frame 2: X=112, Y=713, W=56, H=48
# Frame 3: X=176, Y=713, W=64, H=48

frames = [
    (24, 713, 32, 48),
    (56, 713, 48, 48),
    (112, 713, 56, 48),
    (176, 713, 64, 48)
]

def is_bg(pixel):
    if len(pixel) == 4 and pixel[3] == 0:
        return True
    r, g, b = pixel[:3]
    return g > 60 and r < 50 and b < 50

for idx, (x, y, w, h) in enumerate(frames):
    print(f"\n--- Frame {idx} ({w}x{h}) ---")
    crop = img.crop((x, y, x + w, y + h))
    c_w, c_h = crop.size
    for cy in range(0, c_h, 3):
        line = ""
        for cx in range(0, c_w, 2):
            has_pixel = False
            for dy in range(min(3, c_h - cy)):
                for dx in range(min(2, c_w - cx)):
                    if not is_bg(crop.getpixel((cx + dx, cy + dy))):
                        has_pixel = True
                        break
                if has_pixel:
                    break
            line += "#" if has_pixel else " "
        print(line)
