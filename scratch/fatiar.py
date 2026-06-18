import os
import collections
from PIL import Image, ImageDraw, ImageFont

def fatiar():
    img_path = 'resources/imagens/sprites/badniks-base.png'
    if not os.path.exists(img_path):
        print(f"Erro: {img_path} nao existe!")
        return
        
    img = Image.open(img_path)
    width, height = img.size
    print(f"Resolucao da imagem: {width}x{height}")
    
    # Cores de background conhecidas
    # #0D4807 = (13, 72, 7) e #25661A = (37, 102, 26)
    # Vamos considerar qualquer verde escuro como background
    def is_bg(p):
        if len(p) >= 4 and p[3] == 0:
            return True
        r, g, b = p[0], p[1], p[2]
        # Verde escuro tipico do fundo do sprite sheet
        if g > r and g > b and g > 30 and r < 120 and b < 120:
            return True
        return False

    # Encontrar caixas de pixels nao-background
    pixels = img.load()
    visited = [[False for _ in range(height)] for _ in range(width)]
    
    bboxes = []
    
    for y in range(0, height, 2):  # Passo de 2 para velocidade
        for x in range(0, width, 2):
            if not visited[x][y] and not is_bg(pixels[x, y]):
                # Encontramos um pixel de objeto, vamos fazer BFS/flood-fill para pegar a caixa
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
                    
                    # Vizinhos (4-conectividade ou 8-conectividade com passo de 2/3 para ser rápido)
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
                    
    print(f"Total de caixas encontradas: {len(bboxes)}")
    
    # Criar uma imagem combinada para inspeccao visual
    # Vamos organizar as caixas numa grade ordenada por tamanho ou posicao
    bboxes.sort(key=lambda b: (b[1], b[0])) # Ordenar por Y, depois X
    
    # Salvar crops individuais e gerar imagem de grid
    os.makedirs('scratch/crops_detectados', exist_ok=True)
    
    grid_w = 2000
    grid_h = 2000
    grid_img = Image.new('RGB', (grid_w, grid_h), (50, 50, 50))
    draw = ImageDraw.Draw(grid_img)
    
    gx, gy = 10, 10
    max_h_in_row = 0
    
    for idx, box in enumerate(bboxes):
        bx, by, bw, bh = box
        # Recortar da imagem original
        crop = img.crop((bx, by, bx + bw, by + bh))
        crop.save(f'scratch/crops_detectados/crop_{idx}_x{bx}_y{by}_w{bw}_h{bh}.png')
        
        # Colar no grid para inspecao
        # Se ultrapassar a largura do grid, vai para a proxima linha
        if gx + bw + 20 > grid_w:
            gx = 10
            gy += max_h_in_row + 40
            max_h_in_row = 0
            
        if gy + bh + 40 > grid_h:
            # Aumentar tamanho da imagem ou parar
            break
            
        grid_img.paste(crop, (gx, gy))
        # Desenhar borda e texto
        draw.rectangle([gx, gy, gx + bw, gy + bh], outline=(255, 255, 0), width=1)
        draw.text((gx, gy + bh + 2), f"{idx}: ({bx},{by})\n{bw}x{bh}", fill=(255, 255, 255))
        
        gx += bw + 30
        if bh > max_h_in_row:
            max_h_in_row = bh
            
    grid_img.save('scratch/badniks_grid.png')
    print("Grid de badniks salvo em scratch/badniks_grid.png")

if __name__ == '__main__':
    fatiar()
