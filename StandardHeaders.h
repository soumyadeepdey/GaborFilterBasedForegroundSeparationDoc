#ifndef StandardHeaders_H
#define StandardHeaders_H


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <queue>
#include <list>
#include <stack>
#include <search.h>
#include <time.h>

#include "opencv2/opencv_modules.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"


#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;




#define PI 3.14159265

typedef struct Statistics
{
  double mean;
  double var;
  int number;
}Stat;



#endif