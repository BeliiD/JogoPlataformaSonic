from PIL import Image
import os

img = Image.open("resources/imagens/itens/objetos-base-s1.png")
out_dir = "C:/Users/ninja/.gemini/antigravity/brain/93bc52cb-5abc-4480-b0d2-75a065caf794/scratch"

# Crop Y=678 region (X=244 to 388, Y=678 to 726)
crop_678 = img.crop((244, 678, 388, 726))
crop_678.save(os.path.join(out_dir, "crop_y678.png"))

# Crop Y=734 region (X=244 to 388, Y=734 to 782)
crop_734 = img.crop((244, 734, 388, 782))
crop_734.save(os.path.join(out_dir, "crop_y734.png"))

def is_bg(pixel):
    if len(pixel) == 4 and pixel[3] == 0:
        return True
    r, g, b = pixel[:3]
    return g > 60 and r < 50 and b < 50

print("ASCII for Y=678 region:")
c_w, c_h = crop_678.size
for y in range(0, c_h, 3):
    line = ""
    for x in range(0, c_w, 2):
        has_pixel = False
        for dy in range(min(3, c_h - y)):
            for dx in range(min(2, c_w - x)):
                if not is_bg(crop_678.getpixel((x + dx, y + dy))):
                    has_pixel = True
                    break
            if has_pixel:
                break
        line += "#" if has_pixel else " "
    print(line)

print("\nASCII for Y=734 region:")
c_w, c_h = crop_734.size
for y in range(0, c_h, 3):
    line = ""
    for x in range(0, c_w, 2):
        has_pixel = False
        for dy in range(min(3, c_h - y)):
            for dx in range(min(2, c_w - x)):
                if not is_bg(crop_734.getpixel((x + dx, y + dy))):
                    has_pixel = True
                    break
            if has_pixel:
                break
        line += "#" if has_pixel else " "
    print(line)
