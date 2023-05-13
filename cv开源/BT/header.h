#pragma once
#include<iostream>
#include<vector>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<vector>
#include<string>
#include <opencv2/core/utils/logger.hpp>
using namespace cv;
using namespace std;

class PolygonRecognition
{
public:
	
	void High_recognition(Mat& img);
	void Junior_recognition(Mat& img);


	void picture(Mat& result, Point2f& center, Mat& img);
	void drawapp(Mat result, Mat& img);
	void Findmax(vector<vector<Point>>& contours);
	void ShowJunior()
	{
		namedWindow("识别结果", 0);
		resizeWindow("识别结果", Size(740, 580));
		imshow("识别结果", this->result_junior);
		waitKey(0);
		destroyAllWindows();
	}
	void ShowHigh()
	{
		namedWindow("识别结果", 0);
		resizeWindow("识别结果", Size(740, 580));
		imshow("识别结果", this->result_high);
		waitKey(0);
		destroyAllWindows();
	}
private:
	Mat result_junior;
	Mat result_high;
};

void PolygonRecognition::Junior_recognition(Mat& img)
{
	Mat src;
	bilateralFilter(img, src, 30, 35, 100);
	
	Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));

	cvtColor(src, src, COLOR_BGR2HSV);


	//通过分割图像通道获取二值化灰度图
	Mat test[3], test01, test02, test03;
	split(src, test);
	test01 = test[0];
	test02 = test[1];
	test03 = test[2];

	//边缘检测
	Mat canny, canny1, canny2, canny3;
	Canny(test01, canny1, 50, 270, 3, false);
	Canny(test02, canny2, 50, 270, 3, false);
	Canny(test03, canny3, 50, 270, 3, false);

	//进行形态学操作
	dilate(canny1, canny1, kernel);
	erode(canny1, canny1, kernel);
	dilate(canny2, canny2, kernel);
	erode(canny2, canny2, kernel);
	dilate(canny3, canny3, kernel);
	erode(canny3, canny3, kernel);
	canny = canny1 | canny2 | canny3;

	//轮廓绘制
	vector<vector<Point>>contours, contours1;
	vector<Vec4i>hierarchy;
	findContours(canny, contours, hierarchy, 0, 2, Point());
	for (int i = 0; i < contours.size(); i++)
	{
		double aera = contourArea(contours[i]);
		if (aera > 500)
		{
			vector<Point>A = contours[i];
			contours1.push_back(A);
		}
	}


	for (int i = 0; i < contours1.size(); i++)
	{
		//用最小矩形求取轮廓中心
		RotatedRect rrect = minAreaRect(contours1[i]);
		Point2f center = rrect.center;
		circle(img, center, 2, Scalar(0, 255, 0), -1, 8, 0);

		Mat result;
		approxPolyDP(contours1[i], result, 6, true);
		drawapp(result, img);
		for (int t = 0; t < contours1.size(); t++)
		{
			drawContours(img, contours1, t, Scalar(0, 255, 0), 4, 8);
		}

		cout << "corners:" << result.rows << endl;
		picture(result, center, img);
	}
	this->result_junior = img;
}

