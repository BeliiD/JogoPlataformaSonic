from PIL import Image
import os

img = Image.open("resources/imagens/sprites/objects-base.png")
width, height = img.size

out_dir = "C:/Users/ninja/.gemini/antigravity/brain/93bc52cb-5abc-4480-b0d2-75a065caf794/scratch/objects_base"
os.makedirs(out_dir, exist_ok=True)

# Save the four quadrants
q1 = img.crop((0, 0, width // 2, height // 2))
q1.save(os.path.join(out_dir, "quadrant_1.png"))

q2 = img.crop((width // 2, 0, width, height // 2))
q2.save(os.path.join(out_dir, "quadrant_2.png"))

q3 = img.crop((0, height // 2, width // 2, height))
q3.save(os.path.join(out_dir, "quadrant_3.png"))

q4 = img.crop((width // 2, height // 2, width, height))
q4.save(os.path.join(out_dir, "quadrant_4.png"))

print("Saved quadrants.")
