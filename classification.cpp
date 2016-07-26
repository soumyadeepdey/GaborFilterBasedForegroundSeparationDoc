#include "classification.h"



TDC Training(char* TrainFile)
{
  vector<vector<float> > TrainData;
  vector<int> TrainClass;
  vector<int> ClassNumber;
  vector<int> NumberPerCluster;
  
  int classnumber = 0;
  
  FILE *f;
  f = fopen(TrainFile,"r");
  while(!feof(f))
  {
      
      char filename[2000];
      fscanf(f,"%s",&filename);
      printf("%s\n",filename);
  }
}
