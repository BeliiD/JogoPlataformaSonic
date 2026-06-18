from PIL import Image
import os
import math

img = Image.open("resources/imagens/itens/objetos-base-s1.png")
width, height = img.size

out_dir = "C:/Users/ninja/.gemini/antigravity/brain/93bc52cb-5abc-4480-b0d2-75a065caf794/scratch/circles"
os.makedirs(out_dir, exist_ok=True)

def is_bg(pixel):
    if len(pixel) == 4 and pixel[3] == 0:
        return True
    r, g, b = pixel[:3]
    return g > 60 and r < 50 and b < 50

# We will scan the image with a sliding box or connected components.
# Let's use simple connected component detection.
visited = set()

def get_component(sx, sy):
    queue = [(sx, sy)]
    visited.add((sx, sy))
    component = []
    
    idx = 0
    while idx < len(queue):
        x, y = queue[idx]
        idx += 1
        component.append((x, y))
        
        # Check 4-neighbors
        for dx, dy in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
            nx, ny = x + dx, y + dy
            if 0 <= nx < width and 0 <= ny < height:
                if (nx, ny) not in visited and not is_bg(img.getpixel((nx, ny))):
                    visited.add((nx, ny))
                    queue.append((nx, ny))
    return component

components = []
for y in range(height):
    for x in range(width):
        if (x, y) not in visited and not is_bg(img.getpixel((x, y))):
            comp = get_component(x, y)
            if len(comp) > 100:  # ignore very small noise
                components.append(comp)

print(f"Detected {len(components)} components.")

for idx, comp in enumerate(components):
    xs = [p[0] for p in comp]
    ys = [p[1] for p in comp]
    x1, x2 = min(xs), max(xs)
    y1, y2 = min(ys), max(ys)
    w = x2 - x1 + 1
    h = y2 - y1 + 1
    
    # Check if component is roughly square and between 32x32 and 96x96
    if 32 <= w <= 80 and 32 <= h <= 80 and abs(w - h) < 15:
        # Save crop
        crop = img.crop((x1, y1, x2 + 1, y2 + 1))
        # check average color
        avg_r = sum(img.getpixel(p)[0] for p in comp) / len(comp)
        avg_g = sum(img.getpixel(p)[1] for p in comp) / len(comp)
        avg_b = sum(img.getpixel(p)[2] for p in comp) / len(comp)
        
        name = f"comp_{idx}_Y{y1}_X{x1}_W{w}_H{h}.png"
        crop.save(os.path.join(out_dir, name))
        print(f"Saved {name} (Avg RGB = {avg_r:.1f}, {avg_g:.1f}, {avg_b:.1f})")
