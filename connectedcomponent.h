#ifndef ConnectedComponent_H
#define ConnectedComponent_H



#include "StandardHeaders.h"




/*-------------------------------------------------------- Structure Declarations--------------------------------------------------------*/


typedef struct imagestructure
{
    int x;
    int y;
    int label;
    int value;
    int mapped;
    Point p;

}is;


typedef struct connected_component
{
    struct imagestructure im;
    struct connected_component *nxt;
}cc;



typedef struct numberofcc
{
    struct connected_component *start;
    struct connected_component *last;
    float gray_hist[256];
    /*int text_nontext
     * @value : 0 if text
     *          1 if non_text
     */
    int text_nontext;
    /*int label
     * @value : value of the color label from k-mean
     */
    int label;
    int number_of_component;
    int flag;
    int xmin;
    int ymin;
    int xmax;
    int ymax;
    int centroidx;
    int centroidy;
    int blk_no;
    int valid_blk_no;
    float histogram[256];
    double mean;
    double var;
    double std_dev;
    double skew;
    void calc_centroid()
    {
      centroidx =  xmin+((xmax-xmin)/2);
      centroidy =  ymin+((ymax-ymin)/2);
    }
    void calc_hist()
    {

      for(int i=0;i<256;i++)
        histogram[i]=gray_hist[i]/number_of_component;
    }

    void calc_mean()
    {
      mean = 0;
      for(int i=0;i<256;i++)
      {
        mean = mean + (gray_hist[i]*i);
      }
      mean = mean/number_of_component;
    }


    void calc_dev()
    {
      calc_mean();
      int temp;
      std_dev = 0.0;
      var = 0.0;
      skew = 0.0;
      for(int i=0;i<256;i++)
      {
        for(int j=0;j<gray_hist[i];j++)
        {
          temp = i - mean;
          var = var + (temp * temp);
          skew = skew + (temp * temp *temp);
        }
      }

      var = var / number_of_component;
      std_dev = sqrt(var);

    }

    void calc_distribution()
    {
      calc_hist();
      calc_mean();
      calc_dev();
    }

}nocc;




namespace IITkgp_functions {
  
  

    /**
     * @function labelingdfs
     * @param : input param: input image structure(of (IS(datatype))) input co-ordinate(m,n) to belabeled and image dimention(row,col)
     * @brief : labeling based on DFS algorithm
     * @return : image structure(of (IS(datatype)))
     */
    is ** labelingdfs(is **tempstructuredimage,int *m, int *n, int temp_row, int temp_col);

    /**
     * @function cclabeling
     * @param : input param: input image structure(of (IS(datatype))) and image dimention(row,col)
     * @brief : labeling of ConnectedComponent based on DFS algorithm
     * @return : image structure(of (IS(datatype)))
     */


    is** cclabeling(is **tempstructuredimage,int temp_row, int temp_col);

    /**
     * @function FindConnectedComponent
     * @param : input param: input image(binary) Mat data-type
     *                             src_gray(gray scale) Mat data-type
     * @brief : Find connected component from the input binary image
     * @return : array of connected components (nocc(data-type))
     */


    nocc* FindConnectedComponent(Mat image, Mat src_gray);
    
    
    vector<vector<Point> > Convert_nocc2pointvector(nocc *component);


}




#endif