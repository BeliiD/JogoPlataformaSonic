from PIL import Image

img = Image.open("resources/imagens/sprites/shields-base.png")
width, height = img.size

def is_bg(pixel):
    if len(pixel) == 4 and pixel[3] == 0:
        return True
    r, g, b = pixel[:3]
    return g > 60 and r < 50 and b < 50

rows_to_check = [472, 528, 584, 640]

for y_start in rows_to_check:
    print(f"\n--- ASCII for Y={y_start} region ---")
    crop_img = img.crop((0, y_start, 240, y_start + 48))
    
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
