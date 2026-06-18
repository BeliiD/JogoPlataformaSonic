from PIL import Image
import os

img = Image.open("resources/imagens/itens/objetos-base-s1.png")
out_dir = "C:/Users/ninja/.gemini/antigravity/brain/93bc52cb-5abc-4480-b0d2-75a065caf794/scratch"

# Crop the region of interest
# We'll crop X=20 to 280, Y=287 to 335
crop_img = img.crop((20, 287, 280, 335))
crop_img.save(os.path.join(out_dir, "crop_group16.png"))

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
