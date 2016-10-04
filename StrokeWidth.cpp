
#include "PixelValidation.h"
#include "Image_proc_functions.h"
#include "Morphology.h"
#include "StatisticalFunctions.h"
#include "StrokeWidth.h"

using namespace IITkgp_functions;
/*-------------------------------------------------------------------------------------------------------------------------------------------*/


/**
 * @function FindGradient
 * @param input Color BGR image
 * @brief Calculate Gradient of the input image
 *@output Mat Object with 4 channel data double (CV_64FC4)
 * 			
 * 			channel1 = gradx; // calculated using =>(SOBEL);
 * 			channel2 = grady; // calculated using =>(SOBEL);			
 * 			channel3 = Gradient Direction (Angle); //calculatted using =>(atan2(y_dir, x_dir)*180)/PI;
 * 			channel4 = Gradient Magnitude ; //calculated using =>sqrt((x_dir*x_dir)+(y_dir*y_dir));
 */



Mat IITkgp_functions::FindGradient(Mat Image)
{
  

 
 
  Mat GrayImage;
  cvtColor(Image,GrayImage,CV_BGR2GRAY);
 

 
  
  int scale = 1;
  int delta = 0;
  int ddepth = CV_64F;
  
   /// Generate grad_x and grad_y
  Mat grad_x, grad_y;
  

  /// Gradient X
  //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
  Sobel( GrayImage, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
  

  /// Gradient Y
  //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  Sobel( GrayImage, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );

  Mat grad = Mat::zeros(GrayImage.rows,GrayImage.cols,CV_64FC1);
  Mat TempGradxyGradDirMag;
  TempGradxyGradDirMag = Mat(GrayImage.rows,GrayImage.cols,CV_64FC4);
//   Mat NormalizeGradxy;
//   NormalizeGradxy = Mat(GrayImage.rows,GrayImage.cols,CV_64FC2);
  int k = 0;
  double x_dir,y_dir;
  for(int i=0;i<GrayImage.rows;i++)
  {
    for(int j=0;j<GrayImage.cols;j++)
    {
      
	x_dir = grad_x.at<double>(i,j);
	//TempGradx.at<double>(i,j) = x_dir;
	y_dir = grad_y.at<double>(i,j);
	//TempGrady.at<double>(i,j) = y_dir;
	grad.at<double>(i,j) = (atan2(y_dir, x_dir)*180)/PI;
	if(grad.at<double>(i,j) < 0)
	  grad.at<double>(i,j) = 180 - grad.at<double>(i,j);
	TempGradxyGradDirMag.at<Vec4d>(i,j)[0] = x_dir;
	TempGradxyGradDirMag.at<Vec4d>(i,j)[1] = y_dir;
	TempGradxyGradDirMag.at<Vec4d>(i,j)[2] = (atan2(y_dir, x_dir)*180)/PI;
	TempGradxyGradDirMag.at<Vec4d>(i,j)[3] = sqrt((x_dir*x_dir)+(y_dir*y_dir));
// 	NormalizeGradxy.at<Vec2d>(i,j)[0] = x_dir/TempGradxyGradDirMag.at<Vec4d>(i,j)[3];
// 	NormalizeGradxy.at<Vec2d>(i,j)[1] = y_dir/TempGradxyGradDirMag.at<Vec4d>(i,j)[3];
// 	
	//printf("grad x = %lf\t grad y = %lf and \tgrad dir = %lf\n",x_dir,y_dir,grad.at<double>(i,j));
      
    }
  }
  
  
  
  return(TempGradxyGradDirMag);
}



/*-------------------------------------------------------------------------------------------------------------------------------------------*/

/**
 * @function CreateBean
 * @param input NumberOfBean, MaxElement = Maximum value of a element possible
 * @brief Calculate structure of a bean for gradiend angles and generate other informations related to a bean
 * @output array of BeanStucture with n number of elements, where n = NumberOfBean
 * 
 */

BeanStucture * IITkgp_functions::CreateBean(int NumberOfBean, int MaxElement)
{
  BeanStucture *Beans;
  Beans = (BeanStucture *)malloc(NumberOfBean*sizeof(BeanStucture));
  
  int Middle;
  int k;
  k =(int) MaxElement/NumberOfBean;
  
  for(int i=0;i<NumberOfBean;i++)
  {
    Beans[i].BeanNumber = i;
    Beans[i].MaxElement = MaxElement;
    Beans[i].middle = i*k;
    Beans[i].UpperBound =(int) (Beans[i].middle + (k/2));
    Beans[i].UpperBound = Beans[i].UpperBound%MaxElement;
    Beans[i].LowerBound = (int) (Beans[i].middle - (k/2));
    Beans[i].LowerBound = MaxElement + Beans[i].LowerBound;
    Beans[i].LowerBound = Beans[i].LowerBound%MaxElement;
  }
  
  return(Beans);
}


/**
 * @function FindOpositeBean
 * @param input BeanNumber = Bean number for which opposite bean is to find
 * 		NumberOfBean = Total number of Bean Present
 * @Brief 	It calculate the ooposite bean number for a particular bean
 * 
 * @output 	It return the calculated opposite bean number
 * 
 * 
 */

int IITkgp_functions::FindOpositeBean(int BeanNumber, int NumberOfBean)
{
  int OpositeBean;
  OpositeBean =(int) BeanNumber + NumberOfBean/2;
  OpositeBean = OpositeBean%NumberOfBean;
}


/**
 * @function FindBeanNumber
 * @param input angle = Gradient direction / angle for which you want to find the bean
 * 		MaxElement = Maximum value of a element possible
 * 		NumberOfBean = Total number of Bean Present
 * @Brief 	It calculate the bean number for a particular gradient direction or angle or value
 * 
 * @output 	It returns the bean number
 * 
 * 
 */

int IITkgp_functions::FindBeanNumber(int angle, int MaxElement, int NumberOfBean)
{
  //int MaxElement = Beans[0].MaxElement;
  int BeanedAngle = MaxElement/NumberOfBean;
  int temp_angle =(int) angle + (BeanedAngle/2);
  int BeanNum;
  BeanNum =(int) (temp_angle/BeanedAngle);
  BeanNum = BeanNum % NumberOfBean;
  
  return(BeanNum);
}



Point2i IITkgp_functions::FindNextPixel8Bean(Point2i p, int Bean)
{
  Point2i next;
  if(Bean == 0)
  {
    next.y = p.y;
    next.x = p.x + 1;
    return(next);
  }
  else if(Bean == 1)
  {
    next.y = p.y - 1;
    next.x = p.x + 1;
    return(next);
  }
  else if(Bean == 2)
  {
    next.y = p.y - 1;
    next.x = p.x;
    return(next);
  }
  else if(Bean == 3)
  {
    next.y = p.y - 1;
    next.x = p.x - 1;
    return(next);
  }
  else if(Bean == 4)
  {
    next.y = p.y;
    next.x = p.x - 1;
    return(next);
  }
  else if(Bean == 5)
  {
    next.y = p.y + 1;
    next.x = p.x - 1;
    return(next);
  }
  else if(Bean == 6)
  {
    next.y = p.y + 1;
    next.x = p.x;
    return(next);
  }
  else
  {
    next.y = p.y + 1;
    next.x = p.x + 1;
    return(next);
  }
}


Point2i IITkgp_functions::FindNextPixel12Bean(Point2i p, int Bean)
{
  Point2i next;
  if(Bean == 0)
  {
    next.y = p.y;
    next.x = p.x + 1;
    return(next);
  }
  else if(Bean == 1)
  {
    next.y = p.y - 1;
    next.x = p.x + 2;
    return(next);
  }
  else if(Bean == 2)
  {
    next.y = p.y - 2;
    next.x = p.x + 1;
    return(next);
  }
  else if(Bean == 3)
  {
    next.y = p.y - 1;
    next.x = p.x;
    return(next);
  }
  else if(Bean == 4)
  {
    next.y = p.y - 2;
    next.x = p.x - 1;
    return(next);
  }
  else if(Bean == 5)
  {
    next.y = p.y - 1;
    next.x = p.x - 2;
    return(next);
  }
  else if(Bean == 6)
  {
    next.y = p.y;
    next.x = p.x - 1;
    return(next);
  }
  else if(Bean == 7)
  {
    next.y = p.y + 1;
    next.x = p.x - 2;
    return(next);
  }
  else if(Bean == 8)
  {
    next.y = p.y + 2;
    next.x = p.x - 1;
    return(next);
  }
  else if(Bean == 9)
  {
    next.y = p.y + 1;
    next.x = p.x;
    return(next);
  }
  else if(Bean == 10)
  {
    next.y = p.y + 2;
    next.x = p.x + 1;
    return(next);
  }
  else if(Bean == 11)
  {
    next.y = p.y + 1;
    next.x = p.x + 2;
    return(next);
  }
}


Point2i IITkgp_functions::FindNextPixel16Bean(Point2i p, int Bean)
{
  Point2i next;
  if(Bean == 0)
  {
    next.y = p.y;
    next.x = p.x + 1;
    return(next);
  }
  else if(Bean == 1)
  {
    next.y = p.y - 1;
    next.x = p.x + 2;
    return(next);
  }
  else if(Bean == 2)
  {
    next.y = p.y - 1;
    next.x = p.x + 1;
    return(next);
  }
  else if(Bean == 3)
  {
    next.y = p.y - 2;
    next.x = p.x + 1;
    return(next);
  }
  else if(Bean == 4)
  {
    next.y = p.y - 1;
    next.x = p.x;
    return(next);
  }
  else if(Bean == 5)
  {
    next.y = p.y - 2;
    next.x = p.x - 1;
    return(next);
  }
  else if(Bean == 6)
  {
    next.y = p.y - 1;
    next.x = p.x - 1;
    return(next);
  }
  else if(Bean == 7)
  {
    next.y = p.y - 1;
    next.x = p.x - 2;
    return(next);
  }
  else if(Bean == 8)
  {
    next.y = p.y;
    next.x = p.x - 1;
    return(next);
  }
  else if(Bean == 9)
  {
    next.y = p.y + 1;
    next.x = p.x - 2;
    return(next);
  }
  else if(Bean == 10)
  {
    next.y = p.y + 1;
    next.x = p.x - 1;
    return(next);
  }
  else if(Bean == 11)
  {
    next.y = p.y + 2;
    next.x = p.x - 1;
    return(next);
  }
  else if(Bean == 12)
  {
    next.y = p.y + 1;
    next.x = p.x;
    return(next);
  }
  else if(Bean == 13)
  {
    next.y = p.y + 2;
    next.x = p.x + 1;
    return(next);
  }
  else if(Bean == 14)
  {
    next.y = p.y + 1;
    next.x = p.x + 1;
    return(next);
  }
  else if(Bean == 15)
  {
    next.y = p.y + 1;
    next.x = p.x + 2;
    return(next);
  }
}



/**
 * @function FindStrokeWidth
 * @param input	 Point2i p = a Point2I data type 
 * 		 int Bean = Bean number for that particular pointer
 * 		 Mat BoundaryImage = Boundary image of the source image
 * 		 Mat GradBean = Mat object containg Bean number correcponding to the gradient of the source image
 * 		 Mat GrayImage = gray image of the src iamge 
 * 		 NumberOfBean = Total number of Bean Present
 * 
 * @brief	This function calculate the a line from a boundary pixel 'p' to another boundary pixel 'q' if q has oposite bean number that of 'p'
 * 		The obtained line information is stored in a data structute named Ray
 * 
 * @output	The obtained Ray data type (typically it is a line from a point p to point q) 
 * 
 */


Ray IITkgp_functions::FindStrokeWidth(Point2i p,int Bean, Mat BoundaryImage, Mat GradBean, Mat GrayImage, int NumberOfBean)
{
  
  
  
  Ray TempRay;
  TempRay.p = p;
  TempRay.dist = 0;
  Point2i Next;
  Next = p;
  int OpositeBean = FindOpositeBean(Bean, NumberOfBean);
  int bean1,bean2;
  int m,n;
  while(1)
  {
    //Next = FindNextPixel8Bean(Next,Bean);
    
    if(NumberOfBean == 8)
      Next = FindNextPixel8Bean(Next,Bean);
    else if(NumberOfBean == 12)
      Next = FindNextPixel12Bean(Next,Bean);
    else if(NumberOfBean == 16)
      Next = FindNextPixel16Bean(Next,Bean);
	 
    
    if(validate(Next.y,Next.x,BoundaryImage.rows,BoundaryImage.cols))
    {
      if(GrayImage.at<uchar>(Next.y,Next.x) == 255)
      {
	for( m=Next.y-1;m<=Next.y+1;m++)
	{
	  for( n=Next.x-1;n<=Next.x+1;n++)
	  {
	    if(validate(m,n,BoundaryImage.rows,BoundaryImage.cols))
	    {
	      if(BoundaryImage.at<uchar>(m,n) == 0)
	      {
		Next.y = m;
		Next.x = n;
		break;
	      }
	    }
	  }
	  if(n<Next.x+2)
	   break;
	}
	if(m==Next.y+2 && n==Next.x+2)
	{
	  TempRay.dist = 0;
	  break;
	}
      }
      
      TempRay.points.push_back(Next);
      TempRay.dist = TempRay.dist + 1;
      if(BoundaryImage.at<uchar>(Next.y,Next.x) == 0)
      {
	bean1 = (OpositeBean + 1)%NumberOfBean;
	bean2 = (OpositeBean + NumberOfBean-1)%NumberOfBean;
	if(GradBean.at<int8_t>(Next.y,Next.x) == OpositeBean || GradBean.at<int8_t>(Next.y,Next.x) == bean1 || GradBean.at<int8_t>(Next.y,Next.x) == bean2)
	{
	  TempRay.q = Next;
	  TempRay.CalcEcluiDist();
	  break;
	}
	else
	{
	  TempRay.dist = 0;
	  break;
	}
      }
      
      if(TempRay.dist > (GrayImage.cols/3) || TempRay.dist > (GrayImage.rows/3))
      {
	TempRay.dist = 0;
	break;
      }
      
    }
    else
    {
      TempRay.dist = 0;
      break;
    }
  }
  
  
  return(TempRay);
}


/**
 * @function SWT
 * @param input Mat Image = src image for which we want to find the stroke widths
 * 		Mat BinaryImage = Binary image corresponding to the input image
 * 
 * @brief 	This function calculate a set of RAY or stroke widths for a given image
 * 
 * @output 	vector<Ray> ==> A set of stroke widths
 * 
 */


vector<Ray> IITkgp_functions::SWT(Mat Image, Mat BinaryImage)
{
  int row,col;
  row = Image.rows;
  col = Image.cols;
  
   int i,j; 
   
  Mat ForeGroundImage;
  ForeGroundImage = foreground_masked_image(Image, BinaryImage);
  
 
 
  Mat GrayImage;
 // Image.copyTo(GrayImage);
  cvtColor(ForeGroundImage,GrayImage,CV_BGR2GRAY);
 // imshow("Gray",GrayImage);
  
 
  Mat BoundaryImage;
  BoundaryImage = boundaryextraction(BinaryImage);
  //imshow("boundary",BoundaryImage);
 
  
  int scale = 1;
  int delta = 0;
  int ddepth = CV_64F;
  
   /// Generate grad_x and grad_y
  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;

  /// Gradient X
  //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
  Sobel( GrayImage, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
  //convertScaleAbs( grad_x, abs_grad_x );

  /// Gradient Y
  //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  Sobel( GrayImage, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
 // convertScaleAbs( grad_y, abs_grad_y );

  /// Total Gradient (approximate)
 // addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
  int no_edge_pixel = 0;
  for(int i=0;i<GrayImage.rows;i++)
  {
    for(int j=0;j<GrayImage.cols;j++)
    {
      if(BoundaryImage.data[i*col+j]==0)
      {
	no_edge_pixel = no_edge_pixel + 1;
      }
    }
  }
  
  Mat grad = Mat::zeros(GrayImage.rows,GrayImage.cols,CV_64FC1);
  Mat TempGradxyGradDirMag;
  TempGradxyGradDirMag = Mat(GrayImage.rows,GrayImage.cols,CV_64FC4);
  Mat NormalizeGradxy;
  NormalizeGradxy = Mat(GrayImage.rows,GrayImage.cols,CV_64FC2);
  int k = 0;
  double x_dir,y_dir;
  for(int i=0;i<GrayImage.rows;i++)
  {
    for(int j=0;j<GrayImage.cols;j++)
    {
      if(BoundaryImage.at<uchar>(i,j)==0)
      {
	x_dir = grad_x.at<double>(i,j);
	//TempGradx.at<double>(i,j) = x_dir;
	y_dir = grad_y.at<double>(i,j);
	//TempGrady.at<double>(i,j) = y_dir;
	grad.at<double>(i,j) = (atan2(y_dir, x_dir)*180)/PI;
	if(grad.at<double>(i,j) < 0)
	  grad.at<double>(i,j) = 180 - grad.at<double>(i,j);
	TempGradxyGradDirMag.at<Vec4d>(i,j)[0] = x_dir;
	TempGradxyGradDirMag.at<Vec4d>(i,j)[1] = y_dir;
	TempGradxyGradDirMag.at<Vec4d>(i,j)[2] = (atan2(y_dir, x_dir)*180)/PI;
	TempGradxyGradDirMag.at<Vec4d>(i,j)[3] = sqrt((x_dir*x_dir)+(y_dir*y_dir));
	NormalizeGradxy.at<Vec2d>(i,j)[0] = x_dir/TempGradxyGradDirMag.at<Vec4d>(i,j)[3];
	NormalizeGradxy.at<Vec2d>(i,j)[1] = y_dir/TempGradxyGradDirMag.at<Vec4d>(i,j)[3];
	
	//printf("grad x = %lf\t grad y = %lf and \tgrad dir = %lf\n",x_dir,y_dir,grad.at<double>(i,j));
      }
    }
  }
  
  double max_elem,min_elem;
  int posi;
  
  FindMaxElementPosi(grad,&max_elem,&posi);
  FindMinElementPosi(grad,&min_elem,&posi);
  
  //printf("MAX Angle is %lf\tand MIN Angle is %lf\n",max_elem,min_elem);
 
  
   
  Mat absgrad = Mat(grad.rows,grad.cols,CV_16UC1);
  
  
  for(int i=0;i<GrayImage.rows;i++)
  {
    for(int j=0;j<GrayImage.cols;j++)
    {
      if(BoundaryImage.at<uchar>(i,j)==0)
      {
	absgrad.at<u_int16_t>(i,j) =(u_int16_t) floor(grad.at<double>(i,j));
      }
    }
  }


//    namedWindow( "grad", CV_WINDOW_KEEPRATIO );
//    imshow("grad", grad);
//    waitKey(0);
  
  Mat abs_grad;
  convertScaleAbs( grad, abs_grad );
  //imshow("grad",abs_grad);

int flag = 1; 

  int BeanNum;
  int NumberOfBean;
  
  
  
  if(flag == 1)
    NumberOfBean = 8;
  else if(flag == 2)
    NumberOfBean = 12;
  else if(flag == 3)
    NumberOfBean = 16;
  else
  {
    printf("Bean not Selected Properly\n Choosing Default Bean = 8\n");
    NumberOfBean = 8;
  }
  
  
  BeanStucture *BeanData;
 
 BeanData = CreateBean(NumberOfBean,360);
  
  
  Mat GradBean = Mat(grad.rows,grad.cols,CV_8UC1);
 // Mat GradBeanHist = Mat::zeros(NumberOfBean,1,CV_32FC1);
  for(int i=0;i<GrayImage.rows;i++)
  {
    for(int j=0;j<GrayImage.cols;j++)
    {
      if(BoundaryImage.at<uchar>(i,j)==0)
      {
	  BeanNum = FindBeanNumber(absgrad.at<u_int16_t>(i,j),360,NumberOfBean);
	  GradBean.at<int8_t>(i,j) = BeanNum;
	//  GradBeanHist.at<float>(BeanNum,0) = GradBeanHist.at<float>(BeanNum,0) + 1;
      }
    }
  }
  
 
  int NumberOfStrokes = 0;
  vector<Ray> strokes1;
  for(int i=0;i<GrayImage.rows;i++)
  {
    for(int j=0;j<GrayImage.cols;j++)
    {
      if(BoundaryImage.at<uchar>(i,j)==0)
      {
	 int bean;
	 bean = GradBean.at<int8_t>(i,j);
	 Point2i p;
	 p.x = j;
	 p.y = i;
	 
	 Ray temp;
	 temp = FindStrokeWidth(p,bean,BoundaryImage,GradBean,GrayImage, NumberOfBean);
	 if(temp.dist > 2)
	 {
	  strokes1.push_back(temp);
	  NumberOfStrokes = NumberOfStrokes + 1;
	 }
      }
    }
  }
 
 //printf("NumberOfStrokes is %d\n",NumberOfStrokes);

  return (strokes1);
}


vector<float> IITkgp_functions::GetStrokeWidthFeature(Mat Image, Mat BinaryImage)
{
  vector<Ray> strokes = SWT(Image,BinaryImage);
  Mat SW = Mat(1,strokes.size(),CV_32FC1);
  for(int s=0;s<strokes.size();s++)
  {
    SW.at<float>(0,s) = strokes[s].dist * 1.0;
  }
  vector<float> SWFeature;
  SWFeature.push_back((float) FindMean(SW));
  SWFeature.push_back((float) FindStdDev(SW));
  SWFeature.push_back((float) strokes.size()*1.0);
  
  return SWFeature;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------*/