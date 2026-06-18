from PIL import Image

img = Image.open("resources/imagens/sprites/objects-base.png")
width, height = img.size

bg_color = (16, 112, 132)

def is_bg(pixel):
    r, g, b = pixel[:3]
    return abs(r - bg_color[0]) < 10 and abs(g - bg_color[1]) < 10 and abs(b - bg_color[2]) < 10

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
            components.append(comp)

print(f"All {len(components)} components:")
for idx, comp in enumerate(components):
    xs = [p[0] for p in comp]
    ys = [p[1] for p in comp]
    x1, x2 = min(xs), max(xs)
    y1, y2 = min(ys), max(ys)
    w = x2 - x1 + 1
    h = y2 - y1 + 1
    print(f"Component {idx}: X={x1} to {x2} (w={w}), Y={y1} to {y2} (h={h}), size_pixels={len(comp)}")
