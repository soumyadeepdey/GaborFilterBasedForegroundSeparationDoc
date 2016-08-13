#ifndef ClusteringClassification_H
#define ClusteringClassification_H

#include "StandardHeaders.h"
#include "classification.h"
#include "ConcensusBasedClustering.h"
#include "SegmentationUnit.h"

extern bool CCCN_flag;
extern bool CCE_flag;

void LabelSegmentationUnitofEachCluster(vector<SB> K, vector<SB> &blocks, vector<double> alpha);

void ClusteringClassification(vector<SB> &blocks, vector<vector<SB> > &clusters, vector<double> alpha);

#endif