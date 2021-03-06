#include "classification.h"
#include "WriteintoXML.h"
#include "folder.h"

bool CCCN_flag;
bool CCE_flag;

using namespace IITkgp_functions;



int findtypepresence(vector<char*> types, char* type)
{
  int i;
  for( i=0;i<types.size();i++)
  {
    if(strcmp(types[i],type)==0)
    {
      return i;
    }
  }
  return i;
}


bool CheckNoise(SB B, Mat image)
{
  Rect R = boundingRect( Mat(B.Contours) );
  if(R.height>R.width)
		{
		  //printf("Gt Class is %d\n",B.GtClass);
		  Point2i centroid;
		  centroid.x=R.x+(R.width/2);
		  centroid.y=R.y+(R.height/2);
		  int distx,disty;
		  if(centroid.x<(image.cols-centroid.x))
		    distx=centroid.x;
		  else
		    distx=image.cols-centroid.x;
		  if(centroid.y<(image.rows-centroid.y))
		    disty=centroid.y;
		  else
		    disty=image.rows-centroid.y;
		  int distb;
		  if(distx>disty)
		  {
		   // printf("Dist frm Bound: %d\t",disty);
		    distb = disty;
		  }
		  else
		  {
		    distb = distx;
		    //printf("Dist frm Bound: %d\t",distx);
		  }
		  float ratio = R.width*1.0;
		  ratio = ratio/(R.height*1.0);
		  if(distb<30 && R.width<33)
		    return true;
		  else if(distb>30 && distb<100 && R.width<33 && ratio > 0.5)
		    return true;
		  else
		    return false;
		  
		}
		else
		{
		 // printf("Gt Class is %d\n",B.GtClass);
		  Point2i centroid;
		  centroid.x=R.x+(R.width/2);
		  centroid.y=R.y+(R.height/2);
		  int distx,disty;
		  if(centroid.x<(image.cols-centroid.x))
		    distx=centroid.x;
		  else
		    distx=image.cols-centroid.x;
		  if(centroid.y<(image.rows-centroid.y))
		    disty=centroid.y;
		  else
		    disty=image.rows-centroid.y;
		  int distb;
		  if(distx>disty)
		  {
		   // printf("Dist frm Bound: %d\t",disty);
		    distb = disty;
		  }
		  else
		  {
		    distb = distx;
		   // printf("Dist frm Bound: %d\t",distx);
		  }
		  
		  float ratio = R.height*1.0;
		  ratio = ratio/(R.width*1.0);
		  if(distb<30 && R.height<33)
		    return true;
		  else if(distb>30 && distb<100 && R.height<33 && ratio > 0.5)
		    return true;
		  else
		    return false;
		 
		}
 
}

bool CheckSeparator(SB B, Mat image)
{

    Rect R = boundingRect( Mat(B.Contours) );
  if(R.height>R.width)
		{
		  //printf("Gt Class is %d\n",B.GtClass);
		  Point2i centroid;
		  centroid.x=R.x+(R.width/2);
		  centroid.y=R.y+(R.height/2);
		  int distx,disty;
		  if(centroid.x<(image.cols-centroid.x))
		    distx=centroid.x;
		  else
		    distx=image.cols-centroid.x;
		  if(centroid.y<(image.rows-centroid.y))
		    disty=centroid.y;
		  else
		    disty=image.rows-centroid.y;
		  int distb;
		  if(distx>disty)
		  {
		   // printf("Dist frm Bound: %d\t",disty);
		    distb = disty;
		  }
		  else
		  {
		    distb = distx;
		    //printf("Dist frm Bound: %d\t",distx);
		  }
		  
		  float ratio = R.width*1.0;
		  ratio = ratio/(R.height*1.0);
		  
		  if(R.width<70 && ratio < 0.1)
		    return true;
		  else
		    return false;
		    
		  
		}
		else
		{
		 // printf("Gt Class is %d\n",B.GtClass);
		  Point2i centroid;
		  centroid.x=R.x+(R.width/2);
		  centroid.y=R.y+(R.height/2);
		  int distx,disty;
		  if(centroid.x<(image.cols-centroid.x))
		    distx=centroid.x;
		  else
		    distx=image.cols-centroid.x;
		  if(centroid.y<(image.rows-centroid.y))
		    disty=centroid.y;
		  else
		    disty=image.rows-centroid.y;
		  int distb;
		  if(distx>disty)
		  {
		   // printf("Dist frm Bound: %d\t",disty);
		    distb = disty;
		  }
		  else
		  {
		    distb = distx;
		   // printf("Dist frm Bound: %d\t",distx);
		  }
		  
		  float ratio = R.height*1.0;
		  ratio = ratio/(R.width*1.0);
		  
		  if(R.height<70 && ratio < 0.1)
		    return true;
		  else
		    return false;
		 
		}
}

vector<float> GetFeatureFromFVec(vector<float> FeatureVec)
{
  vector<float> Fvec;
  for(int i=0;i<FeatureVec.size();i=i+3)
  {
    Fvec.push_back(FeatureVec[i]);
    Fvec.push_back(FeatureVec[i+1]);
  }
  return Fvec;
}

