import os
import collections
from PIL import Image

def find_all_sprites():
    img_path = 'resources/imagens/sprites/shields-base.png'
    img = Image.open(img_path)
    width, height = img.size
    
    def is_bg(p):
        r, g, b = p[0], p[1], p[2]
        if g > r and g > b and g > 30 and r < 120 and b < 120:
            return True
        return False

    pixels = img.load()
    visited = [[False for _ in range(height)] for _ in range(width)]
    bboxes = []
    
    for y in range(0, height, 2):
        for x in range(0, width, 2):
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
                    
                    for dx, dy in [(-2, 0), (2, 0), (0, -2), (0, 2), (-2, -2), (2, -2), (-2, 2), (2, 2)]:
                        nx, ny = cx + dx, cy + dy
                        if 0 <= nx < width and 0 <= ny < height:
                            if not visited[nx][ny] and not is_bg(pixels[nx, ny]):
                                visited[nx][ny] = True
                                queue.append((nx, ny))
                                
                w = max_x - min_x + 1
                h = max_y - min_y + 1
                if w >= 10 and h >= 10:
                    bboxes.append((min_x, min_y, w, h))
                    
    bboxes.sort(key=lambda b: (b[1], b[0]))
    print(f"Total boxes found: {len(bboxes)}")
    for idx, box in enumerate(bboxes):
        print(f"Box {idx}: x={box[0]}, y={box[1]}, w={box[2]}, h={box[3]}")

if __name__ == '__main__':
    find_all_sprites()
