#include "ConcensusBasedClustering.h"


/*---------------------------------------------------------------AdjacencyMatrix----------------------------------------------------------------------------*/




/*@function FindAdjacencyMatrix
 * @param : input : vector<vector<type> > - A adjacency matrix with relation value between two node is given, in double
 * 		    double connection_condition - It is the value by checking which two nodes are connected
 * 
 * example : connection_condition = x;
 * 	     if(RelationMat.at<double>(i,j) > x)
 * 		
 * return : A AdjacencyMatrix of bool type (vector<vector<bool> >)
 * */

template <typename T>
vector<vector<bool> > FindAdjacencyMatrix(vector<vector<T> > &Relation, T connection_condition)
{
  vector<vector<bool> > AdjMat;
  for(int i=0;i<Relation.size();i++)
  {
    vector<bool> t;
    for(int j=0;j<Relation[i].size();j++)
    {
      
      if(Relation[i][j] > connection_condition)
      {
	t.push_back(true);
      }
      else
      {
	t.push_back(false);
      }
    }
    AdjMat.push_back(t);
    t.clear();
  }
  return(AdjMat);
}



template <typename T>
vector<vector<bool> > FindAdjacencyMatrixNdata(vector<vector<vector<T> > > Relation, vector<T> Threshold)
{
  vector<vector<bool> > AdjMat(Relation[0].size,(Relation[0].size(),true));

    for(int i=0;i<Relation[0].size();i++)
    {
	for(int j=0;j<Relation[0].size();j++)
	{
	  bool temp;
	  for(int k=0;k<Relation.size();k++)
	  {
	    if(Relation[k][i][j] > Threshold[k])
	    {
	      temp = true;
	    }
	    else
	    {
	      temp = false;
	      break;
	    }
	  }
	  AdjMat[i][j] = temp;
	}
    }
    
    return AdjMat;

}

/*-------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------*/





void GiveLabelDFS(vector<vector<bool> > AdjMat, vector<int> &tempcomponent, vector<int> &labels, vector<bool> &ccflag)
{
  vector<bool> tempadj;
  tempadj = AdjMat[tempcomponent.back()];
  for(int i=0;i<tempadj.size();i++)
  {
    if(ccflag[i] == false && tempadj[i] == true)
    {
      ccflag[i] = true;
      labels[i] = labels[tempcomponent.back()];
      tempcomponent.push_back(i);
      GiveLabelDFS(AdjMat,tempcomponent,labels,ccflag);
    }
  }
}








/*@function : 		DFSCC 
 * @desc :		Find ConnectedComponent based on depth First search of the adjacency matrix
 * @param:	input:	vector<vector<bool> > AdjMat :	AdjacencyMatrix in bool format
 * 
 * 			vector<vector<int> > &component: address to the component
 * 
 * */

void DFSCC(vector<vector<bool> > AdjMat, vector<vector<int> > &component, vector<int> &labels)
{
  vector<bool> ccflag;
  for(int i=0;i<AdjMat.size();i++)
  {
    ccflag.push_back(false);
  }
  
  int label = 0;
  
  
  for(int i=0;i<AdjMat.size();i++)
  {
    if(ccflag[i] == false)
    {
      vector<int> tempconnected;
     // printf("In DFSCC i = %d\n",i);
      tempconnected.push_back(i);
      labels[i] = label;
      ccflag[i] = true;
      GiveLabelDFS(AdjMat, tempconnected, labels, ccflag);      
      component.push_back(tempconnected);
      label = label + 1;
    }
  }
  
  
}


/*-------------------------------------------------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------------------------------------------------*/
template <typename T>
vector<T> FindIntersection(vector<T> &v1, vector<T> &v2)
{
  vector<T> v3;
  
  sort(v1.begin(), v1.end());
  sort(v2.begin(), v2.end());
  
  set_intersection(v1.begin(),v1.end(),v2.begin(),v2.end(),back_inserter(v3));
  
  return (v3);
}



