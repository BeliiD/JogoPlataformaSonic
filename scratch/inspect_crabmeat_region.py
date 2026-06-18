import os
from PIL import Image

def inspect():
    img_path = 'resources/imagens/sprites/badniks-base.png'
    img = Image.open(img_path)
    width, height = img.size
    
    # We want to find all bounding boxes that lie in y=[180, 260] and x=[0, 400]
    # Let's search the crops_detectados directory files for ones with x < 400 and y between 180 and 260
    crop_dir = 'scratch/crops_detectados'
    files = os.listdir(crop_dir)
    
    nearby = []
    for f in files:
        if not f.endswith('.png'):
            continue
        parts = f.replace('.png', '').split('_')
        idx = int(parts[1])
        x = int([p for p in parts if p.startswith('x')][0][1:])
        y = int([p for p in parts if p.startswith('y')][0][1:])
        w = int([p for p in parts if p.startswith('w')][0][1:])
        h = int([p for p in parts if p.startswith('h')][0][1:])
        
        if 180 <= y <= 260 and x <= 450:
            nearby.append((idx, x, y, w, h, f))
            
    nearby.sort(key=lambda item: (item[2], item[1]))
    print("Detected bounding boxes in the Crabmeat region:")
    for item in nearby:
        print(f"Index {item[0]}: x={item[1]}, y={item[2]}, w={item[3]}, h={item[4]} (file: {item[5]})")

if __name__ == '__main__':
    inspect()
