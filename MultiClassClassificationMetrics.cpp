#include "MultiClassClassificationMetrics.h"

float ConfusionMatrix::GetAccuracy()
  {
    SetAccuracy();
    return(accuracy);
  }
  float ConfusionMatrix::GetPrecesion()
  {
    SetPrecession();
    return(precesion);
  }
  float ConfusionMatrix::GetRecall()
  {
    SetRecall();
    return(recall);
  }
  
  ConfusionMatrix::ConfusionMatrix(int NoOfClass)
  {
    multiclassCM.resize(NoOfClass,0);
    tp = 0;
    fp = 0;
    tn = 0;
    fn = 0;
    accuracy = 0.0;
    precesion = 0.0;
    recall = 0.0;
  }
  
  ConfusionMatrix::~ConfusionMatrix()
  {
    multiclassCM.clear();
  }
  
  
  void ConfusionMatrix::SetAccuracy()
  {
    accuracy = ((tp+tn)*1.0)/((tp+fp+tn+fn)*1.0);
  }
  
  void ConfusionMatrix::SetPrecession()
  {
    precesion = (tp*1.0)/(tp+fp);
  }
  
  void ConfusionMatrix::SetRecall()
  {
    recall = (tp*1.0)/(tp+fn);
  }
  
  
  
  
  
  
  
  
  MultiClassPerformanceMetrics::MultiClassPerformanceMetrics(vector<ConfusionMatrix> X)
  {
    CM = X;
    AvgAcc = 0.0;
    ErrorRate = 0.0;
    PrecesionM = 0.0;
    PrecesionMu = 0.0;
    RecallM = 0.0;
    RecallMu = 0.0;
    FScoreM = 0.0;
    FScoreMu = 0.0;
    size = CM.size();
  }
  
  MultiClassPerformanceMetrics::~MultiClassPerformanceMetrics()
  {
    CM.clear();
    AvgAcc = 0.0;
    ErrorRate = 0.0;
    PrecesionM = 0.0;
    PrecesionMu = 0.0;
    RecallM = 0.0;
    RecallMu = 0.0;
    FScoreM = 0.0;
    FScoreMu = 0.0;
    size = 0;
  }
  
  float MultiClassPerformanceMetrics::GetAverageAccuracy()
  {
    AvgAcc = 0.0;
    size = CM.size();
    for(int i=0;i<CM.size();i++)
    {
      AvgAcc = AvgAcc + GetAccuracy(CM[i]);
    }
    AvgAcc = AvgAcc/(size * 1.0);
    return(AvgAcc);
  }
  
  float MultiClassPerformanceMetrics::GetErrorRate()
  {
    ErrorRate = 0.0;
    size = CM.size();
    for(int i=0;i<CM.size();i++)
    {
      ErrorRate = ErrorRate + GetPerClassError(CM[i]);
    }
    ErrorRate = ErrorRate/(size * 1.0);
    return(ErrorRate);
  }
  
  float MultiClassPerformanceMetrics::GetPrecesionMu()
  {
    float neu = 0.0;
    float deno = 0.0;
    for(int i=0;i<CM.size();i++)
    {
      neu = (neu +CM[i].tp) * 1.0;
      deno = (deno + CM[i].tp + CM[i].fp) * 1.0;
    }
    PrecesionMu = neu/deno;
    return(PrecesionMu);
  }
  
  float MultiClassPerformanceMetrics::GetRecallMu()
  {
    float neu = 0.0;
    float deno = 0.0;
    for(int i=0;i<CM.size();i++)
    {
      neu = (neu + CM[i].tp) * 1.0;
      deno = (deno + CM[i].tp + CM[i].fn)*1.0;
    }
    RecallMu = neu/deno;
    return(RecallMu);
  }
  
  float MultiClassPerformanceMetrics::GetFScoreMu(int Beta)
  {
    float pre = GetPrecesionMu();
    float re = GetRecallMu();
    float neu = ((Beta * Beta) + 1) * pre * re;
    float deno = ((Beta * Beta) * pre) + re;
    
    FScoreMu = neu/deno;
    return(FScoreMu);
  }
  
  float MultiClassPerformanceMetrics::GetPrecesionM()
  {
    PrecesionM = 0.0;
    size = CM.size();
    for(int i=0;i<CM.size();i++)
    {
      PrecesionM = PrecesionM + GetPrecesion(CM[i]);
    }
    PrecesionM = PrecesionM/(size * 1.0);
    return(PrecesionM);
  }
  
  float MultiClassPerformanceMetrics::GetRecallM()
  {
    RecallM = 0.0;
    size = CM.size();
    for(int i=0;i<CM.size();i++)
    {
      RecallM = RecallM + GetRecall(CM[i]);
    }
    RecallM = RecallM/(size * 1.0);
    return(RecallM);
  }
  
  float MultiClassPerformanceMetrics::GetFScoreM(int Beta)
  {
    float pre = GetPrecesionM();
    float re = GetRecallM();
    float neu = ((Beta * Beta) + 1) * pre * re;
    float deno = ((Beta * Beta) * pre) + re;
    
    FScoreM = neu/deno;
    return(FScoreM);
  }
  
  
  float MultiClassPerformanceMetrics::GetAccuracy(ConfusionMatrix C)
  {
    float acc = 0.0;
    if((C.tp+C.fp+C.tn+C.fn) > 0)
    {
      acc = ((C.tp + C.tn)*1.0)/((C.tp+C.fp+C.tn+C.fn)*1.0);
      return(acc);
    }
    else
    {
      size = size - 1;
      return(acc);
    }
  }
  
  
  float MultiClassPerformanceMetrics::GetPerClassError(ConfusionMatrix C)
  {
    float err = 0.0;
    if((C.tp+C.fp+C.tn+C.fn) > 0)
    {
      err = ((C.fp+C.fn)*1.0)/((C.tp+C.fp+C.tn+C.fn)*1.0);
      return(err);
    }
    else
    {
      size = size - 1;
      return(err);
    }
  }
  
  
  float MultiClassPerformanceMetrics::GetPrecesion(ConfusionMatrix C)
  {
    if((C.tp + C.fp) > 0)
    {
      float pre = (C.tp*1.0)/((C.tp + C.fp)*1.0);
      return(pre);
    }
    else
    {
      size =  size - 1;
      return(0.0);
    }
  }
  
  
  float MultiClassPerformanceMetrics::GetRecall(ConfusionMatrix C)
  {
    if((C.tp + C.fn) > 0)
    {
      float re = (C.tp*1.0)/((C.tp + C.fn)*1.0);
      return(re);
    }
    else
    {
      size = size - 1;
      return(0.0);
    }
  }
  

  template <typename T>
