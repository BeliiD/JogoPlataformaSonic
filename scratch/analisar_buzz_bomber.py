import os
from PIL import Image

def analisar():
    img_path = 'resources/imagens/sprites/badniks-base.png'
    img = Image.open(img_path)
    
    # Coordenadas suspeitas de ser o Buzz Bomber real:
    # 1. x=28, y=204, w=41, h=29
    # 2. x=82, y=204, w=43, h=29
    # 3. x=248, y=204, w=47, h=29
    # 4. x=80, y=278, w=35, h=29 (este era usado como atirando, mas pode ser outra coisa)
    # 5. Onde estaria o tiro? x=28, y=455, w=16, h=16
    
    coordenadas = [
        ("Voo_1", 28, 204, 41, 29),
        ("Voo_2", 82, 204, 43, 29),
        ("Voo_3", 248, 204, 47, 29),
        ("Atira_1", 80, 278, 35, 29),
        ("Outro_1", 40, 426, 36, 29),  # O camaleão voando
        ("Outro_2", 104, 430, 36, 29), # O camaleão voando 2
    ]
    
    os.makedirs('scratch/inspecao', exist_ok=True)
    
    print("Propriedades das imagens cropadas:")
    for nome, cx, cy, cw, ch in coordenadas:
        crop = img.crop((cx, cy, cx + cw, cy + ch))
        crop.save(f'scratch/inspecao/{nome}_x{cx}_y{cy}.png')
        
        # Obter pixels visíveis (desconsiderando o fundo verde escuro)
        pixels = list(crop.getdata())
        cnt_total = 0
        cnt_azul = 0
        cnt_amarelo = 0
        cnt_vermelho = 0
        
        for p in pixels:
            # Pular fundo verde escuro
            r, g, b = p[0], p[1], p[2]
            if g > r and g > b and g > 30 and r < 120 and b < 120:
                continue
            cnt_total += 1
            
            # Contar cores do Genesis (com mais flexibilidade)
            # Azul: b alto
            if b > 100 and b > r and b > g:
                cnt_azul += 1
            # Amarelo: r e g altos, b baixo
            if r > 150 and g > 130 and b < 120:
                cnt_amarelo += 1
            # Vermelho: r alto, g e b baixos
            if r > 150 and g < 100 and b < 100:
                cnt_vermelho += 1
                
        pct_azul = cnt_azul / cnt_total if cnt_total > 0 else 0
        pct_amarelo = cnt_amarelo / cnt_total if cnt_total > 0 else 0
        pct_vermelho = cnt_vermelho / cnt_total if cnt_total > 0 else 0
        
        print(f"Nome: {nome} (x={cx}, y={cy}, w={cw}, h={ch})")
        print(f"  Total pixels visíveis: {cnt_total}")
        print(f"  Azul: {pct_azul:.1%}, Amarelo: {pct_amarelo:.1%}, Vermelho: {pct_vermelho:.1%}")
        print("-" * 50)

if __name__ == '__main__':
    analisar()
