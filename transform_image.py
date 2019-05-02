
from skimage import data, img_as_float, color, io, img_as_uint
from skimage.filters import gaussian, threshold_otsu
from skimage.transform import resize
import numpy as np
import sys

img = io.imread(sys.argv[1])
resized_image = resize(img, (640, 640))
rescaled_image = 255 * resized_image
final_image = rescaled_image.astype(np.uint8)

astro = img_as_float(final_image)
astro_grey = color.rgb2grey(astro)


thresh = threshold_otsu(astro_grey) + 0.1
binary_astro = astro_grey >= thresh
io.imshow(binary_astro)
io.imsave("output.jpg", img_as_uint(binary_astro))

data = []
for (x, y), value in np.ndenumerate(binary_astro):
	if value == 1:
		data.append([x, -y])

m_file = open("output.txt", "wb")

for x,y in data:
	m_file.write(str(x) + "," + str(y) + "\n")

m_file.close()

