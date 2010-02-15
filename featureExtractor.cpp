#include "featureExtractor.h"

using namespace Magick;
using namespace std;
using namespace cv;
using namespace boost::filesystem;

void FeatureExtractor::saveDescriptorsToFile(Dataset * ds){
  cout << "hoi" << endl;
  vector<Category> enabled= ds->getEnabled();
  for(vector<Category>::iterator category = enabled.begin();
      category != enabled.end();
      ++category){
    string name = category->getName();
    vector<DataPoint> files = category->getDataPoints();
    string root = category->getRoot();
    string aap = DESCRIPTOR_LOCATION;
    path p = complete(path(aap+name, native));
    path parameters = complete(path(Parameters::getInstance()->getFile()));
    if(!is_directory(p))
      create_directory(p);
    for(vector<DataPoint>::iterator file = files.begin(); file != files.end(); ++file ){
      renewDescriptor(&*file);
      /*
      string imURL = root+"/"+file->getImageURL();
      string descpath = aap+name+"/"+file->getImageURL()+".desc";
      if(!exists(path(descpath)) || 
	 last_write_time(parameters) < last_write_time(path(imURL))){
	MyImage image(root+"/"+file->getImageURL());
	vector<float> features = extractHistogram(&image);
	writeDescriptor(&features,descpath);
	file->setDescriptorURL(descpath);
      } 
      */
    }
  }
}

void FeatureExtractor::renewDescriptor(DataPoint * dp){
  path parameters = complete(path(Parameters::getInstance()->getFile()));
  if(!exists(path(dp->getDescriptorURL())) || 
     last_write_time(parameters) < last_write_time(path(dp->getImageURL()))){
    MyImage image(dp->getImageURL());
    vector<float> features = extractHistogram(&image);
    writeDescriptor(&features,dp->getDescriptorURL());
  } 
}

vector<float> FeatureExtractor::extractHistogram(MyImage * image){
  Mat * hsv = image->getOpenCVMat();
  Parameters * p = Parameters::getInstance();
  if(!p->hasHistogram()){
    cout << "histogram parameters not found" << endl;
    throw NO_PARAMETERS;
  }
  int hbins = p->getiParameter("hbins"), sbins = p->getiParameter("sbins");
  int histSize[] = {hbins, sbins};

  float hranges[] = { 0, 180 };
  float sranges[] = { 0, 256 };
  float* ranges[] = { hranges, sranges };
  MatND hist;

  int channels[] = {0, 1};
  calcHist( hsv, 1, channels, Mat(), 
	    hist, 2, histSize, (const float **) ranges,
	    true, 
	    false );
  double maxVal=0;
  minMaxLoc(hist, 0, &maxVal, 0, 0);

  int scale = 10;
  Mat histImg = Mat::zeros(sbins*scale, hbins*10, CV_8UC1);
  vector<float> data;
  for( int h = 0; h < hbins; h++ )
    for( int s = 0; s < sbins; s++ )
      {
	float binVal = hist.at<float>(h, s);
	data.push_back(binVal);
	int intensity = cvRound(binVal*255/maxVal);
	rectangle( histImg, Point(h*scale, s*scale),
		     Point( (h+1)*scale - 1, (s+1)*scale - 1),
		     Scalar::all(intensity),
		     CV_FILLED );
      }
  return data;
}
