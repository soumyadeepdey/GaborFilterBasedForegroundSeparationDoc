#ifndef classification_H
#define classification_H

#include "StandardHeaders.h"
#include "SegmentationUnit.h"
#include "AlethiaParser/AlethiaParser.h"
#include "folder.h"
#include "PrepareAlethiaGT.h"
#include "FeatureExtraction.h"
#include "MultiClassClassificationMetrics.h"
#include "ConcensusBasedClustering.h"
#include "ClusteringClassification.h"
#include "CCCN_Classifier.h"


#ifdef HAVE_OPENCV_OCL
#define _OCL_KNN_ 0 // select whether using ocl::KNN method or not, default is using
#define _OCL_SVM_ 0 // select whether using ocl::svm method or not, default is using
#include "opencv2/ocl/ocl.hpp"
#endif




#define _NBC_ 1 // normal Bayessian classifier
#define _KNN_ 1 // k nearest neighbors classifier
#define _SVM_ 1 // support vectors machine
#define _DT_  0 // decision tree
#define _BT_  0 // ADA Boost
#define _GBT_ 0 // gradient boosted trees
#define _RF_  1 // random forest
#define _ERT_ 0 // extremely randomized trees
#define _ANN_ 0 // artificial neural networks
#define _EM_  0 // expectation-maximization
#define _CCCN_ 1 //CCCN Classification


extern bool classifyAll;
extern bool classifySelected;
extern bool classifySelected_ignore;
extern bool classifyTG;
extern bool classifyTG_ignore;


typedef struct TrainDataClass
{
  Mat TrainData;
  Mat TrainClass;
  vector<int> ClassNumber;
}TDC;

int findtypepresence(vector<char*> types, char* type);

bool CheckNoise(SB B, Mat image);

bool CheckSeparator(SB B);

vector<float> GetFeatureFromFVec(vector<float> FeatureVec);

void classify_CCCN(char *trainfile, char *testfile, char *KFoldNum);

void classify(char *TestFILE, char *classifiername, char *KFoldNum, char *ClusteringName, TDC &Data);

TDC Training(char *TrainFile);

void classify_KNN(vector<SB> &blocks, TDC &Data);

void classify_NBC(vector<SB> &blocks, TDC &Data);

void classify_DT(vector<SB> &blocks, TDC &Data);

void classify_RF(vector<SB> &blocks, TDC &Data);

void classify_SVM(vector<SB> &blocks, TDC &Data);

void Classification(vector<SB> &blocks, TDC &Data, char *ClassifierName);

#endif