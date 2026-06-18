import os
from PIL import Image

def cropar_regioes():
    img_path = 'resources/imagens/sprites/badniks-base.png'
    img = Image.open(img_path)
    width, height = img.size
    
    regioes = [
        ("Reg1_570_890", 570, 890, 100, 100),
        ("Reg2_450_620", 450, 620, 100, 100),
        ("Reg3_140_300", 140, 300, 100, 100),
        ("Reg4_115_570", 115, 570, 100, 100),
        ("Reg5_275_695", 275, 695, 100, 100),
        ("Reg6_630_360", 630, 360, 100, 100),
    ]
    
    os.makedirs('scratch/regioes', exist_ok=True)
    
    # Encontrar as caixas delimitadoras reais dentro de badniks-base que intersectam com estas regiões
    # Para ser mais simples, vamos rodar o detector de caixas
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
                if w >= 8 and h >= 8:
                    bboxes.append((min_x, min_y, w, h))
                    
    # Para cada região, encontrar as caixas delimitadoras que estão dentro dela e salvá-las
    for nome, rx, ry, rw, rh in regioes:
        print(f"\nCaixas na região {nome}:")
        count = 0
        for idx, box in enumerate(bboxes):
            bx, by, bw, bh = box
            # Verificar se a caixa intersecta com a região [rx-30, ry-30, rx+rw+30, ry+rh+30]
            if (rx - 40 <= bx <= rx + rw + 40) and (ry - 40 <= by <= ry + rh + 40):
                count += 1
                crop = img.crop((bx, by, bx + bw, by + bh))
                crop_name = f"scratch/regioes/{nome}_box_{idx}_x{bx}_y{by}_w{bw}_h{bh}.png"
                crop.save(crop_name)
                print(f"  Box #{idx}: x={bx}, y={by}, w={bw}, h={bh} | Salvo como {crop_name}")
                
if __name__ == '__main__':
    cropar_regioes()
