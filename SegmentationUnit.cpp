#include "SegmentationUnit.h"
#include "Morphology.h"
#include "binarization.h"
#include "Image_proc_functions.h"
#include "SmoothingGapfilling.h"
#include "RectangleTest.h"
#include "connectedcomponent.h"
#include "FeatureExtraction.h"
#include "ScalarColorFeature.h"

using namespace IITkgp_functions;

Mat BoundarytoFullimage(Mat src)
{
   Mat inv = FindImageInverse(src);
  vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    
    findContours( inv, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    
    //vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    
    for( int j = 0; j < contours.size(); j++ )
    { 
     // approxPolyDP( Mat(contours[j]), contours_poly[j], 3, true );
      boundRect[j] = boundingRect( Mat(contours[j]) );
    }
    imwrite("temp.png",src);
    //Mat temp = imread("temp.png",1);
    Mat temp = Mat(src.rows,src.cols,CV_8UC1,Scalar(255));
    for( int j = 0; j < contours.size(); j++ )
    {
      if(hierarchy[j][3] != -1)
      {
	int posi = hierarchy[j][3];
	if(hierarchy[posi][3] == -1)
	{
	  //rectangle(temp,boundRect[j].br(),boundRect[j].tl(),Scalar(255,0,0),1,8);
	  for(int m=boundRect[j].y;m<boundRect[j].y+boundRect[j].height;m++)
		{
		    for(int n=boundRect[j].x;n<boundRect[j].x+boundRect[j].width;n++)
		    {
		      bool measure_dist = false;
		      {			
			if(pointPolygonTest(contours[j],Point(n,m),measure_dist) >= 0.0)
			{	
			  temp.at<uchar>(m,n)= 0;
			}
		      }
		    }
		}
	}
	else
	{
	  for(int m=boundRect[j].y;m<boundRect[j].y+boundRect[j].height;m++)
		{
		    for(int n=boundRect[j].x;n<boundRect[j].x+boundRect[j].width;n++)
		    {
		      bool measure_dist = false;
		      {			
			if(pointPolygonTest(contours[j],Point(n,m),measure_dist) >= 0.0)
			{	
			  temp.at<uchar>(m,n) = 255;	
			}
		      }
		    }
		}
	}
      }
      else
      {
	for(int m=boundRect[j].y;m<boundRect[j].y+boundRect[j].height;m++)
		{
		    for(int n=boundRect[j].x;n<boundRect[j].x+boundRect[j].width;n++)
		    {
		      bool measure_dist = false;
		      {			
			if(pointPolygonTest(contours[j],Point(n,m),measure_dist) >= 0.0)
			{	
			  temp.at<uchar>(m,n)= 255;	
			}
		      }
		    }
		}
      }
    }
    
    return temp;
}
vector<SB> GetProcessingBlocks(Mat image)
{
  Mat blurimage; 
    GaussianBlur(image,blurimage,Size(3,3),0,0);
    
    Mat erodedimage = Erosion(0,1,blurimage); 
    Mat dilatedimage = Dilation(0,1,blurimage);
    
    Mat boundaryimage; 
    subtract(dilatedimage,erodedimage,boundaryimage);
   
    Mat invimg = FindImageInverse(boundaryimage);
   
    Mat Boundary_Binary = binarization(invimg,1);
    
   // imwrite("Boundary_Binary.png",Boundary_Binary);
    
    Mat BB_B = boundaryextraction(Boundary_Binary);
   // imwrite("Boundary_Binary_bound.png",BB_B);
  
    Mat Binary = binarization(image,1);
    
    Mat Hgap = horizontal_gapfilling(Binary,8);
    Mat Vgap = vertical_gapfilling(Hgap,5);
    
    Hgap.release();
  
    Mat temp;
    Vgap.copyTo(temp);
    temp = FindImageInverse(temp);
    
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    
    findContours( temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    vector<Rect> boundRect( contours.size() );
    
    for( int j = 0; j < contours.size(); j++ )
    { 
      boundRect[j] = boundingRect( Mat(contours[j]) );
    }
    
    vector<SB> blocks;
     int avg_height = 0;
     int count = 0;
     
     Mat Draw;
     blurimage.copyTo(Draw);
     
     for( int j = 0; j < contours.size(); j++ )
     {
       if(hierarchy[j][3] == -1 && boundRect[j].height > 6 &&  boundRect[j].width > 10)
       {
	 avg_height = avg_height +  boundRect[j].height;
	 count++;
	 SB B;
	 B.B = boundRect[j];
	 B.Contours = contours[j];
	 B.gtflag=false;
	 B.Fvecflag = false;	    
	 blocks.push_back(B);
	 //rectangle(Draw,boundRect[j].tl(),boundRect[j].br(),Scalar(255,0,0),8,1);
       }
     }
     //imwrite("INiblocks.png",Draw);
     //Draw.release();
     
     boundRect.clear();
     contours.clear();
     hierarchy.clear();

     avg_height = avg_height/count;
     
     
     for(int i=0;i<blocks.size();i++)
     {
       for(int j=0;j<blocks.size();)
       {
	 if(i!=j)
	 {
	   if(PolygonInsidePolygonTest(blocks[i].Contours,blocks[j].Contours)==2)
	   {
	     blocks.erase(blocks.begin()+j);
	   }
	   else
	     j=j+1;
	 }
	 else
	   j=j+1;
       }
     }
     
     for(int i=0;i<blocks.size();i++)
     {
       printf("Block %d\n",i);
	SB B = blocks[i];
	Rect R = B.B;
	 Mat temp_clrimg = Mat(R.height,R.width,CV_8UC3,Scalar(255,255,255));
	 Mat temp_bin = Mat(R.height,R.width,CV_8UC1,Scalar(255));
	 Mat temp_img = Mat(R.height,R.width,CV_8UC1,Scalar(255));
	 Mat temp =  Mat(R.height,R.width,CV_8UC1,Scalar(255));
	 Mat stroke_clrimg = Mat(R.height,R.width,CV_8UC3,Scalar(255,255,255));
	 int cnt = 0;
	    int p = 0;
	    for(int m=R.y;m<R.y+R.height;m++)
	    {
		int q = 0;
		for(int n=R.x;n<R.x+R.width;n++)
		{
		  bool measure_dist = true;
		  {		    
		    if(pointPolygonTest(B.Contours,Point(n,m),measure_dist) >= 0.0)
		    {
		      for(int k=-0;k<blurimage.channels();k++)
			temp_clrimg.at<Vec3b>(p,q)[k] = blurimage.at<Vec3b>(m,n)[k];
		      temp_bin.at<uchar>(p,q) = Binary.at<uchar>(m,n);	
		      temp_img.at<uchar>(p,q) = Boundary_Binary.at<uchar>(m,n);	
		      cnt++;
		    }
		  }
		  q++;
		}
		p++;
	    }
	    
	    
	    
	 if(R.height>3*avg_height && R.width > 3*avg_height) // Large component
	 {
	   rectangle(Draw,R.tl(),R.br(),Scalar(255,0,0),8,1);
	   contours.clear();
	   hierarchy.clear();
	   boundRect.clear();
	   Rect tr;
	   tr.x=3;tr.y=3;tr.height=R.height-6;tr.width=R.width-6;
	   printf("x=%d\ty=%d\th=%d\tw=%d\n",tr.x,tr.y,tr.height,tr.width);
	   char *name1=(char *)malloc(2001*sizeof(char));
	   sprintf(name1, "Block_inv_%d.png",i);
	   
	   Mat Tmp_clrimg = Mat(tr.height,tr.width,CV_8UC3);
	    Mat inv = Mat(tr.height,tr.width,CV_8UC1);
	   for(int r=0;r<tr.height;r++)
	   {
	     for(int c=0;c<tr.width;c++)
	     {
	       inv.at<uchar>(r,c) = temp_img.at<uchar>(r+3,c+3);
	       for(int k=0;k<temp_clrimg.channels();k++)
		Tmp_clrimg.at<Vec3b>(r,c)[k] = temp_clrimg.at<Vec3b>(r+3,c+3)[k];
	     }
	   }
	  // imwrite(name1,inv);
	   inv = FindImageInverse(inv);
	  // imwrite(name1,inv);
	   
	   findContours( inv, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	   
	   boundRect.resize(contours.size());
    
	    for( int j = 0; j < contours.size(); j++ )
	    { 
	      boundRect[j] = boundingRect( Mat(contours[j]) );
	    }
	   
	   temp =  Mat(inv.rows,inv.cols,CV_8UC1,Scalar(255));
	   stroke_clrimg = Mat(inv.rows,inv.cols,CV_8UC3,Scalar(255,255,255));
	   cnt = 0;
	   for( int j = 0; j < contours.size(); j++ )
	    {
	      if(hierarchy[j][3] != -1)
	      {
		int posi = hierarchy[j][3];
		if(hierarchy[posi][3] == -1)
		{
		  //rectangle(temp,boundRect[j].br(),boundRect[j].tl(),Scalar(255,0,0),1,8);
		  for(int m=boundRect[j].y;m<boundRect[j].y+boundRect[j].height;m++)
			{
			    for(int n=boundRect[j].x;n<boundRect[j].x+boundRect[j].width;n++)
			    {
			      bool measure_dist = false;
			      {			
				if(pointPolygonTest(contours[j],Point(n,m),measure_dist) >= 0.0)
				{	
				  stroke_clrimg.at<Vec3b>(m,n)[0] = 0; stroke_clrimg.at<Vec3b>(m,n)[1] = 255; stroke_clrimg.at<Vec3b>(m,n)[2] = 0;	
				  temp.at<uchar>(m,n) = 0;
				}
			      }
			    }
			}
		}
		else
		{
		  for(int m=boundRect[j].y;m<boundRect[j].y+boundRect[j].height;m++)
			{
			    for(int n=boundRect[j].x;n<boundRect[j].x+boundRect[j].width;n++)
			    {
			      bool measure_dist = false;
			      {			
				if(pointPolygonTest(contours[j],Point(n,m),measure_dist) >= 0.0)
				{	
				  stroke_clrimg.at<Vec3b>(m,n)[0] = 255; stroke_clrimg.at<Vec3b>(m,n)[1] = 255; stroke_clrimg.at<Vec3b>(m,n)[2] = 255;	
				  temp.at<uchar>(m,n) = 255;
				}
			      }
			    }
			}
		}
	      }
	      else
	      {
		for(int m=boundRect[j].y;m<boundRect[j].y+boundRect[j].height;m++)
			{
			    for(int n=boundRect[j].x;n<boundRect[j].x+boundRect[j].width;n++)
			    {
			      bool measure_dist = false;
			      {			
				if(pointPolygonTest(contours[j],Point(n,m),measure_dist) >= 0.0)
				{	
				  stroke_clrimg.at<Vec3b>(m,n)[0] = 255; stroke_clrimg.at<Vec3b>(m,n)[1] = 255; stroke_clrimg.at<Vec3b>(m,n)[2] = 255;
				  temp.at<uchar>(m,n) = 255;
				}
			      }
			    }
			}
	      }
	    }
	   
	   /*
	     contours.clear();
	     hierarchy.clear();
	     inv.release();
	     printf("Finding Child\n");
	     inv = FindImageInverse(temp);
	     
	     char *name=(char *)malloc(2001*sizeof(char));
	      sprintf(name, "srcBlock_%d.png",i);
	      imwrite(name,inv);
	     //temp.copyTo(inv);
	     findContours( inv, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	     for(int j=0;j<contours.size();j++)
	     {
	       if(hierarchy[j][3] == -1)
	       {
		 Rect RT = boundingRect( Mat(contours[j]) );
		 Mat new_temp = Mat(RT.height,RT.width,CV_8UC1,Scalar(255));
		 Mat new_clr_temp = Mat(RT.height,RT.width,CV_8UC3);
		 int cnt1 = 0;
		 int p = 0;
		 for(int m=RT.y;m<RT.y+RT.height;m++)
		 {
		   int q = 0;
		    for(int n=RT.x;n<RT.x+RT.width;n++)
		    {
		      bool measure_dist = false;
		      if(pointPolygonTest(contours[j],Point(n,m),measure_dist) >= 0.0)
		      {
			new_temp.at<uchar>(p,q) = 0;
			cnt1++;
		      }
		      for(int k=0;k<blurimage.channels();k++)
			  new_clr_temp.at<Vec3b>(p,q) = blurimage.at<Vec3b>(m+R.y+3,n+R.x+3);
		      q++;
		    }
		    p++;
		 }
		 float frac = RT.area()*1.0;
		 frac = frac/R.area()*1.0;
		 printf("cnt = %d\t Frac =%f\n",cnt1,frac);
		 if(cnt1>5 && frac<0.7)
		 {
		   rectangle(stroke_clrimg,RT.tl(),RT.br(),Scalar(0,0,0),8,1);
		   printf("Preparing Child\n");
		   SB B_C;
		   B_C.B = RT; B_C.B.x = B_C.B.x + R.x+3;  B_C.B.y = B_C.B.y + R.y+3;
		   B_C.Contours = contours[j];
		   for(int s=0;s<B_C.Contours.size();s++)
		   {
		     Point pp = B_C.Contours[s];
		     pp.x = pp.x + R.x +3;
		     pp.y = pp.y + R.y +3;
		     B_C.Contours[s] = pp;
		   }
		   B_C.gtflag = false;
		   B_C.Fvecflag = false;
		   rectangle(Draw,B_C.B.tl(),B_C.B.br(),Scalar(0,0,0),8,1);
		   vector<float> GaborFeature = GetGaborFeature(new_clr_temp);
		   vector<float> colorFeature = GetColorFeature(new_clr_temp,new_temp);
		   vector<float> swfeature = GetStrokeWidthFeature(new_clr_temp,new_temp);
		   printf("\ngabor feature:\n");
		    for(int f=0;f<GaborFeature.size();f++)
		    {
		      B_C.FeatureVec.push_back(GaborFeature[f]);
		      printf("%f\t",GaborFeature[f]);
		    }
		   bool flag = true;
		   printf("\ncolor feature:\n");
		    for(int f=0;f<colorFeature.size();f++)
		    {
		      if(isnan(colorFeature[f]))
			flag = false;
		      B_C.FeatureVec.push_back(colorFeature[f]);
		       printf("%f\t",colorFeature[f]);
		    }
		    printf("\nSW feature:\n");
		    for(int f=0;f<swfeature.size();f++)
		    {
		      if(isnan(swfeature[f]))
			flag = false;
		      B_C.FeatureVec.push_back(swfeature[f]);
		      printf("%f\t",swfeature[f]);
		    }
		    printf("\n");
		    if(flag)
		      B_C.Fvecflag = true;
		   if(B_C.Fvecflag)
		   {
		     B.childs.push_back(B_C);
		     B.Fvecflag = true;
		   }
		 }
	       }
	     }
	   name=(char *)malloc(2001*sizeof(char));
	   sprintf(name, "IniBlock_%d.png",i);
	   imwrite(name,stroke_clrimg);
	   */
	     
	    vector<float> GaborFeature = GetGaborFeature(Tmp_clrimg);
	    printf("\ngabor feature:\n");
	    for(int f=0;f<GaborFeature.size();f++)
	    {
	      B.FeatureVec.push_back(GaborFeature[f]);
	      printf("%f\t",GaborFeature[f]);
	    }
	    vector<float> colorFeature = GetColorFeature(Tmp_clrimg,temp);
	    vector<float> swfeature = GetStrokeWidthFeature(Tmp_clrimg,temp);
	    bool flag = true;
	    printf("\nColorFeature:\n");
	    for(int f=0;f<colorFeature.size();f++)
	    {
	      if(isnan(colorFeature[f]))
		flag = false;
	      B.FeatureVec.push_back(colorFeature[f]);
	      printf("%f\t",colorFeature[f]);
	    }
	    printf("\nSWFeature:\n");
	    for(int f=0;f<swfeature.size();f++)
	    {
	      if(isnan(swfeature[f]))
		flag = false;
	      B.FeatureVec.push_back(swfeature[f]);
	      printf("%f\t",swfeature[f]);
	    }
	    printf("\n");
	    if(flag)
	      B.Fvecflag = true;
	    
	   
	 }
	 else
	 {
	   if(cnt > 5)
	   {
	     rectangle(Draw,R.tl(),R.br(),Scalar(0,255,0),8,1);
	     //printf("No child\n");
	    vector<float> GaborFeature = GetGaborFeature(temp_clrimg);
	    printf("\ngabor feature:\n");
	    for(int f=0;f<GaborFeature.size();f++)
	    {
	      B.FeatureVec.push_back(GaborFeature[f]);
	      printf("%f\t",GaborFeature[f]);
	    }
	    vector<float> colorFeature = GetColorFeature(temp_clrimg,temp_bin);
	    vector<float> swfeature = GetStrokeWidthFeature(temp_clrimg,temp_bin);
	    bool flag = true;
	    printf("\nColorFeature:\n");
	    for(int f=0;f<colorFeature.size();f++)
	    {
	      if(isnan(colorFeature[f]))
		flag = false;
	      B.FeatureVec.push_back(colorFeature[f]);
	      printf("%f\t",colorFeature[f]);
	    }
	    printf("\nSWFeature:\n");
	    for(int f=0;f<swfeature.size();f++)
	    {
	      if(isnan(swfeature[f]))
		flag = false;
	      B.FeatureVec.push_back(swfeature[f]);
	      printf("%f\t",swfeature[f]);
	    }
	    printf("\n");
	    if(flag)
	      B.Fvecflag = true;
	   }
	 }
	 
	 
	 blocks[i] = B;
	 
     }
     
    // imwrite("blocks.png",Draw);
     Draw.release();
     //exit(0);
     
     for(int i=0;i<blocks.size();)
     {
       if(blocks[i].Fvecflag)
       {
	 i++;
       }
       else
	 blocks.erase(blocks.begin()+i);
     }
     
     return blocks;
}

vector< SB > GetSegmentationUnit(Mat image)
{
    Mat blurimage; 
    GaussianBlur(image,blurimage,Size(3,3),0,0);
    
    Mat erodedimage = Erosion(0,1,blurimage); 
    Mat dilatedimage = Dilation(0,1,blurimage);
    
    Mat boundaryimage; 
    subtract(dilatedimage,erodedimage,boundaryimage);
   
    Mat invimg = FindImageInverse(boundaryimage);
   
    Mat Boundary_Binary = binarization(invimg,1);
    
   // imwrite("Boundary_Binary.png",Boundary_Binary);
    
    Mat BB_B = boundaryextraction(Boundary_Binary);
   // imwrite("Boundary_Binary_bound.png",BB_B);
  
    Mat Binary = binarization(image,1);
    
    Mat Hgap = horizontal_gapfilling(Binary,8);
    Mat Vgap = vertical_gapfilling(Hgap,5);
    
    Hgap.release();
  
    Mat temp;
    Vgap.copyTo(temp);
    temp = FindImageInverse(temp);
    
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    
    findContours( temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    
    for( int j = 0; j < contours.size(); j++ )
    { 
      approxPolyDP( Mat(contours[j]), contours_poly[j], 3, true );
      boundRect[j] = boundingRect( Mat(contours_poly[j]) );
    }
	
    vector<SB> blocks;
    
    Mat Draw = Mat(blurimage.rows,blurimage.cols,CV_8UC3,Scalar(255,255,255));
    // blurimage.copyTo(Draw);
    
    int avg_height = 0;
     int count = 0;
     vector<bool> flag(contours.size(),false);
     for( int j = 0; j < contours.size(); j++ )
     {
       if(hierarchy[j][3] == -1 && boundRect[j].height > 6 &&  boundRect[j].width > 10)
       {
	 //rectangle( Draw, boundRect[j].tl(), boundRect[j].br(), Scalar(100,100,10), 2, 8, 0 );
	// drawContours(Draw, contours, j, Scalar(0, 0, 250), CV_FILLED);
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
       for(int j=0;j<blocks.size();)
       {
	 if(i!=j)
	 {
	   if(PolygonInsidePolygonTest(blocks[i].Contours,blocks[j].Contours)==2)
	   {
	     blocks.erase(blocks.begin()+j);
	   }
	   else
	     j=j+1;
	 }
	 else
	   j=j+1;
       }
     }
     
     /*
     for(int i=0;i<blocks.size();i++)
     {
       SB B = blocks[i];
       vector<Point> poly;
	  approxPolyDP( Mat(B.Contours), poly, 3, true );
	  Rect R = boundingRect( Mat(poly) );
       rectangle( Draw, R.tl(), R.br(), Scalar(100,100,10), 2, 8, 0 );
     }
     */
     
     for(int i=0;i<blocks.size();i++)
     {
	SB B = blocks[i];
	vector<Point> poly;
	approxPolyDP( Mat(B.Contours), poly, 3, true );
	Rect R = boundingRect( Mat(poly) );
	if(R.height>3*avg_height && R.width > 3*avg_height)
	{
	    Mat temp_img = Mat(R.height,R.width,CV_8UC1,Scalar(255));
	    int p = 0;
	    for(int m=R.y;m<R.y+R.height;m++)
	    {
		int q = 0;
		for(int n=R.x;n<R.x+R.width;n++)
		{
		  bool measure_dist = false;
		  {
		    
		    if(pointPolygonTest(poly,Point(n,m),measure_dist) >= 0.0)
		    {
		      //temp_img.at<uchar>(p,q) = Boundary_Binary.at<uchar>(m,n);	
		      temp_img.at<uchar>(p,q) = BB_B.at<uchar>(m,n);	
		    }
		  }
		  q++;
		}
		p++;
	    }
	    nocc *component = FindConnectedComponent(temp_img,temp_img);
	    vector<vector<Point> > temp_contours = Convert_nocc2pointvector(component);
	    vector<vector<Point> > temp_contours_poly( temp_contours.size() );
	    vector<Rect> temp_boundRect( temp_contours.size() );
	    for( int l = 0; l < temp_contours.size(); l++ )
	    {
	      for(int m=0;m< temp_contours[l].size();m++)
	      {
		temp_contours[l][m].x = temp_contours[l][m].x + R.x;
		temp_contours[l][m].y = temp_contours[l][m].y + R.y;
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
	      float frac = (temp_boundRect[l].area()*1.0)/(R.area()*1.0);
	      if(frac < 0.7)
	      {
		for(int m=0;m< temp_contours.size();m++)
		{
		  float frac1 = (temp_boundRect[m].area()*1.0)/(R.area()*1.0);
		  if(frac1 < 0.7 && l!=m)
		  {
		      int val = PolygonInsidePolygonTest(temp_contours_poly[l],temp_contours_poly[m]);
		      if(val==2)
		      {
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
		float frac = (temp_boundRect[l].area()*1.0)/(R.area()*1.0);
		if(temp_hierarchy[l][3] == -1 && frac < 0.7)
		{
		  
		  SB child_B;
		  child_B.B = temp_boundRect[l];
		  child_B.Contours = temp_contours[l];
		  child_B.gtflag = false;
		  child_B.Fvecflag = false;
		  blocks[i].childs.push_back(child_B);
		}	    
	    }
	    
	    temp_img.release();
	}
	
     }
     
    
    int cnt; 
    for(int i=0;i<blocks.size();i++)
     {
	SB B = blocks[i];
	vector<Point> poly;
	approxPolyDP( Mat(B.Contours), poly, 3, true );
	Rect R = boundingRect( Mat(poly) );
	if(B.childs.empty())
	{
	    //printf("I am in no child\n");
	    
	    Mat tempimg = Mat(R.height,R.width,CV_8UC3,Scalar(255,255,255));
	    int p = 0;
	    cnt = 0;
	    for(int m=R.y;m<R.y+R.height;m++)
	    {
	        int q = 0; 
		for(int n=R.x;n<R.x+R.width;n++)
		{
		  bool measure_dist = false;
		    if(pointPolygonTest(poly,Point(n,m),measure_dist) >= 0.0)
		    {
		      cnt++;
		      Draw.at<Vec3b>(m,n)[0] = 255;Draw.at<Vec3b>(m,n)[1] = 0;Draw.at<Vec3b>(m,n)[2] = 0;
		      for(int k=0;k<blurimage.channels();k++)
			tempimg.at<Vec3b>(p,q)[k] = blurimage.at<Vec3b>(m,n)[k];
		    }
		    q++;
		}
		p++;
	    }
	    if(cnt>5)
	    {
	      B.FeatureVec = ExtractFeature(tempimg);
	      B.Fvecflag = true;
	      for(int k=0;k<B.FeatureVec.size();k++)
		printf("%f\t",B.FeatureVec[k]);
	      printf("\n\n");
	    }
	    tempimg.release();
	    blocks[i] = B;
	}
	else
	{
	    //printf("I am in child\n");
	      Mat tempimg = Mat(R.height,R.width,CV_8UC3,Scalar(255,255,255));
	      int p = 0;
	      cnt = 0;
	      for(int m=R.y;m<R.y+R.height;m++)
	      {
		int q = 0;
		for(int n=R.x;n<R.x+R.width;n++)
		{
		  int temp_col = R.width;
		  bool measure_dist = false;
		    if(pointPolygonTest(poly,Point(n,m),measure_dist) >= 0.0)
		    {
		      cnt++;
		      Draw.at<Vec3b>(m,n)[0] = 0;Draw.at<Vec3b>(m,n)[1] = 255;Draw.at<Vec3b>(m,n)[2] = 0;
		      for(int k=0;k<blurimage.channels();k++)
			tempimg.at<Vec3b>(p,q)[k] = blurimage.at<Vec3b>(m,n)[k];
		    }
		    q++;
		}
		p++;
	      }
	      if(cnt > 5)
	      {
		B.FeatureVec = ExtractFeature(tempimg);
		B.Fvecflag = true;
		for(int k=0;k<B.FeatureVec.size();k++)
		  printf("%f\t",B.FeatureVec[k]);
		printf("\n\n");
	      }
	     tempimg.release();
	     blocks[i] = B;
	    
	    vector<SB> C = B.childs;
	    
	    printf("Initial Child Size %d\n",C.size());
	    
	    for(int j=0;j<C.size();j++)
	    {
	      for(int k=0;k<C.size();)
	      {
		if(j!=k)
		{
		  int val = PolygonInsidePolygonTest(C[j].Contours,C[k].Contours);
		  //if(val==2||val==3)
		  //int val = FindOverlappingRectangles(C[j].B,C[k].B);
		  if(val == 2 || val == 3)
		  {
		    C.erase(C.begin()+k);
		  }
		  else
		    k = k + 1;
		}
		else
		  k = k + 1;
	      }
	    }
	    blocks[i].childs = C;
	    
	   
	    printf("Later Child Size %d\n",C.size());
	    for(int j=0;j<C.size();)
	    {
		SB B_C = C[j];
		vector<Point> poly_c;
		approxPolyDP( Mat(B_C.Contours), poly_c, 3, true );
		Rect R_C = boundingRect( Mat(poly_c) );
		Mat tempimg_C = Mat(R_C.height,R_C.width,CV_8UC3,Scalar(255,255,255));
		int p_c = 0;
		cnt = 0;
		for(int m=R_C.y;m<R_C.y+R_C.height;m++)
		{
		  int q_c = 0;
		  for(int n=R_C.x;n<R_C.x+R_C.width;n++)
		  {
		    cnt++;
		    Draw.at<Vec3b>(m,n)[0] = 0;Draw.at<Vec3b>(m,n)[1] = 0;Draw.at<Vec3b>(m,n)[2] = 255;
		    for(int k=0;k<blurimage.channels();k++)
		      tempimg_C.at<Vec3b>(p_c,q_c)[k] = blurimage.at<Vec3b>(m,n)[k];
		    q_c++;
		  }
		  p_c++;
		}
		if(cnt > 5)
		{
		  B_C.FeatureVec = ExtractFeature(tempimg_C);
		  B_C.Fvecflag = true;
		  for(int k=0;k<B.FeatureVec.size();k++)
		    printf("%f\t",B.FeatureVec[k]);
		  printf("\n\n");
		  C[j] = B_C;
		  j++;
		}
		else
		{
		  C.erase(C.begin()+j);
		}
		
		tempimg_C.release();
	      }
	      
	      blocks[i].childs = C;
	    
	}
	
       
     }
     
     
    
     //imwrite("SEGUNIT.png",Draw);
     
     for(int i=0;i<blocks.size();)
     {
       SB B = blocks[i];
       if(B.childs.empty())
       {
	 if(B.Fvecflag)
	   i++;
	 else
	   blocks.erase(blocks.begin()+i);
       }
       else
       {
	 for(int j=0;j<B.childs.size();)
	 {
	   if(B.childs[j].Fvecflag)
	     j++;
	   else
	     B.childs.erase(B.childs.begin()+j);
	 }
	 if(B.childs.empty())
	   blocks.erase(blocks.begin()+i);
	 else
	 {
	   blocks[i] = B;
	   i++;
	 }
       }
     }

     //exit(0);
     printf("Printing Features from Blocks\n");
     for(int i=0;i<blocks.size();i++)
     {
       SB B = blocks[i];
       if(B.childs.empty())
       {
	  for(int k=0;k<B.FeatureVec.size();k++)
	      printf("%f\t",B.FeatureVec[k]);
	    printf("\n\n");
       }
       else
       {
	 for(int k=0;k<B.FeatureVec.size();k++)
	      printf("%f\t",B.FeatureVec[k]);
	    printf("\n\n");
	 
	 for(int j=0;j<B.childs.size();j++)
	 {
	   SB B_C = B.childs[j];
	   for(int k=0;k<B_C.FeatureVec.size();k++)
	      printf("%f\t",B_C.FeatureVec[k]);
	    printf("\n\n");
	 }
       }
     }
     //exit(0);
     return blocks;
    
}
