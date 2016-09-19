#include "Segment.h"



Segment::Segment()
{
}

void Segment::initialize()
{
	
	//sampleImage = imread("sample image.jpg", CV_LOAD_IMAGE_COLOR);
	//resize(sampleImage, sampleImage, Size(sampleImage.cols / 4, sampleImage.rows / 4));
	resize(sampleImage, sampleImage, Size(FRAME_HEIGHT,FRAME_WIDTH));
	

	blur(sampleImage, src, Size(7, 7));
	//imshow("blurred", src);


}

Mat Segment::histEqualization(Mat image)
{
	vector<Mat> channels;
	Mat imgHistEqualized;

	cvtColor(image, imgHistEqualized, CV_BGR2YCrCb); //change the color image from BGR to YCrCb format

	split(imgHistEqualized, channels); //split the image into channels

	equalizeHist(channels[0], channels[0]); //equalize histogram on the 1st channel (Y)

	merge(channels, imgHistEqualized); //merge 3 channels including the modified 1st channel into one image

	cvtColor(imgHistEqualized, imgHistEqualized, CV_YCrCb2BGR);
	
	return imgHistEqualized;
}

Mat Segment::colorCluster(Mat img, int clusterCount)
{
	
	Mat labels;
	int attempts = 5;
	Mat centers;
	//Mat img = histEqualization(image);
	Mat new_image(img.size(), img.type());//output
    Mat new_centers(centers.size(), centers.type());
	
	Mat samples(img.rows * img.cols, 3, CV_32F);
	
	for (int y = 0; y < img.rows; y++)
		for (int x = 0; x < img.cols; x++)
			for (int z = 0; z < 3; z++)
				samples.at<float>(y + x*img.rows, z) = img.at<Vec3b>(y, x)[z];

	kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 1000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);

	cout << centers << endl;
	//cout << labels << endl;

	for (int y = 0; y < img.rows; y++)
		for (int x = 0; x < img.cols; x++)
		{
			int cluster_idx = labels.at<int>(y + x*img.rows, 0);
			new_image.at<Vec3b>(y, x)[0] = centers.at<float>(cluster_idx, 0);
			new_image.at<Vec3b>(y, x)[1] = centers.at<float>(cluster_idx, 1);
			new_image.at<Vec3b>(y, x)[2] = centers.at<float>(cluster_idx, 2);
		}

	imshow("clustered image", new_image);
	//waitKey(0);
	return new_image;


}



Mat Segment::findDominantColorMask(Mat image)
{

	Mat hist;
	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange = range;
	double maxVal;
	int maxLoc;
    Mat morphShirt;
	
	cvtColor(image, morphShirt, CV_BGR2GRAY);

	calcHist(&morphShirt, 1, 0, Mat(), hist, 1, &histSize, &histRange, true, false);

	
	locateLocalMaxima1D(hist, maxLoc, maxVal);
	
	
	cout << "max value is: "<<maxVal<<" located at"<< maxLoc << endl;
	
	threshold(morphShirt, morphShirt, maxLoc - 1, 255, THRESH_TOZERO);
	threshold(morphShirt, morphShirt, maxLoc + 1, 255, THRESH_TOZERO_INV);
	
	//imshow("mask old", morphShirt);

	threshold(morphShirt, morphShirt, maxLoc - 1, 1, THRESH_BINARY);
	
	Mat mask;
	morphShirt.convertTo(mask, CV_8UC1);
	
	

	Mat maskLabels, labels8U, tempLabels;
	
	dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5), Point(-1, -1)));
	erode(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(-1, -1)));
	
	
	//testing
	Mat temp;
	multiply(mask, 255, temp);
	//imshow("old mask", temp);
	//end of test

	//connected components of the mask
	int numberOfLabels = connectedComponents(mask, maskLabels, 8, CV_16U) - 1;
	maskLabels.convertTo(labels8U, CV_8U);

	Scalar area, largestArea = 0;
	int largestAreaComponent;
	for (int i = 1; i <= numberOfLabels; i++)
	{

		threshold(labels8U, tempLabels, (double)i, 255, THRESH_TOZERO_INV);

		threshold(tempLabels, tempLabels, (double)i - 0.5, 255, THRESH_BINARY);


		area = sum(tempLabels) / 255;

		if (area[0]>largestArea[0])
		{
			largestArea = area;
			largestAreaComponent = i;
		}
	}

	threshold(labels8U, tempLabels, (double)largestAreaComponent, 255, THRESH_TOZERO_INV);

	threshold(tempLabels, labels8U, (double)largestAreaComponent - 0.5, 1, THRESH_BINARY);
	
	labels8U.convertTo(mask, CV_8UC1);
	dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5), Point(-1, -1)));
	// end of connected components algorithm

	//fill gaps
	Mat tempMask;

	mask.copyTo(tempMask);

	
	copyMakeBorder(tempMask,tempMask,1,1,1,1,BORDER_CONSTANT,0);
	imwrite("tempMask.jpg",tempMask);
	//imshow("temp mask", tempBound);
	//waitKey(0);
	int i = floodFill(tempMask, Point(0, 0), Scalar(1), 0, Scalar(0), Scalar(0), 4);
	multiply(tempMask, 255, tempMask);
	bitwise_not(tempMask, tempMask);
	
	Rect myRoi(1,1,mask.cols,mask.rows);
	Mat newTempMask = tempMask(myRoi);
	divide(newTempMask, 255, newTempMask);
	
	imwrite("tempMask.jpg", tempMask);

	add(newTempMask,mask,mask);

	//end of fill gaps
	//imshow("temp mask", tempMask);

	//imshow("mask", mask);
	//waitKey(0);
	imwrite("mask.jpg", mask);
	return mask;
}

Mat Segment::findDominantObject(Mat image)
{
	Mat clusteredImage = colorCluster(image, CLUSTER_COUNT);
	
	Mat mask = findDominantColorMask(clusteredImage);


	vector<Mat> channels;
	split(sampleImage, channels);


	//Mat newH, newS, newV;
	multiply(channels[0], mask, channels[0]);
	multiply(channels[1], mask, channels[1]);
	multiply(channels[2], mask, channels[2]);

	Mat shirtOnly;
	merge(channels, shirtOnly);

	//imshow("extracted shirt", shirtOnly);
	//waitKey(0);

	
	return shirtOnly;
}



void Segment::locateLocalMaxima1D(Mat dataArray, int & maxLoc, double & maxVal)
{
	
	Mat data;
	dataArray.convertTo(data,CV_32FC1);
	int length = data.rows;
	maxLoc = 0;
	//maxVal = 0;
	int value;
	for (int i = 0; i < length; i++) {
		value = data.at<float>(i,0);

		if (value > maxVal)
		{
			maxVal = value;
			maxLoc = i;
		}
	
	}

}


Segment::~Segment()
{
}

