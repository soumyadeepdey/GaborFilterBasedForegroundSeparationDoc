#ifndef ConcensusBasedClustering_H
#define ConcensusBasedClustering_H

#include "StandardHeaders.h"
#include "SegmentationUnit.h"

#include "alglib/alglibmisc.h"
#include "alglib/linalg.h"
#include "alglib/specialfunctions.h"
#include "alglib/statistics.h"


using namespace alglib;




/**
 * @function FindAdjacencyMatrix
 * @param : input : vector<vector<type> > - A adjacency matrix with relation value between two node is given, in double
 * 		    double connection_condition - It is the value by checking which two nodes are connected
 * 
 * example : connection_condition = x;
 * 	     if(RelationMat.at<double>(i,j) > x)
 * 		
 * return : A AdjacencyMatrix of bool type (vector<vector<bool> >)
 * */

template <typename T>
vector<vector<bool> > FindAdjacencyMatrix(vector<vector<T> > &Relation, T connection_condition);

template <typename T>
vector<vector<bool> > FindAdjacencyMatrixNdata(vector<vector<vector<T> > > Relation, vector<T> Threshold);

void GiveLabelDFS(vector<vector<bool> > AdjMat, vector<int> &tempcomponent, vector<int> &labels, vector<bool> &ccflag);


/**
 * @func : 		DFSCC 
 * @desc :		Find ConnectedComponent based on depth First search of the adjacency matrix
 * @param:	input:	vector<vector<bool> > AdjMat :	AdjacencyMatrix in bool format
 * 
 * 			vector<vector<int> > &component: address to the component
 * 
 * */

void DFSCC(vector<vector<bool> > AdjMat, vector<vector<int> > &component, vector<int> &labels);


template <typename T>
vector<T> FindIntersection(vector<T> &v1, vector<T> &v2);


double CalculateZValue(double SampleMean, double PopulationMean, double PopulationSD, int SampleSize);

double CalculateZValue(double SampleMean, double PopulationMean, double SEM);

double oneSampleTtest(double sample_mean, double population_mean, double sample_sd, double sample_size);

double twoSampleTtest_UnequalVariance(int sample1_size, int sample2_size, double sample1_mean, double sample2_mean, double sample1_sd, double sample2_sd);

double twoSampleTtest_EqualVariance(int sample1_size, int sample2_size, double sample1_mean, double sample2_mean, double sample1_sd, double sample2_sd);

int DegreeofFreedom_UnequalVariance(int sample1_size, int sample2_size, double sample1_sd, double sample2_sd);

int DegreeofFreedom_EqualVariance(int sample1_size, int sample2_size);

void FindCommonNodes(vector<vector<int> > &K, vector<vector<int> > D, vector<vector<vector<int> > > N, int z, int t);

vector<vector<SB> > ClusteringCCN(vector< SB > SU, vector< double > Threshold);
    
vector<vector<SB> >  ClusteringCE(vector<SB> SU, vector<double> Threshold);


#endif
