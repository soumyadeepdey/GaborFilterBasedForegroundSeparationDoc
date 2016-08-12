#include "ClusteringClassification.h"


void LabelSegmentationUnitofEachCluster(vector< SB > K, vector<SB> &blocks, vector< double > alpha)
{
  vector<float> homogeneity(5,0.0);
  for(int i=0;i<K.size();i++)
  {
    SB b = K[i];
    homogeneity[b.PredictedClass] = homogeneity[b.PredictedClass] + 1.0;
  }
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
  
  if(homogeneity[ProbableClass] > 0.75)
  {
    for(int i=0;i<K.size();i++)
    {
      SB b = K[i];
      blocks[b.blockid].PredictedClass = ProbableClass;
    }
  }
  else
  {
    if(alpha[0]>0.1)
    {
      for(int i=0;i<alpha.size();i++)
	alpha[i] = alpha[i] - 0.1;
      vector<vector<SB> > newcluster; // call clustering
      if(CCCN_flag)
      {
	newcluster = ClusteringCCN(K,alpha);
      }
      if(CCE_flag)
      {
	newcluster = ClusteringCE(K,alpha);
      }
      for(int j=0;j<newcluster.size();j++)
	LabelSegmentationUnitofEachCluster(newcluster[j],blocks,alpha);
    }
    else
    {
      for(int i=0;i<K.size();i++)
      {
	 SB b = K[i];
	 blocks[b.blockid].PredictedClass = ProbableClass;
      }
    }
  }
  
}

void ClusteringClassification(vector<SB> &blocks, vector<vector<SB> > &clusters, vector<double> alpha, char *clusteringname)
{
  for(int i=0;i<clusters.size();i++)
  {
    if(strcmp(clusteringname,"CCCN")==0)
      CCCN_flag = true;
    if(strcmp(clusteringname,"CCE")==0)
      CCE_flag = true;
    
    LabelSegmentationUnitofEachCluster(clusters[i],blocks,alpha);
  }
}
