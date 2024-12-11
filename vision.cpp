#include<math.h>
#include"vision.hpp"
using namespace cv;

void setFrame(Mat& frame){
	frame = frame(Rect(Point(0,frame.rows/4*3),Point(frame.cols,frame.rows)));  
	cvtColor(frame,frame,COLOR_RGB2GRAY);
	frame +=  Scalar(100) - mean(frame);
	threshold(frame, frame, 0, 255, THRESH_BINARY | THRESH_OTSU);
}

void drawBoundingBox(Mat& frame,Mat stats,Mat centroids, int lable, int index, Point po){
	cvtColor(frame, frame, COLOR_GRAY2BGR);
	Scalar sc;
	 for(int i = 1;i<lable;i++){
		if(i == index) sc = Scalar(0,0,255);
        else sc =stats.at<int>(i,4)<50 ? Scalar(0,255,255):Scalar(255,0,0);
        rectangle(frame,Rect(stats.at<int>(i,0),stats.at<int>(i,1),stats.at<int>(i,2),stats.at<int>(i,3)),sc,2);
        rectangle(frame,Rect(centroids.at<double>(i,0),centroids.at<double>(i,1),3,3),sc,2);
        } 
	if(index == 0){
		 rectangle(frame,Rect(po.x,po.y,3,3),Scalar(0,0,255),2);
	}
}

int findMinIndex(Mat stats,Mat centroids, int lable, Point& po,int MINDISTANCE){
	int index = 0;
	double mindistance = MINDISTANCE;
	for(int i = 1;i<lable;i++){
        if(stats.at<int>(i,4)<50) continue;
		double distance = sqrt(pow((po.x-centroids.at<double>(i,0)),2)+pow((po.y- centroids.at<double>(i,1)),2));
		if(distance<mindistance){
            mindistance = distance;
            index = i;
        }
     }
	if(mindistance <  MINDISTANCE && po != Point(centroids.at<double>(index,0),centroids.at<double>(index,1))) 
		po = Point(centroids.at<double>(index,0),centroids.at<double>(index,1));
	return index;
}

double getErr(Mat frame, Point po,double gain){
	return (frame.cols/2 -po.x)*gain;
}
