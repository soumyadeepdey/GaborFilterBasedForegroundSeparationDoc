#ifndef MultiClassClassificationMetrics_H
#define MultiClassClassificationMetrics_H

#include "StandardHeaders.h"



class ConfusionMatrix
{
public:
  int tp;
  int fp;
  int tn;
  int fn;
  vector<int> multiclassCM;
  
 
  float GetAccuracy();
  
  float GetPrecesion();
  
  float GetRecall();
  
  
  ConfusionMatrix(int NoOfClass);
  
  ~ConfusionMatrix();
  
  
private:
  float accuracy;
  float precesion;
  float recall;
  
  void SetAccuracy();
  
  
  void SetPrecession();
  
  void SetRecall();
  
};

class MultiClassPerformanceMetrics
{
public:
  MultiClassPerformanceMetrics(vector<ConfusionMatrix> X);
  
  
  ~MultiClassPerformanceMetrics();
  
  
  float GetAverageAccuracy();
  
  
  float GetErrorRate();
  
  float GetPrecesionMu();
  
  float GetRecallMu();
  
  float GetFScoreMu(int Beta);
  
  float GetPrecesionM();
  
  float GetRecallM();
  
  float GetFScoreM(int Beta);
 
  
private:
  vector<ConfusionMatrix> CM;
  float AvgAcc;
  float ErrorRate;
  float PrecesionMu;
  float RecallMu;
  float FScoreMu;
  float PrecesionM;
  float RecallM;
  float FScoreM;
  int size;
  
  float GetAccuracy(ConfusionMatrix C);
  float GetPerClassError(ConfusionMatrix C);
  float GetPrecesion(ConfusionMatrix C);
  float GetRecall(ConfusionMatrix C);
    
  
};


template <typename T>
int FindPosition(vector<T> x, T val);

/**
 * @func GetConfusionMatrix
 * @param Mat : Groundtruthlabel
 * @param Mat : ClassifiedLabel
 * @param vector<int> : Training Data Class
 * @return vector<ConfusionMatrix> : Return Confution matrix for the Processing blocks
 * */
vector<ConfusionMatrix> GetConfusionMatrix(Mat Groundtruthlabel, Mat ClassifiedLabel, vector<int> ClassNumber);


#endif