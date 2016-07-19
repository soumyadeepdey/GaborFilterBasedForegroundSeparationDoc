#include "StandardHeaders.h"
#include "GaborAngleFeature.h"
#include "folder.h"
#include "ColorQuantization.h"
#include "SmoothingGapfilling.h"
#include "Image_proc_functions.h"
#include "binarization.h"

using namespace IITkgp_functions;

int main(int argc, char* argv[])
{
  Mat src = imread(argv[1],1);
  char *name;
  name = input_image_name_cut(argv[1]);
  makedir(name);
  
  Mat blurimage; 
   GaussianBlur(src,blurimage,Size(3,3),0,0);
  
   char *output;
  
  output=(char *)malloc(2001*sizeof(char));
  output = CreateNameIntoFolder(name,"bluredimage.png");
  imwrite(output,blurimage);
  
  /*
  Mat QuantizedImage = ColorQuantize(blurimage,24);
  
  output=(char *)malloc(2001*sizeof(char));
  output = CreateNameIntoFolder(name,"quantizeimage1.png");
  imwrite(output,QuantizedImage);
  
  QuantizedImage = ColorQuantize(QuantizedImage,8);
  
  output=(char *)malloc(2001*sizeof(char));
  output = CreateNameIntoFolder(name,"quantizeimage2.png");
  imwrite(output,QuantizedImage);
  */
  
  /************************Gradient Computation***************************************/
  
  Mat gray;
  int ddepth = CV_8U;
  int scale = 1;
  int delta = 0;
  Mat grad;
  
  cvtColor(blurimage,gray,CV_BGR2GRAY);
  
  /// Generate grad_x and grad_y
  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;

  /// Gradient X
  //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
  Sobel( gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( grad_x, abs_grad_x );

  /// Gradient Y
  //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  Sobel( gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( grad_y, abs_grad_y );

  /// Total Gradient (approximate)
  addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
  
  
  output=(char *)malloc(2001*sizeof(char));
  output = CreateNameIntoFolder(name,"GradientImage.png");
  imwrite(output,grad);
  
  
  /***************************************************************/
  
  Mat erodedimage = Erosion(0,1,blurimage);
  
  
  
  Mat dilatedimage = Dilation(0,1,blurimage);
  

  
  Mat boundaryimage; 
  subtract(dilatedimage,erodedimage,boundaryimage);

  output=(char *)malloc(2001*sizeof(char));
  output = CreateNameIntoFolder(name,"boundaryimage.png");
  imwrite(output,boundaryimage);
  erodedimage.release();
  dilatedimage.release();
 
  /***************************************************************/
  
 
  
  Mat invimg = FindImageInverse(boundaryimage);
  output=(char *)malloc(2001*sizeof(char));
  output = CreateNameIntoFolder(name,"InvBoundary.png");
  imwrite(output,invimg);
  
  Mat Boundary_Binary = binarization(invimg,1);
  
  Mat Binary = binarization(src,1);
  
  output=(char *)malloc(2001*sizeof(char));
  output = CreateNameIntoFolder(name,"BinImage.png");
  imwrite(output,Binary);
  
  
  /*
  Mat Gr;
  cvtColor(boundaryimage,Gr,CV_BGR2GRAY);  // Change this for checking gradient
  
  //grad.copyTo(Gr);
  
  threshold(Gr,Gr,35,255,1);
  
  
  
  Mat bin = Mat(Gr.rows,Gr.cols,CV_8UC1,Scalar(255));
  for(int i=0;i<boundaryimage.rows;i++)
  {
    for(int j=0;j<boundaryimage.cols;j++)
    {
      if(Gr.at<uchar>(i,j) == 0)
	bin.at<uchar>(i,j) = 0;
      else
	bin.at<uchar>(i,j) = 255;
    }
  }
  
  output=(char *)malloc(2001*sizeof(char));
  output = CreateNameIntoFolder(name,"BinImage.png");
  imwrite(output,bin);
  */
  /*
  Mat Uniformimage;
  boundaryimage.copyTo(Uniformimage);
  for(int i=0;i<boundaryimage.rows;i++)
  {
    for(int j=0;j<boundaryimage.cols;j++)
    {
      if(Binary.at<uchar>(i,j) == 255)
      {
	Uniformimage.at<Vec3b>(i,j)[0] = 0;
	Uniformimage.at<Vec3b>(i,j)[1] = 0;
	Uniformimage.at<Vec3b>(i,j)[2] = 0;
      }
    }
  }
  
  output=(char *)malloc(2001*sizeof(char));
  output = CreateNameIntoFolder(name,"uniboundaryimage.png");
  imwrite(output,Uniformimage);
  */
  boundaryimage.release();
  
  Mat Hgap = horizontal_gapfilling(Binary,8);
  Mat Vgap = vertical_gapfilling(Hgap,5);
  
  Hgap.release();
  
   vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  
  Mat temp;
  Vgap.copyTo(temp);
  temp = FindImageInverse(temp);
  
  /// Find contours
      //findContours( newdest, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
      //findContours( newdest, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
      //findContours( temp, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
      findContours( temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
      //findContours( Gr, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
   temp.release();   
      
      /// Approximate contours to polygons + get bounding rects and circles
      vector<vector<Point> > contours_poly( contours.size() );
      vector<Rect> boundRect( contours.size() );
      //vector<RotatedRect> boundRotatedRect ( contours.size() );


      
      for( int j = 0; j < contours.size(); j++ )
	{ 
	  approxPolyDP( Mat(contours[j]), contours_poly[j], 3, true );
	  boundRect[j] = boundingRect( Mat(contours_poly[j]) );
	 // boundRotatedRect[j] = minAreaRect( Mat(contours_poly[j]) );
	}
	
      Mat cont = Mat(src.rows,src.cols,CV_8UC3,Scalar(255,255,255));;
      src.copyTo(cont);

     int avg_height = 0;
     int count = 0;
     for( int j = 0; j < contours.size(); j++ )
     {
       if(hierarchy[j][3] == -1)
       {
	 rectangle( cont, boundRect[j].tl(), boundRect[j].br(), Scalar(100,100,10), 2, 8, 0 );
	 //drawContours(cont, contours, j, Scalar(0, 0, 250), CV_FILLED);
	 avg_height = avg_height +  boundRect[j].height;
	 count++;
       }
     }

     avg_height = avg_height/count;
     
     
     Mat temp_img;
     int p,q;
     for( int j = 0; j < contours.size(); j++ )
     {
       if(hierarchy[j][3] == -1 && boundRect[j].height > 4*avg_height && boundRect[j].width > 5*avg_height)
       {
	 printf("I am there\n");
	 //rectangle( cont, boundRect[j].tl(), boundRect[j].br(), Scalar(100,10,200), 2, 8, 0 );
	 
	 temp_img = Mat(boundRect[j].height,boundRect[j].width,CV_8UC1,Scalar(255));
	 p = 0;
	 for(int m=boundRect[j].y;m<boundRect[j].y+boundRect[j].height;m++)
	 {
	    q = 0;
	    for(int n=boundRect[j].x;n<boundRect[j].x+boundRect[j].width;n++)
	    {
	      int temp_col = boundRect[j].width;
	      bool measure_dist;
	      {
		if(pointPolygonTest(contours_poly[j],Point(n,m),measure_dist) > 0.0)
		{
		  temp_img.at<uchar>(p,q) = Boundary_Binary.at<uchar>(m,n);		  
		}
	      }
	      q++;
	    }
	    p++;
	 }
	 temp_img = FindImageInverse(temp_img);
	 vector<vector<Point> > temp_contours;
	 vector<Vec4i> temp_hierarchy;
	 findContours( temp_img, temp_contours, temp_hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	 vector<vector<Point> > temp_contours_poly( temp_contours.size() );
	 vector<Rect> temp_boundRect( temp_contours.size() );
	 for( int l = 0; l < temp_contours.size(); l++ )
	 { 
	    approxPolyDP( Mat(temp_contours[l]), temp_contours_poly[l], 3, true );
	    temp_boundRect[l] = boundingRect( Mat(temp_contours_poly[l]) );
	 }
	 for( int l = 0; l < temp_contours.size(); l++ )
	 {
	    if(temp_hierarchy[l][3] == -1 && ((temp_boundRect[l].area()/boundRect[j].area()) < 0.8))
	    {
	      Point top = boundRect[j].tl(); Point bottom = boundRect[j].tl();
	      top.x = top.x+temp_boundRect[l].x;
	      top.y = top.y+temp_boundRect[l].y;
	      bottom.x = bottom.x+temp_boundRect[l].x+temp_boundRect[l].width;
	      bottom.y = bottom.y+temp_boundRect[l].y+temp_boundRect[l].height;
	      rectangle( cont, top, bottom, Scalar(0,10,230), 2, 8, 0 );
	    }
	  }
	 temp_img.release();
	 
       }
     }

  output = CreateNameIntoFolder(name,"contour10.png");
  imwrite(output,cont);
  
 // Binary.release();
  /*
  Gr.release();
  Gr = FindImageInverse(Vgap);
  Hgap.release();
  */
  /*
  Mat newdest = Dilation(0,2,Uniformimage);
  output=(char *)malloc(2001*sizeof(char));
  output = CreateNameIntoFolder(name,"Boundary_dilatedimage.png");
  imwrite(output,newdest);
  
  cvtColor(newdest,newdest,CV_BGR2GRAY);
  */
  
 // Mat Gr;
 // cvtColor(QuantizedImage,Gr,CV_BGR2GRAY);
  
  
  //threshold(Gr,Gr,30,255,0);
 
  
  /*vector<vector<float> > GFData;
  GFData = GetMyGaborFeature(QuantizedImage,name);*/
  
  return 0;
  
}