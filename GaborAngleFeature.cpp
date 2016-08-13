
#include "GaborAngleFeature.h"

vector<vector<float> >  GetMyGaborFeature(Mat image, char *im_name)
{
  
  printf("%s\n",im_name);
  makedir(im_name);
  
  
   //Mat blurimage; 
  // GaussianBlur(image,blurimage,Size(11,11),0,0);
  //
   char *output;
  
//   output=(char *)malloc(2001*sizeof(char));
//   output = CreateNameIntoFolder(im_name,"bluredimage.png");
//   imwrite(output,blurimage);
   
  
  
  
  int kernel_size = 3; // 5
  Mat GaborDest,GaborSrc,Gaborkernel;
  double sig = 1, th = 0, lm = 1.0, gm = 0.02; // sig = 1
  vector<vector<float> > GaborAngleHist;
  vector<float> GaborFeature;
  
  vector<double> th_gm;
  th_gm.push_back(0.02);
  //th_gm.push_back(0.025);
  th_gm.push_back(0.03);
 // th_gm.push_back(0.05);
 // th_gm.push_back(0.1);
 // th_gm.push_back(0.2);
  
  
  vector<double> th_sig;
 // th_sig.push_back(1.0);
 // th_sig.push_back(1.5);
 // th_sig.push_back(2.0);
 // th_sig.push_back(2.5);
  th_sig.push_back(0.3);
  th_sig.push_back(0.5);
  
  
  vector<double> theta;
  theta.push_back(30.0);
  theta.push_back(45.0);
  theta.push_back(60.0);
  theta.push_back(90.0);
  theta.push_back(120.0);
  theta.push_back(135.0);
  
  Mat Wtgabor;
  
  for(int sg=0;sg<th_sig.size();sg++)
  {
    sig = th_sig[sg];
    for(int gam=0;gam<th_gm.size();gam++)
    {
      gm = th_gm[gam];
      //for(int th_i=30;th_i<=150;)
      for(int th_i=0;th_i<theta.size();th_i++)
      {
	//th = th_i*1.0;
	th = theta[th_i];
      
	Gaborkernel = getGaborKernel(Size(kernel_size, kernel_size), sig, th, lm, gm);
	
	Mat gray;
	cvtColor(image,gray,CV_BGR2GRAY);
	gray.convertTo(GaborSrc,CV_32F);
	
	filter2D(GaborSrc, GaborDest, CV_32F, Gaborkernel);
	
	

	GaborSrc.release();
	Gaborkernel.release();
	
	GaborFeature.push_back((float) FindMean(GaborDest));
	GaborFeature.push_back((float) FindStdDev(GaborDest));
	//float gf_mean = (float) FindMean(GaborDest);
	//float gf_stddev = (float)  FindStdDev(GaborDest);
	
	double max,min;
	int posi;
	
	Mat test;
	convertScaleAbs( GaborDest, test );
	//GaborDest.copyTo(test);
	GaborDest.release();
	if(Wtgabor.empty())
	  test.copyTo(Wtgabor);
	else
	  /// Total Gradient (approximate)
	  addWeighted( Wtgabor, 0.5, test, 0.5, 0, Wtgabor );

	
	char *name;
	
	char *output;
	
	
	
	
	name=(char *)malloc(2001*sizeof(char));
	sprintf(name, "Gresponse%lf_%lf_%d.png",sig,gm,th_i);
	
	output=(char *)malloc(2001*sizeof(char));
	output = CreateNameIntoFolder(im_name,name);
	imwrite(output,test);
	
	//
	test.release();
	
	//th_i = th_i + 15;
      }
    }
  }
/*
  //Mat nWtgdest;
  //convertScaleAbs( Wtgabor, nWtgdest );
  output=(char *)malloc(2001*sizeof(char));
  output = CreateNameIntoFolder(im_name,"overallgaborfeature.png");
  imwrite(output,Wtgabor);
  Mat dest;
  threshold(Wtgabor,dest,10,255,0);
  output=(char *)malloc(2001*sizeof(char));
  output = CreateNameIntoFolder(im_name,"overallgaborfeature_threshold.png");
  imwrite(output,dest);
  */
  Wtgabor.release();
  
  
  
  
  
  printf("Done\n");
  
  image.release();
  return GaborAngleHist;
  
}


