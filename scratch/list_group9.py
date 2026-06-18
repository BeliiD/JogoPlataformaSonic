from PIL import Image

img = Image.open("resources/imagens/itens/objetos-base-s1.png")
width, height = img.size

def is_bg(pixel):
    if len(pixel) == 4 and pixel[3] == 0:
        return True
    r, g, b = pixel[:3]
    return g > 60 and r < 50 and b < 50

ystart, yend = 198, 381

# Find active columns in this row range
active_cols = []
for x in range(width):
    if any(not is_bg(img.getpixel((x, y))) for y in range(ystart, yend + 1)):
        active_cols.append(x)

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

print(f"Group 9: Y={ystart} to {yend} has {len(col_groups)} column groups:")
for cidx, (xstart, xend) in enumerate(col_groups):
    w = xend - xstart + 1
    # Find active rows in this column range
    active_rows_in_col = []
    for y in range(ystart, yend + 1):
        if any(not is_bg(img.getpixel((x, y))) for x in range(xstart, xend + 1)):
            active_rows_in_col.append(y)
    
    # We might have multiple vertical sprites stacked in the same column range.
    # Let's find contiguous vertical blocks.
    v_groups = []
    if active_rows_in_col:
        vstart = active_rows_in_col[0]
        vprev = active_rows_in_col[0]
        for y in active_rows_in_col[1:]:
            if y == vprev + 1:
                vprev = y
            else:
                v_groups.append((vstart, vprev))
                vstart = y
                vprev = y
        v_groups.append((vstart, vprev))
        
    for vidx, (ys, ye) in enumerate(v_groups):
        sh = ye - ys + 1
        # average color
        pixels = []
        for y in range(ys, ye + 1):
            for x in range(xstart, xend + 1):
                p = img.getpixel((x, y))
                if not is_bg(p):
                    pixels.append(p[:3])
        avg = (0,0,0)
        if pixels:
            avg = (sum(p[0] for p in pixels)/len(pixels), sum(p[1] for p in pixels)/len(pixels), sum(p[2] for p in pixels)/len(pixels))
        print(f"  Col {cidx}, Sprite {vidx}: X={xstart} to {xend} (w={w}), Y={ys} to {ye} (h={sh}), Avg RGB = ({avg[0]:.1f}, {avg[1]:.1f}, {avg[2]:.1f})")
