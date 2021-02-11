#!/usr/bin/env python3

import random
from PIL import Image
img = Image.open('blendMap2.png')

coordenadas=[]
for i in range(512):
  for j in range(512):
    r, g, b, alpha = img.getpixel((i, j))
    if r:
      num = random.random()
      if num < 0.04:
        num = random.uniform(-1, 1)
        x = i * 200 / 512 - 100 + num
        num = random.uniform(-1, 1)
        z = j * 200 / 512 - 100 + num
        coordenadas.append((x, z))
    if not(r+g+b):
      num = random.random()
      if num < 0.004:
        num = random.uniform(-1, 1)
        x = i * 200 / 512 - 100 + num
        num = random.uniform(-1, 1)
        z = j * 200 / 512 - 100 + num
        coordenadas.append((x, z))    

f= open("coords3.txt","w+")
for i in coordenadas:
  f.write(str(i[0]) + " " + str(i[1]) + "\n" )
f.close()
print(len(coordenadas))