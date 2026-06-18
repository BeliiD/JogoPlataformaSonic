import os
from PIL import Image

def crop_row():
    img_path = 'resources/imagens/sprites/badniks-base.png'
    img = Image.open(img_path)
    # Crop the Crabmeat area: x from 20 to 300, y from 195 to 245
    cropped = img.crop((20, 195, 300, 245))
    os.makedirs('scratch/inspecao', exist_ok=True)
    cropped.save('scratch/inspecao/crabmeat_row.png')
    print("Cropped Crabmeat row saved to scratch/inspecao/crabmeat_row.png")
    
    # Let's print the colors of the first few pixels of each frame in the sheet
    # to understand if we can use a uniform size.
    # Standing frame is x=28, y=204, w=41, h=29
    # Walk frame 1 is x=82, y=204, w=43, h=29
    # Walk frame 2 is x=140, y=200, w=41, h=37
    # Walk frame 3 is x=194, y=200, w=43, h=37
    # Firing frame is x=248, y=204, w=47, h=29

if __name__ == '__main__':
    crop_row()
