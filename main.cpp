#include "StandardHeaders.h"
#include "GaborAngleFeature.h"
#include "folder.h"
#include "ColorQuantization.h"
#include "SmoothingGapfilling.h"
#include "Image_proc_functions.h"
#include "binarization.h"
#include "RectangleTest.h"
#include "connectedcomponent.h"
#include "AlethiaParser/AlethiaParser.h"
#include "PrepareAlethiaGT.h"
#include "SegmentationUnit.h"
#include "FeatureExtraction.h"
#include "classification.h"
#include "ClusteringClassification.h"

RNG rng(12345);


using namespace IITkgp_functions;

int main(int argc, char* argv[])
{
  
  if(argc!=3)
  {printf("Wrong input style\n Correct style: ./gabor <traindata.txt> <testdata.txt>"); return -1; }
  
  TDC Tdata = Training(argv[1]);
  
  for(int i=0;i<Tdata.TrainData.rows;i++)
  {
    for(int j=0;j<Tdata.TrainData.cols;j++)
    {
      printf("%f\t",Tdata.TrainData.at<float>(i,j));
    }
    printf("\n\n");
  }
  
  //exit(0);
  
  vector<char*> ClassifierName;
 
    ClassifierName.push_back("RF");
    ClassifierName.push_back("SVM");
    ClassifierName.push_back("KNN");
    ClassifierName.push_back("DT");
    
  vector<char *> ClusteringName;
  
  ClusteringName.push_back("CCCN");
  //ClusteringName.push_back("CCE");
 
  
 for(int j=0;j<ClusteringName.size();j++)
 {
  for(int i=0;i<ClassifierName.size();i++)
  {
      makedir(ClassifierName[i]);
      printf("ClassifierName = %s\n",ClassifierName[i]);
      classify(argv[2],ClassifierName[i], ClusteringName[j], Tdata);
  }
 }
  
  //system("rm *.yml");
  
  return 0;
  
}