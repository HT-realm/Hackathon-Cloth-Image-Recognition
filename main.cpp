
#include "opencv2/highgui/highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <iostream>
#include <random>


using namespace cv;
using namespace std;


int main()
{
	int largest_area = 0;
	int largest_contour_index = 0;
	Rect bounding_rect;

	Mat src = imread("sample image.jpg", CV_LOAD_IMAGE_COLOR);
	resize(src, src, Size(src.cols / 4, src.rows / 4));
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

	blur(V,V,Size(3,3),Point(-1,-1), BORDER_DEFAULT);

	Mat cannyH,cannyC,cannyV;
	Canny(V, cannyH, 150, 100);
	//Canny(V, cannyH, 200, 150);
	//Canny(V, cannyH, 200, 150);

	/*/
	std::vector<std::vector<cv::Point> > contoursH;
	std::vector<cv::Vec4i> hierarchyH;
	cv::findContours(cannyH, contoursH, hierarchyH, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	// draw the contours to a copy of the input image:
	cv::Mat outputH = src.clone();
	for (int i = 0; i< contoursH.size(); i++)
	{
		cv::drawContours(outputH, contoursH, i, cv::Scalar(0, 0, 255), 2, 8, hierarchyH, 0);
	}
	*/


	namedWindow("edges", CV_WINDOW_AUTOSIZE);
	imshow("edges", cannyH);
	waitKey(0);
	
}