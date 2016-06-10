#include "StandardHeaders.h"
#include "GaborAngleFeature.h"
#include "folder.h"

int main(int argc, char* argv[])
{
  Mat src = imread(argv[1],1);
  char *name;
  name = input_image_name_cut(argv[1]);
  
  vector<vector<float> > GFData;
  GFData = GetMyGaborFeature(src,name);
  
  return 0;
  
}