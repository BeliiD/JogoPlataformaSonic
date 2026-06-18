import os
from PIL import Image

def analyze_sheet(name, path):
    if not os.path.exists(path):
        print(f"{name} ({path}) does not exist!")
        return
    img = Image.open(path)
    w, h = img.size
    print(f"\nSheet: {name} ({path}) -> {w}x{h}")
    
    # Let's check colors
    pixels = img.load()
    colors = {}
    for y in range(h):
        for x in range(w):
            p = pixels[x, y]
            colors[p] = colors.get(p, 0) + 1
            
    # List top 5 colors
    sorted_colors = sorted(colors.items(), key=lambda x: x[1], reverse=True)
    print("Top colors:")
    for color, count in sorted_colors[:5]:
        print(f"  {color}: {count} ({count/(w*h):.1%})")

if __name__ == '__main__':
    analyze_sheet('itens.png', 'resources/imagens/itens/itens.png')
    analyze_sheet('shields-base.png', 'resources/imagens/sprites/shields-base.png')
    analyze_sheet('objects-base.png', 'resources/imagens/sprites/objects-base.png')
