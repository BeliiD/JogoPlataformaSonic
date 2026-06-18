from PIL import Image
import os

def analisar():
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
                if w >= 10 and h >= 10:
                    bboxes.append((min_x, min_y, w, h))
                    
    # Analisar cores de cada caixa
    candidatos = []
    for idx, box in enumerate(bboxes):
        bx, by, bw, bh = box
        
        # Pular tamanhos incompatíveis
        if not (20 <= bw <= 65 and 18 <= bh <= 50):
            continue
            
        crop = img.crop((bx, by, bx + bw, by + bh))
        crop_pixels = list(crop.getdata())
        
        cnt_visible = 0
        cnt_azul_escuro = 0
        cnt_amarelo = 0
        cnt_vermelho = 0
        cnt_asa = 0
        
        for p in crop_pixels:
            if len(p) >= 4 and p[3] == 0:
                continue
            r, g, b = p[0], p[1], p[2]
            if g > r and g > b and g > 30 and r < 120 and b < 120:
                continue
            
            cnt_visible += 1
            
            # Azul escuro / roxo clássico do Buzz Bomber:
            # B alto, R e G mais baixos
            if b > 90 and b > r and b > g:
                cnt_azul_escuro += 1
                
            # Amarelo clássico da cauda:
            # R e G bem altos, B baixo
            if r > 180 and g > 130 and b < 100:
                cnt_amarelo += 1
                
            # Vermelho clássico do olho:
            if r > 150 and g < 90 and b < 90:
                cnt_vermelho += 1
                
            # Azul/Ciano claro das asas:
            if r > 100 and g > 150 and b > 160:
                cnt_asa += 1
                
        if cnt_visible > 30:
            p_azul = cnt_azul_escuro / cnt_visible
            p_amarelo = cnt_amarelo / cnt_visible
            p_vermelho = cnt_vermelho / cnt_visible
            p_asa = cnt_asa / cnt_visible
            
            # Buzz Bomber tem azul escuro E amarelo!
            if p_azul > 0.05 and p_amarelo > 0.03:
                candidatos.append({
                    'idx': idx, 'x': bx, 'y': by, 'w': bw, 'h': bh,
                    'p_azul': p_azul, 'p_amarelo': p_amarelo, 'p_vermelho': p_vermelho, 'p_asa': p_asa,
                    'total': cnt_visible
                })
                
    candidatos.sort(key=lambda c: (c['p_azul'] + c['p_amarelo']), reverse=True)
    
    print("Mapeamento final dos candidatos a Buzz Bomber real:")
    for c in candidatos[:25]:
        print(f"Candidato #{c['idx']}: x={c['x']}, y={c['y']}, w={c['w']}, h={c['h']}")
        print(f"  Azul Escuro: {c['p_azul']:.1%}, Amarelo: {c['p_amarelo']:.1%}, Vermelho: {c['p_vermelho']:.1%}, Asa: {c['p_asa']:.1%}")
        print("-" * 50)

if __name__ == '__main__':
    analisar()
