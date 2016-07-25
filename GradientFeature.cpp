#include "GradientFeature.h"



vector<float> GetGradientFeature(Mat image)
{
  Mat gray;
  int ddepth = CV_32F;
  int scale = 1;
  int delta = 0;
  Mat grad;
  
  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;
  Mat abs_grad;
  
  if(image.channels()>1)
    cvtColor(image,gray,CV_BGR2GRAY);
  else
    image.copyTo(gray);
  
  
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
  
  vector<float> GradientFeature;
  
  GradientFeature.push_back((float) FindMean(GradMag));
  GradientFeature.push_back((float) FindStdDev(GradMag));
  GradMag.release();
  GradientFeature.push_back((float) FindMean(GradDir));
  GradientFeature.push_back((float) FindStdDev(GradDir));
  GradDir.release();
  
  return GradientFeature;
}
