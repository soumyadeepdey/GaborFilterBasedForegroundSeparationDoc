#ifndef Rectangle_H
#define Rectangle_H


#include "StandardHeaders.h"

typedef struct RectangularDistance
{
  int hdist;
  int vdist;
}RectDist;

namespace IITkgp_functions {

  
    /**
     * @Function: PointRectangleTest
     * @brief : Take 1 rectangle and a Point as input
     * @brief : Test whether the Given Point is inside the Given Rectangle or Inside
     * @return : 	0: Point is Outside of Rectangle
     * 		1: Point is inside the given Rectangle
     * */

    int PointRectangleTest(Rect GivenRect, Point GivenPoint);

    /**
     * @Function: FindOverlappingRectangles
     * @brief : Take 2 rectangle as an input
     * @return : 	0: Rect 1 and Rect 2 are disjoint
     * 		1: Rect 1 is inside Rect 2
     * 	    	2: Rect 2 is inside Rect 1
     * 	    	3: Rect 1 and 2 are partly overlapped
     *
     *
     * */

    int FindOverlappingRectangles(Rect first, Rect second);
    
    
    /**
     * @Function DistanceBetweenRectangle
     * @brief : Take 2 rectangle <r1>,<r2> as an input and Find Horizontal and Vertical Distance between the rectangles
     * @return: Datatype (RectDist):<x> Give the horizontal and vertical distances between two rectangle
     * 				    <x.hdist> = Horizontal distance between <r1> and <r2>
     * 				    <x.vdist> = vertical distance between <r1> and <r2>
     */
    
    RectDist DistanceBetweenRectangle(Rect r1, Rect r2);
    
    /**
    *@function 	JoinVectorRect2SingleRect
    *@brief	Join Vector of rectangle <vector<Rect> R> to <Rect>
    *@return 	A single Rectagle with top left cornor as min from all rect 
    * 		and bottom right as maximum from all Rects
    */
    
    Rect JoinVectorRect2SingleRect(vector<Rect> R);

  
}


#endif