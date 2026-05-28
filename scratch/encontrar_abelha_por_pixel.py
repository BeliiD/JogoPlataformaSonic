from PIL import Image

def encontrar():
    img_path = 'resources/imagens/sprites/badniks-base.png'
    img = Image.open(img_path)
    width, height = img.size
    pixels = img.load()
    
    # Vamos varrer a imagem e contar pixels com cores típicas da abelha (Buzz Bomber):
    # R, G, B
    # O azul da abelha clássica do Sonic 1:
    # Geralmente é azul royal ou azul-escuro/roxo, por exemplo: (0, 0, 192) ou similar.
    # O amarelo da cauda é amarelo brilhante: (255, 224, 0) ou (224, 160, 0).
    # As asas são azul-celeste claro ou branco.
    
    # Vamos escanear a imagem em blocos de 40x40 e ver onde temos uma alta densidade de azul escuro e amarelo juntos!
    candidatos = []
    
    # Cores de background verde escuro para ignorar
    def is_bg(p):
        r, g, b = p[0], p[1], p[2]
        if g > r and g > b and g > 30 and r < 120 and b < 120:
            return True
        return False

    for y in range(0, height - 35, 5):
        for x in range(0, width - 45, 5):
            # Analisar o bloco [x, y, x+45, y+35]
            azul_count = 0
            amarelo_count = 0
            vermelho_count = 0
            
            for by in range(35):
                for bx in range(45):
                    p = pixels[x + bx, y + by]
                    if is_bg(p):
                        continue
                    r, g, b = p[0], p[1], p[2]
                    
                    # Azul/Roxo do corpo do Buzz Bomber clássico:
                    # R entre 0 e 100, G entre 0 e 100, B entre 100 e 255.
                    # Mas no Mega Drive, as cores do Buzz Bomber são azul-roxo:
                    # por exemplo, R=0..80, G=0..120, B=140..255.
                    if b > 120 and b > r * 1.2 and b > g * 1.2:
                        azul_count += 1
                        
                    # Amarelo/Laranja brilhante:
                    if r > 180 and g > 130 and b < 100:
                        amarelo_count += 1
                        
                    # Vermelho:
                    if r > 160 and g < 80 and b < 80:
                        vermelho_count += 1
                        
            if azul_count > 40 and amarelo_count > 30:
                candidatos.append((x, y, azul_count, amarelo_count, vermelho_count))
                
    # Agrupar candidatos próximos
    candidatos.sort(key=lambda c: c[2] + c[3], reverse=True)
    print("Regiões com alta concentração de cores do Buzz Bomber (Azul + Amarelo):")
    
    visitados = []
    for x, y, az, am, vm in candidatos:
        # Evitar imprimir blocos sobrepostos
        sobreposto = False
        for vx, vy in visitados:
            if abs(x - vx) < 40 and abs(y - vy) < 30:
                sobreposto = True
                break
        if not sobreposto:
            visitados.append((x, y))
            print(f"Região: x={x}, y={y} | Azul: {az} pixels | Amarelo: {am} pixels | Vermelho: {vm} pixels")
            
if __name__ == '__main__':
    encontrar()
