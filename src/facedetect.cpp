#include "facedetect.h"

using namespace std;
using namespace cv;

namespace vito{
namespace features{

String cascadeName =
"data/haarcascades/haarcascade_frontalface_alt.xml";
String nestedCascadeName =
"data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";

int detect( Mat& img,
	     CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
	     double scale)
{
    int i = 0;
    double t = 0;
    vector<Rect> faces;
    const static Scalar colors[] =  { CV_RGB(0,0,255),
        CV_RGB(0,128,255),
        CV_RGB(0,255,255),
        CV_RGB(0,255,0),
        CV_RGB(255,128,0),
        CV_RGB(255,255,0),
        CV_RGB(255,0,0),
        CV_RGB(255,0,255)} ;
    Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );

    cvtColor( img, gray, CV_BGR2GRAY );
    resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
    equalizeHist( smallImg, smallImg );

    t = (double)cvGetTickCount();
    cascade.detectMultiScale( smallImg, faces,
        1.1, 2, 0
        //|CV_HAAR_FIND_BIGGEST_OBJECT
        //|CV_HAAR_DO_ROUGH_SEARCH
        |CV_HAAR_SCALE_IMAGE
        ,
        Size(30, 30) );
    t = (double)cvGetTickCount() - t;
    printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );
    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
    {
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = colors[i%8];
        int radius;
        center.x = cvRound((r->x + r->width*0.5)*scale);
        center.y = cvRound((r->y + r->height*0.5)*scale);
        radius = cvRound((r->width + r->height)*0.25*scale);
        circle( img, center, radius, color, 3, 8, 0 );
        if( nestedCascade.empty() )
            continue;
        smallImgROI = smallImg(*r);
        nestedCascade.detectMultiScale( smallImgROI, nestedObjects,
            1.1, 2, 0
            //|CV_HAAR_FIND_BIGGEST_OBJECT
            //|CV_HAAR_DO_ROUGH_SEARCH
            //|CV_HAAR_DO_CANNY_PRUNING
            |CV_HAAR_SCALE_IMAGE
            ,
            Size(30, 30) );
        for( vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++ )
        {
            center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
            center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
            radius = cvRound((nr->width + nr->height)*0.25*scale);
            circle( img, center, radius, color, 3, 8, 0 );
        }
    }
    return faces.size();
}

Descriptor FaceFeature::extract_(MyImage *Image, 
				 bool makeVisualRepresentation, 
				 Magick::Image * representation){
  Descriptor res;
  string cascadeName =
    "data/haarcascades/haarcascade_frontalface_alt.xml";
  string nestedCascadeName =
    "data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
  cv::Mat im = cv::imread(Image->getLocation(), 1);
  cv::CascadeClassifier cascade, nestedCascade;
  double scale = 1;
  if( !nestedCascade.load( nestedCascadeName ) )
    cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;
  if( !cascade.load( cascadeName ) ) {
      cerr << "ERROR: Could not load classifier cascade" << endl;
      cerr << "Usage: facedetect [--cascade=\"<cascade_path>\"]\n"
            "   [--nested-cascade[=\"nested_cascade_path\"]]\n"
            "   [--scale[=<image scale>\n"
	"   [filename|camera_index]\n" ;
      return 0;
  }
  const int nr = detect(im, cascade, nestedCascade, scale);
  res.push_back(nr);
  return res;
}



}}
