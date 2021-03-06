#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#define OMP_NUM_THREADS 1

#include "experiment.h"
#include <queue>
#include "boost/thread/thread.hpp"
#include "features.h"
#include <sstream>
#include "descriptorWriter.h"
#include "dataset.h"



namespace vito{
namespace features{

class FeatureExtractor:public Singleton<FeatureExtractor>{
  friend class Singleton<FeatureExtractor>;

 private:
  std::vector<Feature*> features;

  //create directory if it doesn't exist
  void                  assertDir(std::string str);

  typedef Descriptor (FeatureExtractor::*map_function)(DataPoint const &dp);
  Descriptor extractDescriptor(DataPoint const &);
  Descriptor extractDescriptorWithLabel(DataPoint const &);

  // this should be a feature of DataPointCollection, but my C++ template
  // skills are not that great.
  DescriptorCollection map(DataPointCollection const &dps, map_function);
  
 public:
  
  bool                  allExtracted(const DataPointCollection &dps);

  std::vector<float>    calcDescriptor(MyImage &image, const DataPoint &dp);
  void                  renewDescriptors(const DataPointCollection &dps);
  // calculate the descriptors to a datapoint, if force is true the
  // descriptor is always calculated
  Descriptor            getDescriptor(const DataPoint &dps,
				      const bool force = false,
				      const bool normalize = true,
				      const bool stacktraining = false);

  Descriptor            getDescriptorWhileTrainingStack(const DataPoint &dp);

  // save all active categories within dataset to their respective
  // files.
  void                  saveDescriptorsToFile(Dataset * ds,
					      const bool force = false);

  // refresh the a datapoints descriptor if it is required or do it
  // anyway if forced
  void                  renewDescriptor(const DataPoint &dp,
					const bool force,
					const bool train = false);

  std::string           getCurrentDescriptorLocation(const DataPoint &dp);

  // fill the openCV type matrices given a vector of datapoints
  void                  getCVMatrices(std::vector<DataPoint*> dps,
				      CvMat * training, 
				      CvMat *  labels);

  DescriptorCollection     getExamples(const DataPointCollection &datapoints);
  DescriptorCollection  getDescriptors(const DataPointCollection &datatpoints);
};

}}

#endif