/*-------------------------------------------------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------------------------------------------------*/

double CalculateZValue(double SampleMean, double PopulationMean, double PopulationSD, int SampleSize)
{
  double z;
  z = SampleMean - PopulationMean;
  double temp = sqrt(SampleSize);
  temp = PopulationSD/temp;
  z = z/temp;
  return(z);
}

double CalculateZValue(double SampleMean, double PopulationMean, double SEM)
{
  double z;
  z = SampleMean - PopulationMean;
  z = z / SEM;
  return(z);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------*/

double oneSampleTtest(double sample_mean, double population_mean, double sample_sd, double sample_size)
{
  double t;
  double sigma;
  sigma = sample_sd/(sqrt(sample_size));
  t = sample_mean - population_mean;
  t = t / sigma;
  return(t);
}

double twoSampleTtest_UnequalVariance(int sample1_size, int sample2_size, double sample1_mean, double sample2_mean, double sample1_sd, double sample2_sd)
{
  double sigma;
  double sigma1,sigma2;
  sigma1 = sample1_sd * sample1_sd;
  sigma1 = sigma1/sample1_size;
  sigma2 = sample2_sd * sample2_sd;
  sigma2 = sigma2/sample2_size;
  sigma = sigma1 + sigma2;
  sigma = sqrt(sigma);
  double t;
  t = sample1_mean - sample2_mean;
  if(sigma>0)
    t = t/sigma;
    
  return(t);
}

double twoSampleTtest_EqualVariance(int sample1_size, int sample2_size, double sample1_mean, double sample2_mean, double sample1_sd, double sample2_sd)
{
  double sp;
  double sigma1,sigma2;
  sigma1 = sample1_sd * sample1_sd;
  sigma2 = sample2_sd * sample2_sd;
  sp = ((sample1_size - 1) * sigma1)+((sample2_size - 1) * sigma2);
  sp = sp/(sample1_size+sample2_size-2);
  double sigma;
  sigma = ((1/sample1_size)+(1/sample2_size));
  sigma = sp * sigma;
  sigma = sqrt(sigma);
  double t;
  t = sample1_mean - sample2_mean;
  if(sigma > 0)
    t = t/sigma;
  return(t);
}

int DegreeofFreedom_UnequalVariance(int sample1_size, int sample2_size, double sample1_sd, double sample2_sd)
{
  double neu1,neu2,neu;
  neu1 = (sample1_sd * sample1_sd)/sample1_size;
  neu2 = (sample2_sd * sample2_sd)/sample2_size;
  neu = neu1 + neu2;
  neu = neu * neu;
  double deno1;
  deno1 = (sample1_sd * sample1_sd)/sample1_size;
  deno1 = deno1 * deno1;
  deno1 = deno1/(sample1_size - 1);
  double deno2;
  deno2 = (sample2_sd * sample2_sd)/sample2_size;
  deno2 = deno2 * deno2;
  deno2 = deno2/(sample2_size - 1);
  
  double deno = deno1 + deno2;
  
  double val = neu/deno;
  val = val + 0.5;
  
  int dof =(int) floor(val);
  return(dof);
  
}

int DegreeofFreedom_EqualVariance(int sample1_size, int sample2_size)
{
  int dof = sample1_size + sample2_size - 2;
  return(dof);
}

/*-------------------------------------------------------------------------------------------------------------------*/


void FindCommonNodes(vector<vector<int> > &K, vector<vector<int> > D, vector<vector<vector<int> > > N, int z, int t)
{
  if(z<N.size()-1)
  {
    int h = 0;
    while(h < N[z].size())
    {
      D[t] = N[z][h];
      FindCommonNodes(K,D,N,z+1,t+1);
      h = h + 1;
    }
  }
  else
  {
    for(int l=0;l<N[z].size();l++)
    {
      D[t] = N[z][l];
      vector<int> k = D[t][0];
      for(int i=1;i<D[t].size();i++)
      k = FindIntersection<int>(k,D[t][i]);
      if(k.empty())
	k.clear();
      else
      {
	K.push_back(k);
	k.clear();
      }
    }
  }
}


vector<vector<int> > ClusteringCCN(vector< SB > blocks, vector< double > Threshold)
{
  vector<SB> SU;
  int FvecSize;
    for(int i=0;i<blocks.size();i++)
    {
      SB B = blocks[i];
      
      if(B.childs.empty())
      {
	if(B.Fvecflag && B.gtflag)
	{
	  SU.push_back(B);
	  FvecSize = B.FeatureVec.size();
	}
      }
      else
      {
	for(int k=0;k<B.childs.size();k++)
	{
	  
	  SB B_C = B.childs[k];
	  if(B_C.Fvecflag && B_C.gtflag);
	  { 
	    SU.push_back(B_C);
	  }
	}
      }
    }
    
    blocks.clear();
    
    vector<vector<vector<double> > > StudentTDistval;
    
    for(int k=0;k<FvecSize;k+2)
    {
	vector<vector<double> > Fstdistval;
	for(int i=0;i<SU.size();i++)
	{
	  SB B_i = SU[i];
	  vector<Point> poly_i;
	  approxPolyDP( Mat(B_i.Contours), poly_i, 3, true );
	  Rect R_i = boundingRect( Mat(poly_i) );
	  poly_i.clear();
	  vector<double> stdistval;
	  for(int j=0;j<SU.size();j++)
	  {	
	    if(i!=j)
	    {
	      SB B_j = SU[j];
	      vector<Point> poly_j;
	      approxPolyDP( Mat(B_j.Contours), poly_j, 3, true );
	      Rect R_j = boundingRect( Mat(poly_j) );
	      poly_j.clear();
	      if(B_i.FeatureVec[k+1]==B_j.FeatureVec[k+1])
		{
		  double tval = twoSampleTtest_EqualVariance(R_i.area(),R_j.area(),B_i.FeatureVec[k],B_j.FeatureVec[k],B_i.FeatureVec[k+1],B_j.FeatureVec[k+1]);
		  int sdof = DegreeofFreedom_EqualVariance(R_i.area(),R_j.area());
		  tval = studenttdistribution(sdof,tval);
		  if(tval > 0.5)
		    tval =  2 * (1 - tval);
		  else
		    tval = 2 * tval;
		  stdistval.push_back(tval);
		}
	      else
		{
		  double tval = twoSampleTtest_UnequalVariance(R_i.area(),R_j.area(),B_i.FeatureVec[k],B_j.FeatureVec[k],B_i.FeatureVec[k+1],B_j.FeatureVec[k+1]);
		  double sdof = DegreeofFreedom_UnequalVariance(R_i.area(),R_j.area(),B_i.FeatureVec[k+1],B_j.FeatureVec[k+1]);
		  tval = studenttdistribution(sdof,tval);
		  if(tval > 0.5)
		    tval =  2 * (1 - tval);
		  else
		    tval = 2 * tval;
		  stdistval.push_back(tval);
		}	      
	    }
	    else
	    { 
	      stdistval.push_back(1.0);
	    }
	  }
	  Fstdistval.push_back(stdistval);
	  stdistval.clear();
	}
	StudentTDistval.push_back(Fstdistval);
	Fstdistval.clear();
    }
    
    
    
    vector<vector<vector<int> > > CC;
    
    //vector<vector<int> > CC_Labels;
    
    
    
    for(int k=0;k<FvecSize/2;k++)
    {
      vector<vector<bool> > AdjMat = FindAdjacencyMatrix(StudentTDistval[k], Threshold[k]);
      vector<int> labels(StudentTDistval[k].size());
      vector<vector<int> > T_CC;
      DFSCC(AdjMat, T_CC, labels);
      AdjMat.clear();
      CC.push_back(T_CC);
      T_CC.clear();
      //CC_Labels.push_back(labels);
      labels.clear();
    }

    vector<vector<int> > K;
    vector<vector<int> > D(FvecSize/2);
    int z = 0;
    int t = 0;
    
    FindCommonNodes(K,D,CC,z,t);
    
    return K;
}

vector<vector<int> > ClusteringCE(vector< SB > blocks, vector< double > Threshold)
{
   vector<SB> SU;
  inr FvecSize;
    for(int i=0;i<blocks.size();i++)
    {
      SB B = blocks[i];
      
      if(B.childs.empty())
      {
	if(B.Fvecflag && B.gtflag)
	{
	  SU.push_back(B);
	  FvecSize = B.FeatureVec.size();
	}
      }
      else
      {
	for(int k=0;k<B.childs.size();k++)
	{
	  
	  SB B_C = B.childs[k];
	  if(B_C.Fvecflag && B_C.gtflag);
	  { 
	    SU.push_back(B_C);
	  }
	}
      }
    }
    
    blocks.clear();
    
    vector<vector<vector<double> > > StudentTDistval;
    
    for(int k=0;k<FvecSize;k+2)
    {
	vector<vector<double> > Fstdistval;
	for(int i=0;i<SU.size();i++)
	{
	  SB B_i = SU[i];
	  vector<Point> poly_i;
	  approxPolyDP( Mat(B_i.Contours), poly_i, 3, true );
	  Rect R_i = boundingRect( Mat(poly_i) );
	  poly_i.clear();
	  vector<double> stdistval;
	  for(int j=0;j<SU.size();j++)
	  {	
	    if(i!=j)
	    {
	      SB B_j = SU[j];
	      vector<Point> poly_j;
	      approxPolyDP( Mat(B_j.Contours), poly_j, 3, true );
	      Rect R_j = boundingRect( Mat(poly_j) );
	      poly_j.clear();
	      if(B_i.FeatureVec[k+1]==B_j.FeatureVec[k+1])
		{
		  double tval = twoSampleTtest_EqualVariance(R_i.area(),R_j.area(),B_i.FeatureVec[k],B_j.FeatureVec[k],B_i.FeatureVec[k+1],B_j.FeatureVec[k+1]);
		  int sdof = DegreeofFreedom_EqualVariance(R_i.area(),R_j.area());
		  tval = studenttdistribution(sdof,tval);
		  if(tval > 0.5)
		    tval =  2 * (1 - tval);
		  else
		    tval = 2 * tval;
		  stdistval.push_back(tval);
		}
	      else
		{
		  double tval = twoSampleTtest_UnequalVariance(R_i.area(),R_j.area(),B_i.FeatureVec[k],B_j.FeatureVec[k],B_i.FeatureVec[k+1],B_j.FeatureVec[k+1]);
		  double sdof = DegreeofFreedom_UnequalVariance(R_i.area(),R_j.area(),B_i.FeatureVec[k+1],B_j.FeatureVec[k+1]);
		  tval = studenttdistribution(sdof,tval);
		  if(tval > 0.5)
		    tval =  2 * (1 - tval);
		  else
		    tval = 2 * tval;
		  stdistval.push_back(tval);
		}	      
	    }
	    else
	    { 
	      stdistval.push_back(1.0);
	    }
	  }
	  Fstdistval.push_back(stdistval);
	  stdistval.clear();
	}
	StudentTDistval.push_back(Fstdistval);
	Fstdistval.clear();
    }
    
    vector<vector<bool> > AdjMat;

    AdjMat = FindAdjacencyMatrixNdata(StudentTDistval,Threshold);
    vector<vector<int> > CE_CC;
    vector<int> CE_labels(StudentTDistval[0].size());
    DFSCC(AdjMat, CE_CC, CE_labels);
    
    
    return CE_CC;
    
    
}
