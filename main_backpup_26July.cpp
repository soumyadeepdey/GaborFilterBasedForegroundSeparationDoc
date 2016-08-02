#include "StandardHeaders.h"
#include "GaborAngleFeature.h"
#include "folder.h"
#include "ColorQuantization.h"
#include "SmoothingGapfilling.h"
#include "Image_proc_functions.h"
#include "binarization.h"
#include "RectangleTest.h"
#include "connectedcomponent.h"
#include "AlethiaParser/AlethiaParser.h"
#include "PrepareAlethiaGT.h"
#include "SegmentationUnit.h"
#include "FeatureExtraction.h"

RNG rng(12345);


using namespace IITkgp_functions;

int main(int argc, char* argv[])
{
  
  page pg = GetPageGroundtruth(argv[1]);
  
  char *name;
  name = input_image_name_cut(pg.GetImageName());
  makedir(name);
  
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
  Mat src = imread(tnm1,1);
  
  
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
  int ddepth = CV_32F;
  int scale = 1;
  int delta = 0;
  Mat grad;
  
  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;
  Mat abs_grad;
  
  /*
  vector<Mat> RGBImage;
  vector<Mat> RGBGrad;
  split(blurimage,RGBImage);
  for(int i=0;i<RGBImage.size();i++)
  {
    Sobel( RGBImage[i], grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );
    Sobel( RGBImage[i], grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_y, abs_grad_y );
    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, abs_grad );
    RGBGrad.push_back(abs_grad);
    char *tname; 
	tname=(char *)malloc(2001*sizeof(char));
	sprintf(tname, "Gradient_%d.png",i);
	
	output=(char *)malloc(2001*sizeof(char));
	output = CreateNameIntoFolder(name,tname);
	imwrite(output,abs_grad);
    abs_grad.release();
    
  }

  */
  
  cvtColor(blurimage,gray,CV_BGR2GRAY);
  
  /// Generate grad_x and grad_y
  

  /// Gradient X
  //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
  Sobel( gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( grad_x, abs_grad_x );

  /// Gradient Y
  //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  Sobel( gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( grad_y, abs_grad_y );

  /// Total Gradient (approximate)
  addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, abs_grad );
  //addWeighted( grad_x, 0.5, grad_y, 0.5, 0, grad );
  //convertScaleAbs( grad, abs_grad );
  
  output=(char *)malloc(2001*sizeof(char));
  output = CreateNameIntoFolder(name,"GradientImage.png");
  imwrite(output,abs_grad);
  
  Mat GradMag = Mat(gray.rows,gray.cols,CV_32FC1);
  Mat GradDir = Mat(gray.rows,gray.cols,CV_32FC1);
  
  for(int i=0;i<gray.rows;i++)
  {
    for(int j=0;j<gray.cols;j++)
    {
      GradMag.at<float>(i,j) = sqrt((grad_x.at<float>(i,j)*grad_x.at<float>(i,j))+(grad_y.at<float>(i,j)*grad_y.at<float>(i,j)));
      GradDir.at<float>(i,j) = (atan2(grad_y.at<float>(i,j), grad_x.at<float>(i,j))*180)/PI;
    }
  }
  grad_x.release();
  grad_y.release();
  abs_grad.release();
  abs_grad_x.release();
  abs_grad_y.release();
  
  /***************************************************************/
  
  //Mat mor_temp = Morphology_Gui(blurimage,0);
  
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
  
  Mat Uniformimage;
  boundaryimage.copyTo(Uniformimage);
  for(int i=0;i<boundaryimage.rows;i++)
  {
    for(int j=0;j<boundaryimage.cols;j++)
    {
      if(Boundary_Binary.at<uchar>(i,j) == 255)
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
  
  //boundaryimage.release();
  
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
	
      vector<SB> blocks;
	
      Mat cont = Mat(src.rows,src.cols,CV_8UC3,Scalar(255,255,255));
      //src.copyTo(cont);

     int avg_height = 0;
     int count = 0;
     vector<bool> flag(contours.size(),false);
     for( int j = 0; j < contours.size(); j++ )
     {
       if(hierarchy[j][3] == -1 && boundRect[j].height > 6 &&  boundRect[j].width > 10)
       {
	 rectangle( cont, boundRect[j].tl(), boundRect[j].br(), Scalar(100,100,10), 2, 8, 0 );
	 //drawContours(cont, contours, j, Scalar(0, 0, 250), CV_FILLED);
	 avg_height = avg_height +  boundRect[j].height;
	 count++;
	 SB B;
	 B.B = boundRect[j];
	 B.Contours = contours_poly[j];
	 B.gtflag=false;
	 B.Fvecflag = false;
	 blocks.push_back(B);
	 
	 flag[j]=true;
       }
     }

     avg_height = avg_height/count;
     
     for(int i=0;i<blocks.size();i++)
     {
       for(int j=0;j<blocks.size();j++)
       {
	 if(i!=j)
	 {
	   if(PolygonInsidePolygonTest(blocks[i].Contours,blocks[j].Contours)==2)
	     blocks.erase(blocks.begin()+j);
	 }
       }
     }
     
     
     Mat temp_img;
     int p,q;
     int bc = 0;
     for( int j = 0; j < contours.size(); j++ )
     {
       if(hierarchy[j][3] == -1 && boundRect[j].height > 3*avg_height && boundRect[j].width > 3*avg_height && flag[j])
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
	      bool measure_dist = false;
	      {
		if(pointPolygonTest(contours_poly[j],Point(n,m),measure_dist) <= 0.0)
		{
		  temp_img.at<uchar>(p,q) = Boundary_Binary.at<uchar>(m,n);		  
		}
	      }
	      q++;
	    }
	    p++;
	 }
	 
	char *tname; 
	tname=(char *)malloc(2001*sizeof(char));
	sprintf(tname, "blk_%d.png",j);
	
	output=(char *)malloc(2001*sizeof(char));
	output = CreateNameIntoFolder(name,tname);
	imwrite(output,temp_img);
	 
	 nocc *component = FindConnectedComponent(temp_img,temp_img);
	 vector<vector<Point> > temp_contours = Convert_nocc2pointvector(component);
	 printf("Done\n");
	 vector<vector<Point> > temp_contours_poly( temp_contours.size() );
	 vector<Rect> temp_boundRect( temp_contours.size() );
	 for( int l = 0; l < temp_contours.size(); l++ )
	 {
	   for(int m=0;m< temp_contours[l].size();m++)
	   {
	     temp_contours[l][m].x = temp_contours[l][m].x + boundRect[j].x;
	     temp_contours[l][m].y = temp_contours[l][m].y + boundRect[j].y;
	   }
	 }
	 for( int l = 0; l < temp_contours.size(); l++ )
	 { 
	    approxPolyDP( Mat(temp_contours[l]), temp_contours_poly[l], 3, true );
	    temp_boundRect[l] = boundingRect( Mat(temp_contours_poly[l]) );
	 }
	 vector<Vec4i> temp_hierarchy(temp_contours.size());
	 for( int l = 0; l < temp_contours.size(); l++ )
	 {
	   temp_hierarchy[l][0] = -1;
	   temp_hierarchy[l][1] = -1;
	   temp_hierarchy[l][2] = -1;
	   temp_hierarchy[l][3] = -1;
	 }
	 for( int l = 0; l < temp_contours.size(); l++ )
	 {
	   float frac = (temp_boundRect[l].area()*1.0)/(boundRect[j].area()*1.0);
	   if(frac < 0.7)
	   {
	    for(int m=0;m< temp_contours.size();m++)
	    {
	      float frac1 = (temp_boundRect[m].area()*1.0)/(boundRect[j].area()*1.0);
	      if(frac1 < 0.7 && l!=m)
	      {
		  //int val = FindOverlappingRectangles(temp_boundRect[l],temp_boundRect[m]);
		  int val = PolygonInsidePolygonTest(temp_contours_poly[l],temp_contours_poly[m]);
		  if(val==2)
		  {
		   // printf("Working\n");
		    temp_hierarchy[m][3] = l;
		    if(temp_hierarchy[l][2]!=-1)
		      temp_hierarchy[l][2] = m;
		  }
		
	      }
	    }
	  }
	 }
	 for( int l = 0; l < temp_contours.size(); l++ )
	 {
	    float frac = (temp_boundRect[l].area()*1.0)/(boundRect[j].area()*1.0);
	    if(temp_hierarchy[l][3] == -1 && frac < 0.7)
	    //if(temp_hierarchy[l][3] == -1)
	    {
	      //Point top = boundRect[j].tl(); Point bottom = boundRect[j].br();
	      //top.x = top.x+temp_boundRect[l].x;
	      //top.y = top.y+temp_boundRect[l].y;
	      //bottom.x = bottom.x+temp_boundRect[l].x+temp_boundRect[l].width;
	      //bottom.y = bottom.y+temp_boundRect[l].y+temp_boundRect[l].height;
	      rectangle( cont, temp_boundRect[l].tl(), temp_boundRect[l].br(), Scalar(0,10,230), 2, 8, 0 );
	      Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
	      drawContours(cont, temp_contours, l, color, CV_FILLED);
	     // Rect R;
	      //R.x = top.x;
	     // R.y = top.y;
	     // R.height = temp_boundRect[l].height;
	     // R.width = temp_boundRect[l].width;
	      //blocks[bc].child.push_back(R);
	      SB child_B;
	      child_B.B = temp_boundRect[l];
	      child_B.Contours = temp_contours[l];
	      child_B.gtflag = false;
	      blocks[bc].childs.push_back(child_B);
	      //blocks[bc].child.push_back(temp_boundRect[l]);
	      //blocks[bc].ChildContour.push_back(temp_contours[l]);
	    }
	    
	 }
	 
	 /*
	  * 
	  Mat Temp_Hgap = horizontal_gapfilling(temp_img,8);
	 Mat Temp_Vgap = vertical_gapfilling(Temp_Hgap,5);
	 Temp_Hgap.release();
	
	 temp_img = FindImageInverse(Temp_Vgap);
	 Temp_Vgap.release();
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
	   float frac = (temp_boundRect[l].area()*1.0)/(boundRect[j].area()*1.0);
	    if(temp_hierarchy[l][3] == -1 && (frac < 0.9) && temp_boundRect[l].height > 6 &&  temp_boundRect[l].width > 10)
	    {
	      Point top = boundRect[j].tl(); Point bottom = boundRect[j].tl();
	      top.x = top.x+temp_boundRect[l].x;
	      top.y = top.y+temp_boundRect[l].y;
	      bottom.x = bottom.x+temp_boundRect[l].x+temp_boundRect[l].width;
	      bottom.y = bottom.y+temp_boundRect[l].y+temp_boundRect[l].height;
	      rectangle( cont, top, bottom, Scalar(0,10,230), 2, 8, 0 );
	      Rect R;
	      R.x = top.x;
	      R.y = top.y;
	      R.height = temp_boundRect[l].height;
	      R.width = temp_boundRect[l].width;
	      blocks[bc].child.push_back(R);
	    }
	  }
	  */
	  
	  
	 temp_img.release();
	 bc++;
       }
     }
      output = CreateNameIntoFolder(name,"contour10.png");
      imwrite(output,cont);
      
      
      
     for(int i=0;i<blocks.size();i++)
     {
       SB B = blocks[i];
       if(!B.childs.empty()) // 
       {
	 vector<SB> C = B.childs;
	 for(int j=0;j<C.size();j++)
	 {
	   for(int k=0;k<C.size();k++)
	   {
	     if(j!=k)
	     {
	       //int val = PolygonInsidePolygonTest(C[j].Contours,C[k].Contours);
	       int val = FindOverlappingRectangles(C[j].B,C[k].B);
	       if(val == 2)
	       {
		 C.erase(C.begin()+k);
	       }
	     }
	   }
	 }
	 blocks[i].childs = C;
       }
     }

     cont.release();
     //cont = Mat(src.rows,src.cols,CV_8UC3,Scalar(255,255,255));
     src.copyTo(cont);
     for(int i=0;i<blocks.size();i++)
     {
       rectangle( cont, blocks[i].B.tl(), blocks[i].B.br(), Scalar(250,10,10), 2, 8, 0 );
       if(!blocks[i].childs.empty())
       {
	 vector<SB> C = blocks[i].childs;
	 for(int j=0;j<C.size();j++)
	 {
	   rectangle( cont, C[j].B.tl(), C[j].B.br(), Scalar(10,10,250), 2, 8, 0 );
	 }
       }
     }
     
     output = CreateNameIntoFolder(name,"contour11.png");
      imwrite(output,cont);
      
      blocks = PrepareAlethiaGt(pg,blocks);
      
      for(int i=0;i<blocks.size();i++)
     {
       SB B = blocks[i];
       vector<Point> poly;
       approxPolyDP( Mat(B.Contours), poly, 3, true );
       Rect R = boundingRect( Mat(poly) );
       Mat tempimg = Mat(R.height,R.width,blurimage.type(),Scalar(255,255,255));
       for(int m=0;m<R.height;m++)
       {
	 for(int n=0;n<R.width;n++)
	 {
	   Point P;P.x=n;P.y=m;
	   if(pointPolygonTest( poly, P, false )<=0)
	   {
	     for(int k=0;k<blurimage.channels();k++)
	       tempimg.at<Vec3b>(m,n)[k] = blurimage.at<Vec3b>(m,n)[k];
	   }
	 }
       }
       B.FeatureVec = ExtractFeature(tempimg);
       B.Fvecflag = true;
       tempimg.release();
       poly.clear();
       
       if(!B.childs.empty())
       {
	 vector<SB> C = B.childs;
	 for(int j=0;j<C.size();j++)
	 {
	    SB B1 = C[j];
	    approxPolyDP( Mat(B1.Contours), poly, 3, true );
	    Rect R1 = boundingRect( Mat(poly) );
	    tempimg = Mat(R1.height,R1.width,blurimage.type(),Scalar(255,255,255));
	    for(int m=0;m<R1.height;m++)
	    {
	      for(int n=0;n<R1.width;n++)
	      {
		Point P1;P1.x=n;P1.y=m;
		if(pointPolygonTest( poly, P1, false )<=0)
		{
		  for(int k=0;k<blurimage.channels();k++)
		    tempimg.at<Vec3b>(m,n)[k] = blurimage.at<Vec3b>(m,n)[k];
		}
	      }
	    }
	    B1.FeatureVec = ExtractFeature(tempimg);
	    B1.Fvecflag = true;
	    tempimg.release();
	    poly.clear();
	    C[j]= B1;
	   
	 }
	 B.childs = C;
	 C.clear();
       }
       blocks[i] = B;
     }
      
      
  
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