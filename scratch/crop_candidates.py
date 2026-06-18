import os
from PIL import Image

def crop_candidates():
    img_path = 'resources/imagens/sprites/shields-base.png'
    img = Image.open(img_path)
    
    os.makedirs('scratch/inspecao', exist_ok=True)
    
    # Crop Shield candidates at y = 472
    # Frame 1: 24, 472, 47, 47
    # Frame 2: 86, 472, 41, 47
    # Frame 3: 136, 472, 47, 47 (approx)
    # Let's crop a single image containing all 3 frames: x from 10 to 200, y from 465 to 525
    img.crop((10, 465, 200, 525)).save('scratch/inspecao/shield_candidate_y472.png')
    print("Saved shield candidate at y=472")
    
    # Crop candidate at y = 714
    # x from 40 to 260, y from 705 to 765
    img.crop((40, 705, 260, 765)).save('scratch/inspecao/candidate_y714.png')
    print("Saved candidate at y=714")
    
    # Let's search if there are other rows. Let's crop x from 0 to 400 for different y levels
    # Let's check y = 820 to 880
    img.crop((10, 815, 360, 885)).save('scratch/inspecao/candidate_y820.png')
    print("Saved candidate at y=820")
    
    # Let's also look at the very top of shields-base.png (y = 0 to 100)
    img.crop((10, 0, 300, 100)).save('scratch/inspecao/candidate_top.png')
    print("Saved candidate at top")

if __name__ == '__main__':
    crop_candidates()
