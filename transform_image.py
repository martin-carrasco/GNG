import cv2 as cv
import numpy as np
from matplotlib import pyplot as plt
import sys

img = cv.imread(sys.argv[1],0)
img = cv.resize(img, (640, 640))
img = cv.medianBlur(img,5)

ret,th1 = cv.threshold(img,240,255,cv.THRESH_BINARY)
th3 = cv.adaptiveThreshold(img,255,cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY,11,2)

cv.imwrite("output.jpg", th1)
cv.imwrite("output_edges.jpg", th3)

height, width = th1.shape
data = []
for (x,y), value in np.ndenumerate(th1):
    if(value == 0):
        data.append((y, x))
m_file = open("output.txt", "wb")

for x,y in data:
	m_file.write(str(x) + "," + str(y) + "\n")

m_file.close()

height_e, width_e = th3.shape
data_e = []
for (x,y), value in np.ndenumerate(th3):
    if(value == 0):
        data_e.append((y,x))
e_file = open("output_edges.txt", "wb")

for x,y in data_e:
    e_file.write(str(x) + "," + str(y) + "\n")
e_file.close()