void classify(char *TestFILE, char *classifiername, char *KFoldNum, char *ClusteringName, TDC &Data)
{
  vector<int> classnumber;
      
      vector<ConfusionMatrix> CM_ALL;
      
     // vector<ConfusionMatrix> CM_ALL(classnumber.size(),ConfusionMatrix(classnumber.size()));
  
  printf("Classifiername = %s\n",classifiername);
  
  char *dirname;
  
if(classifyAll)
{
    dirname = (char *)malloc(2001*sizeof(char));
    strcpy(dirname,"ClassifyAll"); 
}
      
if(classifySelected)
{
    dirname = (char *)malloc(2001*sizeof(char));
    strcpy(dirname,"ClassifySelected");
}
      
if(classifySelected_ignore)
{
    dirname = (char *)malloc(2001*sizeof(char));
    strcpy(dirname,"ClassifySelectedIgnore");
}
 
if(classifyTG)
{
    dirname = (char *)malloc(2001*sizeof(char));
    strcpy(dirname,"ClassifyTG");
}

if(classifyTG_ignore)
{
    dirname = (char *)malloc(2001*sizeof(char));
    strcpy(dirname,"ClassifyTGIgnore");
}
  
    makedir(dirname);
    
    char *classidir = CreateNameIntoFolder(dirname,classifiername);
    makedir(classidir);
    
  char *Classi_KFoldFolder = CreateNameIntoFolder(classidir,KFoldNum);
  makedir(Classi_KFoldFolder);
  
  char *tempfol = CreateNameIntoFolder(classidir,ClusteringName);
  makedir(tempfol);
  char *Classi_Clus_KFoldFolder = CreateNameIntoFolder(tempfol,KFoldNum);
  makedir(Classi_Clus_KFoldFolder);
  
  if(strcmp(ClusteringName,"CCCN")==0)
    CCCN_flag = true;
  else
    CCCN_flag = false;
  
  if(strcmp(ClusteringName,"CCE")==0)
    CCE_flag = true;
  else
    CCE_flag = false;
      
  FILE *f;
  f = fopen(TestFILE,"r");
  while(!feof(f))
  {
    char filename[2000];
      fscanf(f,"%s",&filename);
      printf("%s\n",filename);
      page pg = GetPageGroundtruth(filename);
  
      char *name;
      name = input_image_name_cut(pg.GetImageName());
      //makedir(name);
      
      char *opt;
      opt = (char *) malloc ( 2001 * sizeof(char));
      if(opt == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      strcpy(opt,name);

      char *tnm;
      tnm = (char *) malloc ( 2001 * sizeof(char));
      if(tnm == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      tnm = ".jpg";
      strcat(opt,tnm);
      
      char *tnm1;
      tnm1 = (char *) malloc ( 2001 * sizeof(char));
      if(tnm1 == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      strcpy(tnm1,opt);
      
      printf("imagename %s\n",tnm1);
      
      Mat image = imread(tnm1,1);
      
      //vector<SB> blocks = GetSegmentationUnit(image);
      
      vector<SB> blocks = GetProcessingBlocks(image);
      
      
      
if(classifyAll)
{
      classnumber.push_back(0); classnumber.push_back(1); 
      classnumber.push_back(2); classnumber.push_back(3); 
      classnumber.push_back(4); classnumber.push_back(5);
      classnumber.push_back(6); classnumber.push_back(7);
      
      CM_ALL.clear();
      CM_ALL.resize(classnumber.size(),ConfusionMatrix(classnumber.size()));
      
      blocks = PrepareAlethiaGt(pg,blocks);
      
}
      
if(classifySelected)
{
      classnumber.push_back(0); classnumber.push_back(1);  
      classnumber.push_back(4); classnumber.push_back(5);
      
      CM_ALL.clear();
      CM_ALL.resize(classnumber.size(),ConfusionMatrix(classnumber.size()));
      
      blocks = PrepareAlethiaGt_tgns(pg,blocks);
}
      
if(classifySelected_ignore)
{
      classnumber.push_back(0); classnumber.push_back(1);  
      classnumber.push_back(4); classnumber.push_back(5);
      
      CM_ALL.clear();
      CM_ALL.resize(classnumber.size(),ConfusionMatrix(classnumber.size()));
      
      blocks = PrepareAlethiaGt_tgns_ignore(pg,blocks);
}
 
if(classifyTG)
{
      classnumber.push_back(0); classnumber.push_back(1);  
      classnumber.push_back(4); classnumber.push_back(5);
      
      CM_ALL.clear();
      CM_ALL.resize(classnumber.size(),ConfusionMatrix(classnumber.size()));
      
      blocks = PrepareAlethiaGt_tg(pg,blocks);
}

if(classifyTG_ignore)
{
      classnumber.push_back(0); classnumber.push_back(1);  
      classnumber.push_back(4); classnumber.push_back(5);
      
      CM_ALL.clear();
      CM_ALL.resize(classnumber.size(),ConfusionMatrix(classnumber.size()));
      
      blocks = PrepareAlethiaGt_tg_ignore(pg,blocks);
}

     
      
      
      for(int i=0;i<blocks.size();i++)
      {
	SB B = blocks[i];
	if(B.PredFlag && B.gtflag)
	{
	  if(CheckNoise(B,image))
	  {
	    if(classifyTG)
	    {
	      B.PredictedClass = 1;
	      B.PredFlag = false;
	    }
	    else if(classifyTG_ignore)
	      B.PredictedClass = 1;
	    else
	    {
	      B.PredictedClass = 5;
	      B.PredFlag = false;
	    }
	    
	  }
	  else if(CheckSeparator(B,image))
	  {
	    if(classifyTG)
	    {
	      B.PredictedClass = 1;
	      B.PredFlag = false;
	    }
	    else if(classifyTG_ignore)
	      B.PredictedClass = 1;
	    else
	    {
	      B.PredictedClass = 4;
	      B.PredFlag = false;
	    }
	    
	  }
	}
	blocks[i] = B;
      }
      
      
      
      //WriteGTXMLFile();
      
      vector<SB> SU;
      
      int su_cnt = 0;
      
      
     
      Classification(blocks,Data,classifiername);
      
      
      
       for(int i=0;i<blocks.size();i++)
      {
	SB B = blocks[i];
	
	if(B.childs.empty())
	{
	  if(B.Fvecflag && B.gtflag)
	  {
	    B.blockid = su_cnt;
	    SU.push_back(B);
	    su_cnt++;
	  }
	}
	else
	{
	  for(int k=0;k<B.childs.size();k++)
	  {
	    printf("Wrong move for now\n");
	    SB B_C = B.childs[k];
	    if(B_C.Fvecflag && B_C.gtflag);
	    { 
	      B_C.blockid = su_cnt;
	      SU.push_back(B_C);
	      su_cnt++;
	    }
	  }
	}
      }
      
      blocks.clear();
      
      
      
      char *initial_name = input_image_name_cut(pg.GetImageName()); 
      
      char *output,*tempname, *gtname,*outname;
      output = (char *) malloc ( 2001 * sizeof(char));
      if(output == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      strcpy(output,initial_name);

      tempname = (char *) malloc ( 2001 * sizeof(char));
      if(tempname == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      tempname = "_gt.xml";
      strcat(output,tempname);
      gtname = CreateNameIntoFolder(Classi_KFoldFolder,output);
      
      WriteGTXMLFile(pg,gtname,SU);
      
      
      
      output = (char *) malloc ( 2001 * sizeof(char));
      if(output == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      strcpy(output,initial_name);

      tempname = (char *) malloc ( 2001 * sizeof(char));
      if(tempname == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      tempname = "_out.xml";
      strcat(output,tempname);
      outname = CreateNameIntoFolder(Classi_KFoldFolder,output);
      
      WriteOutputXMLFile(pg,outname,SU);
      
      
      vector<vector<SB> > Clusters;
      vector<double> alpha(classnumber.size(),0.5);
      
      if(CCCN_flag)
      {
	printf("CCCN Clustering\n");
	Clusters = ClusteringCCN(SU,alpha);
	printf("Number of Clusters obetained by  CCCN is %d\n",Clusters.size());
      }
      if(CCE_flag)
      {
	printf("CCE Clustering\n");
	Clusters = ClusteringCE(SU,alpha);
	printf("Number of Clusters obetained by  CCE is %d\n",Clusters.size());
      }
      
      ClusteringClassification(SU,Clusters,alpha);
      
      printf("ClusteringClassification Done\n");
      
      for(int i=0;i<SU.size();i++)
      {
	SB B = SU[i];
	if(B.Contours.size()==0)
	 {
	   printf("There is some problem after Clustering Classification\n");
	   exit(0);
	 }
      }
      
      output = (char *) malloc ( 2001 * sizeof(char));
      if(output == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      strcpy(output,initial_name);

      tempname = (char *) malloc ( 2001 * sizeof(char));
      if(tempname == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      tempname = "_gt.xml";
      strcat(output,tempname);
      gtname = CreateNameIntoFolder(Classi_Clus_KFoldFolder,output);
      
      WriteGTXMLFile(pg,gtname,SU);
      
      printf("GT XML Writing Done\n");
      
      output = (char *) malloc ( 2001 * sizeof(char));
      if(output == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      strcpy(output,initial_name);

      tempname = (char *) malloc ( 2001 * sizeof(char));
      if(tempname == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      tempname = "_out.xml";
      strcat(output,tempname);
      outname = CreateNameIntoFolder(Classi_Clus_KFoldFolder,output);
      
      WriteOutputXMLFile(pg,outname,SU);
      
      printf("Out XML Writing Done\n");
      
      vector<int> gtcls;
      vector<int> pcls;
      
      for(int i=0;i<SU.size();i++)
      {
	SB B = SU[i];
	printf("Block id =%d\t gt= %d\t predict = %d\n",B.blockid,B.GtClass,B.PredictedClass);
	gtcls.push_back(B.GtClass);
	pcls.push_back(B.PredictedClass);
      }
      if(!SU.empty())
	SU.clear();
      
      Mat Gtlabels = Mat(gtcls.size(),1,CV_8UC1);
      Mat PredictedLabels = Mat(gtcls.size(),1,CV_8UC1);
      
      for(int i=0;i<gtcls.size();i++)
      {
	Gtlabels.at<int>(i,0) = gtcls[i];
	if(gtcls[i]>7)
	{
	  printf("B.GtClass = %d\n",gtcls[i]);
	  exit(0);
	}
	if(pcls[i]>7)
	{
	   printf(" Error B.predictedclass = %d\n",pcls[i]);
	  exit(0);
	}
	PredictedLabels.at<int>(i,0) = pcls[i];
      }
      if(!gtcls.empty())
	gtcls.clear();
      if(!pcls.empty())
	pcls.clear();
      
      vector<ConfusionMatrix> CM = GetConfusionMatrix(Gtlabels,PredictedLabels,classnumber);
      //char *tempname;
	 opt = (char *)malloc(2000*sizeof(char));
	 tempname = (char *)malloc(2000*sizeof(char));
	 
	 tempname = "image_Classification_Result.xls";
	 
	opt = CreateNameIntoFolder(tempfol,tempname);
	 
	 
	 
	 FILE *res;
	 
	 res = fopen(opt,"a+");
	 
	 
	 fprintf(res,"%s\t",name);
	 for(int j=0;j<CM.size();j++)
	 {
	   ConfusionMatrix cm_j = CM[j];
	   fprintf(res,"%s\t%d\t%d\t%d\t%d\t%d\t",classifiername,classnumber[j],cm_j.tp,cm_j.fp,cm_j.tn,cm_j.fn);
	   
	   CM_ALL[j].tp = CM_ALL[j].tp + cm_j.tp;
	   CM_ALL[j].fp = CM_ALL[j].fp + cm_j.fp;
	   CM_ALL[j].tn = CM_ALL[j].tn + cm_j.tn;
	   CM_ALL[j].fn = CM_ALL[j].fn + cm_j.fn;
	   
	 }
	 fprintf(res,"\n");
	 fclose(res);
	 if(!CM.empty()) 
	  CM.clear();
  }
  
  
 printf("ALL Input Image classification Completed\n");
 
  char *tempname,*name;
  name = (char *)malloc(2000*sizeof(char));
  tempname = (char *)malloc(2000*sizeof(char));	 
  tempname = "Overall_Classification_Result.xls";
  name = CreateNameIntoFolder(tempfol,tempname);
  
  FILE *res;
  res = fopen(name,"a+");

    MultiClassPerformanceMetrics M(CM_ALL);
  
    fprintf(res,"%s\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",classifiername,M.GetAverageAccuracy(),M.GetErrorRate(),M.GetPrecesionMu(),M.GetRecallMu(),M.GetFScoreMu(1),M.GetPrecesionM(),M.GetRecallM(),M.GetFScoreM(1));
 
    //M.~MultiClassPerformanceMetrics();

  
  fclose(res);

  
}


TDC Training(char* TrainFile)
{
  vector<vector<float> > TrainData;
  vector<int> TrainClass;
  vector<int> NumberPerCluster;
  
  vector<int> TrainClassHist(8,0);
  //int classnumber = 0;
  
  //makedir("TrainClassifiers");
  
  FILE *f;
  f = fopen(TrainFile,"r");
  while(!feof(f))
  {
      
      char filename[2000];
      fscanf(f,"%s",&filename);
      //printf("%s\n",filename);
      page pg = GetPageGroundtruth(filename);
  
      char *name;
      name = input_image_name_cut(pg.GetImageName());
      //makedir(name);
      
      char *opt;
      opt = (char *) malloc ( 2001 * sizeof(char));
      if(opt == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      strcpy(opt,name);

      char *tnm;
      tnm = (char *) malloc ( 2001 * sizeof(char));
      if(tnm == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      tnm = ".jpg";
      strcat(opt,tnm);
      
      char *tnm1;
      tnm1 = (char *) malloc ( 2001 * sizeof(char));
      if(tnm1 == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      strcpy(tnm1,opt);
      
    //  printf("imagename %s\n",tnm1);
      
      Mat image = imread(tnm1,1);
      
      //vector<SB> blocks = GetSegmentationUnit(image);
      
      vector<SB> blocks = GetProcessingBlocks(image);
      
     // printf("Initial blocks size %d\n",blocks.size());
      
     // blocks = PrepareAlethiaGt(pg,blocks);
      
      if(classifyAll)
{     
      blocks = PrepareAlethiaGt(pg,blocks);
      
}
      
if(classifySelected)
{      
      blocks = PrepareAlethiaGt_tgns(pg,blocks);
}
      
if(classifySelected_ignore)
{
      
      blocks = PrepareAlethiaGt_tgns_ignore(pg,blocks);
}
 
if(classifyTG)
{
      
      blocks = PrepareAlethiaGt_tg(pg,blocks);
}

if(classifyTG_ignore)
{
      
      blocks = PrepareAlethiaGt_tg_ignore(pg,blocks);
}
  
  
  FILE *ns,*sep;
  ns=fopen("Noise.xls","a+");
  sep=fopen("Separator.xls","a+");
    //  printf("GT block size %d\n",blocks.size());
      vector<char*> type;
      vector<int> cnttype;
      for(int i=0;i<blocks.size();i++)
      {
	
	
	SB B = blocks[i];
	if(B.childs.empty())
	{
	  
	  if(B.Fvecflag && B.gtflag)
	  {
	    //printf("Hello\n");
	    if(B.GtClass>7)
	    {
	      printf("Error .... value of gt is %d\n",B.GtClass);
	      exit(0);
	    }
	    if(B.GtClass == 0)
	    {
	      if(strcmp(B.bgcolor,"White")==0)
	      {
		if(type.empty())
		{
		  type.push_back(B.type);
		  cnttype.push_back(0);
		  TrainData.push_back( GetFeatureFromFVec(B.FeatureVec));
		  TrainClass.push_back(B.GtClass);
		}
		else
		{
		  int po = findtypepresence(type,B.type);
		  if(po==type.size())
		  {
		    type.push_back(B.type);
		    cnttype.push_back(0);
		    TrainData.push_back(GetFeatureFromFVec(B.FeatureVec));
		    TrainClass.push_back(B.GtClass);
		  }
		  else
		  {
		    if(cnttype[po]<7)
		    {
		      TrainData.push_back(GetFeatureFromFVec(B.FeatureVec));
		      TrainClass.push_back(B.GtClass);
		      cnttype[po] = cnttype[po] + 1;
		    }
		  }
		}
	      }
	      else
	      {
		TrainData.push_back(GetFeatureFromFVec(B.FeatureVec));
		TrainClass.push_back(B.GtClass);
	      }
	      
	    }
	    else
	    {
	      if(B.GtClass!=4&&B.GtClass!=5)
	      {
		//printf("Gt Class is %d\n",B.GtClass);
		TrainData.push_back(GetFeatureFromFVec(B.FeatureVec));
		TrainClass.push_back(B.GtClass);
	      }
	      else
	      {
		
		Rect R = boundingRect( Mat(B.Contours) );
		if(R.height>R.width)
		{
		  //printf("Gt Class is %d\n",B.GtClass);
		  Point2i centroid;
		  centroid.x=R.x+(R.width/2);
		  centroid.y=R.y+(R.height/2);
		  int distx,disty;
		  if(centroid.x<(image.cols-centroid.x))
		    distx=centroid.x;
		  else
		    distx=image.cols-centroid.x;
		  if(centroid.y<(image.rows-centroid.y))
		    disty=centroid.y;
		  else
		    disty=image.rows-centroid.y;
		  int distb;
		  if(distx>disty)
		  {
		   // printf("Dist frm Bound: %d\t",disty);
		    distb = disty;
		  }
		  else
		  {
		    distb = distx;
		    //printf("Dist frm Bound: %d\t",distx);
		  }
		  //printf("Posi: (%d,%d)\t",centroid.y,centroid.x);
		 // printf("width: %d\t",R.width);
		  //printf("Ratio(h/w): %f\n",(R.width*1.0/R.height));
		  if(B.GtClass==4)
		    fprintf(sep,"%d\t%d\t%f\n",distb,R.width,(R.width*1.0/R.height));
		  if(B.GtClass==5)
		    fprintf(ns,"%d\t%d\t%f\n",distb,R.width,(R.width*1.0/R.height));
		}
		else
		{
		 // printf("Gt Class is %d\n",B.GtClass);
		  Point2i centroid;
		  centroid.x=R.x+(R.width/2);
		  centroid.y=R.y+(R.height/2);
		  int distx,disty;
		  if(centroid.x<(image.cols-centroid.x))
		    distx=centroid.x;
		  else
		    distx=image.cols-centroid.x;
		  if(centroid.y<(image.rows-centroid.y))
		    disty=centroid.y;
		  else
		    disty=image.rows-centroid.y;
		  int distb;
		  if(distx>disty)
		  {
		   // printf("Dist frm Bound: %d\t",disty);
		    distb = disty;
		  }
		  else
		  {
		    distb = distx;
		   // printf("Dist frm Bound: %d\t",distx);
		  }
		 // printf("Posi: (%d,%d)\t",centroid.y,centroid.x);
		 // printf("width: %d\t",R.height);
		 // printf("Ratio(h/w): %f\n",(R.height*1.0/R.width));
		  if(B.GtClass==4)
		    fprintf(sep,"%d\t%d\t%f\n",distb,R.height,(R.height*1.0/R.width));
		  if(B.GtClass==5)
		    fprintf(ns,"%d\t%d\t%f\n",distb,R.height,(R.height*1.0/R.width));
		}
		
	      }
	    }
	  }
	  else
	  {
	    //printf("Gt Class in not FVEC is %d\n",B.GtClass);
	    Rect R = boundingRect( Mat(B.Contours) );
	    if(R.height>R.width)
		{
		  //printf("Gt Class is %d\n",B.GtClass);
		  Point2i centroid;
		  centroid.x=R.x+(R.width/2);
		  centroid.y=R.y+(R.height/2);
		  int distx,disty;
		  if(centroid.x<(image.cols-centroid.x))
		    distx=centroid.x;
		  else
		    distx=image.cols-centroid.x;
		  if(centroid.y<(image.rows-centroid.y))
		    disty=centroid.y;
		  else
		    disty=image.rows-centroid.y;
		  int distb;
		  if(distx>disty)
		  {
		   // printf("Dist frm Bound: %d\t",disty);
		    distb = disty;
		  }
		  else
		  {
		    distb = distx;
		   // printf("Dist frm Bound: %d\t",distx);
		  }
		 // printf("Posi: (%d,%d)\t",centroid.y,centroid.x);
		 // printf("width: %d\t",R.width);
		  //printf("Ratio(h/w): %f\n",(R.width*1.0/R.height));
		  
		  if(B.GtClass==4)
		  {
		    fprintf(sep,"%d\t%d\t%f\n",distb,R.width,(R.width*1.0/R.height));
		    printf("Separator\n");
		  }
		  if(B.GtClass==5)
		  {
		    fprintf(ns,"%d\t%d\t%f\n",distb,R.width,(R.width*1.0/R.height));
		    printf("Noise\n");
		  }
		  
		}
		else
		{
		 // printf("Gt Class is %d\n",B.GtClass);
		  Point2i centroid;
		  centroid.x=R.x+(R.width/2);
		  centroid.y=R.y+(R.height/2);
		  int distx,disty;
		  if(centroid.x<(image.cols-centroid.x))
		    distx=centroid.x;
		  else
		    distx=image.cols-centroid.x;
		  if(centroid.y<(image.rows-centroid.y))
		    disty=centroid.y;
		  else
		    disty=image.rows-centroid.y;
		  int distb;
		  if(distx>disty)
		  {
		   // printf("Dist frm Bound: %d\t",disty);
		    distb = disty;
		  }
		  else
		  {
		    distb = distx;
		   // printf("Dist frm Bound: %d\t",distx);
		  }
		 // printf("Posi: (%d,%d)\t",centroid.y,centroid.x);
		 // printf("width: %d\t",R.height);
		 // printf("Ratio(h/w): %f\n",(R.height*1.0/R.width));
		  
		  if(B.GtClass==4)
		    fprintf(sep,"%d\t%d\t%f\n",distb,R.height,(R.height*1.0/R.width));
		  if(B.GtClass==5)
		    fprintf(ns,"%d\t%d\t%f\n",distb,R.height,(R.height*1.0/R.width));
		  
		}
		
		
	  }
	}
	else
	{
	  for(int k=0;k<B.childs.size();k++)
	  {
	    SB B_C = B.childs[k];
	    
	    if(B_C.Fvecflag && B_C.gtflag)
	    {
	      printf("Hello in child\n");
	      if(B_C.GtClass>7)
	      {
		printf("Error .... value of gt is %d\n",B_C.GtClass);
		exit(0);
	      }
	      if(B_C.GtClass == 0)
	      {
		if(strcmp(B_C.bgcolor,"White")==0)
		{
		  if(type.empty())
		  {
		    type.push_back(B_C.type);
		    cnttype.push_back(0);
		    TrainData.push_back(GetFeatureFromFVec(B_C.FeatureVec));
		    TrainClass.push_back(B_C.GtClass);
		  }
		  else
		  {
		    int po = findtypepresence(type,B_C.type);
		    if(po==type.size())
		    {
		      type.push_back(B_C.type);
		      cnttype.push_back(0);
		      TrainData.push_back(GetFeatureFromFVec(B_C.FeatureVec));
		      TrainClass.push_back(B_C.GtClass);
		    }
		    else
		    {
		      if(cnttype[po]<7)
		      {
			TrainData.push_back(GetFeatureFromFVec(B_C.FeatureVec));
			TrainClass.push_back(B_C.GtClass);
			cnttype[po] = cnttype[po] + 1;
		      }
		    }
		  }
		}
		else
		{
		  TrainData.push_back(GetFeatureFromFVec(B_C.FeatureVec));
		  TrainClass.push_back(B_C.GtClass);
		}
	      }
	      else
	      {
		if(B_C.GtClass!=4&&B_C.GtClass!=5)
		{
		  TrainData.push_back(GetFeatureFromFVec(B_C.FeatureVec));
		  TrainClass.push_back(B_C.GtClass);
		}
	      }
	    }
	  }
	}
      }
      
      fclose(sep);
      fclose(ns);
      
      if(!blocks.empty())
	blocks.clear();
      
  }
  
  //printf("value of Train data is %f",TrainData[882][0]);
  //exit(0);
  
  printf("TrainClass size %d\n",TrainClass.size());
  Mat trainSamples = Mat(TrainData.size(),TrainData[0].size(),CV_32FC1);
  //Mat trainSamples = Mat(TrainData.size(),TrainData[0].size()/2,CV_32FC1);
  
  
  for(int i=0;i<TrainData.size();i++)
  {
    int k = 0; 
    for(int j=0;j<TrainData[i].size();)
    {
      trainSamples.at<float>(i,k) = TrainData[i][j];
      //printf("%f\t",trainSamples.at<float>(i,k));
      k++;
      j++;
      //j=j+2;
    }
   // printf("\n\n");
  }
  
  //exit(0);
  
  Mat trainClasses;
  
  
   // Mat( TrainData ).copyTo( trainSamples );
    Mat( TrainClass ).copyTo( trainClasses );
    if(!TrainData.empty())
      TrainData.clear();
    if(!TrainClass.empty())
      TrainClass.clear();
    
   // printf("Sample Ori row=%d,col=%d,channels=%d\n",trainSamples.rows,trainSamples.cols,trainSamples.channels());
   // printf("Class Ori row=%d,col=%d,channels=%d\n",trainClasses.rows,trainClasses.cols,trainClasses.channels());
    
  
    TDC Data;
    trainSamples.copyTo(Data.TrainData);
    trainClasses.copyTo(Data.TrainClass);
    
    trainSamples.release();
    trainClasses.release();
    

    for(int i=0;i<Data.TrainClass.rows;i++)
    {
    //  printf("class of %d is %d\n",i,Data.TrainClass.at<int>(i,0));
      if(Data.TrainClass.at<int>(i,0)>7)
      {
	printf("Problem in Training Class\n");
	exit(0);
      }
    }

    return Data;

  
}


#if _KNN_

void classify_KNN(vector<SB> &blocks, TDC &Data)
{
  
    
    int K = 10;
    #if defined HAVE_OPENCV_OCL && _OCL_KNN_
      cv::ocl::KNearestNeighbour knnClassifier;
      Mat temp, result;
      knnClassifier.train(Data.TrainData, Data.TrainClass, temp, false, K);
      cv::ocl::oclMat testSample_ocl, reslut_ocl;
    #else
      CvKNearest knnClassifier( Data.TrainData, Data.TrainClass, Mat(), false, K );
    #endif
      

      
       for(int i=0;i<blocks.size();i++)
    {
      SB B = blocks[i];
      if(B.childs.empty())
      {
	if(B.Fvecflag && B.PredFlag)
	{
	  vector<float> FVec = GetFeatureFromFVec(B.FeatureVec);
	  //Mat TestData = Mat(1,B.FeatureVec.size(),CV_32FC1);
	  Mat TestData = Mat(1,FVec.size(),CV_32FC1);
	    //Mat TestData = Mat(1,B.FeatureVec.size()/2,CV_32FC1);
	  //  printf("Training Data size row %d col %d",Data.TrainData.rows,Data.TrainData.cols);
	  //  printf("child TestData size row %d col %d\n and test data is\n",TestData.rows,TestData.cols);
	    int p = 0;
	    for(int j=0;j<FVec.size();j++)
	    {
	      TestData.at<float>(0,p)= FVec[j];
	    //  printf("%f\t",TestData.at<float>(0,j));
	    }
	   // printf("\n");
	  int response;
	  #if defined HAVE_OPENCV_OCL && _OCL_KNN_
	    knnClassifier.find_nearest(testSample_ocl, K, reslut_ocl);

	    reslut_ocl.download(result);
	    response = saturate_cast<int>(result.at<float>(0));
	  
	  #else
	    
	     
	     response = (int)knnClassifier.find_nearest( TestData, K );
	    
	  #endif
	  if(response>7 || response<0)
	    {
	      printf("Error in classifier\nand response is outragious %d\n",response);
	      exit(0);
	    }
	  //printf("response is %d\n",response);
	  B.PredictedClass = response;
	  B.PredFlag = false;
	  blocks[i] = B;
	}
      }
      else
      {
	vector<int> p_C(5,0);
	for(int k=0;k<B.childs.size();k++)
	{
	  
	  SB B_C = B.childs[k];
	  if(B_C.Fvecflag && B_C.PredFlag);
	  { 
	    vector<float> FVec = GetFeatureFromFVec(B_C.FeatureVec);
	  //Mat TestData = Mat(1,B_C.FeatureVec.size(),CV_32FC1);
	  Mat TestData = Mat(1,FVec.size(),CV_32FC1);
	    //Mat TestData = Mat(1,B.FeatureVec.size()/2,CV_32FC1);
	   // printf("Training Data size row %d col %d",Data.TrainData.rows,Data.TrainData.cols);
	   // printf("child TestData size row %d col %d\n and test data is\n",TestData.rows,TestData.cols);
	    int p = 0;
	    for(int j=0;j<FVec.size();j++)
	    {
	      TestData.at<float>(0,p)= FVec[j];
	     // printf("%f\t",TestData.at<float>(0,j));
	    }
	   // printf("\n");
	    int response;
	    #if defined HAVE_OPENCV_OCL && _OCL_KNN_
	    knnClassifier.find_nearest(testSample_ocl, K, reslut_ocl);

	    reslut_ocl.download(result);
	    response = saturate_cast<int>(result.at<float>(0));
	  
	  #else
	    
	     
	     response = (int)knnClassifier.find_nearest( TestData, K );
	    
	  #endif
	    if(response>7 || response<0)
	    {
	      printf("Error in classifier\nand response is outragious %d\n",response);
	      exit(0);
	    }
	    //printf("response is %d\n",response);
	    B_C.PredictedClass = response;
	    B_C.PredFlag = false;
	    B.childs[k] = B_C;
	    p_C[response]++;
	  }
	}
	int maxv = p_C[0];
	for(int k=0;k<p_C.size();k++)
	{
	  if(maxv>=p_C[k])
	  {
	    maxv=p_C[k];
	    B.PredictedClass = k;
	    B.PredFlag = false;
	  }
	}
	blocks[i] = B;
      }
      
    }
    
    
}


#endif

#if _SVM_

void classify_SVM(vector<SB> &blocks, TDC &Data)
{
  CvSVMParams svm_params;
            svm_params.svm_type = CvSVM::C_SVC;
            svm_params.kernel_type = CvSVM::POLY; //CvSVM::LINEAR;
            svm_params.degree = 0.5;
            svm_params.gamma = 1;
            svm_params.coef0 = 1;
            svm_params.C = 6;
            svm_params.nu = 0.5;
            svm_params.p = 0;
            svm_params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 1000, 0.01);
	    
         // learn classifier
#if defined HAVE_OPENCV_OCL && _OCL_SVM_
    cv::ocl::CvSVM_OCL svmClassifier(trainSamples, trainClasses, Mat(), Mat(), svm_params);
#else
    CvSVM svmClassifier( Data.TrainData, Data.TrainClass, Mat(), Mat(), svm_params );
#endif
    
    
    for(int i=0;i<blocks.size();i++)
    {
      SB B = blocks[i];
      if(B.childs.empty())
      {
	if(B.Fvecflag && B.PredFlag)
	{
	  vector<float> FVec = GetFeatureFromFVec(B.FeatureVec);
	  //Mat TestData = Mat(1,B.FeatureVec.size(),CV_32FC1);
	  Mat TestData = Mat(1,FVec.size(),CV_32FC1);
	    //Mat TestData = Mat(1,B.FeatureVec.size()/2,CV_32FC1);
	  //  printf("Training Data size row %d col %d",Data.TrainData.rows,Data.TrainData.cols);
	  //  printf("child TestData size row %d col %d\n and test data is\n",TestData.rows,TestData.cols);
	    int p = 0;
	    for(int j=0;j<FVec.size();j++)
	    {
	      TestData.at<float>(0,p)= FVec[j];
	    //  printf("%f\t",TestData.at<float>(0,j));
	    }
	   // printf("\n");
	  int response = (int)svmClassifier.predict( TestData );
	  if(response>7 || response<0)
	    {
	      printf("Error in classifier\nand response is outragious %d\n",response);
	      exit(0);
	    }
	  //printf("response is %d\n",response);
	  B.PredictedClass = response;
	  B.PredFlag = false;
	  blocks[i] = B;
	}
      }
      else
      {
	vector<int> p_C(5,0);
	for(int k=0;k<B.childs.size();k++)
	{
	  
	  SB B_C = B.childs[k];
	  if(B_C.Fvecflag && B_C.PredFlag);
	  { 
	    vector<float> FVec = GetFeatureFromFVec(B_C.FeatureVec);
	  //Mat TestData = Mat(1,B_C.FeatureVec.size(),CV_32FC1);
	  Mat TestData = Mat(1,FVec.size(),CV_32FC1);
	    //Mat TestData = Mat(1,B.FeatureVec.size()/2,CV_32FC1);
	  //  printf("Training Data size row %d col %d",Data.TrainData.rows,Data.TrainData.cols);
	  //  printf("child TestData size row %d col %d\n and test data is\n",TestData.rows,TestData.cols);
	    int p = 0;
	    for(int j=0;j<FVec.size();j++)
	    {
	      TestData.at<float>(0,p)= FVec[j];
	    //  printf("%f\t",TestData.at<float>(0,j));
	    }
	    //printf("\n");
	    
	    int response = (int)svmClassifier.predict( TestData );
	    if(response>7 || response<0)
	    {
	      printf("Error in classifier\nand response is outragious %d\n",response);
	      exit(0);
	    }
	    //printf("response is %d\n",response);
	    B_C.PredictedClass = response;
	    B_C.PredFlag = false;
	    B.childs[k] = B_C;
	    p_C[response]++;
	  }
	}
	int maxv = p_C[0];
	for(int k=0;k<p_C.size();k++)
	{
	  if(maxv>=p_C[k])
	  {
	    maxv=p_C[k];
	    B.PredictedClass = k;
	    B.PredFlag = false;
	  }
	}
	blocks[i] = B;
      }
      
    }
  
}

    
#endif

#if _RF_

void classify_RF(vector<SB> &blocks, TDC &Data)
{
  CvRTrees  rftrees;
    CvRTParams  rf_params( 8, // max_depth,
                        2, // min_sample_count,
                        0.f, // regression_accuracy,
                        false, // use_surrogates,
                        16, // max_categories,
                        0, // priors,
                        false, // calc_var_importance,
                        1, // nactive_vars,
                        5, // max_num_of_trees_in_the_forest,
                        0, // forest_accuracy,
                        CV_TERMCRIT_ITER // termcrit_type
                       );
/*
    FILE *fp;
    fp=fopen("Features.xls","w");
    for(int i=0;i<Data.TrainData.rows;i++)
    {
      for(int j=0;j<Data.TrainData.cols;j++)
	fprintf(fp,"%f\t",Data.TrainData.at<float>(i,j));
      fprintf(fp,"\n");
    }
    fclose(fp);
    */
    
    rftrees.train( Data.TrainData, CV_ROW_SAMPLE, Data.TrainClass, Mat(), Mat(), Mat(), Mat(), rf_params );
  
  for(int i=0;i<blocks.size();i++)
    {
      SB B = blocks[i];
      if(B.childs.empty())
      {
	if(B.Fvecflag && B.PredFlag)
	{
	 vector<float> FVec = GetFeatureFromFVec(B.FeatureVec);
	  //Mat TestData = Mat(1,B.FeatureVec.size(),CV_32FC1);
	  Mat TestData = Mat(1,FVec.size(),CV_32FC1);
	    //Mat TestData = Mat(1,B.FeatureVec.size()/2,CV_32FC1);
	   // printf("Training Data size row %d col %d",Data.TrainData.rows,Data.TrainData.cols);
	  //  printf("child TestData size row %d col %d\n and test data is\n",TestData.rows,TestData.cols);
	    int p = 0;
	    for(int j=0;j<FVec.size();j++)
	    {
	      TestData.at<float>(0,p)= FVec[j];
	     // printf("%f\t",TestData.at<float>(0,j));
	    }
	   // printf("\n");
	  int response = (int)rftrees.predict( TestData );
	  if(response>7 || response<0)
	    {
	      printf("Error in classifier\nand response is outragious %d\n",response);
	      exit(0);
	    }
	  printf("response is %d\n",response);
	  B.PredictedClass = response;
	  B.PredFlag = false;
	  blocks[i] = B;
	}
      }
      else
      {
	vector<int> p_C(5,0);
	for(int k=0;k<B.childs.size();k++)
	{
	  
	  SB B_C = B.childs[k];
	  if(B_C.Fvecflag && B_C.PredFlag);
	  { 
	    vector<float> FVec = GetFeatureFromFVec(B_C.FeatureVec);
	  //Mat TestData = Mat(1,B_C.FeatureVec.size(),CV_32FC1);
	  Mat TestData = Mat(1,FVec.size(),CV_32FC1);
	    //Mat TestData = Mat(1,B.FeatureVec.size()/2,CV_32FC1);
	   // printf("Training Data size row %d col %d",Data.TrainData.rows,Data.TrainData.cols);
	   // printf("child TestData size row %d col %d\n and test data is\n",TestData.rows,TestData.cols);
	    int p = 0;
	    for(int j=0;j<FVec.size();j++)
	    {
	      TestData.at<float>(0,p)= FVec[j];
	     // printf("%f\t",TestData.at<float>(0,j));
	    }
	    printf("\n");
	    
	    int response = (int)rftrees.predict( TestData );
	    if(response>7 || response<0)
	    {
	      printf("Error in classifier\nand response is outragious %d\n",response);
	      exit(0);
	    }
	    //printf("response is %d\n",response);
	    B_C.PredictedClass = response;
	    B_C.PredFlag = false;
	    B.childs[k] = B_C;
	    p_C[response]++;
	  }
	}
	int maxv = p_C[0];
	for(int k=0;k<p_C.size();k++)
	{
	  if(maxv>=p_C[k])
	  {
	    maxv=p_C[k];
	    B.PredictedClass = k;
	    B.PredFlag = false;
	  }
	}
	blocks[i] = B;
      }
      
    }
  
}

#endif


#if _DT_

void classify_DT(vector<SB> &blocks, TDC &Data)
{
  CvDTree  dtree;

    Mat var_types( 1, Data.TrainData.cols + 1, CV_8UC1, Scalar(CV_VAR_ORDERED) );
    var_types.at<uchar>( Data.TrainData.cols ) = CV_VAR_CATEGORICAL;

    CvDTreeParams dt_params;
    dt_params.max_depth = 8;
    dt_params.min_sample_count = 2;
    dt_params.use_surrogates = false;
    dt_params.cv_folds = 0; // the number of cross-validation folds
    dt_params.use_1se_rule = false;
    dt_params.truncate_pruned_tree = false;

    dtree.train( Data.TrainData, CV_ROW_SAMPLE, Data.TrainClass,
                 Mat(), Mat(), var_types, Mat(), dt_params );
    
   for(int i=0;i<blocks.size();i++)
    {
      SB B = blocks[i];
      if(B.childs.empty())
      {
	if(B.Fvecflag && B.PredFlag)
	{
	  vector<float> FVec = GetFeatureFromFVec(B.FeatureVec);
	  //Mat TestData = Mat(1,B.FeatureVec.size(),CV_32FC1);
	  Mat TestData = Mat(1,FVec.size(),CV_32FC1);
	    //Mat TestData = Mat(1,B.FeatureVec.size()/2,CV_32FC1);
	 //   printf("Training Data size row %d col %d",Data.TrainData.rows,Data.TrainData.cols);
	 //   printf("child TestData size row %d col %d\n and test data is\n",TestData.rows,TestData.cols);
	    int p = 0;
	    for(int j=0;j<FVec.size();j++)
	    {
	      TestData.at<float>(0,p)= FVec[j];
	    //  printf("%f\t",TestData.at<float>(0,j));
	    }
	  //  printf("\n");
	  int response = (int)dtree.predict( TestData )->value;
	  if(response>7 || response<0)
	    {
	      printf("Error in classifier\nand response is outragious %d\n",response);
	      exit(0);
	    }
	  //printf("response is %d\n",response);
	  B.PredictedClass = response;
	  B.PredFlag = false;
	  blocks[i] = B;
	}
      }
      else
      {
	vector<int> p_C(5,0);
	for(int k=0;k<B.childs.size();k++)
	{
	  
	  SB B_C = B.childs[k];
	  if(B_C.Fvecflag && B_C.PredFlag);
	  { 
	    vector<float> FVec = GetFeatureFromFVec(B_C.FeatureVec);
	  //Mat TestData = Mat(1,B_C.FeatureVec.size(),CV_32FC1);
	  Mat TestData = Mat(1,FVec.size(),CV_32FC1);
	    //Mat TestData = Mat(1,B.FeatureVec.size()/2,CV_32FC1);
	  //  printf("Training Data size row %d col %d",Data.TrainData.rows,Data.TrainData.cols);
	  //  printf("child TestData size row %d col %d\n and test data is\n",TestData.rows,TestData.cols);
	    int p = 0;
	    for(int j=0;j<FVec.size();j++)
	    {
	      TestData.at<float>(0,p)= FVec[j];
	    //  printf("%f\t",TestData.at<float>(0,j));
	    }
	  //  printf("\n");
	    
	    int response = (int)dtree.predict( TestData )->value;
	    if(response>7 || response<0)
	    {
	      printf("Error in classifier\nand response is outragious %d\n",response);
	      exit(0);
	    }
	    //printf("response is %d\n",response);
	    B_C.PredictedClass = response;
	    B_C.PredFlag = false;
	    B.childs[k] = B_C;
	    p_C[response]++;
	  }
	}
	int maxv = p_C[0];
	for(int k=0;k<p_C.size();k++)
	{
	  if(maxv>=p_C[k])
	  {
	    maxv=p_C[k];
	    B.PredictedClass = k;
	    B.PredFlag = false;
	  }
	}
	blocks[i] = B;
      }
      
    }
  
}

#endif

#if _NBC_

void classify_NBC(vector<SB> &blocks, TDC &Data)
{
  printf("nRows = %d nCols = %d nchannel = %d\n",Data.TrainData.rows,Data.TrainData.cols,Data.TrainData.channels());
  printf("nRows = %d nCols = %d nchannel = %d\n",Data.TrainClass.rows,Data.TrainClass.cols,Data.TrainClass.channels());
  
   CvNormalBayesClassifier normalBayesClassifier( Data.TrainData, Data.TrainClass );
  
  for(int i=0;i<blocks.size();i++)
    {
      SB B = blocks[i];
      if(B.childs.empty())
      {
	if(B.Fvecflag && B.PredFlag)
	{
	  vector<float> FVec = GetFeatureFromFVec(B.FeatureVec);
	  //Mat TestData = Mat(1,B.FeatureVec.size(),CV_32FC1);
	  Mat TestData = Mat(1,FVec.size(),CV_32FC1);
	    //Mat TestData = Mat(1,B.FeatureVec.size()/2,CV_32FC1);
	   // printf("Training Data size row %d col %d",Data.TrainData.rows,Data.TrainData.cols);
	   // printf("child TestData size row %d col %d\n and test data is\n",TestData.rows,TestData.cols);
	    int p = 0;
	    for(int j=0;j<FVec.size();j++)
	    {
	      TestData.at<float>(0,p)= FVec[j];
	    //  printf("%f\t",TestData.at<float>(0,j));
	    }
	  //  printf("\n");
	  int response = (int)normalBayesClassifier.predict( TestData );
	  if(response>7 || response<0)
	    {
	      printf("Error in classifier\nand response is outragious %d\n",response);
	      exit(0);
	    }
	  //printf("response is %d\n",response);
	  B.PredictedClass = response;
	  B.PredFlag = false;
	  blocks[i] = B;
	}
      }
      else
      {
	vector<int> p_C(5,0);
	for(int k=0;k<B.childs.size();k++)
	{
	  
	  SB B_C = B.childs[k];
	  if(B_C.Fvecflag && B_C.PredFlag);
	  { 
	    vector<float> FVec = GetFeatureFromFVec(B_C.FeatureVec);
	    //Mat TestData = Mat(1,B_C.FeatureVec.size(),CV_32FC1);
	    Mat TestData = Mat(1,FVec.size(),CV_32FC1);
	    //Mat TestData = Mat(1,B.FeatureVec.size()/2,CV_32FC1);
	   // printf("Training Data size row %d col %d",Data.TrainData.rows,Data.TrainData.cols);
	   // printf("child TestData size row %d col %d\n and test data is\n",TestData.rows,TestData.cols);
	    int p = 0;
	    for(int j=0;j<FVec.size();j++)
	    {
	      TestData.at<float>(0,p)= FVec[j];
	    //  printf("%f\t",TestData.at<float>(0,j));
	    }
	   // printf("\n");
	    
	    int response = (int)normalBayesClassifier.predict( TestData );
	    if(response>7 || response<0)
	    {
	      printf("Error in classifier\nand response is outragious %d\n",response);
	      exit(0);
	    }
	    //printf("response is %d\n",response);
	    B_C.PredictedClass = response;
	    B_C.PredFlag = false;
	    B.childs[k] = B_C;
	    p_C[response]++;
	  }
	}
	int maxv = p_C[0];
	for(int k=0;k<p_C.size();k++)
	{
	  if(maxv>=p_C[k])
	  {
	    maxv=p_C[k];
	    B.PredictedClass = k;
	    B.PredFlag = false;
	  }
	}
	blocks[i] = B;
      }
      
    }
}


#endif

#if _CCCN_

void classify_CCCN(char *trainfile, char *testfile, char *KFoldNum)
{
  vector<int> classnumber;
    
  vector<ConfusionMatrix> CM_ALL;
  
  //vector<ConfusionMatrix> CM_ALL(classnumber.size(),ConfusionMatrix(classnumber.size()));    
      
      
  char *classifiername = "CCCN_Classifier";
   char *dirname;
  
if(classifyAll)
{
    dirname = (char *)malloc(2001*sizeof(char));
    strcpy(dirname,"ClassifyAll"); 
}
      
if(classifySelected)
{
    dirname = (char *)malloc(2001*sizeof(char));
    strcpy(dirname,"ClassifySelected");
}
      
if(classifySelected_ignore)
{
    dirname = (char *)malloc(2001*sizeof(char));
    strcpy(dirname,"ClassifySelectedIgnore");
}
 
if(classifyTG)
{
    dirname = (char *)malloc(2001*sizeof(char));
    strcpy(dirname,"ClassifyTG");
}

if(classifyTG_ignore)
{
    dirname = (char *)malloc(2001*sizeof(char));
    strcpy(dirname,"ClassifyTGIgnore");
}
  
    makedir(dirname);
    
    char *classidir = CreateNameIntoFolder(dirname,classifiername);
    makedir(classidir);
    
  char *Classi_KFoldFolder = CreateNameIntoFolder(classidir,KFoldNum);
  makedir(Classi_KFoldFolder);
  
  CCCN_Classifier CCCN_classify;
  CCCN_classify.Train(trainfile);
  
  FILE *f;
  f = fopen(testfile,"r");
  while(!feof(f))
  {
    char filename[2000];
      fscanf(f,"%s",&filename);
      printf("%s\n",filename);
      page pg = GetPageGroundtruth(filename);
  
      char *name;
      name = input_image_name_cut(pg.GetImageName());
      //makedir(name);
      
      char *opt;
      opt = (char *) malloc ( 2001 * sizeof(char));
      if(opt == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      strcpy(opt,name);

      char *tnm;
      tnm = (char *) malloc ( 2001 * sizeof(char));
      if(tnm == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      tnm = ".jpg";
      strcat(opt,tnm);
      
      char *tnm1;
      tnm1 = (char *) malloc ( 2001 * sizeof(char));
      if(tnm1 == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      strcpy(tnm1,opt);
      
      printf("imagename %s\n",tnm1);
      
      Mat image = imread(tnm1,1);
      
      //vector<SB> blocks = GetSegmentationUnit(image);
      
      vector<SB> blocks = GetProcessingBlocks(image);
      
if(classifyAll)
{
      classnumber.push_back(0); classnumber.push_back(1); 
      classnumber.push_back(2); classnumber.push_back(3); 
      classnumber.push_back(4); classnumber.push_back(5);
      classnumber.push_back(6); classnumber.push_back(7);
      if(!CM_ALL.empty())
	CM_ALL.clear();
      CM_ALL.resize(classnumber.size(),ConfusionMatrix(classnumber.size()));
      
      blocks = PrepareAlethiaGt(pg,blocks);
      
}
      
if(classifySelected)
{
      classnumber.push_back(0); classnumber.push_back(1);  
      classnumber.push_back(4); classnumber.push_back(5);
      
      if(!CM_ALL.empty())
	CM_ALL.clear();
      CM_ALL.resize(classnumber.size(),ConfusionMatrix(classnumber.size()));
      
      blocks = PrepareAlethiaGt_tgns(pg,blocks);
}
      
if(classifySelected_ignore)
{
      classnumber.push_back(0); classnumber.push_back(1);  
      classnumber.push_back(4); classnumber.push_back(5);
      
      if(!CM_ALL.empty())
	CM_ALL.clear();
      CM_ALL.resize(classnumber.size(),ConfusionMatrix(classnumber.size()));
      
      blocks = PrepareAlethiaGt_tgns_ignore(pg,blocks);
}
 
if(classifyTG)
{
      classnumber.push_back(0); classnumber.push_back(1);  
      classnumber.push_back(4); classnumber.push_back(5);
      
      if(!CM_ALL.empty())
	CM_ALL.clear();
      CM_ALL.resize(classnumber.size(),ConfusionMatrix(classnumber.size()));
      
      blocks = PrepareAlethiaGt_tg(pg,blocks);
}

if(classifyTG_ignore)
{
      classnumber.push_back(0); classnumber.push_back(1);  
      classnumber.push_back(4); classnumber.push_back(5);
      
      if(!CM_ALL.empty())
	CM_ALL.clear();
      CM_ALL.resize(classnumber.size(),ConfusionMatrix(classnumber.size()));
      
      blocks = PrepareAlethiaGt_tg_ignore(pg,blocks);
}
 
      
      
      for(int i=0;i<blocks.size();i++)
      {
	SB B = blocks[i];
	if(B.PredFlag && B.gtflag)
	{
	  if(CheckNoise(B,image))
	  {
	    if(classifyTG)
	    {
	      B.PredictedClass = 1;
	      B.PredFlag = false;
	    }
	    else if(classifyTG_ignore)
	      B.PredictedClass = 1;
	    else
	    {
	      B.PredictedClass = 5;
	      B.PredFlag = false;
	    }
	    
	  }
	  else if(CheckSeparator(B,image))
	  {
	    if(classifyTG)
	    {
	      B.PredictedClass = 1;
	      B.PredFlag = false;
	    }
	    else if(classifyTG_ignore)
	      B.PredictedClass = 1;
	    else
	    {
	      B.PredictedClass = 4;
	      B.PredFlag = false;
	    }
	  }
	  else
	  {
	    B.PredictedClass = CCCN_classify.Predict(B.FeatureVec,0.5);
	    B.PredFlag = false;
	  }
	}
	blocks[i] = B;
      }
      
      
      
      //WriteGTXMLFile();
      
      vector<SB> SU;
      
      int su_cnt = 0;
      
       for(int i=0;i<blocks.size();i++)
      {
	SB B = blocks[i];
	
	if(B.childs.empty())
	{
	  if(B.Fvecflag && B.gtflag)
	  {
	    B.blockid = su_cnt;
	    SU.push_back(B);
	    su_cnt++;
	  }
	}
	else
	{
	  for(int k=0;k<B.childs.size();k++)
	  {
	    
	    SB B_C = B.childs[k];
	    if(B_C.Fvecflag && B_C.gtflag);
	    { 
	      B_C.blockid = su_cnt;
	      SU.push_back(B_C);
	      su_cnt++;
	    }
	  }
	}
      }
      
      if(!blocks.empty())
	blocks.clear();
      
      char *initial_name = input_image_name_cut(pg.GetImageName()); 
      
      char *output,*tempname, *gtname,*outname;
      output = (char *) malloc ( 2001 * sizeof(char));
      if(output == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      strcpy(output,initial_name);

      tempname = (char *) malloc ( 2001 * sizeof(char));
      if(tempname == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      tempname = "_gt.xml";
      strcat(output,tempname);
      gtname = CreateNameIntoFolder(Classi_KFoldFolder,output);
      
      WriteGTXMLFile(pg,gtname,SU);
      
      
      
      output = (char *) malloc ( 2001 * sizeof(char));
      if(output == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      strcpy(output,initial_name);

      tempname = (char *) malloc ( 2001 * sizeof(char));
      if(tempname == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      tempname = "_out.xml";
      strcat(output,tempname);
      outname = CreateNameIntoFolder(Classi_KFoldFolder,output);
      
      WriteOutputXMLFile(pg,outname,SU);
      
      
      vector<vector<SB> > Clusters;
      vector<double> alpha(classnumber.size(),0.5);
      
      if(CCCN_flag)
      {
	printf("CCCN Clustering\n");
	Clusters = ClusteringCCN(SU,alpha);
	printf("Number of Clusters obetained by  CCCN is %d\n",Clusters.size());
      }
      if(CCE_flag)
      {
	printf("CCE Clustering\n");
	Clusters = ClusteringCE(SU,alpha);
	printf("Number of Clusters obetained by  CCE is %d\n",Clusters.size());
      }
      
      ClusteringClassification(SU,Clusters,alpha);
      
      output = (char *) malloc ( 2001 * sizeof(char));
      if(output == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      strcpy(output,initial_name);

      tempname = (char *) malloc ( 2001 * sizeof(char));
      if(tempname == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      tempname = "_gt.xml";
      strcat(output,tempname);
      gtname = CreateNameIntoFolder(Classi_KFoldFolder,output);
      
      WriteGTXMLFile(pg,gtname,SU);
      
      
      
      output = (char *) malloc ( 2001 * sizeof(char));
      if(output == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      strcpy(output,initial_name);

      tempname = (char *) malloc ( 2001 * sizeof(char));
      if(tempname == NULL)
      {
	printf("Memory can not be allocated\n");
	exit(0);
      }
      tempname = "_out.xml";
      strcat(output,tempname);
      outname = CreateNameIntoFolder(Classi_KFoldFolder,output);
      
      WriteOutputXMLFile(pg,outname,SU);
      
      
      vector<int> gtcls;
      vector<int> pcls;
      
      for(int i=0;i<SU.size();i++)
      {
	SB B = SU[i];
	printf("Block id =%d\t gt= %d\t predict = %d\n",B.blockid,B.GtClass,B.PredictedClass);
	gtcls.push_back(B.GtClass);
	pcls.push_back(B.PredictedClass);
      }
      if(!SU.empty())
	SU.clear();
      
      Mat Gtlabels = Mat(gtcls.size(),1,CV_8UC1);
      Mat PredictedLabels = Mat(gtcls.size(),1,CV_8UC1);
      
      for(int i=0;i<gtcls.size();i++)
      {
	Gtlabels.at<int>(i,0) = gtcls[i];
	if(gtcls[i]>7)
	{
	  printf("B.GtClass = %d\n",gtcls[i]);
	  exit(0);
	}
	if(pcls[i]>7)
	{
	   printf(" Error B.predictedclass = %d\n",pcls[i]);
	  exit(0);
	}
	PredictedLabels.at<int>(i,0) = pcls[i];
      }
      if(!gtcls.empty())
	gtcls.clear();
      if(!pcls.empty())
	pcls.clear();
      
      vector<ConfusionMatrix> CM = GetConfusionMatrix(Gtlabels,PredictedLabels,classnumber);
      //char *tempname;
	 opt = (char *)malloc(2000*sizeof(char));
	 tempname = (char *)malloc(2000*sizeof(char));
	 
	 tempname = "image_Classification_Result.xls";
	 
	opt = CreateNameIntoFolder(classidir,tempname);
	 
	 
	 
	 FILE *res;
	 
	 res = fopen(opt,"a+");
	 
	 
	 fprintf(res,"%s\t",name);
	 for(int j=0;j<CM.size();j++)
	 {
	   ConfusionMatrix cm_j = CM[j];
	   fprintf(res,"%s\t%d\t%d\t%d\t%d\t%d\t",classifiername,classnumber[j],cm_j.tp,cm_j.fp,cm_j.tn,cm_j.fn);
	   
	   CM_ALL[j].tp = CM_ALL[j].tp + cm_j.tp;
	   CM_ALL[j].fp = CM_ALL[j].fp + cm_j.fp;
	   CM_ALL[j].tn = CM_ALL[j].tn + cm_j.tn;
	   CM_ALL[j].fn = CM_ALL[j].fn + cm_j.fn;
	   
	 }
	 fprintf(res,"\n");
	 fclose(res);
	  
	 if(!CM.empty())
	  CM.clear();
      
  }
  
  
  
 printf("ALL Input Image classification Completed\n");
 
  char *tempname,*name;
  name = (char *)malloc(2000*sizeof(char));
  tempname = (char *)malloc(2000*sizeof(char));	 
  tempname = "Overall_Classification_Result.xls";
  name = CreateNameIntoFolder(classidir,tempname);
  
  FILE *res;
  res = fopen(name,"a+");

    MultiClassPerformanceMetrics M(CM_ALL);
  
    fprintf(res,"%s\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",classifiername,M.GetAverageAccuracy(),M.GetErrorRate(),M.GetPrecesionMu(),M.GetRecallMu(),M.GetFScoreMu(1),M.GetPrecesionM(),M.GetRecallM(),M.GetFScoreM(1));
 
    //M.~MultiClassPerformanceMetrics();

  
  fclose(res);
}

#endif


void Classification(vector<SB> &blocks, TDC &Data, char *ClassifierName)
{
  printf("classifiername =%s\n",ClassifierName);
  if(strcmp(ClassifierName,"NBC")==0)
  {
#if _NBC_
    classify_NBC(blocks,Data);
#endif
  }
  else if(strcmp(ClassifierName,"KNN")==0)
  {
#if _KNN_
    classify_KNN(blocks,Data);
#endif
  }
  else if(strcmp(ClassifierName,"DT")==0)
  {
#if _DT_
    classify_DT(blocks,Data);
#endif
  }
  else if(strcmp(ClassifierName,"RF")==0)
  {
#if _RF_
    classify_RF(blocks,Data);
#endif
  }
  else if(strcmp(ClassifierName,"SVM")==0)
  {
#if _SVM_
    classify_SVM(blocks,Data);
#endif
  }
  else
  {
    printf("given Classsifier '%s' is not supported\nSupported classifiers are 'NBC' 'KNN' 'DT' 'RF' and 'SVM'\n",ClassifierName);
    exit(0);
  }
}