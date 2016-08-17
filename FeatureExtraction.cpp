#include "FeatureExtraction.h"

vector< float > ExtractFeature(Mat clr_image)
{
  
  
  vector<float> Feature;
  
  //Extract Gradient Feature
  //vector<float> GradF = GetGradientFeature(clr_image);
 // for(int i=0;i<GradF.size();i++)
  //  Feature.push_back(GradF[i]);
  //copy<float>(GradF.begin(),GradF.end(),inserter(Feature,Feature.end())); // Append Gradient feature into Feature vector
  //GradF.clear();
  
  // Extract Gabor Feature
  vector<float> GaborF = GetGaborFeature(clr_image);
  for(int i=0;i<GaborF.size();i++)
    Feature.push_back(GaborF[i]);
  //copy<float>(GaborF.begin(),GaborF.end(),inserter(Feature,Feature.end())); // Append Gabor feature into Feature vector
  GaborF.clear();
  
  
  
  printf("All Features\n");
  
  for(int i=0;i<Feature.size();i++)
    printf("%f\t",Feature[i]);
  printf("\n\n");
  
  return Feature;
}
