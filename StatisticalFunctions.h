#ifndef StatisticalFunctions_H
#define StatisticalFunctions_H


#include "StandardHeaders.h"
#include "sorting.h"


namespace IITkgp_functions {

/**
 * Input single Channel 8 bit Data
 * Output vector of float of size 256
 * 
 */
vector<float> FindHist(Mat Data);



  /**
 * @function FindMedian
 * @param input vector of data
 * @brief Calculate Median of Given data array
 * @return median(double) of the given array
 */
template <typename T>
double FindMedian(vector<T> data);
void initializemedian();

    /**
     * @function FindMean
     * @param input Single Channel Mat data
     * @brief Calculate Mean of Given data array
     * @return mean(double) of the given array
     */
    double FindMean(Mat data);


/**
 * @function FindVar
 * @param input Single Channel Mat data
 * @brief Calculate variance of Given data array
 * @return Variance(double) of the given array
 */
    double FindVar(Mat data);



/**
 * @function FindStdDev
 * @param input Single Channel Mat data
 * @brief Calculate Standard Deviation of Given data array
 * @return Standard Deviation(double) of the given array
 */
    double FindStdDev(Mat data);

/**
 * @function FindSkew
 * @param input Single Channel Mat data
 * @brief Calculate Skewness of Given data array
 * @return Skewness(double) of the given array
 */
    double FindSkew(Mat data);


/**
 * @function FindMinElementPosi
 * @param input Single Channel Mat data and pointer to min element  and its position
 * @brief Calculate Min value of Given data array and its position
 *
 */
    void FindMinElementPosi(Mat data, double *value, int *posi);

/**
 * @function FindMaxElement
 * @param input Single Channel Mat data and pointer to max element and pointer to position
 * @brief Calculate Max value of Given data array and its position
 *
 */
    void FindMaxElementPosi(Mat data, double *value, int *posi);
    
    /**
 * @function FindMode
 * @param input Single Channel Mat data
 * @brief Calculate Mode of Given data array
 * @return mean(double) of the given array
 */


double FindMode(Mat data);

/**
 * @function FindVarMode
 * @param input Single Channel Mat data
 * @brief Calculate variance of Given data array
 * @return Variance(double) of the given array
 */


double FindVarMode(Mat data);

/**
 * @function FindStdDevMode
 * @param input Single Channel Mat data
 * @brief Calculate Standard Deviation of Given data array based on Mode of the data
 * @return Standard Deviation(double) of the given array
 */

double FindStdDevMode(Mat data);

/**
 * @function FindHistogram
 * @param input Single Channel Mat data
 * @brief Calculate Histogram of the data
 * @return Histogram data of the element
 */
    Mat FindHistogram(Mat data);

/**
 * @function DrawHistogram
 * @param input Single Channel Mat data
 * @brief Calculate Histogram of the data and Draw it
 *
 */
    void DrawHistogram(Mat data);



}



#endif