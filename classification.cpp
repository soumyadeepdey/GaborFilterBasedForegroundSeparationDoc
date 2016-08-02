#include "classification.h"

using namespace IITkgp_functions;


void classify(char *TestFILE, char *classifiername)
{
  vector<int> classnumber;
      classnumber.push_back(0); classnumber.push_back(1); 
      classnumber.push_back(2); classnumber.push_back(3); 
      classnumber.push_back(4);
      
      
      vector<ConfusionMatrix> CM_ALL(classnumber.size(),ConfusionMatrix(classnumber.size()));
  
  
      
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
      
      vector<SB> blocks = GetSegmentationUnit(image);
      
      blocks = PrepareAlethiaGt(pg,blocks);
      
      Classification(blocks,classifiername);
      
      Mat Gtlabels = Mat(blocks.size(),1,CV_8UC1);
      Mat PredictedLabels = Mat(blocks.size(),1,CV_8UC1);
      
      for(int i=0;i<blocks.size();i++)
      {
	SB B = blocks[i];
	Gtlabels.at<uchar>(i,0) = B.GtClass;
	PredictedLabels.at<uchar>(i,0) = B.PredictedClass;
      }
      
      vector<ConfusionMatrix> CM = GetConfusionMatrix(Gtlabels,PredictedLabels,classnumber);
      char *tempname;
	 opt = (char *)malloc(2000*sizeof(char));
	 tempname = (char *)malloc(2000*sizeof(char));
	 
	 tempname = "image_Classification_Result.xls";
	 
	opt = CreateNameIntoFolder(classifiername,tempname);
	 
	 
	 
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

	 CM.clear();
  }
  
  
 printf("ALL Input Image classification Completed\n");
 
  char *tempname,*name;
  name = (char *)malloc(2000*sizeof(char));
  tempname = (char *)malloc(2000*sizeof(char));	 
  tempname = "Overall_Classification_Result.xls";
  name = CreateNameIntoFolder(classifiername,tempname);
  
  FILE *res;
  res = fopen(name,"a+");

    MultiClassPerformanceMetrics M(CM_ALL);
  
    fprintf(res,"%s\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",classifiername,M.GetAverageAccuracy(),M.GetErrorRate(),M.GetPrecesionMu(),M.GetRecallMu(),M.GetFScoreMu(1),M.GetPrecesionM(),M.GetRecallM(),M.GetFScoreM(1));
 
    M.~MultiClassPerformanceMetrics();

  
  fclose(res);

  
}

void Training(char* TrainFile)
{
  vector<vector<float> > TrainData;
  vector<int> TrainClass;
  vector<int> NumberPerCluster;
  
  int classnumber = 0;
  
  FILE *f;
  f = fopen(TrainFile,"r");
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
      
      vector<SB> blocks = GetSegmentationUnit(image);
      
      blocks = PrepareAlethiaGt(pg,blocks);
      
      for(int i=0;i<blocks.size();i++)
      {
	SB B = blocks[i];
	if(B.Fvecflag && B.gtflag)
	{
	  TrainData.push_back(B.FeatureVec);
	  TrainClass.push_back(B.GtClass);
	}
      }
      
  }
  
  Mat trainSamples, trainClasses;
  
    Mat( TrainData ).copyTo( trainSamples );
    Mat( TrainClass ).copyTo( trainClasses );
    
    TrainData.clear();
    TrainClass.clear();
    
    printf("Sample Ori row=%d,col=%d,channels=%d\n",trainSamples.rows,trainSamples.cols,trainSamples.channels());
    // reshape trainData and change its type
    trainSamples = trainSamples.reshape( 1, trainSamples.rows );
    
    trainSamples.convertTo( trainSamples, CV_32FC1 );
    
    printf("Sample later row=%d,col=%d,channels=%d\n",trainSamples.rows,trainSamples.cols,trainSamples.channels());
  
    TDC Data;
    trainSamples.copyTo(Data.TrainData);
    trainClasses.copyTo(Data.TrainClass);
    
    trainSamples.release();
    trainClasses.release();
    

#if _KNN_
    
    int K = 10;
    #if defined HAVE_OPENCV_OCL && _OCL_KNN_
      cv::ocl::KNearestNeighbour knnClassifier;
      Mat temp, result;
      knnClassifier.train(Data.TrainData, Data.TrainClass, temp, false, K);
      cv::ocl::oclMat testSample_ocl, reslut_ocl;
    #else
      CvKNearest knnClassifier( Data.TrainData, Data.TrainClass, Mat(), false, K );
    #endif
      
      knnClassifier.save("KNN_classifier.yml");
    
#endif
    
#if _NBC_
    
    CvNormalBayesClassifier normalBayesClassifier( Data.TrainData, Data.TrainClass );
    normalBayesClassifier.save("normalBayesClassifier.yml");
    
#endif
    

#if _SVM_
    
    
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
    
    svmClassifier.save("SVM_Classifier.yml");
    
#endif
    
#if _DT_
    
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
    
    dtree.save("DT_classifier.yml");
    
#endif
    
#if _RF_
    
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

    rftrees.train( Data.TrainData, CV_ROW_SAMPLE, Data.TrainClass, Mat(), Mat(), Mat(), Mat(), rf_params );
    
    rftrees.save("RF_classifier.yml");
    
