import os
from PIL import Image

def analyze_y714():
    img_path = 'scratch/inspecao/candidate_y714.png'
    if not os.path.exists(img_path):
        print(f"Error: {img_path} does not exist!")
        return
        
    img = Image.open(img_path)
    w, h = img.size
    pixels = img.load()
    
    # Let's count white pixels per column
    col_whites = [0] * w
    for x in range(w):
        for y in range(h):
            p = pixels[x, y]
            # White pixel check
            if p[0] > 240 and p[1] > 240 and p[2] > 240:
                col_whites[x] += 1
                
    # Print columns with white pixels
    print("White pixels per column:")
    for x in range(w):
        if col_whites[x] > 0:
            print(f"  Col {x}: {col_whites[x]} pixels")

if __name__ == '__main__':
    analyze_y714()
