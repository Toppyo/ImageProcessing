#include "stdafx.h"
#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\highgui\highgui.hpp"
#include <iostream>
#include <ctime>
using namespace cv;
using namespace std;

uchar Median(uchar n1, uchar n2, uchar n3, uchar n4, uchar n5,
    uchar n6, uchar n7, uchar n8, uchar n9) {  
    uchar arr[9];  
    arr[0] = n1;  
    arr[1] = n2;  
    arr[2] = n3;  
    arr[3] = n4;  
    arr[4] = n5;  
    arr[5] = n6;  
    arr[6] = n7;  
    arr[7] = n8;  
    arr[8] = n9;  
    for (int gap = 9 / 2; gap > 0; gap /= 2)
        for (int i = gap; i < 9; ++i)  
            for (int j = i - gap; j >= 0 && arr[j] > arr[j + gap]; j -= gap)  
                swap(arr[j], arr[j + gap]);  
    return arr[4];  
}  
  
void salt(Mat &image, int num) {  
    int i, j;
    srand(time(NULL));  
    for (int x = 0; x < num; ++x) {  
        i = rand() % image.rows;  
        j = rand() % image.cols;  
        image.at<Vec3b>(i, j)[0] = 255;  
        image.at<Vec3b>(i, j)[1] = 255;  
        image.at<Vec3b>(i, j)[2] = 255;  
    }  
}

void pepper(Mat &image, int num) {
    int i, j;
    srand(time(NULL));
    for (int x = 0; x < num; ++x) {
        i = rand() % image.rows;
        j = rand() % image.cols;
        image.at<Vec3b>(i, j)[0] = 0;
        image.at<Vec3b>(i, j)[1] = 0;
        image.at<Vec3b>(i, j)[2] = 0;
    }
}

void addNoise(Mat &image, int num){
    if(!image.data) return;
    salt(image, num);
    pepper(image, num);
}
  
void MedianFlitering(const Mat &src, Mat &dst) {  
    if (!src.data)return;  
    Mat _dst(src.size(), src.type());  
    for(int i=0;i<src.rows;++i)  
        for (int j=0; j < src.cols; ++j) {  
            if ((i - 1) > 0 && (i + 1) < src.rows && (j - 1) > 0 && (j + 1) < src.cols) {  
                _dst.at<Vec3b>(i, j)[0] = Median(src.at<Vec3b>(i, j)[0], src.at<Vec3b>(i + 1, j + 1)[0],  
                    src.at<Vec3b>(i + 1, j)[0], src.at<Vec3b>(i, j + 1)[0], src.at<Vec3b>(i + 1, j - 1)[0],  
                    src.at<Vec3b>(i - 1, j + 1)[0], src.at<Vec3b>(i - 1, j)[0], src.at<Vec3b>(i, j - 1)[0],  
                    src.at<Vec3b>(i - 1, j - 1)[0]);  
                _dst.at<Vec3b>(i, j)[1] = Median(src.at<Vec3b>(i, j)[1], src.at<Vec3b>(i + 1, j + 1)[1],  
                    src.at<Vec3b>(i + 1, j)[1], src.at<Vec3b>(i, j + 1)[1], src.at<Vec3b>(i + 1, j - 1)[1],  
                    src.at<Vec3b>(i - 1, j + 1)[1], src.at<Vec3b>(i - 1, j)[1], src.at<Vec3b>(i, j - 1)[1],  
                    src.at<Vec3b>(i - 1, j - 1)[1]);  
                _dst.at<Vec3b>(i, j)[2] = Median(src.at<Vec3b>(i, j)[2], src.at<Vec3b>(i + 1, j + 1)[2],  
                    src.at<Vec3b>(i + 1, j)[2], src.at<Vec3b>(i, j + 1)[2], src.at<Vec3b>(i + 1, j - 1)[2],  
                    src.at<Vec3b>(i - 1, j + 1)[2], src.at<Vec3b>(i - 1, j)[2], src.at<Vec3b>(i, j - 1)[2],  
                    src.at<Vec3b>(i - 1, j - 1)[2]);  
            }  
            else  
                _dst.at<Vec3b>(i, j) = src.at<Vec3b>(i, j);  
        }  
    _dst.copyTo(dst);
}  
  
  
void main() {  
    Mat image = imread("example.jpg");  
  
    Mat Noise_Image;
    image.copyTo(Noise_Image);
    addNoise(Noise_Image, 3000);
  
    Mat image3, image4;  
    MedianFlitering(Noise_Image, image3);
    medianBlur(Noise_Image, image4, 3);
    imshow("After adding noise...", Noise_Image);
    imshow("After manually median filtering...", image3);  
    imshow("After OpenCV median filtering...", image4);
    imwrite("NoisyImage.jpg", Noise_Image);
    imwrite("ManuallyProcessed.jpg", image3);
    imwrite("OpenCVProcessed.jpg", image4);
    waitKey();  
}  
