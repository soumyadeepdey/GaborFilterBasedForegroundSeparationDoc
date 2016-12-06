#include "SpatialAndAotocorrelatedFeature.h"
#include <gsl/gsl_fit.h>

using namespace IITkgp_functions;



void SFtoFVector(SF x, vector<float> &y)
{
  float z;
  
  z = (float) x.h.mean;
  y.push_back(z);  
  z = (float) sqrt(x.h.var);
  y.push_back(z);
  z = (float) x.h.number;
  y.push_back(z);
  
  z = (float) x.v.mean;
  y.push_back(z);  
  z = (float) sqrt(x.v.var);
  y.push_back(z);
  z = (float) x.v.number;
  y.push_back(z);
  
  z = (float) x.l.mean;
  y.push_back(z);
  z = (float) sqrt(x.l.var);
  y.push_back(z);
  z = (float) x.l.number;
  y.push_back(z);
  
  z = (float) x.r.mean;
  y.push_back(z);
  z = (float) sqrt(x.r.var);
  y.push_back(z);
  z = (float) x.r.number;
  y.push_back(z);
  
}


void AFtoFVector(AF x, vector<float> &y)
{
  float z;
  
  z =(float) x.h.proj.posi * 1.0;
  y.push_back(z);
  z =(float) x.h.proj.slope;
  y.push_back(z);
  
  z =(float) x.v.proj.posi * 1.0;
  y.push_back(z);
  z =(float) x.v.proj.slope;
  y.push_back(z);
  
  z =(float) x.l.proj.posi * 1.0;
  y.push_back(z);
  z =(float) x.l.proj.slope;
  y.push_back(z);
  
  z =(float) x.r.proj.posi * 1.0;
  y.push_back(z);
  z =(float) x.r.proj.slope;
  y.push_back(z);
  
  z =(float) x.h.fgrun.posi * 1.0;
  y.push_back(z);
  z =(float) x.h.fgrun.slope;
  y.push_back(z);
  
  z =(float) x.v.fgrun.posi * 1.0;
  y.push_back(z);
  z =(float) x.v.fgrun.slope;
  y.push_back(z);
  
  z =(float) x.l.fgrun.posi * 1.0;
  y.push_back(z);
  z =(float) x.l.fgrun.slope;
  y.push_back(z);
  
  z =(float) x.r.fgrun.posi * 1.0;
  y.push_back(z);
  z =(float) x.r.fgrun.slope;
  y.push_back(z);
  
  
  z =(float) x.h.rlmean.posi * 1.0;
  y.push_back(z);
  z =(float) x.h.rlmean.slope;
  y.push_back(z);
  
  z =(float) x.v.rlmean.posi * 1.0;
  y.push_back(z);
  z =(float) x.v.rlmean.slope;
  y.push_back(z);
  
  z =(float) x.l.rlmean.posi * 1.0;
  y.push_back(z);
  z =(float) x.l.rlmean.slope;
  y.push_back(z);
  
  
  z =(float) x.r.rlmean.posi * 1.0;
  y.push_back(z);
  z =(float) x.r.rlmean.slope;
  y.push_back(z);
  
  z =(float) x.h.spmean.posi * 1.0;
  y.push_back(z);
  z =(float) x.h.spmean.slope;
  y.push_back(z);
  
  z =(float) x.v.spmean.posi * 1.0;
  y.push_back(z);
  z =(float) x.v.spmean.slope;
  y.push_back(z);
  
  z =(float) x.l.spmean.posi * 1.0;
  y.push_back(z);
  z =(float) x.l.spmean.slope;
  y.push_back(z);
  
  z =(float) x.r.spmean.posi * 1.0;
  y.push_back(z);
  z =(float) x.r.spmean.slope;
  y.push_back(z);
  
  
}

void Pass::setweight()
{
  if(Pass::type == 0)// horizontal pass
    Pass::weight = Pass::start.y;
  else if(Pass::type == 1)//vertical pass
    Pass::weight = Pass::start.x;
  else if(Pass::type == 2)//left-diagonal
    Pass::weight = Pass::start.x + Pass::start.y;
  else if(Pass::type == 3)//right-diagonal
    Pass::weight = Pass::end.y + Pass::end.x;
}


