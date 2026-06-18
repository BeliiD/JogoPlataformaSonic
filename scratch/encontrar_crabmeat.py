import os
from PIL import Image

def filtrar_crabmeat():
    crop_dir = 'scratch/crops_detectados'
    if not os.path.exists(crop_dir):
        print(f"Diretório {crop_dir} não existe. Vamos rodar o fatiar.py primeiro.")
        import sys
        sys.path.append('scratch')
        from fatiar import fatiar
        fatiar()
        
    files = os.listdir(crop_dir)
    candidatos = []
    
    for f in files:
        if not f.endswith('.png'):
            continue
            
        filepath = os.path.join(crop_dir, f)
        img = Image.open(filepath)
        w, h = img.size
        
        # Dimensões típicas do Crabmeat: largura entre 30 e 70, altura entre 25 e 60
        if not (20 <= w <= 80 and 20 <= h <= 60):
            continue
            
        pixels = list(img.getdata())
        
        cnt_vermelho = 0
        cnt_total_visivel = 0
        
        for p in pixels:
            # Pular pixels de background green
            r, g, b = p[0], p[1], p[2]
            
            # background
            if g > r and g > b and g > 30 and r < 120 and b < 120:
                continue
                
            cnt_total_visivel += 1
            
            # Vermelho: r alto, g e b relativamente baixos
            if r > 140 and g < 110 and b < 110:
                cnt_vermelho += 1
                
        if cnt_total_visivel > 40:
            pct_vermelho = cnt_vermelho / cnt_total_visivel
            
            # Se tiver mais de 20% de vermelho, é um forte candidato a Crabmeat
            if pct_vermelho > 0.15:
                parts = f.replace('.png', '').split('_')
                # extrair x, y, w, h
                x = -1
                y = -1
                for part in parts:
                    if part.startswith('x'):
                        x = int(part[1:])
                    elif part.startswith('y'):
                        y = int(part[1:])
                    elif part.startswith('w'):
                        w = int(part[1:])
                    elif part.startswith('h'):
                        h = int(part[1:])
                
                candidatos.append({
                    'file': f,
                    'x': x,
                    'y': y,
                    'w': w,
                    'h': h,
                    'pct_vermelho': pct_vermelho,
                    'cnt_total': cnt_total_visivel
                })
                
    # Ordenar por percentual de vermelho
    candidatos.sort(key=lambda c: c['pct_vermelho'], reverse=True)
    
    print(f"Encontrados {len(candidatos)} candidatos a Crabmeat:")
    for idx, c in enumerate(candidatos):
        print(f"{idx}: {c['file']} -> x={c['x']}, y={c['y']}, w={c['w']}, h={c['h']} (Vermelho: {c['pct_vermelho']:.2%})")

if __name__ == '__main__':
    filtrar_crabmeat()
