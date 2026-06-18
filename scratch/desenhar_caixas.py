import os
from PIL import Image, ImageDraw

def desenhar():
    img_path = 'resources/imagens/sprites/badniks-base.png'
    img = Image.open(img_path)
    width, height = img.size
    
    # Vamos rodar o mesmo detector de caixas
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
                if w >= 10 and h >= 10:
                    bboxes.append((min_x, min_y, w, h))
                    
    # Desenhar caixas e índices na imagem
    draw_img = img.copy()
    draw = ImageDraw.Draw(draw_img)
    
    # Filtrar caixas de tamanho compatível com Badniks voadores (ex: w=30-50, h=25-45)
    print("Caixas candidatas (w=30-55, h=20-45):")
    candidatos_count = 0
    for idx, box in enumerate(bboxes):
        bx, by, bw, bh = box
        if 25 <= bw <= 55 and 18 <= bh <= 45:
            # Imprimir info
            candidatos_count += 1
            print(f"Candidato #{idx}: x={bx}, y={by}, w={bw}, h={bh} | area={bw*bh}")
            # Desenhar caixa em vermelho
            draw.rectangle([bx, by, bx + bw, by + bh], outline=(255, 0, 0), width=2)
            draw.text((bx, by - 12), f"#{idx}:{bx},{by}", fill=(255, 0, 0))
        else:
            # Desenhar em amarelo fino
            draw.rectangle([bx, by, bx + bw, by + bh], outline=(255, 255, 0), width=1)
            
    draw_img.save('scratch/badniks_analise.png')
    print(f"Total candidatos badniks de tamanho compativel: {candidatos_count}")
    print("Imagem anotada salva em scratch/badniks_analise.png")

if __name__ == '__main__':
    desenhar()