int Pass::getweight()
{
  Pass::setweight();
  return weight;
}

Pass::Pass(int t)
{
  Pass::type = t;
}

void Pass::setpassvalues(int i)
{
  Pass::passvalues.push_back(i);
  
}

void Pass::setpasspoints(Point x)
{
  Pass::passpoints.push_back(x);
}


void Pass::setpassprojection()
{
  int cnt = 0; 
    for(int j=0;j<passvalues.size();j++)
    {
      if(passvalues[j]!=255)
      {
	cnt = cnt + 1;
      }
    }
    Pass::PassProjection = cnt;

}


void Pass::setPassRunLengths()
{
  bool flag = false;
  for(int i=0;i<passvalues.size();i++)
  {
    if(passvalues[i]!=255)
    {
      SRL temp;
      flag = true; int cnt =0;
      while(flag)
      {
	temp.start=passpoints[i];
	cnt = cnt + 1;
	i = i + 1;
	if(i<passvalues.size())
	{
	  if(passvalues[i]==255)
	  {
	    flag = false;
	    temp.end=passpoints[i-1];
	    break;
	  }
	}
	else
	{
	  flag = false;
	  break;
	}
      }
      if(type==0)
      {
	temp.pos=temp.start.x;
	temp.leng=temp.end.x-temp.start.x;
      }
      if(type==1)
      {
	temp.pos=temp.start.y;
	temp.leng=temp.end.y-temp.start.y;
      }
      if(type==2)
      {
	temp.pos=temp.start.x;
	temp.leng=temp.end.x-temp.start.x;
      }
      if(type==3)
      {
	temp.pos=temp.start.x;
	temp.leng=temp.end.x-temp.start.x;
      }
      temp.proj = cnt;
      RunLengths.push_back(temp);
    }
  }
}







Pass::~Pass()
{
  RunLengths.clear();
}



vector< Pass > HPass(Mat image)
{
  vector<Pass> val;
  for(int r=0;r<image.rows;r++)
  {
    Pass temp(0);
   // temp.Pass(0);
    temp.start.y = r;
    temp.start.x = 0;
    Point cur;
    for(int c=0;c<image.cols;c++)
    { 
      cur.y = r;
      cur.x = c;
      temp.setpasspoints(cur);
      temp.setpassvalues(image.at<uchar>(r,c));
    }
    temp.end.y=r;
    temp.end.x=image.cols - 1;
    int weight = temp.getweight();
    temp.setpassprojection();
    temp.setPassRunLengths();
    val.push_back(temp);
   // temp.~Pass();
  }
  return val;
}


vector< Pass > VPass(Mat image)
{
  vector<Pass> val;
  for(int c=0;c<image.cols;c++)
  {
    Pass temp(1);
    temp.start.x = c;
    temp.start.y = 0;
    Point cur;
    for(int r=0;r<image.rows;r++)
    {
      cur.y = r;
      cur.x = c;
      temp.setpasspoints(cur);
      //printf("printf %d\n",image.at<uchar>(r,c));
      temp.setpassvalues(image.at<uchar>(r,c));
    }
    temp.end.y = image.rows - 1;
    temp.end.x = c;
    int weight = temp.getweight();
    temp.setpassprojection();
    temp.setPassRunLengths();
    val.push_back(temp);
  }
  return val;
}

