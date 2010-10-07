#ifndef FEATURES_H
#define FEATURES_H

#include "facedetect.h"
#include <vector>
#include "feature.h"
#include "mpeg7.h"
#include "sift.h"
#include "histogram.h"
#include "clusterFeatureExtractor.h"


namespace vito{
namespace features{

std::vector<Feature*> getExistingFeatures();
std::vector<Feature*> getActiveFeatures();

}}

#endif
