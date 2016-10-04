#ifndef GaborAngleFeature_H
#define GaborAngleFeature_H



#include "StandardHeaders.h"
#include "folder.h"
#include "StatisticalFunctions.h"
#include "Morphology.h"
#include "Image_proc_functions.h"

using namespace IITkgp_functions;



vector<float>  GetGaborFeature(Mat image);

vector<float>  GetGaborFeatureWithMask(Mat image, Mat MaskedImage);







#endif