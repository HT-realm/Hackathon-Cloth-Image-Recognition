#include "opencv2/highgui/highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <iostream>
#include <random>

#include "Segment.h"

using namespace cv;
using namespace std;


int main()
{
	Segment newCloth;
	newCloth.sampleImage = imread("sample image.jpg", CV_LOAD_IMAGE_COLOR);

	newCloth.initialize();
	Mat segmentedCloth = newCloth.findDominantObject(newCloth.sampleImage);
    
	imshow("shirt",segmentedCloth);
	waitKey(0);
	
	
	
	//resize(mask,mask,morphShirt.size());
	
	//imshow("clustered image gray", mask);


	//imwrite("mask.jpg",mask);
	//imshow("clustered image gray", morphShirt);
	//waitKey(0);
	
	//Mat hsvImage(src.rows, src.cols, CV_8UC1);
	//cvtColor(src, hsvImage, CV_BGR2HSV); //Convert to gray
										 //threshold(thr, thr, 25, 255, THRESH_BINARY);


	/*
	Mat hsvImage(src.rows, src.cols, CV_8UC1);
	
	

	
	//Mat dst(src.rows, src.cols, CV_8UC1, Scalar::all(0));
	cvtColor(src, hsvImage, CV_BGR2HSV); //Convert to gray
	//threshold(thr, thr, 25, 255, THRESH_BINARY);

	vector<Mat> channels;
	split(hsvImage, channels);

	Mat H = channels[0];
	Mat S = channels[1];
	Mat V = channels[2];

	Mat shiftedH = H.clone();
	int shift = 12; // in openCV hue values go from 0 to 180 (so have to be doubled to get to 0 .. 360) because of byte range from 0 to 255
	for (int j = 0; j < shiftedH.rows; ++j)
	{	
		for (int i = 0; i < shiftedH.cols; ++i)
		{
			shiftedH.at<unsigned char>(j, i) = (shiftedH.at<unsigned char>(j, i) + shift) % 180;
		}
	}

	imshow("H", V); 

	(V,V,Size(3,3),Point(-1,-1), BORDER_DEFAULT);
	blur(S, S, Size(3, 3), Point(-1, -1), BORDER_DEFAULT);
	blur(shiftedH, shiftedH, Size(3, 3), Point(-1, -1), BORDER_DEFAULT);

	Mat cannyH,cannyS,cannyV;
	Canny(shiftedH, cannyH, 100, 50);
	Canny(S, cannyS, 200, 150);
	Canny(V, cannyV, 200, 150);


	namedWindow("H edge", CV_WINDOW_AUTOSIZE);
	imshow("H edge", cannyH);
	
	("S edge", CV_WINDOW_AUTOSIZE);
	imshow("S edge", cannyS);
	
	("V edge", CV_WINDOW_AUTOSIZE);
	imshow("V edge", cannyV);
	
	waitKey(0);
	*/


}