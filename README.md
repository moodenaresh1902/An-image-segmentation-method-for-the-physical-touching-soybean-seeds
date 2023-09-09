# An-image-segmentation-method-for-the-physical-touching-soybean-seeds
This algorithm is designed for create individual soybean seed images (227x227 pixels) from the images of the physical touching soybean seeds (3072x2048 pixels).

**03.04.2023**

Note：main.cpp should be recompiled. 

## Requirements

- C/C++
- OpenCV Library(3.4.8)

## Operating environments and dependencies

- Windows ≥10
- Ubuntu 18.04.6

## Content

- create_file.h: creates files that are used to save images.
- create_img: creates the paths that use to save images.
- External_matrix.h: locates those individual soybean seed on the original soybean image and then crops out them from the original soybean image. Finally, it resizes those individual soybean seed images to 227*227 pixels.
- MSRCR.h，MSRCR_Function.h，MSRetinexCR.h : they are three parts of Multi-scale Retinex with Color Restoration (MSRCR). MSRCR is used to enhance the images' contrast and address the effect of uneven illumination.
- Otsu.h: creates a threshold that is used to .
- Read_txt.h: reads the image's name in the path.txt.
- Watershed.h: is used to divide the physical touching seeds on the masking soybean images.

- main.cpp: is the overall flow of the image segmentation algorithm where other modules are called.

 ./src: there are 2 images of the physical touching soybean seeds for testing this algorithm.


## 
path.txt: is used to save images' name. In path.txt, those images would be addressed by this algorithm.

Example path.txt:
Image_20220426214755980
Image_20220430064718171
.
.

