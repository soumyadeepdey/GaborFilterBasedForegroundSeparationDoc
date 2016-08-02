#ifndef PrepareAlethiaGt_H
#define PrepareAlethiaGt_H

#include "StandardHeaders.h"
#include "AlethiaParser/AlethiaParser.h"
#include "RectangleTest.h"
#include "SegmentationUnit.h"


using namespace IITkgp_functions;



/**
 * vector<ChartRegion> CR = P.GetChartRegion(); // value 1
   vector<TextRegion> TR = P.GetTextRegion(); // value 0
   vector<GraphicRegion> GR = P.GetGraphicRegion(); // value 2
   vector<ImageRegion> IR = P.GetImageRegion(); // value 3
   vector<SeparatorRegion> SR = P.GetSeparatorRegion(); // value 4
 * 
 */
vector<SB> PrepareAlethiaGt(page p, vector<SB> blocks);






#endif