#endif
    
  
}


#if _KNN_

void classify_KNN(vector<SB> &blocks)
{
  #if defined HAVE_OPENCV_OCL && _OCL_KNN_
      cv::ocl::KNearestNeighbour knnClassifier;
      Mat temp, result;
      knnClassifier.load("KNN_classifier.yml");
      cv::ocl::oclMat testSample_ocl, reslut_ocl;
    #else
      CvKNearest knnClassifier;
      knnClassifier.load("KNN_classifier.yml");
    #endif
      
      for(int i=0;i<blocks.size();i++)
    {
      SB B = blocks[i];
      B.FeatureVec;
      Mat TestData;
      //Mat(elem).copyTo(TestData);
      Mat(B.FeatureVec).copyTo(TestData);
      //printf("Rows = %d Cols = %d channel = %d\n",TestData.rows,TestData.cols,TestData.channels());
      transpose(TestData,TestData);
      int response;
      
      #if defined HAVE_OPENCV_OCL && _OCL_KNN_
	  
	    testSample_ocl.upload(TestData);
	    int K = knnClassifier.get_max_k();
	    knnClassifier.find_nearest(testSample_ocl, K, reslut_ocl);

	    reslut_ocl.download(result);
	    response = saturate_cast<int>(result.at<float>(0));
	  
	  #else
	    
	     int K = knnClassifier.get_max_k();
	     response = (int)knnClassifier.find_nearest( TestData, K );
	    
	  #endif
      
      B.PredictedClass = response;
      blocks[i] = B;
    }
}


#endif

#if _SVM_

void classify_SVM(vector<SB> &blocks)
{
  


#if defined HAVE_OPENCV_OCL && _OCL_SVM_
    cv::ocl::CvSVM_OCL svmClassifier;
#else
    CvSVM svmClassifier;
#endif 
    
    svmClassifier.load("SVM_Classifier.yml");
    
    for(int i=0;i<blocks.size();i++)
    {
      SB B = blocks[i];
      B.FeatureVec;
      Mat TestData;
      //Mat(elem).copyTo(TestData);
      Mat(B.FeatureVec).copyTo(TestData);
      //printf("Rows = %d Cols = %d channel = %d\n",TestData.rows,TestData.cols,TestData.channels());
      transpose(TestData,TestData);
      int response = (int)svmClassifier.predict( TestData );
      B.PredictedClass = response;
      blocks[i] = B;
    }
  
}

    
#endif

#if _RF_

void classify_RF(vector<SB> &blocks)
{
  CvRTrees  rtrees;
  rtrees.load("RF_classifier.yml");
  
  for(int i=0;i<blocks.size();i++)
    {
      SB B = blocks[i];
      B.FeatureVec;
      Mat TestData;
      //Mat(elem).copyTo(TestData);
      Mat(B.FeatureVec).copyTo(TestData);
      //printf("Rows = %d Cols = %d channel = %d\n",TestData.rows,TestData.cols,TestData.channels());
      transpose(TestData,TestData);
      int response = (int)rtrees.predict( TestData );
      B.PredictedClass = response;
      blocks[i] = B;
    }
  
}

#endif


#if _DT_

void classify_DT(vector<SB> &blocks)
{
  CvDTree  dtree;
  dtree.load("DT_classifier.yml");
  
  for(int i=0;i<blocks.size();i++)
    {
      SB B = blocks[i];
      B.FeatureVec;
      Mat TestData;
      //Mat(elem).copyTo(TestData);
      Mat(B.FeatureVec).copyTo(TestData);
      //printf("Rows = %d Cols = %d channel = %d\n",TestData.rows,TestData.cols,TestData.channels());
      transpose(TestData,TestData);
      int response = (int)dtree.predict( TestData );
      B.PredictedClass = response;
      blocks[i] = B;
    }
  
}

#endif

#if _NBC_

void classify_NBC(vector<SB> &blocks)
{
  CvNormalBayesClassifier normalBayesClassifier;
  normalBayesClassifier.load("normalBayesClassifier.yml");
  
  for(int i=0;i<blocks.size();i++)
    {
      SB B = blocks[i];
      B.FeatureVec;
      Mat TestData;
      //Mat(elem).copyTo(TestData);
      Mat(B.FeatureVec).copyTo(TestData);
      //printf("Rows = %d Cols = %d channel = %d\n",TestData.rows,TestData.cols,TestData.channels());
      transpose(TestData,TestData);
      int response = (int)normalBayesClassifier.predict( TestData );
      B.PredictedClass = response;
      blocks[i] = B;
    }
}


#endif


void Classification(vector<SB> &blocks, char *ClassifierName)
{
  if(ClassifierName == "NBC")
  {
    classify_NBC(blocks);
  }
  else if(ClassifierName == "KNN")
  {
    classify_KNN(blocks);
  }
  else if(ClassifierName == "DT")
  {
    classify_DT(blocks);
  }
  else if(ClassifierName == "RF")
  {
    classify_RF(blocks);
  }
  else if(ClassifierName == "SVM")
  {
    classify_SVM(blocks);
  }
  else
  {
    printf("given Classsifier '%s' is not supported\nSupported classifiers are 'NBC' 'KNN' 'DT' 'RF' and 'SVM'\n");
    exit(0);
  }
}