#ifndef PrepareAlethiaGt_H
#define PrepareAlethiaGt_H

#include "StandardHeaders.h"
#include "AlethiaParser/AlethiaParser.h"
#include "RectangleTest.h"
#include "SegmentationUnit.h"


using namespace IITkgp_functions;



/**
 *  vector<ChartRegion> CR = P.GetChartRegion(); // value 1
   vector<TextRegion> TR = P.GetTextRegion(); // value 0
   vector<GraphicRegion> GR = P.GetGraphicRegion(); // value 2
   vector<ImageRegion> IR = P.GetImageRegion(); // value 3
   vector<SeparatorRegion> SR = P.GetSeparatorRegion(); // value 4
   vector<NoiseRegion> NR = P.GetNoiseRegion(); // value 5
   vector<MathsRegion> MR = P.GetMathsRegion(); // value 6
   vector<TableRegion> TabR = P.GetTableRegion(); // value 7
 * 
 */
vector<SB> PrepareAlethiaGt(page p, vector<SB> blocks);


/**
 *  vector<ChartRegion> CR = P.GetChartRegion(); // value 1
   vector<TextRegion> TR = P.GetTextRegion(); // value 0
   vector<GraphicRegion> GR = P.GetGraphicRegion(); // value 2
   vector<ImageRegion> IR = P.GetImageRegion(); // value 3
   vector<SeparatorRegion> SR = P.GetSeparatorRegion(); // value 4
   vector<NoiseRegion> NR = P.GetNoiseRegion(); // value 5
   vector<MathsRegion> MR = P.GetMathsRegion(); // value 6
   vector<TableRegion> TabR = P.GetTableRegion(); // value 7
 * 
 // text 0 (text math table) 
// graphics 1 (image, graphics, chart)
// noise 5 (noise)
// separator 4 (separator)

 */
vector< SB > PrepareAlethiaGt_tgns(page P, vector< SB > blocks);



/**
 *  vector<ChartRegion> CR = P.GetChartRegion(); // value 1
   vector<TextRegion> TR = P.GetTextRegion(); // value 0
   vector<GraphicRegion> GR = P.GetGraphicRegion(); // value 2
   vector<ImageRegion> IR = P.GetImageRegion(); // value 3
   vector<SeparatorRegion> SR = P.GetSeparatorRegion(); // value 4
   vector<NoiseRegion> NR = P.GetNoiseRegion(); // value 5
   vector<MathsRegion> MR = P.GetMathsRegion(); // value 6
   vector<TableRegion> TabR = P.GetTableRegion(); // value 7
 * 
 // text 0 (text) 
// graphics 1 (image)
// noise 5 (noise)
// separator 4 (separator)
 */
vector< SB > PrepareAlethiaGt_tgns_ignore(page P, vector< SB > blocks);


/**
 *  vector<ChartRegion> CR = P.GetChartRegion(); // value 1
   vector<TextRegion> TR = P.GetTextRegion(); // value 0
   vector<GraphicRegion> GR = P.GetGraphicRegion(); // value 2
   vector<ImageRegion> IR = P.GetImageRegion(); // value 3
   vector<SeparatorRegion> SR = P.GetSeparatorRegion(); // value 4
   vector<NoiseRegion> NR = P.GetNoiseRegion(); // value 5
   vector<MathsRegion> MR = P.GetMathsRegion(); // value 6
   vector<TableRegion> TabR = P.GetTableRegion(); // value 7
 * 
 // text 0 (text=text,math,table) 
// graphics 1 (graphics=(graphics,chart,image))
// noise 5 (noise)
// separator 4 (separator)
 */
vector< SB > PrepareAlethiaGt_tg(page P, vector< SB > blocks);


/**
 *  vector<ChartRegion> CR = P.GetChartRegion(); // value 1
   vector<TextRegion> TR = P.GetTextRegion(); // value 0
   vector<GraphicRegion> GR = P.GetGraphicRegion(); // value 2
   vector<ImageRegion> IR = P.GetImageRegion(); // value 3
   vector<SeparatorRegion> SR = P.GetSeparatorRegion(); // value 4
   vector<NoiseRegion> NR = P.GetNoiseRegion(); // value 5
   vector<MathsRegion> MR = P.GetMathsRegion(); // value 6
   vector<TableRegion> TabR = P.GetTableRegion(); // value 7
 * 
 // text 0 (text=text) 
// graphics 1 (graphics=(image))
// noise 5 (noise)
// separator 4 (separator)
 */
vector< SB > PrepareAlethiaGt_tg_ignore(page P, vector< SB > blocks);

#endif