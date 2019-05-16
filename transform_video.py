import numpy as np
import cv2

cap = cv2.VideoCapture("video/video.gif")

while(cap.isOpened()):
    ret, frame = cap.read()
    grey_grame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    ret2,th2 = cv2.threshold(grey_grame,255,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU)
    blur = cv2.GaussianBlur(grey_grame,(5,5),0)
    ret3,th3 = cv2.threshold(blur,220,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU)
    #frame = cv2.medianBlur(frame, 5)
   # ret2,g_frame = cv2.threshold(frame,0,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU)

    cv2.imshow("frame", th3)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
cap.release()
cv2.destroyAllWindows()

