#include "RunLengthFeature.h"


using namespace IITkgp_functions;


void RLFtoFVector(RLF x, vector<float> &y)
{
  float z;
  
  
  
  z = (float) x.B.h.mean;
  y.push_back(z);
  z = (float) sqrt(x.B.h.var);
  y.push_back(z);
  z =(float) x.B.h.number*1.0;
  y.push_back(z);
  
  
  z = (float) x.B.v.mean;
  y.push_back(z);
  z = (float) sqrt(x.B.v.var);
  y.push_back(z);
  z =(float) x.B.v.number*1.0;
  y.push_back(z);
  
  
  
  z = (float) x.B.l.mean;
  y.push_back(z);
  z = (float) sqrt(x.B.l.var);
  y.push_back(z);
  z =(float) x.B.l.number*1.0;
  y.push_back(z);
  
  
  
  z = (float) x.B.r.mean;
  y.push_back(z);
  z = (float) sqrt(x.B.r.var);
  y.push_back(z);
  z =(float) x.B.r.number*1.0;
  y.push_back(z);
  
  
  
  z = (float) x.F.h.mean;
  y.push_back(z);
  z = (float) sqrt(x.F.h.var);
  y.push_back(z);
  z =(float) x.F.h.number*1.0;
  y.push_back(z);
  
  
  z = (float) x.F.v.mean;
  y.push_back(z);
  z = (float) sqrt(x.F.v.var);
  y.push_back(z);
  z =(float) x.F.v.number*1.0;
  y.push_back(z);
  
  
  
  z = (float) x.F.l.mean;
  y.push_back(z);
  z = (float) sqrt(x.F.l.var);
  y.push_back(z);
  z =(float) x.F.l.number*1.0;
  y.push_back(z);
  
  
  
  z = (float) x.F.r.mean;
  y.push_back(z);
  z = (float) sqrt(x.F.r.var);
  y.push_back(z);
  z =(float) x.F.r.number*1.0;
  y.push_back(z);
  
  
}

int GetHRL(Mat image, Point p, int pvalue)
{
  int length = 0;
  int nx = p.x;
  bool flag = true;
  while(flag)
  {
    nx = nx + 1;
    if(validate(p.y,nx,image.rows,image.cols)==1)
    {
      if(image.at<uchar>(p.y,nx)==pvalue)
	length = length + 1;
      else
	flag = false;
    }
    else
      flag = false;
  }
  
  return length;

}


int GetVRL(Mat image, Point p, int pvalue)
{
  int length = 0;
  int ny = p.y;
  bool flag = true;
  while(flag)
  {
    ny = ny + 1;
    if(validate(ny,p.x,image.rows,image.cols)==1)
    {
      if(image.at<uchar>(ny,p.x)==pvalue)
	length = length + 1;
      else
	flag = false;
    }
    else
      flag = false;
  }
  
  return length;
}


int GetRRL(Mat image, Point p, int pvalue)
{
  int length = 0;
  int ny = p.y;
  int nx = p.x;
  bool flag = true;
  while(flag)
  {
    ny = ny - 1;
    nx = nx + 1;
    if(validate(ny,nx,image.rows,image.cols)==1)
    {
      if(image.at<uchar>(ny,nx)==pvalue)
	length = length + 1;
      else
	flag = false;
    }
    else
      flag = false;
  }
  
  return length;
}


int GetLRL(Mat image, Point p, int pvalue)
{
  int length = 0;
  int ny = p.y;
  int nx = p.x;
  bool flag = true;
  while(flag)
  {
    ny = ny + 1;
    nx = nx + 1;
    if(validate(ny,nx,image.rows,image.cols)==1)
    {
      if(image.at<uchar>(ny,nx)==pvalue)
	length = length + 1;
      else
	flag = false;
    }
    else
      flag = false;
  }
  
  return length;
}


L GetRLofPixel(Mat image, Point p, int pvalue)
{
  L rlp;
  rlp.h = GetHRL(image,p,pvalue);
  rlp.v = GetVRL(image,p,pvalue);
  rlp.r = GetRRL(image,p,pvalue);
  rlp.l = GetLRL(image,p,pvalue);
  
  return rlp;
  
}