vector< Pass > LPass(Mat image)
{
  vector<Pass> val;
  int x1,y1,x2,y2;
  y1 = 0; x1 = 0;
  
  for( ;x1<image.cols;x1++)
  {
    Pass temp(2);
    temp.start.x=x1;
    temp.start.y=y1;
    Point cur;
    y2 = y1; x2 = x1;
    
    bool flag = true;
    while(flag)
    {
      
      if(validate(y2,x2,image.rows,image.cols) == 1)
      {
	cur.y=y2;cur.x=x2;
	temp.setpasspoints(cur);
	temp.setpassvalues(image.at<uchar>(y2,x2));
	x2 = x2 + 1;
	y2 = y2 + 1;
      }
      else
	flag = false;
    }
    temp.end.x = x2;
    temp.end.y = y2;
    int weight = temp.getweight();
    temp.setpassprojection();
    temp.setPassRunLengths();
    val.push_back(temp);
   // temp.~Pass();
  }
  
  y1 = 1; x1 = 0;
  
  for( ;y1<image.rows;y1++)
  {
    Pass temp(2);
    temp.start.x=x1;
    temp.start.y=y1;
    
    y2 = y1; x2 = x1;
    
    Point cur;

    bool flag = true;
    while(flag)
    {

      if(validate(y2,x2,image.rows,image.cols) == 1)
      {
	cur.y=y2;cur.x=x2;
	temp.setpasspoints(cur);
	temp.setpassvalues(image.at<uchar>(y2,x2));
	x2 = x2 + 1;
	y2 = y2 + 1;
      }
      else
	flag = false;
    }
    temp.end.x = x2;
    temp.end.y = y2;
    int weight = temp.getweight();
    temp.setpassprojection();
    temp.setPassRunLengths();
    val.push_back(temp);
   // temp.~Pass();
  }
  
  return val;
  
}

vector< Pass > RPass(Mat image)
{
  
  vector<Pass> val;
  int x1,y1,x2,y2;
  
  y1 = image.rows - 1; x1 = 0;
  
  for(x1=0;x1<image.cols;x1++)
  {
    Pass temp(3);
    temp.start.x=x1;
    temp.start.y=y1;
    Point cur;
    y2 = y1; x2 = x1;
    bool flag = true;
    while(flag)
    { 
      if(validate(y2,x2,image.rows,image.cols) == 1)
      {
	cur.y=y2;cur.x=x2;
	temp.setpasspoints(cur);
	temp.setpassvalues(image.at<uchar>(y2,x2));
	x2 = x2 + 1;
	y2 = y2 - 1;
      }
      else
	flag = false;
    }
    temp.end.x = x2;
    temp.end.y = y2;
    int weight = temp.getweight();
    temp.setpassprojection();
    temp.setPassRunLengths();
    val.push_back(temp);
  }
  
  y1 = image.rows - 2; x1 = 0;
  
  for(y1=image.rows-2;y1>=0;y1--)
  {
    Pass temp(3);
    temp.start.x=x1;
    temp.start.y=y1;
    Point cur;
    y2 = y1; x2 = x1;
    bool flag = true;
    while(flag)
    { 
      if(validate(y2,x2,image.rows,image.cols) == 1)
      {
	cur.y=y2;cur.x=x2;
	temp.setpasspoints(cur);
	temp.setpassvalues(image.at<uchar>(y2,x2));
	x2 = x2 + 1;
	y2 = y2 - 1;
      }
      else
	flag = false;
    }
    temp.end.x = x2;
    temp.end.y = y2;
    int weight = temp.getweight();
    temp.setpassprojection();
    temp.setPassRunLengths();
    val.push_back(temp);
  }
  
  
  
  return val;
}

Stat ComputeSpatialFeatureInaDirection(vector<Pass> P,int n)
{
  Stat temp;
  temp.number = n;
  double mean = 0.0;
  
  for(int i=0;i<P.size();i++)
  {
    Pass x = P[i];
    mean = mean + (x.weight*x.PassProjection);
  }
  mean = mean/n;
  temp.mean = mean;
  
  double var = 0.0;
  for(int i=0;i<P.size();i++)
  {
    Pass x = P[i];
    double v = x.PassProjection*((x.weight-mean)*(x.weight-mean))*1.0;
    var = var + v;
  }
  var = var/n;
  temp.var = var;
  return temp;
}

