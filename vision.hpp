#ifndef VISION_HPP
#define VISION_HPP

#include "opencv2/opencv.hpp"


void setFrame(cv::Mat& frame);

void drawBoundingBox(cv::Mat& frame,cv::Mat stats,cv::Mat centroids, int lable, int index);

int findMinIndex(cv::Mat stats,cv::Mat centroids, int lable, cv::Point& po,int MINDISTANCE);

double getErr(cv::Mat frame, cv::Point po,int err);
#endif 
