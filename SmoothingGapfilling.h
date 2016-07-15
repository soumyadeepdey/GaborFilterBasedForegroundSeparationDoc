#ifndef SmoothingGapfillinh_H
#define SmoothingGapfillinh_H

#include "StandardHeaders.h"


namespace IITkgp_functions {


    /**
     * @function horizontal_smoothing
     * @param input an image(binary) in Mat format and integer value whitespace that need to be smoothen or filled up
     * @brief with this function from a foreground position , next whitespace number of pixel is filled in horizontal direction
     * @brief it produce smoothed image of the input image in horizontal direction by filling up with foreground with whitespace amount
     * @return Return horizontally smoothed image of input image(binary in nature)
     */

    Mat horizontal_smoothing(Mat image, int whitespace);

    /**
     * @function horizontal_gapfilling
     * @param input an image(binary) in Mat format and integer value whitespace that need to be smoothen or filled up
    * @brief with this function gap btween two foreground pixel is filled only if the gap between two foreground pixel in horizontal direction has a gap less than or equal to whitespace
     * @brief it produce smoothed image of the input image in horizontal direction by filling up with foreground with whitespace amount
     * @return Return gap-filled image of input image(binary in nature)
     */

    Mat horizontal_gapfilling(Mat image, int whitespace);

    /**
     * @function vertical_smoothing
     * @param input an image(binary) in Mat format and integer value whitespace that need to be smoothen or filled up
     * @brief with this function from a foreground position , next whitespace number of pixel is filled
     * @brief it produce smoothed image of the input image in vertical direction by filling up with foreground with whitespace amount
     * @return Return vertically smoothed image of input image(binary in nature)
     */

    Mat vertical_smoothing(Mat image,int whitespace);

    /**
     * @function vertical_gapfilling
     * @param input an image(binary) in Mat format and integer value whitespace that need to be gap filled or filled up
     * @brief with this function gap btween two foreground pixel is filled only if the gap between two foreground pixel in vertival direction has a gap less than or equal to whitespace
     * @brief it produce gap filled image of the input image in vertical direction by filling up with foreground with whitespace amount
     * @return Return vertically gap-filled image of input image(binary in nature)
     */


    Mat vertical_gapfilling(Mat image,int whitespace);



    Mat smoothinggapfilling_GUI(Mat image, int sgtype);

  
}


#endif