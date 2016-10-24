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
  
  if(argc!=6)
  {printf("Wrong input style\n Correct style: ./gabor <traindata.txt> <testdata.txt> <NoForKFold> <type={0,1,2,3,4}> <classi_type={0,1,2,3,4}>"); return -1; }
  
  int type = atoi(argv[4]);
  int classi_type = atoi(argv[5]);
  
  char *dirname;
  
  if(type == 0)
  {
    classifyAll = true;
   
    dirname = (char *)malloc(2001*sizeof(char));
    strcpy(dirname,"ClassifyAll");
  }
  else if(type == 1)
  {
    classifySelected = true;
    
    dirname = (char *)malloc(2001*sizeof(char));
    strcpy(dirname,"ClassifySelected");
  }
  else if(type == 2)
  {
    classifySelected_ignore = true;

    dirname = (char *)malloc(2001*sizeof(char));
    strcpy(dirname,"ClassifySelectedIgnore");
  }
  else if(type == 3)
  {
   classifyTG = true;
    
    dirname = (char *)malloc(2001*sizeof(char));
    strcpy(dirname,"ClassifyTG");
  }
  else if(type == 4)
  {
   classifyTG_ignore = true;
    
    dirname = (char *)malloc(2001*sizeof(char));
    strcpy(dirname,"ClassifyTGIgnore");
  }
  else
  {   
    printf("Wrong Choise!!!\nValue must be 0,1,2,3,4\n");
    exit(0);
  }
  
  
  makedir(dirname);
  
  char *ClassifierName;
  if(classi_type==0)
  {
    ClassifierName = (char *)malloc(2001*sizeof(char));
    strcpy(ClassifierName,"RF");
    TDC Tdata = Training(argv[1]);
    printf("ClassifierName = %s\n",ClassifierName);
    classify(argv[2],ClassifierName, argv[3],"CCCN", Tdata);
    
  }
  else if(classi_type==1)
  {
    ClassifierName = (char *)malloc(2001*sizeof(char));
    strcpy(ClassifierName,"KNN");
    TDC Tdata = Training(argv[1]);
    printf("ClassifierName = %s\n",ClassifierName);
    classify(argv[2],ClassifierName, argv[3],"CCCN", Tdata);
  }
  else if(classi_type==2)
  {
    ClassifierName = (char *)malloc(2001*sizeof(char));
    strcpy(ClassifierName,"NBC");
    TDC Tdata = Training(argv[1]);
    printf("ClassifierName = %s\n",ClassifierName);
    classify(argv[2],ClassifierName, argv[3],"CCCN", Tdata);
  }
  else if(classi_type==3)
  {
    ClassifierName = (char *)malloc(2001*sizeof(char));
    strcpy(ClassifierName,"SVM");
    TDC Tdata = Training(argv[1]);
    printf("ClassifierName = %s\n",ClassifierName);
    classify(argv[2],ClassifierName, argv[3],"CCCN", Tdata);
  }
  else if(classi_type==4)
  {
    ClassifierName = (char *)malloc(2001*sizeof(char));
    strcpy(ClassifierName,"CNNN_Classifier");
    classify_CCCN(argv[1],argv[2],argv[3]);
  }
  
  
  
 // TDC Tdata = Training(argv[1]);
  
  /*
  for(int i=0;i<Tdata.TrainData.rows;i++)
  {
    for(int j=0;j<Tdata.TrainData.cols;j++)
    {
      printf("%f\t",Tdata.TrainData.at<float>(i,j));
    }
    printf("\n\n");
  }
  */
  //exit(0);
  /*
  vector<char*> ClassifierName;
 
    ClassifierName.push_back("RF");
    //ClassifierName.push_back("SVM");
    ClassifierName.push_back("KNN");
    //ClassifierName.push_back("DT");
    ClassifierName.push_back("NBC");
    
  vector<char *> ClusteringName;
  
  ClusteringName.push_back("CCCN");
  //ClusteringName.push_back("CCE");
 
  */
  /*
 for(int j=0;j<ClusteringName.size();j++)
 {
  for(int i=0;i<ClassifierName.size();i++)
  {
      //makedir(ClassifierName[i]);
      
      printf("ClassifierName = %s\n",ClassifierName[i]);
      classify(argv[2],ClassifierName[i], argv[3],ClusteringName[j], Tdata);
  }
 }
  */
  //system("rm *.yml");
  
  return 0;
  
}