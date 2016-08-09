


#include "StatisticalFunctions.h"
#include "sorting.h"

using namespace IITkgp_functions;




/*-----------------------------Statistical function--------------------------------*/


vector<float> IITkgp_functions::FindHist(Mat Data)
{
  double maxe;
  int maxposi;
  FindMaxElementPosi(Data,&maxe, &maxposi);
  
  Data.convertTo(Data,CV_64FC1);
  
  int nele =(int) (floor(maxe) + 1);
  
  vector<float> hist(nele,0.0);
  for(int i=0;i<Data.rows;i++)
  {
    for(int j=0;j<Data.cols;j++)
    {
      int posi =(int) floor(Data.at<double>(i,j));
      hist[posi]++; 
    }
  }
  for(int i=0;i<hist.size();i++)
    hist[i] = hist[i]*1.0/(Data.rows*Data.cols*1.0);
  return hist;
}

/**
 * @function FindMedian
 * @param input vector of data
 * @brief Calculate Median of Given data array
 * @return median(double) of the given array
 */


template <typename T>
double IITkgp_functions::FindMedian(vector<T> data)
{
  
  
  vector<T> sortedarray;
   
  for(int i=0;i<data.size();i++)
    sortedarray.push_back(data[i]);
  
  printf("Hello going to sort\n");
 //MergeSort<T>(sortedarray,0,data.size()-1);
  BubbleSort<T>(sortedarray);
  printf("Hello sorting is done\n");
  double Median;
  //check even 
  if(sortedarray.size()%2 == 0)
  {
    int MedPosi = (sortedarray.size()/2);
    Median = (sortedarray[MedPosi] + sortedarray[MedPosi - 1])*1.0/2;
  }
  else
  {
    int MedPosi = (sortedarray.size()/2) - 1;
    Median = sortedarray[MedPosi]*1.0;  
  }
  
  sortedarray.clear();
  
  return(Median);
}


void initializemedian()
{
  vector<int> data(10,0);
  double med = FindMedian<int>(data);
  vector<float> data1(10,0);
  med = FindMedian<float>(data1);
  vector<double> data2(10,0);
  med = FindMedian<double>(data2);
}


/**
 * @function FindMean
 * @param input Single Channel Mat data
 * @brief Calculate Mean of Given data array
 * @return mean(double) of the given array
 */



double IITkgp_functions::FindMean(Mat data)
{
  data.convertTo(data,CV_64FC1);
  double mean;
  double sum;
  sum = 0.0;
  int data_size;
  data_size = data.rows*data.cols;
  for(int i=0;i<data.rows;i++)
  {
    for(int j=0;j<data.cols;j++)
      sum = sum + data.at<double>(i,j);
  }
  
  mean = sum/data_size;
  
  return(mean);
}


/**
 * @function FindVar
 * @param input Single Channel Mat data
 * @brief Calculate variance of Given data array
 * @return Variance(double) of the given array
 */


double IITkgp_functions::FindVar(Mat data)
{
  data.convertTo(data,CV_64FC1);
  double var,mean;
  mean = FindMean(data);
  double temp;
  double sum=0.0;
  int data_size;
  data_size = data.rows*data.cols;
  for(int i=0;i<data.rows;i++)
  {
    for(int j=0;j<data.cols;j++)
    {
      temp = data.at<double>(i,j) - mean;
      sum = sum + (temp * temp);
    }
  }
  var = sum/data_size;
  
  return(var);
}


/**
 * @function FindStdDev
 * @param input Single Channel Mat data
 * @brief Calculate Standard Deviation of Given data array
 * @return Standard Deviation(double) of the given array
 */


double IITkgp_functions::FindStdDev(Mat data)
{
  
 
  data.convertTo(data,CV_64FC1);
  double std_dev,var;
  
  var = FindVar(data);
  std_dev = sqrt(var);
  
  return(std_dev);
}


/**
 * @function FindSkew
 * @param input Single Channel Mat data
 * @brief Calculate Skewness of Given data array
 * @return Skewness(double) of the given array
 */


double IITkgp_functions::FindSkew(Mat data)
{
  data.convertTo(data,CV_64FC1);
  double skew;
  double sum = 0.0;
  double mean;
  double temp;
  double std_dev;
  int data_size;
  data_size = data.rows*data.cols;
  mean = FindMean(data);
  
  for(int i=0;i<data.rows;i++)
  {
    for(int j=0;j<data.cols;j++)
    {
      temp = data.at<double>(0,i) - mean;
      sum = sum + (temp * temp * temp);
    }
  }
  sum = sum / data_size;
  
  std_dev = FindStdDev(data);
  skew = sum/(std_dev * std_dev *std_dev);
  
}


/**
 * @function FindMinElementPosi
 * @param input Single Channel Mat data and pointer to min element  and its position
 * @brief Calculate Min value of Given data array and its position
 * 
 */



