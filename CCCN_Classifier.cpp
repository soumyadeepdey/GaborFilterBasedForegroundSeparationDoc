#include "CCCN_Classifier.h"



void CCCN_Classifier::PrepareTrainData(vector< SB > K, vector< double > alpha)
{

  vector<float> homogeneity = ComputeHomogeneityatTrain(K,alpha.size());
  float max = homogeneity[0];
  int ProbableClass = 0;
  for(int i=0;i<homogeneity.size();i++)
  {
    homogeneity[i] = homogeneity[i]/K.size();
    if(homogeneity[i] > max)
    {
      max = homogeneity[i];
      ProbableClass = i;
    }
  }
  
  if(homogeneity[ProbableClass] > Hom_criteria)
  {
    vector<float> MeanVec(K[0].FeatureVec.size(),0.0);
    int cnt = 0;
    for(int i=0;i<K.size();i++)
    {
      SB b = K[i]; 
      if(b.GtClass==ProbableClass)
      {
	for(int j=0;j<b.FeatureVec.size();j++)
	  MeanVec[j] = MeanVec[j] + b.FeatureVec[j];
	cnt++;
      }
    }
    for(int j=0;j<K[0].FeatureVec.size();j++)
      MeanVec[j] = MeanVec[j]/cnt;
    TrainVectors.push_back(MeanVec);
    TrainVectorClass.push_back(ProbableClass);
  }
  else
  {
    if(alpha[0]>0.5)//modified
    {
      for(int i=0;i<alpha.size();i++)
	alpha[i] = alpha[i] - 0.1;
      vector<vector<SB> > newcluster; // call clustering
      
      newcluster = ClusteringCCN(K,alpha);
      
      for(int j=0;j<newcluster.size();j++)
	PrepareTrainData(newcluster[j], alpha);
    }
    else
    {
      for(int i=0;i<K.size();i++)
      {
	  vector<float> MeanVec(K[0].FeatureVec.size(),0.0);
	  int cnt = 0;
	  for(int i=0;i<K.size();i++)
	  {
	    SB b = K[i]; 
	    if(b.GtClass==ProbableClass)
	    {
	      for(int j=0;j<b.FeatureVec.size();j++)
		MeanVec[j] = MeanVec[j] + b.FeatureVec[j];
	      cnt++;
	    }
	  }
	  for(int j=0;j<K[0].FeatureVec.size();j++)
	    MeanVec[j] = MeanVec[j]/cnt;
	  TrainVectors.push_back(MeanVec);
	  TrainVectorClass.push_back(ProbableClass);	
      }
    }
  }
}


vector< float > CCCN_Classifier::ComputeHomogeneityatTrain(vector< SB > K, int NumofClass)
{

  vector<float> homogeneity(NumofClass,0.0);
  for(int i=0;i<K.size();i++)
  {
    SB b = K[i];
    homogeneity[b.GtClass] = homogeneity[b.GtClass] + 1.0;
  }
  for(int i=0;i<homogeneity.size();i++)
  {
    homogeneity[i] = homogeneity[i]/K.size();
  }
  return homogeneity;
}





