#ifndef cvMat2Qiamge_H
#define cvMat2Qiamge_H
#include <QImage>
#include <opencv2/opencv.hpp>

using  namespace std;
using  namespace cv;
QImage cvMat2QImage(const cv::Mat& mat);
cv::Mat QImage2cvMat(QImage& image);
cv::Mat cvMatInt2cvMatChar(const cv::Mat& TempImage);
cv::Mat intMat2Char(cv::Mat& TempImage, int value);
#endif // cvMat2Qiamge_H
