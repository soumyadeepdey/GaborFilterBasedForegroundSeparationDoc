#ifndef RunLengthFeature_H
#define RunLengthFeature_H


#include "StandardHeaders.h"
#include "StatisticalFunctions.h"
#include "PixelValidation.h"




typedef struct Lengths
{
  int h;
  int v;
  int l;
  int r;
}L;

typedef struct RunLengthElement
{
  vector<int> h;
  vector<int> v;
  vector<int> l;
  vector<int> r;
}RLE;

typedef struct RunLength
{
  RLE B; // Background pvalue = 255
  RLE F; // Foreground pvalue = 0
}RL;


typedef struct RLFeatureElement
{
  Stat h;
  Stat v;
  Stat l;
  Stat r;
}RLFE;

typedef struct RunLengthFeature
{
  RLFE B; // Background pvalue = 255
  RLFE F; // Foreground pvalue = 0
}RLF;


/**
   * @func GetHRL : Get the run length from a pixel p in horizontal direction
   * @input: An input image, a pixel p, and the value at pixel p 
   * @def: This function calculate the number of pixels present in horizontal direction 
   * 	   from pixel p having same value as p
   * @output: This function return the length in the given direction
   */  
  int GetHRL(Mat image, Point p, int pvalue);
  
  /**
   * @func GetVRL : Get the run length from a pixel p in vertical direction
   * @input: An input image, a pixel p, and the value at pixel p 
   * @def: This function calculate the number of pixels present in vertical direction 
   * 	   from pixel p having same value as p
   * @output: This function return the length in the given direction
   */  
  int GetVRL(Mat image, Point p, int pvalue);
  
  /**
   * @func GetRRL : Get the run length from a pixel p in Right diagonal direction
   * @input: An input image, a pixel p, and the value at pixel p 
   * @def: This function calculate the number of pixels present in Right diagonal direction 
   * 	   from pixel p having same value as p
   * @output: This function return the length in the given direction
   */  
  
  int GetRRL(Mat image, Point p, int pvalue);
  
  /**
   * @func GetLRL : Get the run length from a pixel p in Left diagonal direction
   * @input: An input image, a pixel p, and the value at pixel p 
   * @def: This function calculate the number of pixels present in Left diagonal direction 
   * 	   from pixel p having same value as p
   * @output: This function return the length in the given direction
   */  
  
  int GetLRL(Mat image, Point p, int pvalue);
  
  
  /**
   * @func GetRLofPixel : Get the run length from a pixel p in horizontal, vertical, right-diagonal and left-diagonal direction
   * @input: An input image, a pixel p, and the value at pixel p 
   * @def: This function calculate the number of pixels present in horizontal, vertical, right-diagonal and left-diagonal direction 
   * 	   from pixel p having same value as p
   * @output: This function return the set of length in 4 directions
   */ 
  
  L GetRLofPixel(Mat image, Point p, int pvalue);

 
  /**
   * @func ComputeRLElements : Get the run length elements for the given binary image
   * @input: An input image
   * @def: This function calculate the run length feature of an image
   * @output: This function return RunLength Elements
   */ 
  
  RLF ComputeRLFeatures(Mat image);
  

  void RLFtoFVector(RLF x, vector<float> &y);

#endif