#ifndef SegmentationUnit_H
#define SegmentationUnit_H

#include "StandardHeaders.h"


typedef struct SegmentationBlocks
{
  Rect B;
  vector<Point> Contours;
  vector<SegmentationBlocks> childs;
  vector<float> FeatureVec;
  int GtClass;
  int PredictedClass;
  bool gtflag;
  bool Fvecflag;
}SB;


vector<SB> GetSegmentationUnit(Mat image);

#endif