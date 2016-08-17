#ifndef StrokeWidth_H
#define StrokeWidth_H


#include "StandardHeaders.h"


struct BeanStucture{
  int BeanNumber;
  int MaxElement;
  int LowerBound;
  int UpperBound;
  int middle;
};

struct Ray {
        Point2i p;
        Point2i q;
        vector<Point2i> points;
    int dist;
    void CalcEcluiDist()
    {
      dist =(int) sqrt( ((p.x - q.x)*(p.x-q.x)) + ((p.y-q.y)*(p.y-q.y)) );
    }
};

namespace IITkgp_functions {

  
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

Mat FindGradient(Mat Image);

/**
 * @function CreateBean
 * @param input NumberOfBean, MaxElement = Maximum value of a element possible
 * @brief Calculate structure of a bean for gradiend angles and generate other informations related to a bean
 * @output array of BeanStucture with n number of elements, where n = NumberOfBean
 * 
 */

BeanStucture * CreateBean(int NumberOfBean, int MaxElement);


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

int FindOpositeBean(int BeanNumber, int NumberOfBean);


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

int FindBeanNumber(int angle, int MaxElement, int NumberOfBean);


/**
 * @function FindNextPixel8Bean
 * @param input	 p = a Point2I data type 
 * 		 bean = Bean number for that particular pointer
 * @brief	This function calculate the next pixel position of the given pixel 'p' in the direction of the given bean number.
 * 		This function is made for 8 bean.
 * @output	Next Pixel position of a particular pixel 'p' in the direction of the given bean number
 * 
 */

Point2i FindNextPixel8Bean(Point2i p, int Bean);

/**
 * @function FindNextPixel12Bean
 * @param input	 p = a Point2I data type 
 * 		 bean = Bean number for that particular pointer
 * @brief	This function calculate the next pixel position of the given pixel 'p' in the direction of the given bean number.
 * 		This function is made for 12 bean.
 * @output	Next Pixel position of a particular pixel 'p' in the direction of the given bean number
 * 
 */

Point2i FindNextPixel12Bean(Point2i p, int Bean);

/**
 * @function FindNextPixel16Bean
 * @param input	 p = a Point2I data type 
 * 		 bean = Bean number for that particular pointer
 * @brief	This function calculate the next pixel position of the given pixel 'p' in the direction of the given bean number.
 * 		This function is made for 16 bean.
 * @output	Next Pixel position of a particular pixel 'p' in the direction of the given bean number
 * 
 */

Point2i FindNextPixel16Bean(Point2i p, int Bean);


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


Ray FindStrokeWidth(Point2i p,int Bean, Mat BoundaryImage, Mat GradBean, Mat GrayImage, int NumberOfBean);



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


vector<Ray> SWT(Mat Image, Mat BinaryImage);

vector<float> GetStrokeWidthFeature(Mat Image, Mat BinaryImage);


}


#endif
