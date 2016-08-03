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

RNG rng(12345);


using namespace IITkgp_functions;

int main(int argc, char* argv[])
{
  
  if(argc!=3)
  {printf("Wrong input style\n Correct style: ./gabor <traindata.txt> <testdata.txt>"); return -1; }
  
  Training(argv[1]);
  
  vector<char*> ClassifierName;
 ClassifierName.push_back("NBC");
 ClassifierName.push_back("KNN");
 ClassifierName.push_back("SVM");
 ClassifierName.push_back("DT");
 ClassifierName.push_back("RF");
 
  
 for(int i=0;i<ClassifierName.size();i++)
 {
    makedir(ClassifierName[i]);
//     char *tempname, *name;
//     tempname = (char *)malloc(2000*sizeof(char));	 
//     tempname = "Overall_Classification_Result.xls";
//     name = CreateNameIntoFolder(ClassifierName[i],tempname);
//     FILE *res;
//     res = fopen(name,"a+");
//     fclose(res);
    classify(argv[2],ClassifierName[i]);
 }
  
  //system("rm *.yml");
  
  return 0;
  
}