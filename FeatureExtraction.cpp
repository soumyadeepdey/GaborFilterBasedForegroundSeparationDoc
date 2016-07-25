#include "FeatureExtraction.h"


vector< float > ExtractFeature(Mat image)
{
  vector<float> Feature;
  
  //Extract Gradient Feature
  vector<float> GradF = GetGradientFeature(image);
  for(int i=0;i<GradF.size();i++)
    Feature.push_back(GradF[i]);
  //copy<float>(GradF.begin(),GradF.end(),inserter(Feature,Feature.end())); // Append Gradient feature into Feature vector
  GradF.clear();
  
  // Extract Gabor Feature
  vector<float> GaborF = GetGaborFeature(image);
  for(int i=0;i<GaborF.size();i++)
    Feature.push_back(GaborF[i]);
  //copy<float>(GaborF.begin(),GaborF.end(),inserter(Feature,Feature.end())); // Append Gabor feature into Feature vector
  GaborF.clear();
  
  
  
  return Feature;
}
