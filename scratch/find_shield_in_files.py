from PIL import Image
import os

files_to_scan = [
    ("resources/imagens/itens/objetos-base-s1.png", (13, 72, 7)),
    ("resources/imagens/sprites/objects-base.png", (16, 112, 132))
]

out_dir = "C:/Users/ninja/.gemini/antigravity/brain/93bc52cb-5abc-4480-b0d2-75a065caf794/scratch/find_shield"
os.makedirs(out_dir, exist_ok=True)

for filepath, bg_color in files_to_scan:
    img = Image.open(filepath)
    w, h = img.size
    print(f"\nScanning file: {filepath} ({w}x{h})")
    
    def is_bg(pixel):
        r, g, b = pixel[:3]
        return abs(r - bg_color[0]) < 10 and abs(g - bg_color[1]) < 10 and abs(b - bg_color[2]) < 10

    # Let's find rows with non-bg pixels
    row_has_pixels = [any(not is_bg(img.getpixel((x, y))) for x in range(w)) for y in range(h)]
    
    # Let's search for rows that have height of ~48 or ~64
    y = 0
    while y < h:
        if row_has_pixels[y]:
            ystart = y
            while y < h and row_has_pixels[y]:
                y += 1
            yend = y - 1
            rh = yend - ystart + 1
            
            # If height is between 32 and 80
            if 32 <= rh <= 80:
                # Let's find columns in this row group
                active_cols = []
                for x in range(w):
                    if any(not is_bg(img.getpixel((x, cy))) for cy in range(ystart, yend + 1)):
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
                
                # Print details
                print(f"Row group Y: {ystart} to {yend} (height={rh}) has {len(col_groups)} sprites:")
                for cidx, (xs, xe) in enumerate(col_groups):
                    sw = xe - xs + 1
                    print(f"  Sprite {cidx}: X={xs} to {xe} (w={sw})")
                    
                    # If we have 3 consecutive sprites of similar width
                    # Let's crop and save them
                    if 32 <= sw <= 80:
                        crop = img.crop((xs, ystart, xe + 1, yend + 1))
                        name = f"{os.path.basename(filepath)}_Y{ystart}_X{xs}_W{sw}_H{rh}.png"
                        crop.save(os.path.join(out_dir, name))
        else:
            y += 1
