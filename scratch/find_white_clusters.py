import os
from PIL import Image

def find_white_clusters():
    img_path = 'resources/imagens/sprites/shields-base.png'
    if not os.path.exists(img_path):
        print(f"Error: {img_path} does not exist!")
        return
        
    img = Image.open(img_path)
    width, height = img.size
    
    # We want to scan the image in a grid or find regions where there is a high concentration of white pixels
    # Let's count white pixels in blocks of 48x48
    pixels = img.load()
    
    def is_white(p):
        r, g, b = p[0], p[1], p[2]
        return r > 240 and g > 240 and b > 240

    # Let's search for regions of size 48x48 that have at least 50 white pixels
    step = 8
    block_size = 48
    blocks = []
    
    for y in range(0, height - block_size, step):
        for x in range(0, width - block_size, step):
            white_count = 0
            for by in range(block_size):
                for bx in range(block_size):
                    if is_white(pixels[x + bx, y + by]):
                        white_count += 1
            if white_count > 30:
                blocks.append((x, y, white_count))
                
    # Merge overlapping blocks to find unique regions
    blocks.sort(key=lambda b: b[2], reverse=True)
    unique_regions = []
    
    for x, y, count in blocks:
        # Check if this overlaps with any existing region
        overlap = False
        for rx, ry, rc in unique_regions:
            if abs(x - rx) < 30 and abs(y - ry) < 30:
                overlap = True
                break
        if not overlap:
            unique_regions.append((x, y, count))
            
    print(f"Found {len(unique_regions)} unique white regions:")
    unique_regions.sort(key=lambda r: (r[1], r[0]))
    for idx, (x, y, count) in enumerate(unique_regions):
        print(f"Region {idx}: x={x}, y={y} (white count: {count})")

if __name__ == '__main__':
    find_white_clusters()
