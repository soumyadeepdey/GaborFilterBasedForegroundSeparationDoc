
#include "GaborAngleFeature.h"

vector<float>  GetGaborFeature(Mat image)
{
  
  
  
  int kernel_size = 3; // 5
  Mat GaborDest,GaborSrc,Gaborkernel;
  double sig = 1, th = 0, lm = 1.0, gm = 0.5; // sig = 1
  vector<float> GaborFeature;
  
  vector<double> th_gm;
  th_gm.push_back(0.5);
  //th_gm.push_back(1.0);
  //th_gm.push_back(2.0);
  
  vector<double> th_lm;
 // th_lm.push_back(0.3);
 // th_lm.push_back(0.5);
 // th_lm.push_back(1.0);
 // th_lm.push_back(2.0);
 // th_lm.push_back(2.5);
  th_lm.push_back(3.0);
 // th_lm.push_back(3.5);
 // th_lm.push_back(4.0);
 // th_lm.push_back(5.0);
  
 // th_gm.push_back(0.05);
 // th_gm.push_back(0.1);
 // th_gm.push_back(0.2);
  
  
  vector<double> th_sig;
  
  th_sig.push_back(0.5);
  th_sig.push_back(1.0);
  th_sig.push_back(2.0);
  th_sig.push_back(3.0);
  th_sig.push_back(4.0);
  th_sig.push_back(5.0);
  th_sig.push_back(10.0);
  th_sig.push_back(12.0);
  
  
  vector<double> theta;
  theta.push_back(15.0);
  theta.push_back(30.0);
  theta.push_back(45.0);
  theta.push_back(60.0);
  theta.push_back(90.0);
  theta.push_back(120.0);
  theta.push_back(135.0);
  
  
  
  for(int ga=0;ga<th_gm.size();ga++)
  {
    gm = th_gm[ga];
    for(int lam=0;lam<th_lm.size();lam++)
    {
      lm = th_lm[lam];
      
      for(int sg=0;sg<th_sig.size();sg++)
      {
	sig = th_sig[sg];
	Mat Wtgabor;
	for(int th_i=0;th_i<theta.size();th_i++)
	{
	  th = theta[th_i];
	  int type = CV_32F;
	
	  Mat AntSymGKernel,SymGKernel;
	  
	  SymGKernel = getGaborKernel(Size(kernel_size, kernel_size), sig, th, lm, gm,0.0,type);
	  AntSymGKernel = getGaborKernel(Size(kernel_size, kernel_size), sig, th, lm, gm,0.5*PI,type);
	  
	  Mat gray;
	  cvtColor(image,gray,CV_BGR2GRAY);
	  //cvtColor(blurimage,gray,CV_BGR2GRAY);
	  gray.convertTo(GaborSrc,CV_32F);
	  
	  //filter2D(GaborSrc, GaborDest, CV_32F, Gaborkernel);
	  
	  Mat SymGDest,AntSymGDest;
	  
	  filter2D(GaborSrc, SymGDest, CV_32F, SymGKernel);
	  
	  filter2D(GaborSrc, AntSymGDest, CV_32F, AntSymGKernel);
	  
	  Mat GaborEnergy = Mat(GaborSrc.rows,GaborSrc.cols,CV_32FC1);
	  
	  for(int r=0;r<GaborSrc.rows;r++)
	  {
	    for(int c=0;c<GaborSrc.cols;c++)
	    {
	      float sym = SymGDest.at<float>(r,c);
	      float antsym = AntSymGDest.at<float>(r,c);
	      float energy = sym*sym + antsym*antsym;
	      energy =(float) sqrt(energy);
	      GaborEnergy.at<float>(r,c) = energy;
	    }
	  }
	  
	  
	  Mat test;
	  convertScaleAbs( GaborEnergy, test );
	  //GaborDest.copyTo(test);
	  GaborDest.release();
	  if(Wtgabor.empty())
	  {
	    //Wtgabor.copyTo(GaborEnergy);
	    test.copyTo(Wtgabor);
	  }
	  else
	  {
	    /// Total Gradient (approximate)
	    addWeighted( Wtgabor, 0.5, test, 0.5, 0, Wtgabor );
	    //addWeighted( Wtgabor, 0.5, GaborEnergy, 0.5, 0, Wtgabor );
	  }
	}
	GaborFeature.push_back((float) FindMean(Wtgabor));
	GaborFeature.push_back((float) FindStdDev(Wtgabor));
	GaborFeature.push_back((float) Wtgabor.rows*Wtgabor.cols*1.0);
	Wtgabor.release();
      }
      
    }
  }
  
  
  
  image.release();
  
  return GaborFeature;
  
}



