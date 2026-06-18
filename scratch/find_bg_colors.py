from PIL import Image

img = Image.open("resources/imagens/itens/objetos-base-s1.png")
width, height = img.size

# Let's check the pixel at (0, 0)
print("Pixel at (0, 0):", img.getpixel((0, 0)))

# Let's count the frequency of colors in the first row (which is usually background)
colors = {}
for x in range(width):
    p = img.getpixel((x, 0))
    colors[p] = colors.get(p, 0) + 1

print("Colors in top row:")
for c, count in sorted(colors.items(), key=lambda x: x[1], reverse=True):
    print(f"  Color {c}: count={count}")
