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

# Group rows
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

# Let's print only Group 0 to 11
print("Printing first 12 row groups:")
for idx in range(min(12, len(row_groups))):
    ystart, yend = row_groups[idx]
    h = yend - ystart + 1
    # Find active columns in this row group
    active_cols = []
    for x in range(width):
        has_val = any(not is_bg(img.getpixel((x, y))) for y in range(ystart, yend + 1))
        if has_val:
            active_cols.append(x)
    
    # Group columns
    col_groups = []
    if active_cols:
        cstart = active_cols[0]
        cprev = active_cols[0]
        for x in active_cols[1:]:
            if x == cprev + 1:
                cprev = x
            else:
                col_groups.append((cstart, cprev))
                cstart = x
                cprev = x
        col_groups.append((cstart, cprev))
    
    print(f"Group {idx}: Y={ystart} to {yend} (height={h})")
    for cidx, (xstart, xend) in enumerate(col_groups):
        w = xend - xstart + 1
        print(f"  Sprite {cidx}: X={xstart} to {xend} (width={w})")