void CCCN_Classifier::Train(char *trainfile, double alpha, float homogeneitycriteria)
{

  FILE *f;
  f = fopen(trainfile,"r");
  vector<SB> SU;
  vector<int> Labels;
  int su_cnt = 0;
  while(!feof(f))
  {
    char filename[2000];
      fscanf(f,"%s",&filename);
      printf("%s\n",filename);
      page pg = GetPageGroundtruth(filename);
  
      char *name;
      name = input_image_name_cut(pg.GetImageName());
      //makedir(name);
      
      char *opt;
      opt = (char *) malloc ( 2001 * sizeof(char));
      if(opt == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      strcpy(opt,name);

      char *tnm;
      tnm = (char *) malloc ( 2001 * sizeof(char));
      if(tnm == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      tnm = ".jpg";
      strcat(opt,tnm);
      
      char *tnm1;
      tnm1 = (char *) malloc ( 2001 * sizeof(char));
      if(tnm1 == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      strcpy(tnm1,opt);
      
      printf("imagename %s\n",tnm1);
      
      Mat image = imread(tnm1,1);
      
      //vector<SB> blocks = GetSegmentationUnit(image);
      
      vector<SB> blocks = GetProcessingBlocks(image);
      
      //blocks = PrepareAlethiaGt(pg,blocks);
      
      if(classifyAll)
{     
      blocks = PrepareAlethiaGt(pg,blocks);
      
}
      
if(classifySelected)
{      
      blocks = PrepareAlethiaGt_tgns(pg,blocks);
}
      
if(classifySelected_ignore)
{
      
      blocks = PrepareAlethiaGt_tgns_ignore(pg,blocks);
}
 
if(classifyTG)
{
      
      blocks = PrepareAlethiaGt_tg(pg,blocks);
}

if(classifyTG_ignore)
{
      
      blocks = PrepareAlethiaGt_tg_ignore(pg,blocks);
}
      
    /*************************************************************/
      /* for(int i=0;i<blocks.size();i++)
      {
	SB B = blocks[i];
	
	if(B.childs.empty())
	{
	  if(B.Fvecflag && B.gtflag)
	  {
	    B.blockid = su_cnt;
	    SU.push_back(B);
	    if(Labels.empty())
	      Labels.push_back(B.GtClass);
	    else
	    {
	      if(find<int>(Labels.begin(),Labels.end(),B.GtClass)==Labels.end())
		Labels.push_back(B.GtClass);
	    }
	    su_cnt++;
	  }
	}
	else
	{
	  for(int k=0;k<B.childs.size();k++)
	  {
	    
	    SB B_C = B.childs[k];
	    if(B_C.Fvecflag && B_C.gtflag);
	    { 
	      B_C.blockid = su_cnt;
	      SU.push_back(B_C);
	      if(Labels.empty())
		Labels.push_back(B.GtClass);
	      else
	      {
		if(find<int>(Labels.begin(),Labels.end(),B.GtClass)==Labels.end())
		  Labels.push_back(B.GtClass);
	      }
	      su_cnt++;
	    }
	  }
	}
      }
      
      blocks.clear();
      */
      
      /*****************************************************************************/
      
      vector<char*> type;
      vector<int> cnttype;
      for(int i=0;i<blocks.size();i++)
      {
	SB B = blocks[i];
	if(B.childs.empty())
	{
	  
	  if(B.Fvecflag && B.gtflag)
	  {
	    //printf("Hello\n");
	    if(B.GtClass>7)
	    {
	      printf("Error .... value of gt is %d\n",B.GtClass);
	      exit(0);
	    }
	    if(B.GtClass == 0)
	    {
	      if(strcmp(B.bgcolor,"White")==0)
	      {
		if(type.empty())
		{
		  type.push_back(B.type);
		  cnttype.push_back(0);
		  SU.push_back(B);
		  Labels.push_back(B.GtClass);
		}
		else
		{
		  int po = findtypepresence(type,B.type);
		  if(po==type.size())
		  {
		    type.push_back(B.type);
		    cnttype.push_back(0);
		    SU.push_back(B);
		    Labels.push_back(B.GtClass);
		  }
		  else
		  {
		    if(cnttype[po]<7)
		    {
		      SU.push_back(B);
		      Labels.push_back(B.GtClass);
		      cnttype[po] = cnttype[po] + 1;
		    }
		  }
		}
	      }
	      else
	      {
		SU.push_back(B);
		Labels.push_back(B.GtClass);
	      }
	      
	    }
	    else
	    {
	      if(B.GtClass!=4&&B.GtClass!=5)
	      {
		SU.push_back(B);
		Labels.push_back(B.GtClass);
	      }
	    }
	  }
	}
	else
	{
	  for(int k=0;k<B.childs.size();k++)
	  {
	    SB B_C = B.childs[k];
	    
	    if(B_C.Fvecflag && B_C.gtflag)
	    {
	     // printf("Hello\n");
	      if(B_C.GtClass>7)
	      {
		printf("Error .... value of gt is %d\n",B_C.GtClass);
		exit(0);
	      }
	      if(B_C.GtClass == 0)
	      {
		if(strcmp(B_C.bgcolor,"White")==0)
		{
		  if(type.empty())
		  {
		    type.push_back(B_C.type);
		    cnttype.push_back(0);
		    SU.push_back(B_C);
		    Labels.push_back(B_C.GtClass);
		  }
		  else
		  {
		    int po = findtypepresence(type,B_C.type);
		    if(po==type.size())
		    {
		      type.push_back(B_C.type);
		      cnttype.push_back(0);
		      SU.push_back(B_C);
		      Labels.push_back(B_C.GtClass);
		    }
		    else
		    {
		      if(cnttype[po]<7)
		      {
			SU.push_back(B_C);
			Labels.push_back(B_C.GtClass);
			cnttype[po] = cnttype[po] + 1;
		      }
		    }
		  }
		}
		else
		{
		  SU.push_back(B_C);
		  Labels.push_back(B_C.GtClass);
		}
	      }
	      else
	      {
		if(B_C.GtClass!=4&&B_C.GtClass!=5)
		{
		  SU.push_back(B_C);
		  Labels.push_back(B_C.GtClass);
		}
	      }
	    }
	  }
	}
      }
      
      blocks.clear();
      
      
      /*****************************************************************************/
  }
  
  
  
  
  Hom_criteria = homogeneitycriteria;
  LabelList = Labels;
  
  vector<double> th(Labels.size(),alpha);
  
  vector<vector<SB> > Clusters = ClusteringCCN(SU,th);
  
  for(int i=0;i<Clusters.size();i++)
    PrepareTrainData(Clusters[i],th);  
 
}


void CCCN_Classifier::Train(vector< SB > Traindata, vector<int> Labels, double alpha, float homogeneitycriteria)
{
  
  Hom_criteria = homogeneitycriteria;
  LabelList = Labels;
  
  vector<double> th(Labels.size(),alpha);
  
  vector<vector<SB> > Clusters = ClusteringCCN(Traindata,th);
  
  for(int i=0;i<Clusters.size();i++)
    PrepareTrainData(Clusters[i],th);
}



int CCCN_Classifier::Predict(vector<float> FeatureVec, double alpha)
{
  
  vector<vector<double> > DistanceVector;

  for(int i=0;i<TrainVectors.size();i++)
  {
    vector<double> StudentTVal;
    for(int k=0;k<FeatureVec.size();k=k+3)
    {
      if(TrainVectors[i][k+1]==FeatureVec[k+1])
      {
	double tval = twoSampleTtest_EqualVariance(TrainVectors[i][k+2],FeatureVec[k+2],TrainVectors[i][k],FeatureVec[k],TrainVectors[i][k+1],FeatureVec[k+1]);
	int sdof = DegreeofFreedom_EqualVariance(TrainVectors[i][k+2],FeatureVec[k+2]);
	tval = studenttdistribution(sdof,tval);
	if(tval > 0.5)
	  tval =  2 * (1 - tval);
	else
	  tval = 2 * tval;
	StudentTVal.push_back(tval);
      }
      else
      {
	double tval = twoSampleTtest_UnequalVariance(TrainVectors[i][k+2],FeatureVec[k+2],TrainVectors[i][k],FeatureVec[k],TrainVectors[i][k+1],FeatureVec[k+1]);
	double sdof = DegreeofFreedom_UnequalVariance(TrainVectors[i][k+2],FeatureVec[k+2],TrainVectors[i][k+1],FeatureVec[k+1]);
	tval = studenttdistribution(sdof,tval);
	if(tval > 0.5)
	  tval =  2 * (1 - tval);
	else
	  tval = 2 * tval;
	StudentTVal.push_back(tval);
      }
    }
    DistanceVector.push_back(StudentTVal);
    StudentTVal.clear();
  }
  
  vector<int> PositiveCluster;
  vector<double> clusterwt;
  
  vector<double> Accumulatedwt;
  vector<int> GtClusters;
  
  for(int i=0;i<DistanceVector.size();i++)
  {
    bool flag = true;
    double w8 = 0.0;
    for(int j=0;j<DistanceVector[i].size();j++)
    {
      if(flag)
      {
	if(DistanceVector[i][j]>alpha)
	  flag = true;
	else
	  flag = false;
      }
      w8 = w8 + DistanceVector[i][j];
    }
    w8 = w8/DistanceVector[i].size();
    Accumulatedwt.push_back(w8);
    GtClusters.push_back(i);
    if(flag)
    {
      PositiveCluster.push_back(i);
      clusterwt.push_back(w8);
    }
    
  }
  
  vector<int> PredictionHist(LabelList.size(),0);
  
  if(clusterwt.size()>0)
  {
  
    int length = clusterwt.size();

    
      for (int i = 0; i < length; ++i)
      {
	  bool swapped = false;
	  for (int j = 0; j < length - (i+1); ++j)
	  {
	      if (clusterwt[j] > clusterwt[j+1])
	      {
		  swaping<double>(clusterwt, j, j+1);
		  swaping<int>(PositiveCluster, j, j+1);
		  swapped = true;
	      }
	  }
	  
	  if (!swapped) break;
      }
      
      int loop;
      if(NumNeighbour<=length)
	loop = NumNeighbour;
      else
	loop = length;
      for (int i = 0; i < loop; i++)
	PredictionHist[PositiveCluster[i]]++;
      int max=PredictionHist[0],maxposi=0;
      for(int i=0;i<PredictionHist.size();i++)
      {
	if(max<PredictionHist[i])
	{
	  max = PredictionHist[i];
	  maxposi = i;
	}
      }
      if(NeighbourFlag)
	return LabelList[maxposi];
      else
	return LabelList[PositiveCluster[0]];
  }
  else
  {
   
    int  length = Accumulatedwt.size();

      for (int i = 0; i < length; ++i)
      {
	  bool swapped = false;
	  for (int j = 0; j < length - (i+1); ++j)
	  {
	      if (Accumulatedwt[j] > Accumulatedwt[j+1])
	      {
		  swaping<double>(Accumulatedwt, j, j+1);
		  swaping<int>(GtClusters, j, j+1);
		  swapped = true;
	      }
	  }
	  
	  if (!swapped) break;
      }
      
      int loop;
      if(NumNeighbour<=length)
	loop = NumNeighbour;
      else
	loop = length;
      for (int i = 0; i < loop; i++)
	PredictionHist[GtClusters[i]]++;
      int max=PredictionHist[0],maxposi=0;
      for(int i=0;i<PredictionHist.size();i++)
      {
	if(max<PredictionHist[i])
	{
	  max = PredictionHist[i];
	  maxposi = i;
	}
      }
      if(NeighbourFlag)
	return LabelList[maxposi];
      else
	return LabelList[GtClusters[0]];
  }
  
}

