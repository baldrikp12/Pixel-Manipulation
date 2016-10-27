# Pixel-Manipulation

Reads in two .bmp files of same dimensions (file location hardcoded) and assuming image sizes are multiples of eight for simplicity.

Code then creates two new .bmp files:
  * One is a blend of the two read images by averaging their colors on the pixel level.
  * Second is a checkerboard of the two read images, writing sections from each to make the new .bmp image.
