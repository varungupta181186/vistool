#include "gluiEvaluate.h"

void evaluateClassifier(){
  extractFeatures();
  train();
  classify();
}

void crossValidate(){
  ToolState &state = *ToolState::getInstance();
  extractFeatures();
  state.train_data.clear();
  state.test_data.clear();
  delete state.current_classifier;
  if(state.enabled_classifier == states::NearestNeighbor)
    state.current_classifier = new NNClassifier();
  if(state.enabled_classifier == states::SupportVectorMachine)
    state.current_classifier = new SVMClassifier();
  vector<DataPoint> enabsp = state.current_db->enabledPoints();
  state.train_data = enabsp;
  state.test_data = enabsp;
  state.test_result = state.current_classifier->crossvalidation(&enabsp);
  for(vector<int>::iterator cl = state.current_classes.begin(); 
      cl != state.current_classes.end(); 
      cl++)
    state.selected_class_listbox->delete_item(*cl);
  state.current_classes.clear();
  vector<Category*> enabs = state.current_db->getEnabled();
  for(size_t i = 0; i <  enabs.size(); ++i){
    state.selected_class_listbox->add_item(enabs[i]->getLabel(), 
				     enabs[i]->getName().c_str());
    state.current_classes.push_back(enabs[i]->getLabel());
  }
  delete state.current_evaluation;

  printf("making eval out of %d points and %d evaluations", enabsp.size(), state.test_result.size());
  for(size_t i = 0 ; i < state.train_data.size(); i ++)
    printf("at %d, should %d, is %d\n ", i, state.train_data[i].getLabel(),state.test_result[i]);
  state.current_evaluation = new Evaluation(&state.train_data, &state.test_result);
  viewDataset();
  showStatistics();
}

void showStatistics(){
  ToolState &state = *ToolState::getInstance();
  GLUI *&stats = state.stats;
  Evaluation *&current_evaluation = state.current_evaluation;
  Dataset *& current_db = state.current_db;
  if(stats != NULL)
    stats->close();
  stats = GLUI_Master.create_glui("statistics", 0, 1125, 0);
  {
    stringstream strstr;
    strstr  << "Instances: " << current_evaluation->getInstances() << endl;
    stats->add_statictext(strstr.str().c_str());
  } {
    stringstream strstr;
    strstr  << "Correct: " << current_evaluation->getCorrect() << endl;
    stats->add_statictext(strstr.str().c_str());
  } {
     stringstream strstr;
     strstr  << "Performance: " << current_evaluation->getPrecision() << endl;
     stats->add_statictext(strstr.str().c_str());
  }
   // show confusion info
  map<int,int> cormap = current_evaluation->getCorrectMap();
  map<int, int> totmap = current_evaluation->getTotalMap();
  for(map<int, int>::iterator it = cormap.begin();
      it!= cormap.end(); ++it){
    stringstream strstr;
     strstr  << (*it).first << "." <<  current_db->getCatName((*it).first) 
	     << " " << (*it).second <<  "/" << totmap[(*it).first] << endl;
     stats->add_statictext(strstr.str().c_str());
  }
  
 }
