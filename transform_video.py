import numpy as np
import cv2

cap = cv2.VideoCapture(0)
frames_kept = []
is_recording = False

#width = int(Video.get(cv2.CAP_PROP_FRAME_WIDTH))
#height = int(Video.get(cv2.CAP_PROP_FRAME_HEIGHT))

fourcc = cv2.VideoWriter_fourcc(*'mp4v')
out = cv2.VideoWriter("output.mp4", fourcc, 20.0, (640,640), False)
while(cap.isOpened()):
    ret, frame = cap.read()
    if ret == True:
        frame = cv2.resize(frame, (640, 640))
        grey_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        #ret2,th2 = cv2.threshold(grey_frame,255,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU)
        blur = cv2.GaussianBlur(grey_frame,(5,5),0)
        ret3,th3 = cv2.threshold(blur,220,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU)
        #frame = cv2.medianBlur(frame, 5)
       # ret2,g_frame = cv2.threshold(frame,0,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU)

        cv2.imshow("frame", th3)
        if is_recording:
            print("Frame recorded")
            out.write(th3)

        if cv2.waitKey(1) == 32:
            if is_recording:
                print("Stopped recording")
            else:
                print("Recording...")
            is_recording = not is_recording

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break;

out.release()
cap.release()
cv2.destroyAllWindows()

