#include "descriptor.h"

using std::vector;
using std::map;
using std::cout;
using std::endl;

namespace vito{

void Descriptor::normalize(){
  const float min = *std::min_element(begin(), end());
  const float max = *std::max_element(begin(), end());
  float multiplier = 2 / (max - min);
  if((min - max) == 0)
    multiplier = 0.0;
  for(Descriptor::iterator it = begin(); it != end(); ++it)
    *it = -1 + (*it - min) * multiplier;
}

void Descriptor::print() const{
  for(size_t i =0; i < size(); ++i)
    printf("%.5f ", at(i));
}


void ExampleCollection::print(){
  map<int,int> label_histogram;
  // make histogram of all labels
  for(ExampleCollection::iterator it = begin(); 
      it != end();
      ++it)
    label_histogram[it->get_label()]++;
  // print histogram values
  for(map<int,int>::iterator it = label_histogram.begin(); 
      it != label_histogram.end();
      ++it)
    cout << it->first << " has: " << it->second << endl;
}

int Example::get_label() const{
  return label;
}
void Example::set_label(const int l){
  label = l;
}



}
