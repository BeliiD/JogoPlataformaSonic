from PIL import Image
import os

img = Image.open("resources/imagens/sprites/shields-base.png")
out_dir = "C:/Users/ninja/.gemini/antigravity/brain/93bc52cb-5abc-4480-b0d2-75a065caf794/scratch"

# Crop the shield row (X=0 to 240, Y=472 to 520)
crop_img = img.crop((0, 472, 240, 520))
crop_img.save(os.path.join(out_dir, "crop_shields_base_472.png"))

def is_bg(pixel):
    if len(pixel) == 4 and pixel[3] == 0:
        return True
    r, g, b = pixel[:3]
    return g > 60 and r < 50 and b < 50

# Print downsampled ASCII
c_w, c_h = crop_img.size
for y in range(0, c_h, 3):
    line = ""
    for x in range(0, c_w, 2):
        has_pixel = False
        for dy in range(min(3, c_h - y)):
            for dx in range(min(2, c_w - x)):
                if not is_bg(crop_img.getpixel((x + dx, y + dy))):
                    has_pixel = True
                    break
            if has_pixel:
                break
        line += "#" if has_pixel else " "
    print(line)
