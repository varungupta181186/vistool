#include "experiment.h"

using std::string;
using std::cout;
using std::endl;
using std::vector;

namespace vito{

using features::KMeansClusterHistogram;
typedef std::vector<Category*> categories;
using features::FeatureExtractor;
using features::Feature;
using classification::SVMClassifier;
using evaluation::Evaluation;

namespace experiment{

float performExperiment(const string str, 
			const string dataset_string,
			const int repetitions){
  cout << "performing experiment " << str << endl;

  Dataset dataset = getDataSet(dataset_string);
  float (*exp_func)(Dataset&) = 0;

  if(str == "svm") exp_func = &svm;
  if(str == "nn") exp_func = &nn;
  if(str == "clustering"){
    exp_func = &svm;
    vector<Dataset> splits = dataset.split();
    features::ClusterFeatureExtractor *cfe = features::ClusterFeatureExtractor::getInstance();
    string name = cfe->addClusterFeature(splits[0].enabledPoints(), 
					 features::mpeg7::EdgeHistogram::getInstance());
    dataset = splits[1];
    Parameters::getInstance()->appointFeature(name);
  }

 if(exp_func == 0){
    cout << "experiment not found" << endl;
  }else{
    Statistics values;
    for(int i = 0; i < repetitions; i++)
      values.push_back(exp_func(dataset));
    cout << "mean: " << values.mean() << endl;
    cout << "std: " << values.std() << endl;
    return values.mean();
  }
  return 0.0;
}

Dataset getDataSet(const string str){
  if(str == "abdullah2010")
    return abdullah2010();
  return Dataset();
}

Dataset abdullah2010(){
  Dataset dataset ("../datasets/Caltech101/");
  dataset.enableCategory("airplanes");
  dataset.enableCategory("camera");
  dataset.enableCategory("car_side");
  dataset.enableCategory("cellphone");
  dataset.enableCategory("cup");
  dataset.enableCategory("helicopter");
  dataset.enableCategory("Motorbikes");
  dataset.enableCategory("scissors");
  dataset.enableCategory("umbrella");
  dataset.enableCategory("watch");
  return dataset;
}

float svm(Dataset &dataset){
  DataPointCollection train, test;
  dataset.randomDataSplit(&train, &test, 0.5, true, 30);

  //get Labels, Examples and Descriptors
  FeatureExtractor *fe = FeatureExtractor::getInstance();
  ExampleCollection training_examples = fe->getExamples(train);
  DescriptorCollection testing_descriptors = fe->getDescriptors(test);

  // train an svm
  classification::SVMClassifier svm;
  svm.train(training_examples);

  // classify the test
  LabelCollection result_labels = svm.classify(testing_descriptors);

  // evaluate the results
  Evaluation evaluation(test, result_labels);
  return evaluation.getPrecision();
}

float nn(Dataset &dataset){
  DataPointCollection train, test;
  dataset.randomDataSplit(&train, &test, 0.5, true, 30);

  //get Labels, Examples and Descriptors
  FeatureExtractor *fe = FeatureExtractor::getInstance();
  ExampleCollection training_examples = fe->getExamples(train);
  DescriptorCollection testing_descriptors = fe->getDescriptors(test);

  // train an svm
  classification::NNClassifier svm;
  svm.train(training_examples);

  // classify the test
  LabelCollection result_labels = svm.classify(testing_descriptors);

  // evaluate the results
  Evaluation evaluation(test, result_labels);
  evaluation.print();
  return evaluation.getPrecision();
}

void cluster(string dataset, string filename){
  Feature * feature = 0;
  {
    vector<Feature*> actives =  features::getActiveFeatures();
    if(actives.size() > 1){
      cout << "too many features active, 1 active feature is best" << endl;
      return;
    }if(actives.size() == 0){
      cout << "no active feature to cluster with!" << endl;
      return;
    }
    feature = actives[0];
  }
  Dataset dats = getDataSet(dataset);
  features::KMeansClusterHistogram histogram(dats.enabledPoints(), feature);
  histogram.save(filename + ".clustercenters");
  Parameters::getInstance()->saveXML(filename + ".xml");
}

}}