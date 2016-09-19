#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <random>

#include "Segment.h"

using namespace cv;
using namespace std;


int main()
{
	Segment newCloth;

	string fileName = "database sample";
	string filetype = ".jpg";
	newCloth.sampleImage = imread(fileName+filetype, CV_LOAD_IMAGE_COLOR);

	newCloth.initialize();
	Mat segmentedCloth = newCloth.findDominantObject(newCloth.sampleImage);
    
	imwrite(fileName+" cropped"+filetype, segmentedCloth);

	imshow("shirt",segmentedCloth);
	waitKey(0);
	
	

}