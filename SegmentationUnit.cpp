#include "SegmentationUnit.h"
#include "Morphology.h"
#include "binarization.h"
#include "Image_proc_functions.h"
#include "SmoothingGapfilling.h"
#include "RectangleTest.h"
#include "connectedcomponent.h"
#include "FeatureExtraction.h"

using namespace IITkgp_functions;


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
    
    int avg_height = 0;
     int count = 0;
     vector<bool> flag(contours.size(),false);
     for( int j = 0; j < contours.size(); j++ )
     {
       if(hierarchy[j][3] == -1 && boundRect[j].height > 6 &&  boundRect[j].width > 10)
       {
	 //rectangle( cont, boundRect[j].tl(), boundRect[j].br(), Scalar(100,100,10), 2, 8, 0 );
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
     
     int p,q;
     int bc = 0;
     for( int j = 0; j < contours.size(); j++ )
     {
       if(hierarchy[j][3] == -1 && boundRect[j].height > 3*avg_height && boundRect[j].width > 3*avg_height && flag[j])
       {
	 
	 Mat temp_img = Mat(boundRect[j].height,boundRect[j].width,CV_8UC1,Scalar(255));
	 p = 0;
	 for(int m=boundRect[j].y;m<boundRect[j].y+boundRect[j].height;m++)
	 {
	    q = 0;
	    for(int n=boundRect[j].x;n<boundRect[j].x+boundRect[j].width;n++)
	    {
	      int temp_col = boundRect[j].width;
	      bool measure_dist = false;
	      {
		if(pointPolygonTest(contours_poly[j],Point(n,m),measure_dist) >= 0.0)
		{
		  temp_img.at<uchar>(p,q) = Boundary_Binary.at<uchar>(m,n);		  
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
	    float frac = (temp_boundRect[l].area()*1.0)/(boundRect[j].area()*1.0);
	    if(temp_hierarchy[l][3] == -1 && frac < 0.7)
	    {
	      
	      SB child_B;
	      child_B.B = temp_boundRect[l];
	      child_B.Contours = temp_contours[l];
	      child_B.gtflag = false;
	      child_B.Fvecflag = false;
	      blocks[bc].childs.push_back(child_B);
	    }	    
	 }	  
	 temp_img.release();
	 bc++;
       }
     }
     
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

     
     return blocks;
    
}
