#include <iostream>
#include <unistd.h>
#include <signal.h>
#include "vision.hpp"  
#include "opencv2/opencv.hpp"
#include "dxl.hpp"
#define MINDISTANCE 75
#define RPM 100
using namespace std;
using namespace cv;
#define GAIN 0.25
bool ctrl_c_pressed;
void ctrlc(int)
{
	ctrl_c_pressed = true;
}

int main(void)
{	
/*
    string src = "nvarguscamerasrc sensor-id=0 ! \
video/x-raw(memory:NVMM), width=(int)640, height=(int)360, \
format=(string)NV12, framerate=(fraction)30/1 ! \
nvvidconv flip-method=0 ! video/x-raw, \
width=(int)640, height=(int)360, format=(string)BGRx ! \
videoconvert ! video/x-raw, format=(string)BGR ! appsink";
	VideoCapture source(src, CAP_GSTREAMER);
*/
	Dxl dxl;
	if (!dxl.open()) { cout << "dxl open error" << endl; return -1; }
    VideoCapture source("5_lt_cw_100rpm_out.mp4");
	//VideoCapture source("7_lt_ccw_100rpm_in.mp4");
	if (!source.isOpened()) { cout << "Camera error" << endl; return -1; }

	string dst1 = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! \
nvvidconv ! nvv4l2h264enc insert-sps-pps=true ! \
h264parse ! rtph264pay pt=96 ! \
udpsink host=203.234.58.157 port=8001 sync=false";
	VideoWriter writer1(dst1, 0, (double)30, Size(640, 360/4), true);
	if (!writer1.isOpened()) {
		cerr << "Writer open failed!" << endl; return -1;
	}

	string dst2 = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! \
nvvidconv ! nvv4l2h264enc insert-sps-pps=true ! \
h264parse ! rtph264pay pt=96 ! \
udpsink host=203.234.58.157 port=8002 sync=false";
	VideoWriter writer2(dst2, 0, (double)30, Size(640, 360), true);
	if (!writer2.isOpened()) {
		cerr << "Writer2 open failed!" << endl; return -1;
	}
	signal(SIGINT, ctrlc); //시그널 핸들러 지정
	Mat stats, centroids, labels,frame, gray;
	TickMeter tm,tm2;
	bool mode = false;
	double lvel = RPM, rvel = RPM*-1;
	while (true)
	{
		tm.reset();
		tm.start(); 
		tm2.reset();
		tm2.start();
		if(dxl.kbhit()) {// 없으면 제어 멈춤
			char ch = dxl.getch();
			if(ch == 'q') break;
			else if(ch == 's') mode = true; 
		}
		
		source >> frame;
		if (frame.empty()) { cerr << "frame empty!" << endl; break; }
		writer2<<frame;

		setFrame(frame);

		int lable =  connectedComponentsWithStats(frame, labels, stats, centroids);

		static Point po = Point(frame.cols/2,frame.rows/2);

		int index = findMinIndex(stats,centroids, lable,po, MINDISTANCE);
		
		drawBoundingBox(frame,stats,centroids, lable, index,po);

		lvel = RPM - getErr(frame, po, GAIN);
		rvel = RPM*-1 - getErr(frame, po, GAIN);

		if(mode) dxl.setVelocity(lvel, rvel);
		writer1 <<frame; 
		if (ctrl_c_pressed) break;
		tm2.stop();
		int sleep = 33 * 1000-tm2.getTimeMicro();
		if(sleep < 0) sleep = 0;
		usleep(sleep);
		tm.stop();
		cout<<"error : "<<getErr(frame, po, GAIN) <<" lvel : " << lvel <<" rvel : " << rvel <<" time : " <<tm.getTimeSec() << "Sec"<<endl;
	}
	dxl.close();
	return 0;
}
