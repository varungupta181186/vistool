#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <string>

#include "clusterFeatureExtractor.h"
#include "featureExtractor.h"
#include "classification.h"
#include "evaluation.h"
#include "statistics.h"
#include "kmeanshistogram.h"
#include "classification.h"
#include "trueClustering.h"

namespace vito{
namespace experiment{

void classify(std::string original);
void estimate(std::string original, float cutoff = 0.0);
std::pair<float,float> recallExperiment(const std::string str,
					const std::string dataset_string);
void recallExperiment(const std::string dataset_string = "");
Dataset abdullah2010DataSet();
Dataset getDataSet(const std::string str);
float   performExperiment(const std::string str,
			  const std::string dataset = "",
			  const int repetitions = 1,
			  const size_t kDataPoints = 500);
Dataset abdullah2010();
float   svm(Dataset &dataset, size_t datapoint = 500);
float   nn(Dataset &dataset, size_t datapoints = 500);
float   clustering( Dataset &dataset);
void cluster(std::string dataset, std::string filename);

}}

#endif
