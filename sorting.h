#ifndef SORTING_H
#define SORTING_H


#include "StandardHeaders.h"



namespace IITkgp_functions {

  
  
  template <typename T>
  void swaping(std::vector<T> & data, int i, int j);
  
  template <typename T>
  void Merge(std::vector<T> & data, int lowl, int highl, int lowr, int highr);
  
  template <typename T>
  void MergeSort(std::vector<T> & data, int low, int high);
  
  template <typename T>
  int Partition(std::vector<T> & data, int low, int high);
  
  template <typename T>
  void QuickSort(std::vector<T> & data, int low, int high);
  
  template <typename T>
  void BubbleSort(std::vector<T> & data);
  
  template <typename T>
  void SelectionSort(std::vector<T> & data);
  
  template <typename T>
  void InsertionSort(std::vector<T> & data);
  
  
 

}

void initiatesort();



#endif

