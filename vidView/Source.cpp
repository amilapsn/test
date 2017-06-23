/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <windows.h>

using namespace cv;
using namespace std;

double PCFreq = 0.0;
__int64 CounterStart = 0;
int toggle = 0;

//variables/arrays needed for pid controller to maintain framerate
double e[2] = { 25, 25 }, dif, summ, pdi[3] = { 1, .5, 0 }, average_fps, pid_output;

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
	SYSTEMTIME time;
	if (!cap.isOpened())//||!cap2.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the video file" << endl;
		return -1;
	}

	//cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms

	double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
	double delay = 1000 / fps;

	cout << "Frame per seconds : " << fps << endl;

	namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
	double counter = 1;
	double total=0,time_ = 0;
	double actual_fps = fps;
	for (;;)
	{
		/*GetSystemTime(&time);
		LONG time_ms2 = (time.wSecond * 1000) + time.wMilliseconds;*/
		Mat frame;
		//Mat frame2;
		StartCounter();
		
		bool bSuccess = cap.read(frame);//&cap2.read(frame2); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read the frame from video file" << endl;
			break;
		}

		/*GetSystemTime(&time);
		LONG time_ms2 = (time.wSecond * 1000) + time.wMilliseconds;*/
		//waitKey(delay-time_ms2+time_ms1);
		//GetSystemTime(&time);
		/*LONG time_ms3 = (time.wSecond * 1000) + time.wMilliseconds;
		LONG actual_fps = 1000 / (time_ms3 - time_ms1);
		total += actual_fps;
		LONG average_fps = total / counter;
		counter += 1;
		cout << "Actual FPS:\t " << actual_fps << "\n";
		cout << "Average FPS:\t " << average_fps << "\n";*/
		
		//pid part:
		/*toggle = !toggle;
		e[toggle] = fps - actual_fps;
		dif = e[toggle] - e[!toggle];
		summ = e[toggle] + e[!toggle];
		pid_output = (pdi[0] * e[toggle] + pdi[1] * dif + pdi[2] * summ);
		time_ = pid_output <= -delay / 2 ? 1 - delay / 2 : pid_output;
		Sleep(time_+delay/2);*/
		//cout << "Average FPS: " << average_fps << "\t" << "Actual FPS: " << actual_fps << "\t" << "Error: " << e[toggle] << "\n";
		imshow("MyVideo", frame); //show the frame in "MyVideo" window
		/*total += actual_fps;
		average_fps = total / counter;
		counter++;
		actual_fps = 1000 / GetCounter();*/
		if (waitKey(32) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}

	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////