int FindPosition(vector<T> x, T val)
{
  for(int i=0;i<x.size();i++)
  {
   // printf("posi %d val %d\n",i,x[i]);
    if(x[i]==val)
    {
      return(i);
      break;
    }
  }
  printf("There is Some problem for val %d\n",val);
  exit(0);
}
  
  

/**
 * @func GetConfusionMatrix
 * @param Mat : Groundtruthlabel
 * @param Mat : ClassifiedLabel
 * @param vector<int> : Training Data Class
 * @return vector<ConfusionMatrix> : Return Confution matrix for the Processing blocks
 * */
vector<ConfusionMatrix> GetConfusionMatrix(Mat Groundtruthlabel, Mat ClassifiedLabel, vector<int> ClassNumber)
{
      printf("In GetConfusionMatrix\n");
      vector<ConfusionMatrix> CM(ClassNumber.size(), ConfusionMatrix(ClassNumber.size()));
      
      for(int i=0;i<Groundtruthlabel.rows;i++)
      {
	int GtClass =(int) Groundtruthlabel.at<uchar>(i,0);
	if(GtClass>4)
	{
	  printf("problem GtClass  = %d\n",GtClass);
	  exit(0);
	}
	//printf("GtClass  = %d\n",GtClass);
	if(GtClass > 0)
	{
	// printf("If Gt not 0\n");
	  int response = ClassifiedLabel.at<uchar>(i,0);
	  printf("response %d\n",response);
	  int posi_res = FindPosition<int>(ClassNumber,response);
	  printf("Not response and GtClass = %d\n",GtClass);
	  int posi_gtcls = FindPosition<int>(ClassNumber,GtClass);
	  printf("Not GtClass\n");
	
	  CM[posi_res].multiclassCM[posi_gtcls] = CM[posi_res].multiclassCM[posi_gtcls] + 1;
	  
	  for(int j=0;j<ClassNumber.size();j++)
	  {
	    ConfusionMatrix cm_j = CM[j];
	    
	    if(response == ClassNumber[j]) // response == A Class j (Positive part (tp,fp))
	    {
	      if(ClassNumber[j] == GtClass) // GT == class j => tp
	      {
		cm_j.tp = cm_j.tp + 1;
		CM[j] = cm_j;
	      }
	      else // GT != class j => fp
	      {
		
		cm_j.fp = cm_j.fp + 1;
		CM[j] = cm_j;
		//printf("Block %d fp = %d of %d\n",i,cm_knn_j.fp,j);
	      }
	    }
	    else // response != Class j (Negetive part (tn,fn))
	    { 
	      if(ClassNumber[j] == GtClass) // GT == class j ==> fn
	      {
		
		cm_j.fn = cm_j.fn + 1;
		CM[j] = cm_j;
	      // printf("Block %d fn = %d of %d\n",i,cm_knn_j.fn,j);
	      }
	      else // GT != class =>  tn
	      {
		cm_j.tn = cm_j.tn + 1;
		CM[j] = cm_j;
	      }
	    }

	  
	  }
	}
      }

      
      
      return(CM);
}