SF ComputeSpatialFeatures(Mat Image)
{
  int NumberofForeground = NumberofForegroundPixel(Image);
  vector<Pass> H = HPass(Image);
  vector<Pass> V = VPass(Image);
  vector<Pass> L = LPass(Image);
  vector<Pass> R = RPass(Image);
  SF featureSF;
  featureSF.h = ComputeSpatialFeatureInaDirection(H,NumberofForeground);
  featureSF.v = ComputeSpatialFeatureInaDirection(V,NumberofForeground);
  featureSF.l = ComputeSpatialFeatureInaDirection(L,NumberofForeground);
  featureSF.r = ComputeSpatialFeatureInaDirection(R,NumberofForeground);
  
  return featureSF;
}


IAC ComputeInitialAutocorelationFeatures(Mat Image)
{
  IAC FeatureIAC;
  
  vector<Pass> H = HPass(Image);
  
  for(int i=0;i<H.size();i++)
  {
    Pass t = H[i];
    
    SFL t1;
    t1.proj = t.PassProjection;
    t1.FgRun = t.RunLengths.size();
    
    if(t1.proj>0)
    {
      double rlmean = (t.PassProjection*1.0)/t.RunLengths.size();
      
      t1.rlmean = rlmean;
      
      double spmean;
      double x = 0.0; double y = 0.0;
      for(int j=0;j<t.RunLengths.size();j++)
      {
	SRL te = t.RunLengths[j];
	x = x + (te.leng * te.pos * 1.0);
	y = y + (((te.leng * te.leng) - te.proj)*1.0);
      }
      spmean = (x + (0.5 * y))/t.PassProjection;
      t1.spmean = spmean;
      
      FeatureIAC.h.push_back(t1);
    }
  }
  
  vector<Pass> V = VPass(Image);
  
  for(int i=0;i<V.size();i++)
  {
    Pass t = V[i];

    SFL t1;
    t1.proj = t.PassProjection;
    t1.FgRun = t.RunLengths.size();
    
    if(t1.proj>0)
    {
      double rlmean = (t.PassProjection*1.0)/t.RunLengths.size();
      
      //printf("posi %d proj %d RunLengths %d rlmen %lf\n",i,t.PassProjection,t.RunLengths.size(),rlmean);
      
      t1.rlmean = rlmean;
      
      double spmean;
      double x = 0.0; double y = 0.0;
      for(int j=0;j<t.RunLengths.size();j++)
      {
	SRL te = t.RunLengths[j];
	x = x + (te.leng * te.pos * 1.0);
	y = y + (((te.leng * te.leng) - te.proj)*1.0);
      }
      spmean = (x + (0.5 * y))/t.PassProjection;
      t1.spmean = spmean;
      
      FeatureIAC.v.push_back(t1);
    }
  }
  
  vector<Pass> L = LPass(Image);
  
  for(int i=0;i<L.size();i++)
  {
    Pass t = L[i];
    SFL t1;
    t1.proj = t.PassProjection;
    t1.FgRun = t.RunLengths.size();
    
    if(t1.proj>0)
    {
      double rlmean = (t.PassProjection*1.0)/t.RunLengths.size();
      
      t1.rlmean = rlmean;
      
      double spmean;
      double x = 0.0; double y = 0.0;
      for(int j=0;j<t.RunLengths.size();j++)
      {
	SRL te = t.RunLengths[j];
	x = x + (te.leng * te.pos * 1.0);
	y = y + (((te.leng * te.leng) - te.proj)*1.0);
      }
      spmean = (x + (0.5 * y))/t.PassProjection;
      t1.spmean = spmean;
      FeatureIAC.l.push_back(t1);
    }
  }
  
  vector<Pass> R = RPass(Image);
  
  for(int i=0;i<R.size();i++)
  {
    Pass t = R[i];
    SFL t1;
    t1.proj = t.PassProjection;
    t1.FgRun = t.RunLengths.size();
    
    if(t1.proj>0)
    {
      double rlmean = (t.PassProjection*1.0)/t.RunLengths.size();
      
      t1.rlmean = rlmean;
      
      double spmean;
      double x = 0.0; double y = 0.0;
      for(int j=0;j<t.RunLengths.size();j++)
      {
	SRL te = t.RunLengths[j];
	x = x + (te.leng * te.pos * 1.0);
	y = y + (((te.leng * te.leng) - te.proj)*1.0);
      }
      spmean = (x + (0.5 * y))/t.PassProjection;
      t1.spmean = spmean;
      FeatureIAC.r.push_back(t1);
    }
  }
  
  //matchTemplate();
  
  return FeatureIAC;
  
}