RLF ComputeRLFeatures(Mat image)
{

  RL feature;
  
  for(int i=0;i<image.rows;i++)
  {
    for(int j=0;j<image.cols;j++)
    {
      Point p;
      p.x = j; p.y = i;
      L tmp;
      tmp = GetRLofPixel(image,p,image.at<uchar>(i,j));
      if(image.at<uchar>(i,j)==0)
      {
	feature.F.h.push_back(tmp.h);
	feature.F.v.push_back(tmp.v);
	feature.F.r.push_back(tmp.r);
	feature.F.l.push_back(tmp.l);
      }
      else
      {
	feature.B.h.push_back(tmp.h);
	feature.B.v.push_back(tmp.v);
	feature.B.r.push_back(tmp.r);
	feature.B.l.push_back(tmp.l);
      }
    }
  }
  
  RLF FeatureRL;
  
  Mat elem;
  
  
  /***************************************F*******************************************/
  
  elem = Mat(1,feature.F.h.size(),CV_8UC1,feature.F.h.data());
  
  FeatureRL.F.h.mean = FindMean(elem);
  FeatureRL.F.h.var = FindVar(elem);
  FeatureRL.F.h.number = feature.F.h.size();
  
  elem.release();
  
  
  elem = Mat(1,feature.F.v.size(),CV_8UC1,feature.F.v.data());
  
  FeatureRL.F.v.mean = FindMean(elem);
  FeatureRL.F.v.var = FindVar(elem);
  FeatureRL.F.v.number = feature.F.v.size();
  
  elem.release();
  
  
  elem = Mat(1,feature.F.l.size(),CV_8UC1,feature.F.l.data());
  
  FeatureRL.F.l.mean = FindMean(elem);
  FeatureRL.F.l.var = FindVar(elem);
  FeatureRL.F.l.number = feature.F.l.size();
  
  elem.release();
  
  
  elem = Mat(1,feature.F.r.size(),CV_8UC1,feature.F.r.data());
  
  FeatureRL.F.r.mean = FindMean(elem);
  FeatureRL.F.r.var = FindVar(elem);
  FeatureRL.F.r.number = feature.F.r.size();
  
  elem.release();
  
  
  /****************************************B******************************************/
  
  
  elem = Mat(1,feature.B.h.size(),CV_8UC1,feature.B.h.data());
  
  FeatureRL.B.h.mean = FindMean(elem);
  FeatureRL.B.h.var = FindVar(elem);
  FeatureRL.B.h.number = feature.B.h.size();
  if(FeatureRL.B.h.number==0)
    printf("hello i am zero in h\n");
  
  elem.release();
  
  
  elem = Mat(1,feature.B.v.size(),CV_8UC1,feature.B.v.data());
  
  FeatureRL.B.v.mean = FindMean(elem);
  FeatureRL.B.v.var = FindVar(elem);
  FeatureRL.B.v.number = feature.B.v.size();
  if(FeatureRL.B.v.number==0)
    printf("hello i am zero in v\n");
  
  elem.release();
  
  
  elem = Mat(1,feature.B.l.size(),CV_8UC1,feature.B.l.data());
  
  FeatureRL.B.l.mean = FindMean(elem);
  FeatureRL.B.l.var = FindVar(elem);
  FeatureRL.B.l.number = feature.B.l.size();
  if(FeatureRL.B.l.number==0)
    printf("hello i am zero in l\n");
  
  elem.release();
  
  
  elem = Mat(1,feature.B.r.size(),CV_8UC1,feature.B.r.data());
  
  FeatureRL.B.r.mean = FindMean(elem);
  FeatureRL.B.r.var = FindVar(elem);
  FeatureRL.B.r.number = feature.B.r.size();
  if(FeatureRL.B.r.number==0)
    printf("hello i am zero in r\n");
  
  elem.release();
  
  
  
  
  return FeatureRL;
  
}
