#ifndef Morphology_H
#define Morphology_H

#include "StandardHeaders.h"


namespace IITkgp_functions {


    /**
     * @function erosion
     * @param input an image(binary) in Mat format
     * @brief it erode an image with a square mask of 3x3
     * @return Return eroded image of Mat data-type
     */

    Mat erosion(Mat image);


    /**
     * @function boundaryextraction
     * @param input an image(binary) in Mat format
     * @brief it find the boundary of the input image
     * @return Return boundary of input image(binary in nature)
     */

    Mat boundaryextraction(Mat image);

    /**  @function Erosion
     * @param input
     * element type
     * 0: kernel = Rectangle
     * 1: kernel = CROSS
     * 2: kernel = ELLIPSE
     * @param input erosion Size(n) : Create a kernel or window of 2n+1
     * @param input an image in Mat format(image).
     * @brief it find Eroded Image of the input image with given kernel type and size
     * @return Return Eroded image of input image
     */
    Mat Erosion( int erosion_elem, int erosion_size, Mat image);

    /**  @function Dilation
     * @param input
     * element type
     * 0: kernel = Rectangle
     * 1: kernel = CROSS
     * 2: kernel = ELLIPSE
     * @param input Dilation Size(n) : Create a kernel or window of 2n+1
     * @param input an image in Mat format(image).
     * @brief it find Dilated Image of the input image with given kernel type and size
     * @return Return Dilateded image of input image
     */
    Mat Dilation( int dilation_elem, int dilation_size, Mat image );

    /**  @function Open
     * @param input
     * element type
     * 0: kernel = Rectangle
     * 1: kernel = CROSS
     * 2: kernel = ELLIPSE
     * @param input element Size(n) : Create a kernel or window of 2n+1
     * @param input an image in Mat format(image).
     * @brief it find Open Image of the input image with given kernel type and size
     * @return Return Open image of input image
     */
    Mat Open(int open_elem, int open_size, Mat image);

    /**  @function Close
     * @param input
     * element type
     * 0: kernel = Rectangle
     * 1: kernel = CROSS
     * 2: kernel = ELLIPSE
     * @param input element Size(n) : Create a kernel or window of 2n+1
     * @param input an image in Mat format(image).
     * @brief it find Close Image of the input image with given kernel type and size
     * @return Return Close image of input image(binary in nature)
     */
    Mat Close(int close_elem, int close_size, Mat image);


    /**  @function Morphology_Gui
     * @param input
     *                 Mat image
     *                 int type
     *                  0: Erosion
     *                  1: Dilation
     *                  2: Open
     *                  3: Close
     *
     * @return Return Morphologycally processed image
     */

    Mat Morphology_Gui(Mat image, int type);


}


#endif
