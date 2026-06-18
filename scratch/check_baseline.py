from PIL import Image

def analyze_baseline():
    img = Image.open('resources/imagens/sprites/badniks-base.png')
    
    frames = [
        ("Standing", 28, 204, 41, 29),
        ("Walk 1", 82, 204, 43, 29),
        ("Walk 2", 140, 200, 41, 37),
        ("Walk 3", 194, 200, 43, 37),
        ("Firing", 248, 204, 47, 29)
    ]
    
    def is_bg(p):
        r, g, b = p[0], p[1], p[2]
        if g > r and g > b and g > 30 and r < 120 and b < 120:
            return True
        return False

    for name, fx, fy, fw, fh in frames:
        crop = img.crop((fx, fy, fx + fw, fy + fh))
        pixels = crop.load()
        
        # Find the bottom-most non-background pixel row
        bottom_y = -1
        top_y = -1
        left_x = -1
        right_x = -1
        
        for y in range(fh):
            for x in range(fw):
                p = pixels[x, y]
                if not is_bg(p):
                    if top_y == -1:
                        top_y = y
                    bottom_y = y
                    if left_x == -1 or x < left_x:
                        left_x = x
                    if right_x == -1 or x > right_x:
                        right_x = x
                        
        print(f"{name}:")
        print(f"  Defined Rect: x={fx}, y={fy}, w={fw}, h={fh}")
        print(f"  Active Pixels Bounds: x_min={left_x}, y_min={top_y}, x_max={right_x}, y_max={bottom_y}")
        print(f"  Absolute Bottom Y in spritesheet: {fy + bottom_y}")
        print(f"  Absolute Top Y in spritesheet: {fy + top_y}")

if __name__ == '__main__':
    analyze_baseline()
