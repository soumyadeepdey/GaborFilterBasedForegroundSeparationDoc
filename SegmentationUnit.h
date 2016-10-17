#ifndef SegmentationUnit_H
#define SegmentationUnit_H

#include "StandardHeaders.h"


typedef struct SegmentationBlocks
{
  Rect B;
  vector<Point> Contours;
  vector<SegmentationBlocks> childs;
  vector<float> FeatureVec;
  int blockid;
  int GtClass;
  int PredictedClass;
  int clusternumber;
  bool clusterflag;
  bool gtflag;
  bool Fvecflag;
  bool PredFlag;
  char *bgcolor;
  char *type;
}SB;


vector<SB> GetSegmentationUnit(Mat image);

vector<SB> GetProcessingBlocks(Mat image);

#endif