
#include "RectangleTest.h"

/*------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------*/


/**
 * @Function: PointRectangleTest
 * @brief : Take 1 rectangle and a Point as input
 * @brief : Test whether the Given Point is inside the Given Rectangle or Inside
 * @return : 	0: Point is Outside of Rectangle
 * 		1: Point is inside the given Rectangle
 * */

int IITkgp_functions::PointRectangleTest(Rect GivenRect, Point GivenPoint)
{
  Point tl,br;
  tl = GivenRect.tl();
  br = GivenRect.br();
  int flag;
  /*
  if((GivenPoint.x>=tl.x && GivenPoint.x<=br.x) && (GivenPoint.y<=tl.y && GivenPoint.y>=br.y))
  {
    flag = 1;
    printf("point inside\n");
    return(flag);
  }
  */
  if((GivenPoint.x>=tl.x && GivenPoint.x<=br.x) && (GivenPoint.y>=tl.y && GivenPoint.y<=br.y))
  {
    flag = 1;
    //printf("point inside\n");
    return(flag);
  }
  else
  {
    flag = 0;
    return(flag);
  }
}

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

int IITkgp_functions::FindOverlappingRectangles(Rect first, Rect second)
{

 Point tl1,tl2,br1,br2;

 int flag;

 tl1 = first.tl();
 tl2 = second.tl();
 br1 = first.br();
 br2 = second.br();
 
 

 if(PointRectangleTest(first,tl2) == 0 && PointRectangleTest(first,br2) == 0)
 {
   flag = 0;
   //return (flag);
 }

 if(PointRectangleTest(first,tl2) == 1 || PointRectangleTest(first,br2) ==1 || PointRectangleTest(second,tl1) == 1 || PointRectangleTest(second,br1) == 1)
 {
   flag = 3;
   //return (flag);
 }
 if(PointRectangleTest(first,tl2) == 1 && PointRectangleTest(first,br2) == 1)
 {
   flag = 2;
   //return (flag);
 }
 if(PointRectangleTest(second,tl1) == 1 && PointRectangleTest(second,br1) == 1)
 {
   flag = 1;
   //return (flag);
 }
 return (flag);

}

/*-------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------*/


    /**
     * @Function DistanceBetweenRectangle
     * @brief : Take 2 rectangle <r1>,<r2> as an input and Find Horizontal and Vertical Distance between the rectangles
     * @return: Datatype (RectDist):<x> Give the horizontal and vertical distances between two rectangle
     * 				    <x.hdist> = Horizontal distance between <r1> and <r2>
     * 				    <x.vdist> = vertical distance between <r1> and <r2>
     */

RectDist IITkgp_functions::DistanceBetweenRectangle(Rect r1, Rect r2)
{
  RectDist RDist;
  int type = FindOverlappingRectangles(r1,r2);
  if(type == 0)// disjoint Rectangle
  {
    Point2i br1 = r1.br();
    Point2i tl1 = r1.tl();
    Point2i br2 = r2.br();
    Point2i tl2 = r2.tl();
    if(tl1.x < tl2.x) // r1 is in left of r2
    {
      RDist.hdist = tl2.x - br1.x;
    }
    else
    {
      RDist.hdist = tl1.x - br2.x;
    }
    if(tl1.y < tl2.y)// r1 is at top of r2
    {
      RDist.vdist = tl2.y - br1.y;
    }
    else
    {
      RDist.vdist = tl1.y - br2.y;
    }
  }
  else // if not disjoint
  {
   RDist.hdist = 0;
   RDist.vdist = 0;
  }
  return(RDist);
}


/*-------------------------------------------------------------------------------------------------------------------------*/



/**
    *@function 	JoinVectorRect2SingleRect
    *@brief	Join Vector of rectangle <vector<Rect> R> to <Rect>
    *@return 	A single Rectagle with top left cornor as min from all rect 
    * 		and bottom right as maximum from all Rects
    */
    
    Rect IITkgp_functions::JoinVectorRect2SingleRect(vector<Rect> R)
    {
      Rect OutRect;
      
      int tlx,tly,height,width,brx,bry,maxbrx,maxbry;
      for(int i=0;i<R.size();i++)
      {
	Rect temp = R[i];
	 tlx = temp.x;
	 tly = temp.y;
	 height = temp.height;
	 width = temp.width;
	 brx= tlx + width;
	 bry = tly + height;
	 if(i == 0)
	 {
	   OutRect.x = tlx;
	   OutRect.y = tly;
	   maxbrx = brx;
	   maxbry = bry;
	 }
	 if(OutRect.x > tlx)
	 {
	   OutRect.x = tlx;
	 }
	 if(OutRect.y > tly)
	 {
	   OutRect.y = tly;
	 }
	 if(maxbrx < brx)
	 {
	   maxbrx = brx;
	 }
	 if(maxbry < bry)
	 {
	   maxbry = bry;
	 }
      }
      OutRect.height = maxbry - OutRect.y;
      OutRect.width = maxbrx - OutRect.x;
      
      return(OutRect);
    }
    
    

/*-------------------------------------------------------------------------------------------------------------------------*/


int IITkgp_functions::PolygonInsidePolygonTest(vector<Point> Poly1, vector<Point> Poly2)
{
  double sum = 0.0;
  for(int i=0;i<Poly2.size();i++)
  {
    bool measuredist = false;
    double dist = pointPolygonTest(Poly1,Poly2[i],measuredist);
    if (dist == 0)
    {
      return 3;
    }
    sum = sum + dist;
  }
  if(sum == Poly2.size())
    return 2;
  if(sum == Poly2.size()*-1)
    return 1;
}

