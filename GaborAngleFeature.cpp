
#include "GaborAngleFeature.h"

vector<vector<float> > GetMyGaborFeature(Mat image, char *im_name)
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
  
  Mat Wtgabor;
  
  for(int sg=0;sg<th_sig.size();sg++)
  {
    sig = th_sig[sg];
    for(int gam=0;gam<th_gm.size();gam++)
    {
      gm = th_gm[gam];
      for(int th_i=30;th_i<=150;)
      {
	th = th_i*1.0;
	
      
	Gaborkernel = getGaborKernel(Size(kernel_size, kernel_size), sig, th, lm, gm);
	
	Mat gray;
	cvtColor(image,gray,CV_BGR2GRAY);
	gray.convertTo(GaborSrc,CV_32F);
	
	filter2D(GaborSrc, GaborDest, CV_32F, Gaborkernel);

	GaborSrc.release();
	Gaborkernel.release();
	
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

	/*
	char *name;
	
	char *output;
	
	
	
	
	name=(char *)malloc(2001*sizeof(char));
	sprintf(name, "Gresponse%lf_%lf_%d.png",sig,gm,th_i);
	
	output=(char *)malloc(2001*sizeof(char));
	output = CreateNameIntoFolder(im_name,name);
	imwrite(output,test);
	*/
	//
	test.release();
	
	th_i = th_i + 15;
      }
    }
  }

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
  Wtgabor.release();
  
  
  
  
  
  printf("Done\n");
  
  image.release();
  return GaborAngleHist;
  
}