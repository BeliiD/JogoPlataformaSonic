from PIL import Image
import os

img = Image.open("resources/imagens/sprites/objects-base.png")
out_dir = "C:/Users/ninja/.gemini/antigravity/brain/93bc52cb-5abc-4480-b0d2-75a065caf794/scratch"

# Crop the component
crop_img = img.crop((300, 269, 476, 409))
crop_img.save(os.path.join(out_dir, "crop_comp2.png"))

bg_color = (16, 112, 132)

def is_bg(pixel):
    r, g, b = pixel[:3]
    return abs(r - bg_color[0]) < 10 and abs(g - bg_color[1]) < 10 and abs(b - bg_color[2]) < 10

# Print downsampled ASCII
c_w, c_h = crop_img.size
for y in range(0, c_h, 4):
    line = ""
    for x in range(0, c_w, 2):
        has_pixel = False
        for dy in range(min(4, c_h - y)):
            for dx in range(min(2, c_w - x)):
                if not is_bg(crop_img.getpixel((x + dx, y + dy))):
                    has_pixel = True
                    break
            if has_pixel:
                break
        line += "#" if has_pixel else " "
    print(line)
