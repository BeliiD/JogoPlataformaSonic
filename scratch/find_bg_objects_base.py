from PIL import Image

img = Image.open("resources/imagens/sprites/objects-base.png")
width, height = img.size
print(f"Image mode: {img.mode}")
print(f"Pixel at (0,0): {img.getpixel((0,0))}")

# Check colors in the first row
colors = {}
for x in range(width):
    p = img.getpixel((x, 0))
    colors[p] = colors.get(p, 0) + 1

print("Top row colors:")
for c, count in sorted(colors.items(), key=lambda x: x[1], reverse=True)[:5]:
    print(f"  {c}: count={count}")
