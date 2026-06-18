from PIL import Image
import os

files = [
    "resources/imagens/itens/objetos-base-s1.png",
    "resources/imagens/sprites/objects-base.png"
]

out_dir = "C:/Users/ninja/.gemini/antigravity/brain/93bc52cb-5abc-4480-b0d2-75a065caf794/scratch/find_shield_crop"
os.makedirs(out_dir, exist_ok=True)

for filepath in files:
    img = Image.open(filepath)
    w, h = img.size
    print(f"\nFile: {filepath} ({w}x{h})")
    
    bg = img.getpixel((0, 0))
    
    def is_bg_pixel(p):
        if len(p) == 4 and p[3] == 0:
            return True
        return p[:3] == bg[:3]

    # Find rows with sprite pixels in the range X=30 to w-30 (ignoring borders)
    active_rows = []
    for y in range(h):
        has_sprite = any(not is_bg_pixel(img.getpixel((x, y))) for x in range(30, w - 30))
        if has_sprite:
            active_rows.append(y)
            
    # Group rows
    row_groups = []
    if active_rows:
        start = active_rows[0]
        prev = active_rows[0]
        for y in active_rows[1:]:
            if y == prev + 1:
                prev = y
            else:
                row_groups.append((start, prev))
                start = y
                prev = y
        row_groups.append((start, prev))
        
    print(f"Detected {len(row_groups)} row groups:")
    for idx, (ys, ye) in enumerate(row_groups):
        rh = ye - ys + 1
        # Find active columns in this row group (ignoring borders)
        active_cols = []
        for x in range(30, w - 30):
            if any(not is_bg_pixel(img.getpixel((x, cy))) for cy in range(ys, ye + 1)):
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
            
        print(f"  Group {idx}: Y={ys} to {ye} (height={rh}) has {len(col_groups)} columns:")
        for cidx, (xs, xe) in enumerate(col_groups):
            cw = xe - xs + 1
            crop = img.crop((xs, ys, xe + 1, ye + 1))
            name = f"{os.path.basename(filepath)}_Y{ys}_X{xs}_W{cw}_H{rh}.png"
            crop.save(os.path.join(out_dir, name))
            print(f"    Saved {name}")
