#ifdef _DEBUG
//Debugモードの場合
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_world300d.lib")            // opencv_core
#else
//Releaseモードの場合
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_world300.lib") 
#endif

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <list>

using namespace std;
using namespace cv;




class Data{
public :
	Mat image;
	Mat init;
	Mat temp;

	vector<Rect> pts;
	int itr;

	int fl;

	Data(){
		itr = 0;
		fl = 1;
	}
};

void Mouse(int ev, int x, int y, int flag, void *param){
	Data* data = static_cast<Data*>(param);


	switch (ev){
	case cv::EVENT_MOUSEMOVE:
		if ((x < 0) || (data->image.size().width <= x) || (y < 0) || (data->image.size().height <= y)){ break; }

		data->image = (data->temp).clone();
		if (flag == EVENT_FLAG_LBUTTON){
			rectangle(data->image, Rect(min(data->pts[data->itr].x, x), min(data->pts[data->itr].y, y), abs(data->pts[data->itr].x - x), abs(data->pts[data->itr].y - y)), Scalar(0, 0, 255));
		}
		else{
			line(data->image, Point(x, 0), Point(x, data->image.size().height), Scalar(0, 255, 0));
			line(data->image, Point(0, y), Point(data->image.size().width, y), Scalar(0, 255, 0));
		}


		break;
	case cv::EVENT_LBUTTONDOWN:
		if ((x < 0) || (data->image.size().width <= x) || (y < 0) || (data->image.size().height <= y)){ break; }

		data->pts[data->itr] = Rect(x, y, 0, 0);
		data->image = (data->init).clone();
		data->temp = (data->init).clone();


		break;
	case cv::EVENT_LBUTTONUP:
		if ((x < 0) || (data->image.size().width <= x) || (y < 0) || (data->image.size().height <= y)){ break; }

		data->pts[data->itr] = Rect(min(data->pts[data->itr].x, x), min(data->pts[data->itr].y, y), abs(data->pts[data->itr].x - x), abs(data->pts[data->itr].y - y));
		rectangle(data->image, data->pts[data->itr], Scalar(0, 0, 255));

		data->temp = (data->image).clone();


		break;
	}

}

int main(){
	Data data;

	char file_name[256];
	int counter = 0;
	int key = -1;
	while (1){
		if (counter < 0){ counter = 0; }


		sprintf(file_name, "C:\\Users\\0133752\\Documents\\Visual Studio 2013\\Projects\\CreateTrainData\\Release\\data\\%d.jpg", counter);
		namedWindow("Image", CV_WINDOW_AUTOSIZE);
		setMouseCallback("Image", Mouse, (void *)&data);
		data.image = imread(file_name);
		if (data.image.empty()){ break; }
		
		data.itr = counter;
		if (data.pts.size() <= data.itr){ data.pts.push_back(Rect(0, 0, 0, 0)); }

		data.init = (data.image).clone();
		data.temp = (data.image).clone();
		rectangle(data.image, data.pts[data.itr], Scalar(0, 0, 255));
		rectangle(data.temp, data.pts[data.itr], Scalar(0, 0, 255));

		cout << counter << ".jpg" << " : ";

		while (1){
			imshow("Image", data.image);


			key = waitKey(1);
			switch (key){
				//<-
			case 2424832:
				--counter;
				break;
				//->
			case 2555904:
				++counter;
				break;
				//space
			case 32:
				++counter;
				break;
				//enter
			case 13:
				++counter;
				break;
				//backspace
			case 8:
				--counter;
				break;
			default:
				break;
			}
			if ((key > 0) || (cvGetWindowHandle("Image") == NULL)){
				cout << data.pts[data.itr] << endl;
				break;
			}
		}
		if (key == 27){ break; }
	}

	char ifile_name[256];
	char ofile_name[256];
	ofstream ofs("C:\\Users\\0133752\\Documents\\Visual Studio 2013\\Projects\\CreateTrainData\\Release\\out\\out.txt");
	for (int i = 0; i < data.pts.size(); ++i){
		sprintf(ifile_name, "C:\\Users\\0133752\\Documents\\Visual Studio 2013\\Projects\\CreateTrainData\\Release\\data\\%d.jpg", i);
		sprintf(ofile_name, "C:\\Users\\0133752\\Documents\\Visual Studio 2013\\Projects\\CreateTrainData\\Release\\out\\%d.jpg", i);

		imwrite(ofile_name, imread(ifile_name)(data.pts[i]));
		ofs << ifile_name << " " << data.pts[i].x << " " << data.pts[i].y << " " << data.pts[i].width << " " << data.pts[i].height << endl;
	}

	return 0;
}