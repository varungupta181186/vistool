#ifndef DATASET_H
#define DATASET_H

#include "category.h"
#include <vector>
#include <string>
#include "boost/filesystem.hpp"
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <iterator>
#include "dataPoint.h"
#include "fileManagement.h"

using namespace std;

class Dataset{
 private:
  vector<Category> categories;
  string root;
 public:
  void setRoot(string str);
  void addCategory(Category cat);

  void enableCategory(size_t i);
  void enableCategory(string str);

  vector<Category> * getCategories();
  string getRoot();
  vector<Category> getEnabled();

  void print();
  void printEnabled();

  void randomSplit(vector<DataPoint> * train, vector<DataPoint> * test, float cut);

  Dataset(string rt);
};



#endif