void IITkgp_functions::FindMinElementPosi(Mat data, double *value, int *posi)
{
  data.convertTo(data,CV_64FC1);
  double min_element;
  min_element = data.at<double>(0,0);
  int min_posi;
  int data_size;
  data_size = data.rows*data.cols;
  for(int i=0;i<data.rows;i++)
  {
    for(int j=0;j<data.cols;j++)
    {
      if(data.at<double>(i,j)<=min_element)
      {
	min_element = data.at<double>(i,j);
	min_posi = i*data.cols+j;
      }
    }
  }
  
  *value = min_element;
  *posi = min_posi;
  
}


/**
 * @function FindMaxElement
 * @param input Single Channel Mat data and pointer to max element and pointer to position
 * @brief Calculate Max value of Given data array and its position
 * 
 */



void IITkgp_functions::FindMaxElementPosi(Mat Mdata, double *value, int *posi)
{
  Mat data;
  Mdata.convertTo(data,CV_64FC1);
  double max_element = 0.0;
 // max_element = data.at<double>(0,0);
  int max_posi = 0;
 // int data_size;
 // data_size = data.rows*data.cols;
  for(int i=0;i<data.rows;i++)
  {
    for(int j=0;j<data.cols;j++)
    {
      if(max_element <= data.at<double>(i,j))
      {
	max_element = data.at<double>(i,j);
	max_posi = i*data.cols+j;
      }
    }
  }
 // printf("max value %lf\n",max_element);
  
  *value = max_element;
  *posi = max_posi;
  
}



/**
 * @function FindMode
 * @param input Single Channel Mat data
 * @brief Calculate Mode of Given data array
 * @return mean(double) of the given array
 */


double IITkgp_functions::FindMode(Mat data)
{
  data.convertTo(data,CV_64FC1);
  double mode;

  double max_elem;
  int max_posi;
 
  
  FindMaxElementPosi(data,&max_elem,&max_posi);
  
  mode = max_elem;
  
  return(mode);
}

/**
 * @function FindVarMode
 * @param input Single Channel Mat data
 * @brief Calculate variance of Given data array
 * @return Variance(double) of the given array
 */


double IITkgp_functions::FindVarMode(Mat data)
{
  data.convertTo(data,CV_64FC1);
  double var,mode;
  mode = FindMode(data);
  double temp;
  double sum=0.0;
  int data_size;
  data_size = data.rows*data.cols;
  for(int i=0;i<data.rows;i++)
  {
    for(int j=0;j<data.cols;j++)
    {
      temp = data.at<double>(0,i) - mode;
      sum = sum + (temp * temp);
    }
  }
  var = sum/data_size;
  
  return(var);
}


/**
 * @function FindStdDevMode
 * @param input Single Channel Mat data
 * @brief Calculate Standard Deviation of Given data array based on Mode of the data
 * @return Standard Deviation(double) of the given array
 */


double IITkgp_functions::FindStdDevMode(Mat data)
{
  data.convertTo(data,CV_64FC1);
  double std_dev,var;
  
  
  var = FindVarMode(data);
  std_dev = sqrt(var);
  
  return(std_dev);
}




/**
 * @function FindHistogram
 * @param input Single Channel Mat data
 * @brief Calculate Histogram of the data
 * @return Histogram data of the element
 */



Mat IITkgp_functions::FindHistogram(Mat data)
{
  
 
  Mat HistData;
  double max_elem;
  int max_posi;
 
  FindMaxElementPosi(data,&max_elem,&max_posi);
  
  bool uniform = true; bool accumulate = false;
  int histSize = (int)max_elem;
  //printf("HistSize is %d\t%lf\n",histSize,max_elem);
 // int histSize = 256;
  /// Set the ranges
  float range[] = { 0, histSize } ;
  const float* histRange = { range };
  
  Mat ConvertedData;
  data.convertTo(ConvertedData,CV_8UC1);
  
  /// Compute the histograms:
  calcHist( &ConvertedData, 1, 0, Mat(), HistData, 1, &histSize, &histRange, uniform, accumulate );
  
 
  return(HistData);
  
}

/**
 * @function DrawHistogram
 * @param input Single Channel Mat data
 * @brief Calculate Histogram of the data and Draw it
 * 
 */

void IITkgp_functions::DrawHistogram(Mat data)
{
  Mat Histogram,NormalizedHistogram;
  
  
  Histogram = FindHistogram(data);
  
  double max_elem;
  int max_posi;
 
  FindMaxElementPosi(data,&max_elem,&max_posi);
  
  int histSize = (int)max_elem;
  //int histSize = 256;
  
  // Draw the histograms for B, G and R
  int hist_w = 512; int hist_h = 400;
  int bin_w = cvRound( (double) hist_w/histSize );

  Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
  
  //Histogram.convertTo(Histogram,CV_8UC1);

  /// Normalize the result to [ 0, histImage.rows ]
  normalize(Histogram, Histogram, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
 
  for( int i = 1; i < histSize; i++ )
  {
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(Histogram.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(Histogram.at<float>(i)) ),
                       Scalar( 255, 0, 0), 2, 8, 0  );
  }
 
 
  /// Display
  namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
  imshow("calcHist Demo", histImage );
  
}

/*-------------------------------------------------------------------------------------------------------------------------------------------*/