void PolygonRecognition::High_recognition(Mat& img)
{
	Mat src;
	bilateralFilter(img, src, 30, 35, 100);
	medianBlur(src, src, 7);
	GaussianBlur(src, src, Size(5, 5), 20, 30);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
	Mat kernel1 = (Mat_<float>(3, 3) << 0, -1, 0, 0, 5, 0, 0, -1, 0);
	filter2D(src, src, CV_8UC3, kernel1);


	cvtColor(src, src, COLOR_BGR2HSV);


	//通过分割图像通道获取二值化灰度图
	Mat test[3], test01, test02, test03;
	split(src, test);
	test01 = test[0];
	test02 = test[1];
	test03 = test[2];

	//边缘检测
	Mat canny, canny1, canny2, canny3;
	Canny(test01, canny1, 50, 270, 3, false);
	Canny(test02, canny2, 50, 270, 3, false);
	Canny(test03, canny3, 50, 270, 3, false);

	//进行形态学操作
	dilate(canny1, canny1, kernel);
	erode(canny1, canny1, kernel);
	dilate(canny2, canny2, kernel);
	erode(canny2, canny2, kernel);
	dilate(canny3, canny3, kernel);
	erode(canny3, canny3, kernel);
	canny = canny1 | canny2 | canny3;

	//轮廓绘制
	vector<vector<Point>>contours, contours1;
	vector<Vec4i>hierarchy;
	findContours(canny, contours, hierarchy, 0, 2, Point());
	for (int i = 0; i < contours.size(); i++)
	{
		double aera = contourArea(contours[i]);
		if (aera > 500)
		{
			vector<Point>A = contours[i];
			contours1.push_back(A);
		}
	}


	for (int i = 0; i < contours1.size(); i++)
	{
		//用最小矩形求取轮廓中心
		RotatedRect rrect = minAreaRect(contours1[i]);
		Point2f center = rrect.center;
		circle(img, center, 2, Scalar(0, 255, 0), -1, 8, 0);

		Mat result;
		approxPolyDP(contours1[i], result, 6, true);
		drawapp(result, img);
		for (int t = 0; t < contours1.size(); t++)
		{
			drawContours(img, contours1, t, Scalar(0, 255, 0), 4, 8);
		}

		cout << "corners:" << result.rows << endl;
		picture(result, center, img);
	}
	this->result_high = img;

}

void PolygonRecognition::picture(Mat& result, Point2f& center, Mat& img)//用于标识多边形形状
{
	if (result.rows == 3)
	{
		putText(img, "Triangle", center, 0, 1, Scalar(0, 0, 255), 4, 8);
	}
	if (result.rows == 4)
	{
		putText(img, "Rectangle", center, 0, 1, Scalar(0, 0, 255), 4, 8);
	}
	if (result.rows == 5)
	{
		putText(img, "P-5", center, 0, 1, Scalar(0, 0, 255), 4, 8);
	}
	if (result.rows == 6)
	{
		putText(img, "P-6", center, 0, 1, Scalar(0, 0, 255), 4, 8);
	}
	if (result.rows == 7)
	{
		putText(img, "P-7", center, 0, 1, Scalar(0, 0, 255), 4, 8);
	}
	if (result.rows == 8)
	{
		putText(img, "P-8", center, 0, 1, Scalar(0, 0, 255), 4, 8);
	}
	if (result.rows == 9)
	{
		putText(img, "P-9", center, 0, 1, Scalar(0, 0, 255), 4, 8);
	}
	if (result.rows == 10)
	{
		putText(img, "P-10", center, 0, 1, Scalar(0, 0, 255), 4, 8);
	}
	if (result.rows == 11)
	{
		putText(img, "P-11", center, 0, 1, Scalar(0, 0, 255), 4, 8);
	}
	if (result.rows > 12)
	{
		putText(img, "Circle", center, 0, 1, Scalar(0, 0, 255), 4, 8);
	}
}


void PolygonRecognition::drawapp(Mat result, Mat& img)//绘制轮廓
{
	for (int i = 0; i < result.rows; i++)
	{
		//最后一个坐标点和第一个坐标点相连
		if (i == result.rows - 1)
		{
			Vec2i point1 = result.at<Vec2i>(i);
			Vec2i point2 = result.at<Vec2i>(0);
			line(img, point1, point2, Scalar(0, 0, 255), 2, 8, 0);
		}
		else
		{
			Vec2i point1 = result.at<Vec2i>(i);
			Vec2i point2 = result.at<Vec2i>(i + 1);
			line(img, point1, point2, Scalar(0, 0, 255), 2, 8, 0);
		}
		
	}
}

void PolygonRecognition::Findmax(vector<vector<Point>>& contours)//寻找最大长度轮廓
{
	int  k;
	double length_max = 0;
	if (contours.size() > 0)
	{
		k = 0;
		for (int i = 0; i < contours.size(); i++)
		{
			if (arcLength(contours[i], 0) > length_max)
			{
				length_max = arcLength(contours[i], 0);
				k = i;
			}
		}
	}
}

