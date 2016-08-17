


#include "ScalarColorFeature.h"
#include "Image_proc_functions.h"
#include "StatisticalFunctions.h"





/**********************************************************************************************************************************************************/

/**
     * @function 	ScalarColorFeature
     * @param input 	vector<Mat>& image =  Vector of Mat object ==> different channels as different Mat Object
     * @brief 		This Function is used to calculate scalar color feature based on first principal component; 
     *			All pixel values are projevted on 1st principal component, where all pixels are considered for computing PCA
     * @output 		vector<float> data => Scalar data of size same as number of pixels in the given image
     */

vector<float> IITkgp_functions::ScalarColorFeature(vector<Mat>& image)
{
  int  nEigens = image.size();
  int sp,i,j,k,l;
    
  double temp_max_val;
  
  PCA ImagePCA;
  Mat ImageDataPCA (image[0].rows*image[0].cols,image.size(), CV_32FC1);
  
  for (i = 0; i < image[0].rows; i++)
   {
     for (j = 0; j < image[0].cols; j++)
     {
       for(l=0;l<image.size();l++)
	{
	  ImageDataPCA.at<float>((i*image[0].cols+j),l) = image[l].at<uchar>(i,j) * 1.0;
	}
     }
   }
   
   
   ImagePCA(ImageDataPCA,Mat(),CV_PCA_DATA_AS_ROW,nEigens);
   
   float *ClrFeature;
   ClrFeature = (float *)malloc(image.size()*sizeof(float));
   
   for(l=0;l<image.size();l++)
   {
     ClrFeature[l] = ImagePCA.eigenvectors.at<float>(0,l);
   }
   
   vector<float> ScalarColorData;
   
   for (i = 0; i < image[0].rows; i++)
   {
     for (j = 0; j < image[0].cols; j++)
     {
       float pdata = 0.0;
       for(l=0;l<image.size();l++)
	{
	  pdata = pdata + image[l].at<uchar>(i,j) * ClrFeature[l];
	}
	
	ScalarColorData.push_back(pdata);
     }
   }
   
   
   return(ScalarColorData);
    
}


 /**
     * @function 	ScalarColorFeatureMasked
     * @param input 	vector<Mat>& image =  Vector of Mat object ==> different channels as different Mat Object;
     * 			Mat MaskedImage = Mat object of 1 channel, and masked pixels have value '0' ;
     * @brief 		This Function is used to calculate scalar color feature based on first principal component; 
     *			All pixel values are projevted on 1st principal component, where masked pixels are considered for computing PCA
     * @output 		vector<float> data => Scalar data of size same as number of masked pixels in the given image
     */
       

vector<float> IITkgp_functions::ScalarColorFeatureMasked(vector<Mat>& image, Mat MaskedImage)
{
  int  nEigens = image.size();
  int sp,i,j,k,l;
  
   
    
   int no_of_foregrnd_pix = NumberofForegroundPixel(MaskedImage);
    

  
  PCA ImagePCA;
  Mat ImageDataPCA (no_of_foregrnd_pix,image.size(), CV_32FC1);
  
  k = 0;
  for (i = 0; i < image[0].rows; i++)
   {
     for (j = 0; j < image[0].cols; j++)
     {
       if(MaskedImage.at<uchar>(i,j) == 0)
       {
	  for(l=0;l<image.size();l++)
	  {
	    ImageDataPCA.at<float>(k,l) = image[l].at<uchar>(i,j) * 1.0;
	  }
	  k++;
       }
     }
   }
   
   
   ImagePCA(ImageDataPCA,Mat(),CV_PCA_DATA_AS_ROW,nEigens);
   
   vector<float> ClrFeature(image.size());
  
   for(l=0;l<image.size();l++)
   {
     ClrFeature[l] = ImagePCA.eigenvectors.at<float>(0,l);
   }
   
   /*
   vector<float> ScalarColorData;
   
    for (i = 0; i < image[0].rows; i++)
   {
     for (j = 0; j < image[0].cols; j++)
     {
       if(MaskedImage.at<uchar>(i,j) == 0)
       {
	 float pdata = 0.0;
	  for(l=0;l<image.size();l++)
	  {
	    pdata = pdata + image[l].at<uchar>(i,j) * ClrFeature[l];
	  }
	  ScalarColorData.push_back(pdata);
       }
     }
   }
   */
   return(ClrFeature);
}


vector< float > IITkgp_functions::GetColorFeature(Mat image, Mat Binimage)
{

  int  nEigens = image.channels();
  Mat cvtimage;
  cvtColor(image,cvtimage,CV_BGR2YCrCb);
  
  
  int no_of_foregrnd_pix = NumberofForegroundPixel(Binimage);
  PCA ImagePCA;
  Mat ImageDataPCA (no_of_foregrnd_pix,image.channels(), CV_32FC1);
  
  int k = 0;
  for (int i = 0; i < image.rows; i++)
   {
     for (int j = 0; j < image.cols; j++)
     {
       if(Binimage.at<uchar>(i,j) == 0)
       {
	  for(int l=0;l<image.channels();l++)
	  {
	    ImageDataPCA.at<float>(k,l) = cvtimage.at<Vec3b>(i,j)[l] * 1.0;
	  }
	  k++;
       }
     }
   }
   
   ImagePCA(ImageDataPCA,Mat(),CV_PCA_DATA_AS_ROW,nEigens);
   vector<float> ClrFeature(image.channels());
   float min_val=0.0;
   for(int l=0;l<image.channels();l++)
   {
     ClrFeature[l] = ImagePCA.eigenvectors.at<float>(0,l);
     min_val = min_val + 255 * ClrFeature[l];
   }
   
   
   
   Mat Data = Mat(1,no_of_foregrnd_pix,CV_32FC1);
   k = 0;
   for (int i = 0; i < image.rows; i++)
   {
     for (int j = 0; j < image.cols; j++)
     {
       if(Binimage.at<uchar>(i,j) == 0)
       {
	  float pdata = 0.0;
	  for(int l=0;l<image.channels();l++)
	  {
	    pdata = pdata + cvtimage.at<Vec3b>(i,j)[l] * ClrFeature[l];
	  }
	  if(min_val>pdata)
	    min_val = pdata;
	  Data.at<float>(0,k) = pdata;
	  k++;
       }
     }
   }
   if(min_val<0)
   {
     for(int i=0;i<no_of_foregrnd_pix;i++)
       Data.at<float>(0,i) = Data.at<float>(0,i) - min_val; 
   }
   
   ClrFeature.clear();
   
   ClrFeature.push_back((float)FindMean(Data));
   ClrFeature.push_back((float)FindStdDev(Data));
   
   return ClrFeature;
}

