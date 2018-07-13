#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\opencv.hpp"
#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <math.h>
#include <sstream>
#include <string>
using namespace cv;
using namespace std;
#define MAX 10000
#define classNum 3

void preProcessing(Mat &image, Mat &_dst){
    Mat dst;
    dst.create(10, 10, CV_8UC1);
    int parentCol = image.cols;
    int parentRow = image.rows;
    int col_pad_10 = (parentCol)%10;
    int col_pad = (parentCol-col_pad_10)/10;
    col_pad_10 += col_pad;
    int row_pad_10 = (parentRow)%10;
    int row_pad = (parentRow-row_pad_10)/10;
    row_pad_10 += row_pad;
//    cvtColor(image, image, COLOR_BGR2GRAY);
    threshold(image, image, 48, 255, CV_THRESH_BINARY);
    image.convertTo(image, CV_8UC1);
    for (int i=0; i<9; i++){
        for (int j=0; j<9; j++) {
            int blockRow = (i==9) ? row_pad_10:row_pad;
            int blockCol = (j==9) ? col_pad_10:col_pad;
            int count = 0;
            for (int m=i*row_pad; m<i*row_pad+blockRow; m++){
                for (int n=j*col_pad; n<i*col_pad+blockCol; n++){
                    if(image.at<uchar>(m, n) > 0){
                        count++;
                    }
                }
            }
            dst.at<uchar>(i, j) = count/(blockRow*blockCol)*255;
        }
    }
    threshold(dst, dst, 48, 255, CV_THRESH_BINARY);
    dst.convertTo(dst, CV_8UC1);
    dst.copyTo(_dst);
}

void findCentroid(Mat &meshed, double* result){
    double ret[2] = {0.0, 0.0};
    int count = 0;
    int x = 0;
    int y = 0;
    for(int i=0; i<9; i++){
        for(int j=0; j<9; j++){
            if(meshed.at<uchar>(i, j) > 0){
                count++;
                x += i;
                y += j;
            }
        }
    }
    ret[0] = x/count;
    ret[1] = y/count;
    result = ret;
}

double euclideanDistance(double* xy1, double* xy2){
    return sqrt(pow((xy1[0]-xy2[0]), 2) + pow((xy1[1]-xy2[1]), 2));
}

int findMin(double* distArr, int length){
    double min = MAX;
    int label = 0;
    for(int i=0; i<length-1; i++){
        if(min > distArr[i]){
            min = distArr[i];
            label = i;
        }
    }
    return label;
}

int main(void){
    Mat testImg = imread("test.jpg");
    imshow("To be classified...", testImg);
    Mat testPre;
    preProcessing(testImg, testPre);
    double* testCentroid = new double[2];
    findCentroid(testPre, testCentroid);
    double* distArr = new double[classNum];
    for(int i=1; i<classNum; i++){
        stringstream ss;
        ss << "class" << i << ".jpg";
        Mat image = imread(ss.str());
        Mat classPre;
        preProcessing(image, classPre);
        double* classCentroid = new double[2];
        findCentroid(classPre, classCentroid);
        distArr[i-1] = euclideanDistance(classCentroid, testCentroid);
    }
    int classfied = findMin(distArr, classNum) + 1;
    stringstream ss;
    ss << "class" << classfied << ".jpg";
    Mat result = imread(ss.str());
    imshow("Classified result", result);
    waitKey(0);
    return 0;
}
