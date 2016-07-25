#ifndef FeatureExtraction_H
#define FeatureExtraction_H

#include "StandardHeaders.h"
#include "SegmentationUnit.h"
#include "GaborAngleFeature.h"
#include "GradientFeature.h"

using namespace IITkgp_functions;

vector<float> ExtractFeature(Mat image);

#endif