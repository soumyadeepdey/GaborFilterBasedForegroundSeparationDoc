#ifndef PrepareAlethiaGt_H
#define PrepareAlethiaGt_H

#include "StandardHeaders.h"
#include "AlethiaParser/AlethiaParser.h"
#include "RectangleTest.h"


using namespace IITkgp_functions;


typedef struct SegmentationBlocks
{
  Rect B;
  vector<Point> Contours;
  vector<SegmentationBlocks> childs;
  //vector<Rect> child;
  //vector<vector<Point> > ChildContour;
  int GtClass;
  int PredictedClass;
  bool gtflag;
}SB;



vector<SB> PrepareAlethiaGt(page p, vector<SB> blocks);






#endif