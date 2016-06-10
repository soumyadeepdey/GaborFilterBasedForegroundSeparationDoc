import numpy as np
import cv2

img = cv2.imread('pp1.png',1)
height, width, channels = img.shape
print height, width, channels
# Set threshold and maxValue
thresh = 120
maxValue = 255
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY);
# Basic threshold example
th, dst = cv2.threshold(gray, thresh, maxValue, cv2.THRESH_OTSU | cv2.THRESH_BINARY);
cv2.imwrite('pp_bin.png',dst)
