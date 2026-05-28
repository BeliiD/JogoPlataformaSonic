from PIL import Image
import os

def confirmar():
    img_path = 'resources/imagens/sprites/badniks-base.png'
    img = Image.open(img_path)
    
    coordenadas = [
        ("Voo_1", 24, 358, 45, 19),
        ("Voo_2", 80, 358, 45, 19),
        ("Atira_1", 24, 390, 45, 19),
        ("Atira_2", 80, 390, 45, 19),
        ("Projetil", 138, 350, 11, 11),
    ]
    
    os.makedirs('scratch/buzz_confirmado', exist_ok=True)
    
    # Criar um mosaico com todos os sprites lado a lado com fundo cinza escuro
    mosaico = Image.new('RGB', (300, 100), (40, 40, 40))
    x_offset = 10
    
    print("Análise dos novos sprites do Buzz Bomber:")
    for nome, x, y, w, h in coordenadas:
        crop = img.crop((x, y, x + w, y + h))
        crop.save(f'scratch/buzz_confirmado/{nome}_x{x}_y{y}_w{w}_h{h}.png')
        
        # Colar no mosaico
        # Redimensionar para ficar visível (escala 2x no mosaico)
        scaled_crop = crop.resize((w * 2, h * 2), Image.Resampling.NEAREST)
        mosaico.paste(scaled_crop, (x_offset, 50 - h))
        x_offset += w * 2 + 10
        
        # Contagem de cores para verificação
        pixels = list(crop.getdata())
        cnt_azul = 0
        cnt_amarelo = 0
        cnt_vermelho = 0
        for p in pixels:
            # Pular fundo verde escuro
            r, g, b = p[0], p[1], p[2]
            if g > r and g > b and g > 30 and r < 120 and b < 120:
                continue
            if b > 100 and b > r and b > g:
                cnt_azul += 1
            if r > 160 and g > 130 and b < 100:
                cnt_amarelo += 1
            if r > 160 and g < 80 and b < 80:
                cnt_vermelho += 1
                
        print(f"{nome} (x={x}, y={y}, w={w}, h={h}):")
        print(f"  Pixels Azuis: {cnt_azul} | Pixels Amarelos: {cnt_amarelo} | Pixels Vermelhos: {cnt_vermelho}")
        
    mosaico.save('scratch/buzz_mosaico_real.png')
    print("Mosaico salvo em scratch/buzz_mosaico_real.png")

if __name__ == '__main__':
    confirmar()