SAF ComputeSingleAutoCorrelationFeature(real_1d_array& x)
{
  real_1d_array res;
  
  corrr1d(x, x.length(), x, x.length(), res);

  
  printf("Original size = %d Result size %d\n",x.length(),res.length());
  int maxposi; // one feature
  double tmp = res[0]/10;
  double tmp2 = res[0]/20;
  tmp2 = res[0] - tmp2;
  vector<double> datay;
  int y = 7;
  for(int i=0;i<x.length();i++)
  {
    //if(res[i] >= tmp2)
      //datay.push_back(res[i]*1.0);
    if(y>=0)
    {
      datay.push_back(res[i]*1.0);
      y = y - 1;
    }
    if(res[i] <= tmp)
    {
      maxposi = i;
      break;
    }
    //printf("%d\t%lf\n",i,res[i]);
  }
  vector<double> datax(datay.size());
  for(int i=0;i<datax.size();i++)
    datax[i] = i;
  //tangent 2nd feature
  double c0, c1, cov00, cov01, cov11, sumsq;
  gsl_fit_linear (datax.data(), 1, datay.data(), 1, datax.size(), &c0, &c1, &cov00, &cov01, &cov11, &sumsq);
  
  
  SAF ans;
  ans.posi = maxposi;
  
  if(cvIsNaN(c1))
  {
    if(c1>0)
      ans.slope = 90.0;
    else
      ans.slope = -90.0;
  }
  else   
    ans.slope = atan(c1)*180/PI;
  printf("Slope %lf\n",ans.slope);
  
  return ans;
}


