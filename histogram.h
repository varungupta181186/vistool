#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "feature.h"

class Histogram:public Feature{
 private:
  static bool instanceFlag;
  static Histogram * singleton;
 public:
  static Histogram * getInstance();
  ~Histogram();
  Histogram();
  vector<float> extract(MyImage * Image);
};

#endif