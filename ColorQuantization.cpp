#include "ColorQuantization.h"



Mat ColorQuantize(Mat image, int n)
{
  Mat QuantizeImage;
  image.copyTo(QuantizeImage);
  Mat labels,center;
  Mat pt = Mat(image.rows*image.cols,image.channels(),CV_32F);
  for(int i=0;i<image.rows;i++)
  {
    for(int j=0;j<image.cols;j++)
    {
      for(int k=0;k<image.channels();k++)
	pt.at<float>(i*image.cols+j,k)= image.at<Vec3b>(i,j)[k]*1.0;
    }
  }
  kmeans(pt,n,labels,TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0),3,KMEANS_PP_CENTERS, center);
  printf("center rows = %d cols = %d\n",center.rows,center.cols);
  for(int i=0;i<image.rows;i++)
  {
    for(int j=0;j<image.cols;j++)
    {
      int posi = labels.at<int>(i*image.cols+j);
      for(int k=0;k<image.channels();k++)
      {
	QuantizeImage.at<Vec3b>(i,j)[k] =(uchar) floor(center.at<float>(posi,k));
      }
    }
  }
  
  return QuantizeImage;
}
