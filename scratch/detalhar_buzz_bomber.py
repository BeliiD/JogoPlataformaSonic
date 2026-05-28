from PIL import Image

def detalhar():
    img_path = 'resources/imagens/sprites/badniks-base.png'
    img = Image.open(img_path)
    width, height = img.size
    
    def is_bg(p):
        if len(p) >= 4 and p[3] == 0:
            return True
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
                
                import collections
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
                bboxes.append((min_x, min_y, w, h))
                
    # Listar caixas na região de interesse: x de 0 a 300, y de 340 a 420
    print("Caixas na área x=0..300, y=340..420:")
    area_boxes = []
    for idx, box in enumerate(bboxes):
        bx, by, bw, bh = box
        if (0 <= bx <= 300) and (340 <= by <= 420):
            area_boxes.append((idx, bx, by, bw, bh))
            print(f"Caixa #{idx}: x={bx}, y={by}, w={bw}, h={bh} | area={bw*bh}")
            
    # Salvar crops individuais destas caixas para podermos confirmar
    import os
    os.makedirs('scratch/buzz_bomber_candidatos', exist_ok=True)
    for idx, bx, by, bw, bh in area_boxes:
        crop = img.crop((bx, by, bx + bw, by + bh))
        crop.save(f'scratch/buzz_bomber_candidatos/box_{idx}_x{bx}_y{by}_w{bw}_h{bh}.png')
        
if __name__ == '__main__':
    detalhar()
