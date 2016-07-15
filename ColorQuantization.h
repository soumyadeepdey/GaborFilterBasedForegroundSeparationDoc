#ifndef ColorQuantization_H
#define ColorQuantization_H



#include "StandardHeaders.h"



/**
 * @func: ColorQuantize
 * @param: iamge: input image whose color need to be ColorQuantize, n = number of color 
 * @output: An image with quantized number of color
 * 
 * This function uses k-means clustering algorithm to quantize color to n number of colors
 */
Mat ColorQuantize(Mat image, int n);



#endif