vector<float>  GetGaborFeature(Mat image)
{
  printf("\n\n\n");
  int kernel_size = 3; // 5
  Mat GaborDest,GaborSrc,Gaborkernel;
  Mat AntSymGKernel,SymGKernel;
  double sig = 1, th = 0, lm = 1.0, gm = 0.02; // sig = 1
  vector<float> GaborFeature;
  
  vector<double> th_gm;
  th_gm.push_back(0.02);
  //th_gm.push_back(0.025);
  th_gm.push_back(0.03);
 // th_gm.push_back(0.05);
 // th_gm.push_back(0.1);
 // th_gm.push_back(0.2);
  
  
  vector<double> th_sig;
 // th_sig.push_back(1.0);
 // th_sig.push_back(1.5);
 // th_sig.push_back(2.0);
 // th_sig.push_back(2.5);
  th_sig.push_back(0.3);
  th_sig.push_back(0.5);
  
  vector<double> theta;
  theta.push_back(30.0);
  theta.push_back(45.0);
  theta.push_back(60.0);
  theta.push_back(90.0);
  theta.push_back(120.0);
  theta.push_back(135.0);

  printf("Gabor Feature\n");
  
  for(int sg=0;sg<th_sig.size();sg++)
  {
    sig = th_sig[sg];
    for(int gam=0;gam<th_gm.size();gam++)
    {
      gm = th_gm[gam];
      //for(int th_i=30;th_i<=150;)
      //{
	//th = th_i*1.0;
      //th_i = th_i + 15;
      for(int th_i=0;th_i<theta.size();th_i++)
      {
	th = theta[th_i];
	int type = CV_32F;
	
	Gaborkernel = getGaborKernel(Size(kernel_size, kernel_size), sig, th, lm, gm,0.5*PI,type);
	
	SymGKernel = getGaborKernel(Size(kernel_size, kernel_size), sig, th, lm, gm,0.0,type);
	AntSymGKernel = getGaborKernel(Size(kernel_size, kernel_size), sig, th, lm, gm,0.5*PI,type);
	
	Mat gray;
	cvtColor(image,gray,CV_BGR2GRAY);
	gray.convertTo(GaborSrc,CV_32F);
	
	//filter2D(GaborSrc, GaborDest, CV_32F, Gaborkernel);
	
	Mat SymGDest,AntSymGDest;
	
	filter2D(GaborSrc, SymGDest, CV_32F, SymGKernel);
	
	filter2D(GaborSrc, AntSymGDest, CV_32F, AntSymGKernel);
	
	Mat GaborEnergy = Mat(GaborSrc.rows,GaborSrc.cols,CV_32FC1);
	Mat GaborMeanAmpli = Mat(GaborSrc.rows,GaborSrc.cols,CV_32FC1);
	
	for(int r=0;r<GaborSrc.rows;r++)
	{
	  for(int c=0;c<GaborSrc.cols;c++)
	  {
	    float sym = SymGDest.at<float>(r,c);
	    float antsym = AntSymGDest.at<float>(r,c);
	    float energy = sym*sym + antsym*antsym;
	    energy =(float) sqrt(energy);
	   // printf("Energy = %f\n",energy);
	    GaborEnergy.at<float>(r,c) = energy;
	  }
	}
	
	//printf("%lf\t",FindMean(GaborEnergy));
	//printf("StdDev Energy = %lf\n",FindStdDev(GaborEnergy));
	
	//printf("Mean Energy = %lf\t",FindMean(GaborDest));
	//printf("StdDev Energy = %lf\n",FindStdDev(GaborDest));
	GaborFeature.push_back((float) FindMean(GaborEnergy));
	GaborFeature.push_back((float) FindMean(GaborEnergy));
	
	//exit(0);
	
	//Mat abs_gabor;
	//convertScaleAbs( GaborDest, abs_gabor );

	GaborSrc.release();
	Gaborkernel.release();
	SymGKernel.release();
	AntSymGKernel.release();
	GaborEnergy.release();
	
	//GaborFeature.push_back((float) FindMean(abs_gabor));
	//GaborFeature.push_back((float) FindStdDev(abs_gabor));
	//abs_gabor.release();
	//GaborFeature.push_back((float) FindMean(GaborDest));
	//GaborFeature.push_back((float) FindStdDev(GaborDest));
	
	//Mat test;
	//convertScaleAbs( GaborDest, test );
	//GaborDest.copyTo(test);
	//GaborDest.release();
	
	
	
	
      }
    }
  }
  
  for(int i=0;i<GaborFeature.size();i++)
    printf("%f\t",GaborFeature[i]);
  printf("\n\n");
  
  image.release();
  return GaborFeature;
  
}