AF ComputeAutoCoRelatedFeatures(Mat Image)
{
  IAC FeatureIAC = ComputeInitialAutocorelationFeatures(Image);
  //Mat temp1 = Mat(1,FeatureIAC.h.size(),CV_32FC1);
  
  AF FeatureAF;

  real_1d_array x;
  
 /***************************************H*************************************************/

 
  x.setlength(FeatureIAC.h.size());
  for(int i=0;i<FeatureIAC.h.size();i++)
  {
    SFL t1 = FeatureIAC.h[i];
    x(i) = t1.proj*1.0;
  }
  
  FeatureAF.h.proj = ComputeSingleAutoCorrelationFeature(x);

  
  x.setlength(0);
  
  
  x.setlength(FeatureIAC.h.size());
  for(int i=0;i<FeatureIAC.h.size();i++)
  {
    SFL t1 = FeatureIAC.h[i];
    x(i) = t1.FgRun*1.0;
  }
  
  FeatureAF.h.fgrun = ComputeSingleAutoCorrelationFeature(x);
  
  x.setlength(0);
  
  x.setlength(FeatureIAC.h.size());
  for(int i=0;i<FeatureIAC.h.size();i++)
  {
    SFL t1 = FeatureIAC.h[i];
    x(i) = t1.rlmean*1.0;
  }
  
  FeatureAF.h.rlmean = ComputeSingleAutoCorrelationFeature(x);
  
  x.setlength(0);
  
  x.setlength(FeatureIAC.h.size());
  for(int i=0;i<FeatureIAC.h.size();i++)
  {
    SFL t1 = FeatureIAC.h[i];
    x(i) = t1.spmean*1.0;
  }
  

  FeatureAF.h.spmean = ComputeSingleAutoCorrelationFeature(x);
  
  x.setlength(0);
  
  /***************************************V*************************************************/

 
  x.setlength(FeatureIAC.v.size());
  for(int i=0;i<FeatureIAC.v.size();i++)
  {
    SFL t1 = FeatureIAC.v[i];
    x(i) = t1.proj*1.0;
  }
  
  FeatureAF.v.proj = ComputeSingleAutoCorrelationFeature(x);
  
  
  x.setlength(0);
  
  
  x.setlength(FeatureIAC.v.size());
  for(int i=0;i<FeatureIAC.v.size();i++)
  {
    SFL t1 = FeatureIAC.v[i];
    x(i) = t1.FgRun*1.0;
  }
  
  FeatureAF.v.fgrun = ComputeSingleAutoCorrelationFeature(x);
  
  x.setlength(0);
  
  x.setlength(FeatureIAC.v.size());
  for(int i=0;i<FeatureIAC.v.size();i++)
  {
    SFL t1 = FeatureIAC.v[i];
    x(i) = t1.rlmean*1.0;
  }
  
  FeatureAF.v.rlmean = ComputeSingleAutoCorrelationFeature(x);
  
  x.setlength(0);
  
  x.setlength(FeatureIAC.v.size());
  for(int i=0;i<FeatureIAC.v.size();i++)
  {
    SFL t1 = FeatureIAC.v[i];
    x(i) = t1.spmean*1.0;
  }
  
  FeatureAF.v.spmean = ComputeSingleAutoCorrelationFeature(x);
  
  x.setlength(0);
  
  /***************************************L*************************************************/

 
  x.setlength(FeatureIAC.l.size());
  for(int i=0;i<FeatureIAC.l.size();i++)
  {
    SFL t1 = FeatureIAC.l[i];
    x(i) = t1.proj*1.0;
  }
  
  FeatureAF.l.proj = ComputeSingleAutoCorrelationFeature(x);
  
  
  x.setlength(0);
  
  
  x.setlength(FeatureIAC.l.size());
  for(int i=0;i<FeatureIAC.l.size();i++)
  {
    SFL t1 = FeatureIAC.l[i];
    x(i) = t1.FgRun*1.0;
  }
  
  FeatureAF.l.fgrun = ComputeSingleAutoCorrelationFeature(x);
  
  x.setlength(0);
  
  x.setlength(FeatureIAC.l.size());
  for(int i=0;i<FeatureIAC.l.size();i++)
  {
    SFL t1 = FeatureIAC.l[i];
    x(i) = t1.rlmean*1.0;
  }
  
  FeatureAF.l.rlmean = ComputeSingleAutoCorrelationFeature(x);
  
  x.setlength(0);
  
  x.setlength(FeatureIAC.l.size());
  for(int i=0;i<FeatureIAC.l.size();i++)
  {
    SFL t1 = FeatureIAC.l[i];
    x(i) = t1.spmean*1.0;
  }
  
  FeatureAF.l.spmean = ComputeSingleAutoCorrelationFeature(x);
  
  x.setlength(0);
  
  /***************************************R*************************************************/

 
  x.setlength(FeatureIAC.r.size());
  for(int i=0;i<FeatureIAC.r.size();i++)
  {
    SFL t1 = FeatureIAC.r[i];
    x(i) = t1.proj*1.0;
  }
  
  FeatureAF.r.proj = ComputeSingleAutoCorrelationFeature(x);
  
  
  x.setlength(0);
  
  
  x.setlength(FeatureIAC.r.size());
  for(int i=0;i<FeatureIAC.r.size();i++)
  {
    SFL t1 = FeatureIAC.r[i];
    x(i) = t1.FgRun*1.0;
  }
  
  FeatureAF.r.fgrun = ComputeSingleAutoCorrelationFeature(x);
  
  x.setlength(0);
  
  x.setlength(FeatureIAC.r.size());
  for(int i=0;i<FeatureIAC.r.size();i++)
  {
    SFL t1 = FeatureIAC.r[i];
    x(i) = t1.rlmean*1.0;
  }
  
  FeatureAF.r.rlmean = ComputeSingleAutoCorrelationFeature(x);
  
  x.setlength(0);
  
  x.setlength(FeatureIAC.r.size());
  for(int i=0;i<FeatureIAC.r.size();i++)
  {
    SFL t1 = FeatureIAC.r[i];
    x(i) = t1.spmean*1.0;
  }
  
  FeatureAF.r.spmean = ComputeSingleAutoCorrelationFeature(x);
  
  x.setlength(0);
  
  
  /****************************************************************************************/
  
  return FeatureAF;
  
}



