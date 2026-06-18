from PIL import Image

def analisar():
    try:
        img = Image.open('resources/imagens/sprites/badniks.png')
        print(f"Resolucoes de badniks.png: {img.size}")
    except Exception as e:
        print(f"Erro ao abrir badniks.png: {e}")

if __name__ == '__main__':
    analisar()
