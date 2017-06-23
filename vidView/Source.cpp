/*
Branch : Using_QPC
> This brach used QueryPerformanceCounter method
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <windows.h>

using namespace cv;
using namespace std;

double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		cout << "QueryPerformanceFrequency failed!\n";

	PCFreq = double(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}
double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}

int main(int argc, char* argv[])
{
	CommandLineParser parser(argc, argv, "{@input||}");
	string arg = parser.get<string>("@input");
	if (arg.empty()) {
		//help(av);
		return 1;
	}
	VideoCapture cap(arg); // open the video file for reading
	if (!cap.isOpened()) // if not success, exit program
	{
		cout << "Cannot open the video file" << endl;
		return -1;
	}
	cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms

	double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
	double delay = 1000 / fps;//this is the delay that sould be between 2 frames to keep framerate

	cout << "Frame per seconds : " << fps << endl;

	namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
	Mat frame;
	for (;;)
	{
		StartCounter();		
		bool bSuccess = cap.read(frame);//&cap2.read(frame2); // read a new frame from video
		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read the frame from video file" << endl;
			break;
		}
		imshow("MyVideo", frame); //show the frame in "MyVideo" window
		if (waitKey(delay-GetCounter()) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}

	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////