
from skimage import data, img_as_float, color, io, img_as_uint
from skimage.filters import gaussian, threshold_otsu
import numpy as np

img = io.imread("image.jpg")
astro = img_as_float(img)
astro_grey = color.rgb2grey(astro)


thresh = threshold_otsu(astro_grey) + 0.1
binary_astro = astro_grey >= thresh
io.imshow(binary_astro)
io.imsave("output.jpg", img_as_uint(binary_astro))

data = [] 
for (x, y), value in np.ndenumerate(binary_astro):
	if value == 1:
		data.append([y, -x])

m_file = open("output.txt", "wb")

for x,y in data:
	m_file.write(str(x) + "," + str(y))

m_file.close()

