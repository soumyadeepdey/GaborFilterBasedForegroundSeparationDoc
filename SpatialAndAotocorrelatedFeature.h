#ifndef SpatialAndAotocorrelatedFeature_H
#define SpatialAndAotocorrelatedFeature_H


#include "StandardHeaders.h"
#include "StatisticalFunctions.h"
#include "PixelValidation.h"
#include "Image_proc_functions.h"





#include "alglib/alglibmisc.h"
#include "alglib/ap.h"
#include "alglib/dataanalysis.h"
#include "alglib/diffequations.h"
#include "alglib/fasttransforms.h"
#include "alglib/integration.h"
#include "alglib/interpolation.h"
#include "alglib/linalg.h"
#include "alglib/optimization.h"
#include "alglib/solvers.h"
#include "alglib/specialfunctions.h"
#include "alglib/statistics.h"
#include "alglib/stdafx.h"

using namespace alglib;


typedef struct SpatialRunLength
{
  Point start;
  Point end;
  int pos;
  int leng;
  int proj;
}SRL;


class Pass
{
public:
  Point start;
  Point end;
  vector<SRL> RunLengths;
  int PassProjection;
  int weight;
   // type = 0,1,2,3: 0: horizontal; 1: Vertical; 2: Left-Diagonal; 3: Right-Diagonal;
  Pass(int t);
  int getweight();
  void setpassprojection();
  void setpassvalues(int i);
  void setPassRunLengths();
  void setpasspoints(Point x);
  ~Pass();
  
private:
  int type;
  void setweight();
  vector<int> passvalues;
  vector<Point> passpoints;
  
  
};



typedef struct SpatialFeature
{
  Stat h;
  Stat v;
  Stat l;
  Stat r;
}SF;

typedef struct SpatialFeatureLine
{
  int proj;
  int FgRun;
  double rlmean;
  double spmean;
}SFL;

typedef struct InitialAutocorelationFeatures
{
  vector<SFL> h;
  vector<SFL> v;
  vector<SFL> l;
  vector<SFL> r;
}IAC;

typedef struct SingleAutoCorrelationFeature
{
  int posi;
  double slope;
}SAF;

typedef struct SAFDirection
{
  SAF proj;
  SAF fgrun;
  SAF rlmean;
  SAF spmean;
}SAFD;

typedef struct AutocorrelatedFeature
{
  SAFD h;
  SAFD v;
  SAFD l;
  SAFD r;
}AF;

/**
 * @func: HPass 
 * @def: This function compute all passes in an image in horizontal direction
 * @input: This function takes a binary or gray scale image (image)as an input
 * @return: This funtion return a vector of integer values along this pass
 */
vector< Pass > HPass(Mat image);

/**
 * @func: VPass 
 * @def: ThThis function compute all passes in an image in Vertical direction
 * @input: This function takes a binary or gray scale image (image)as an input.
 * @return: This funtion return a vector of integer values along this pass
 */
vector< Pass > VPass(Mat image);

/**
 * @func: LPass 
 * @def: This function compute all passes in an image in Left-diagonal direction
 * @input: This function takes a binary or gray scale image (image)as an input
 * @return: This funtion return a vector of integer values along this pass
 */
vector< Pass > LPass(Mat image);

/**
 * @func: RPass 
 * @def: This function compute all passes in an image in Right-diagonal direction
 * @input: This function takes a binary or gray scale image (image)as an input
 * @return: This funtion return a vector of integer values along this pass
 */
vector< Pass > RPass(Mat image);

/**
 * @func: ComputeSpatialFeatures
 * @input: This fu=nction takes an binary or gray scale image as an input.
 * @def: Compute the spatial features of the input image
 * @return: This function returns the computed spatial features of the given image
 */
SF ComputeSpatialFeatures(Mat Image);


IAC ComputeInitialAutocorelationFeatures(Mat Image);

Stat ComputeSpatialFeatureInaDirection(vector<Pass> P,int n);

SAF ComputeSingleAutoCorrelationFeature(real_1d_array &x);

AF ComputeAutoCoRelatedFeatures(Mat Image);

void SFtoFVector(SF x, vector<float> &y);

void AFtoFVector(AF x, vector<float> &y);

#endif