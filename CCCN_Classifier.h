#ifndef CCCN_Classifier_H
#define CCCN_Classifier_H

#include "StandardHeaders.h"
#include "ConcensusBasedClustering.h"
#include "folder.h"
#include "AlethiaParser/AlethiaParser.h"
#include "PrepareAlethiaGT.h"
#include "classification.h"
#include "SegmentationUnit.h"
#include "sorting.h"

using namespace IITkgp_functions;

class CCCN_Classifier
{
public:
  /**
   * Initialize with KNN Flag for prediction
   * KNNFlag = true : means check top K neighbours and return the mostly occured neghbour 
   *         = false: means return the top neighbour
   */
  CCCN_Classifier(bool KNNFlag, int K=10)
  {
    NeighbourFlag = KNNFlag;
    NumNeighbour = K;
  }
  /**
   * By default KNN flag is set as false
   */
  CCCN_Classifier()
  {
    NeighbourFlag = true;
    NumNeighbour = 10;
  }
  ~CCCN_Classifier()
  {
    TrainVectors.clear();
    TrainVectorClass.clear();
    LabelList.clear();
  }
  /**
 * @func:Train
 * @def: This Function Train the CCCN classifier using CCCN Clustering with alpha 0.5,
 * This function takes a file containing name of trining images
 */
void Train(char *trainfile, double alpha=0.5, float homogeneitycriteria=0.9);

/**
 * @func:Train
 * @def: This Function Train the CCCN classifier using CCCN Clustering with alpha 0.5,
 * This function takes a vector of training data, and number of distict class of the training data as an input to the algorithm
 */
void Train(vector<SB> Traindata, vector<int> Labels, double alpha=0.5, float homogeneitycriteria=0.9);

int Predict(vector<float> FeatureVec, double alpha=0.5);


private:
  
  vector<vector<float> > TrainVectors;
  vector<int> TrainVectorClass;
  vector<int> LabelList;
  bool NeighbourFlag;
  int NumNeighbour;
  float Hom_criteria;
  
  /**
   * @func:PrepareTrainData
   * @def: This function recursively call itself and CCCN clustering to prepare the training data
   * 
   */
  void PrepareTrainData(vector<SB> K, vector<double> alpha);
  
  /**
   * @func:ComputeHomogeneityatTrain This function compute the homogeneity of a cluster during training period,
   * i.e. this function compute homogeneity of a cluster based on the ground truth class of the data within the cluster
   */
  vector<float> ComputeHomogeneityatTrain(vector<SB> K,int NumofClass);
  
};






#endif