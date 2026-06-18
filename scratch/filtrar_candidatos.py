import os
from PIL import Image

def filtrar():
    crop_dir = 'scratch/crops_detectados'
    files = os.listdir(crop_dir)
    
    candidatos = []
    
    for f in files:
        if not f.endswith('.png'):
            continue
            
        filepath = os.path.join(crop_dir, f)
        img = Image.open(filepath)
        w, h = img.size
        
        # Buzz Bomber tamanho tipico: w entre 30 e 70, h entre 20 e 60
        if not (28 <= w <= 80 and 20 <= h <= 60):
            continue
            
        pixels = list(img.getdata())
        
        cnt_azul = 0
        cnt_amarelo = 0
        cnt_vermelho = 0
        cnt_total_visivel = 0
        
        for p in pixels:
            # Pular pixels transparentes ou pretos (se considerados bg ou se muito escuros)
            r, g, b = p[0], p[1], p[2]
            
            # Verificar se e pixel transparente na nossa lógica de transparente:
            # #0D4807 ou #25661A
            if g > r and g > b and g > 30 and r < 120 and b < 120:
                continue
                
            cnt_total_visivel += 1
            
            # Azul do corpo: azul dominante
            if b > 120 and b > r * 1.3 and b > g * 1.3:
                cnt_azul += 1
            # Amarelo/Laranja do ferrão ou asas: r e g altos, b baixo
            elif r > 150 and g > 120 and b < 100:
                cnt_amarelo += 1
            # Vermelho dos olhos/detalhes: r alto, g e b baixos
            elif r > 150 and g < 100 and b < 100:
                cnt_vermelho += 1
                
        if cnt_total_visivel > 50:
            pct_azul = cnt_azul / cnt_total_visivel
            pct_amarelo = cnt_amarelo / cnt_total_visivel
            pct_vermelho = cnt_vermelho / cnt_total_visivel
            
            # Buzz Bomber tem azul e amarelo!
            if pct_azul > 0.05 and pct_amarelo > 0.05:
                # Extrair x, y, w, h do nome do arquivo
                # crop_idx_xBX_yBY_wBW_hBH.png
                parts = f.replace('.png', '').split('_')
                x = int([p for p in parts if p.startswith('x')][0][1:])
                y = int([p for p in parts if p.startswith('y')][0][1:])
                w = int([p for p in parts if p.startswith('w')][0][1:])
                h = int([p for p in parts if p.startswith('h')][0][1:])
                
                candidatos.append({
                    'file': f,
                    'x': x,
                    'y': y,
                    'w': w,
                    'h': h,
                    'pct_azul': pct_azul,
                    'pct_amarelo': pct_amarelo,
                    'pct_vermelho': pct_vermelho,
                    'cnt_total': cnt_total_visivel
                })
                
    # Ordenar por percentual de azul + amarelo
    candidatos.sort(key=lambda c: (c['pct_azul'] + c['pct_amarelo']), reverse=True)
    
    print(f"Encontrados {len(candidatos)} candidatos com características de Buzz Bomber (azul + amarelo):")
    for c in candidatos[:20]:
        print(f"Arquivo: {c['file']}")
        print(f"  Coord: x={c['x']}, y={c['y']}, w={c['w']}, h={c['h']}")
        print(f"  Azul: {c['pct_azul']:.2%}, Amarelo: {c['pct_amarelo']:.2%}, Vermelho: {c['pct_vermelho']:.2%}, Total pixels visiveis: {c['cnt_total']}")
        print("-" * 50)

if __name__ == '__main__':
    filtrar()
