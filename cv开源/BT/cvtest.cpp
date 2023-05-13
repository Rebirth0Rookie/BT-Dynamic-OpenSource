#include"header.h"

int main()
{
	//关闭日志输出
	cv::utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);

	Mat img; Mat answer;
	img = imread("/home/yang/tempfile/middle/1/1Q.jpg");
	answer=imread("/home/yang/tempfile/high/1/1A.jpg");
	imshow("answer", answer);
	if (img.empty())
	{
		cout << "图像地址错误，请确认输入路径是否正确" << endl;
		return -1;
	}
	
	PolygonRecognition A;
	A.Junior_recognition(img);
	A.ShowJunior();
	//A.High_recognition(img);
	//A.ShowHigh();

}
 
