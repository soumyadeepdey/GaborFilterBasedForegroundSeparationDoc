
#include "GaborAngleFeature.h"

vector<vector<float> > GetMyGaborFeature(Mat image, char *im_name)
{
  
  printf("%s\n",im_name);
  makedir(im_name);
  
  Mat erodedimage = Erosion(0,1,image);
  
  char *output;
  
  
  Mat dilatedimage = Dilation(0,1,image);
  
  
  Mat boundaryimage; 
  subtract(dilatedimage,erodedimage,boundaryimage);

  output=(char *)malloc(2001*sizeof(char));
  output = CreateNameIntoFolder(im_name,"boundaryimage.png");
  imwrite(output,boundaryimage);
  erodedimage.release();
  dilatedimage.release();
  
  Mat Gr;
  cvtColor(boundaryimage,Gr,CV_BGR2GRAY);
  threshold(Gr,Gr,30,255,0);
  
  Mat Uniformimage;
  boundaryimage.copyTo(Uniformimage);
  for(int i=0;i<boundaryimage.rows;i++)
  {
    for(int j=0;j<boundaryimage.cols;j++)
    {
      if(Gr.at<uchar>(i,j) == 0)
      {
	Uniformimage.at<Vec3b>(i,j)[0] = 0;
	Uniformimage.at<Vec3b>(i,j)[1] = 0;
	Uniformimage.at<Vec3b>(i,j)[2] = 0;
      }
    }
  }
  
  output=(char *)malloc(2001*sizeof(char));
  output = CreateNameIntoFolder(im_name,"uniboundaryimage.png");
  imwrite(output,Uniformimage);
  boundaryimage.release();
  
  
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
	GaborDest.release();
	if(Wtgabor.empty())
	  test.copyTo(Wtgabor);
	
	/// Total Gradient (approximate)
	addWeighted( Wtgabor, 0.5, test, 0.5, 0, Wtgabor );
      
	vector<uchar> h_data;

	for(int i=0;i<test.rows;i++)
	{
	  for(int j=0;j<test.cols;j++)
	  {
	    if(test.at<uchar>(i,j) != 0)
	      h_data.push_back(test.at<uchar>(i,j));
	  }
	}
	
	Mat HistD = Mat(h_data.size(),1,CV_8UC1,h_data.data());
	h_data.clear();
	vector<float> thist = FindHist(HistD);
	GaborAngleHist.push_back(thist);
	

	
	char *name;
	
	char *output;
	
	thist.clear();
	
	
	name=(char *)malloc(2001*sizeof(char));
	sprintf(name, "Gresponse%lf_%lf_%d.png",sig,gm,th_i);
	
	output=(char *)malloc(2001*sizeof(char));
	output = CreateNameIntoFolder(im_name,name);
	
	//imwrite(output,test);
	test.release();
	
	th_i = th_i + 15;
      }
    }
  }

  output=(char *)malloc(2001*sizeof(char));
  output = CreateNameIntoFolder(im_name,"overallgaborfeature.png");
  imwrite(output,Wtgabor);
  Mat dest;
  threshold(Wtgabor,dest,30,255,0);
  output=(char *)malloc(2001*sizeof(char));
  output = CreateNameIntoFolder(im_name,"overallgaborfeature_threshold.png");
  imwrite(output,dest);
  Wtgabor.release();
  
  Mat newdest = Dilation(0,1,dest);
  
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  
  /// Find contours
      findContours( newdest, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );	
      
      
      /// Approximate contours to polygons + get bounding rects and circles
      vector<vector<Point> > contours_poly( contours.size() );
      vector<Rect> boundRect( contours.size() );
      vector<RotatedRect> boundRotatedRect ( contours.size() );


      
      for( int j = 0; j < contours.size(); j++ )
	{ approxPolyDP( Mat(contours[j]), contours_poly[j], 3, true );
	  boundRect[j] = boundingRect( Mat(contours_poly[j]) );
	  boundRotatedRect[j] = minAreaRect( Mat(contours_poly[j]) );
	}
	
      Mat cont;
      image.copyTo(cont);
      
     for( int j = 0; j < contours.size(); j++ )
     {
       if(hierarchy[j][2] == -1)
       {
	 rectangle( cont, boundRect[j].tl(), boundRect[j].br(), Scalar(120,70,150), 2, 8, 0 );
       }
     }
  
  output = CreateNameIntoFolder(im_name,"contour.png");
  imwrite(output,cont);
  
  printf("Done\n");
  
  image.release();
  return GaborAngleHist;
  
}