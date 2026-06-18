from PIL import Image

img = Image.open("resources/imagens/itens/objetos-base-s1.png")
width, height = img.size

def is_bg(pixel):
    if len(pixel) == 4 and pixel[3] == 0:
        return True
    r, g, b = pixel[:3]
    return g > 60 and r < 50 and b < 50

# Scan all row groups first
non_bg_rows = []
for y in range(height):
    non_bg = any(not is_bg(img.getpixel((x, y))) for x in range(width))
    if non_bg:
        non_bg_rows.append(y)

row_groups = []
if non_bg_rows:
    start = non_bg_rows[0]
    prev = non_bg_rows[0]
    for y in non_bg_rows[1:]:
        if y == prev + 1:
            prev = y
        else:
            row_groups.append((start, prev))
            start = y
            prev = y
    row_groups.append((start, prev))

print(f"Total row groups found: {len(row_groups)}")
for idx, (start, end) in enumerate(row_groups):
    pixels = []
    for y in range(start, end + 1):
        for x in range(width):
            p = img.getpixel((x, y))
            if not is_bg(p):
                pixels.append(p[:3])
    if pixels:
        avg_r = sum(p[0] for p in pixels) / len(pixels)
        avg_g = sum(p[1] for p in pixels) / len(pixels)
        avg_b = sum(p[2] for p in pixels) / len(pixels)
        print(f"Group {idx} (Y: {start}-{end}, height={end-start+1}): Avg RGB = ({avg_r:.1f}, {avg_g:.1f}, {avg_b:.1f})")
