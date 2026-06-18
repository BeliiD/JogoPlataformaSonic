import os
import collections
from PIL import Image

def find_sprites():
    img_path = 'resources/imagens/itens/itens.png'
    if not os.path.exists(img_path):
        print(f"Error: {img_path} does not exist!")
        return
        
    img = Image.open(img_path)
    width, height = img.size
    print(f"Image resolution: {width}x{height}")
    
    # Check top colors
    pixels = img.load()
    colors = {}
    for y in range(height):
        for x in range(width):
            p = pixels[x, y]
            colors[p] = colors.get(p, 0) + 1
    sorted_colors = sorted(colors.items(), key=lambda x: x[1], reverse=True)
    print("Top colors:")
    for color, count in sorted_colors[:5]:
        print(f"  {color}: {count} ({count/(width*height):.1%})")
        
    bg_color = sorted_colors[0][0] # usually (16, 112, 132, 255)
    
    def is_bg(p):
        if len(p) >= 4 and p[3] == 0:
            return True
        return p[0] == bg_color[0] and p[1] == bg_color[1] and p[2] == bg_color[2]

    visited = [[False for _ in range(height)] for _ in range(width)]
    bboxes = []
    
    for y in range(height):
        for x in range(width):
            if not visited[x][y] and not is_bg(pixels[x, y]):
                min_x, max_x = x, x
                min_y, max_y = y, y
                
                queue = collections.deque([(x, y)])
                visited[x][y] = True
                
                while queue:
                    cx, cy = queue.popleft()
                    
                    if cx < min_x: min_x = cx
                    if cx > max_x: max_x = cx
                    if cy < min_y: min_y = cy
                    if cy > max_y: max_y = cy
                    
                    for dx, dy in [(-1, 0), (1, 0), (0, -1), (0, 1), (-1, -1), (1, -1), (-1, 1), (1, 1)]:
                        nx, ny = cx + dx, cy + dy
                        if 0 <= nx < width and 0 <= ny < height:
                            if not visited[nx][ny] and not is_bg(pixels[nx, ny]):
                                visited[nx][ny] = True
                                queue.append((nx, ny))
                                
                w = max_x - min_x + 1
                h = max_y - min_y + 1
                bboxes.append((min_x, min_y, w, h))
                    
    print(f"Total bounding boxes found: {len(bboxes)}")
    bboxes.sort(key=lambda b: (b[1], b[0]))
    for idx, box in enumerate(bboxes):
        print(f"Box {idx}: x={box[0]}, y={box[1]}, w={box[2]}, h={box[3]}")

if __name__ == '__main__':
    find_sprites()
