#include "Segment.h"



Segment::Segment()
{
}

void Segment::initialize()
{
	
	//sampleImage = imread("sample image.jpg", CV_LOAD_IMAGE_COLOR);
	resize(sampleImage, sampleImage, Size(sampleImage.cols / 4, sampleImage.rows / 4));

	

	blur(sampleImage, src, Size(7, 7));
	imshow("blurred", src);


}

Mat Segment::colorCluster(Mat img, int clusterCount)
{
	Mat labels;
	int attempts = 5;
	Mat centers;
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
	waitKey(0);
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

	//cout << hist << endl;

	
	//int minLoc;

	//minMaxLoc(hist, NULL, maxVal, NULL,NULL);
	//minMaxLoc(hist, NULL, &maxVal, NULL, NULL);
	locateLocalMaxima1D(hist, maxLoc, maxVal);
	
	//minMaxLoc(hist, NULL, NULL, NULL, &maxLoc);
	cout << "max value is: "<<maxVal<<" located at"<< maxLoc << endl;
	
	//int loc = maxLoc;

	threshold(morphShirt, morphShirt, maxLoc - 1, 255, THRESH_TOZERO);
	threshold(morphShirt, morphShirt, maxLoc + 1, 255, THRESH_TOZERO_INV);
	
	imshow("mask old", morphShirt);

	threshold(morphShirt, morphShirt, maxLoc - 1, 1, THRESH_BINARY);
	//divide(morphShirt,maxLoc,morphShirt,-1);

	Mat mask;
	morphShirt.convertTo(mask, CV_8UC1);

	

	Mat maskLabels, labels8U, tempLabels;
	int numberOfLabels = connectedComponents(mask, maskLabels, 8, CV_16U) - 1;
	dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(-1, -1)));

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


	imshow("mask", mask);
	waitKey(0);
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

	imshow("extracted shirt", shirtOnly);
	waitKey(0);

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

