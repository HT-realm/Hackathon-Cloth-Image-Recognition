#pragma once

#include "opencv2/highgui/highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <iostream>
#include <random>
using namespace cv;
using namespace std;

class Segment
{
public:
	const int CLUSTER_COUNT = 8;
	Segment();
	Mat src;
	
	
	void initialize();
	Mat colorCluster(Mat, int);
	Mat findDominantColorMask(Mat);
	Mat findDominantObject(Mat);
	
	void locateLocalMaxima1D(Mat, int&, double&);

	~Segment();
    Mat sampleImage;
};