vector<float>  GetGaborFeatureWithMask(Mat image, Mat MaskedImage)
{
  int fp = NumberofForegroundPixel(MaskedImage);
  int kernel_size = 3; // 5
  Mat GaborDest,GaborSrc,Gaborkernel;
  double sig = 1, th = 0, lm = 1.0, gm = 0.5; // sig = 1
  vector<float> GaborFeature;
  
  vector<double> th_gm;
  th_gm.push_back(0.5);
  //th_gm.push_back(1.0);
  //th_gm.push_back(2.0);
  
  vector<double> th_lm;
 // th_lm.push_back(0.3);
 // th_lm.push_back(0.5);
 // th_lm.push_back(1.0);
 // th_lm.push_back(2.0);
 // th_lm.push_back(2.5);
  th_lm.push_back(3.0);
 // th_lm.push_back(3.5);
 // th_lm.push_back(4.0);
 // th_lm.push_back(5.0);
  
 // th_gm.push_back(0.05);
 // th_gm.push_back(0.1);
 // th_gm.push_back(0.2);
  
  
  vector<double> th_sig;
  
  th_sig.push_back(0.5);
  th_sig.push_back(1.0);
  th_sig.push_back(2.0);
  th_sig.push_back(3.0);
  th_sig.push_back(4.0);
  th_sig.push_back(5.0);
  th_sig.push_back(10.0);
  th_sig.push_back(12.0);
  
  
  vector<double> theta;
  theta.push_back(15.0);
  theta.push_back(30.0);
  theta.push_back(45.0);
  theta.push_back(60.0);
  theta.push_back(90.0);
  theta.push_back(120.0);
  theta.push_back(135.0);
  
  
  
  for(int ga=0;ga<th_gm.size();ga++)
  {
    gm = th_gm[ga];
    for(int lam=0;lam<th_lm.size();lam++)
    {
      lm = th_lm[lam];
      
      for(int sg=0;sg<th_sig.size();sg++)
      {
	sig = th_sig[sg];
	Mat Wtgabor;
	for(int th_i=0;th_i<theta.size();th_i++)
	{
	  th = theta[th_i];
	  int type = CV_32F;
	
	  Mat AntSymGKernel,SymGKernel;
	  
	  SymGKernel = getGaborKernel(Size(kernel_size, kernel_size), sig, th, lm, gm,0.0,type);
	  AntSymGKernel = getGaborKernel(Size(kernel_size, kernel_size), sig, th, lm, gm,0.5*PI,type);
	  
	  Mat gray;
	  cvtColor(image,gray,CV_BGR2GRAY);
	  //cvtColor(blurimage,gray,CV_BGR2GRAY);
	  gray.convertTo(GaborSrc,CV_32F);
	  
	  //filter2D(GaborSrc, GaborDest, CV_32F, Gaborkernel);
	  
	  Mat SymGDest,AntSymGDest;
	  
	  filter2D(GaborSrc, SymGDest, CV_32F, SymGKernel);
	  
	  filter2D(GaborSrc, AntSymGDest, CV_32F, AntSymGKernel);
	  
	  Mat GaborEnergy = Mat(fp,1,CV_32FC1);
	  
	  int cnt = 0;
	  for(int r=0;r<GaborSrc.rows;r++)
	  {
	    for(int c=0;c<GaborSrc.cols;c++)
	    {
	      if(MaskedImage.at<uchar>(r,c) == 0)
	      {
		float sym = SymGDest.at<float>(r,c);
		float antsym = AntSymGDest.at<float>(r,c);
		float energy = sym*sym + antsym*antsym;
		energy =(float) sqrt(energy);
		GaborEnergy.at<float>(cnt,0) = energy;
		cnt++;
	      }
	    }
	  }
	  
	  
	  Mat test;
	  convertScaleAbs( GaborEnergy, test );
	  //GaborDest.copyTo(test);
	  GaborDest.release();
	  if(Wtgabor.empty())
	  {
	    //Wtgabor.copyTo(GaborEnergy);
	    test.copyTo(Wtgabor);
	  }
	  else
	  {
	    /// Total Gradient (approximate)
	    addWeighted( Wtgabor, 0.5, test, 0.5, 0, Wtgabor );
	    //addWeighted( Wtgabor, 0.5, GaborEnergy, 0.5, 0, Wtgabor );
	  }
	}
	GaborFeature.push_back((float) FindMean(Wtgabor));
	GaborFeature.push_back((float) FindStdDev(Wtgabor));
	GaborFeature.push_back((float) Wtgabor.rows*Wtgabor.cols*1.0);
	Wtgabor.release();
      }
      
    }
  }
  
  
  
  image.release();
  
  return GaborFeature;
  
}
