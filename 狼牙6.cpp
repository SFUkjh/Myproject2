#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
int main() {
    Mat image = cv::imread("kjh.jpg");
    if (image.empty()) {
        cout << "无法读取图像文件" << endl;
        return -1;
    }

    int rows = image.rows;
    int cols = image.cols;
    float gamma = 2;

    // 像素值查找表
    Mat lut(1, 256, CV_8UC1);
    for (int i = 0; i < 256; i++) {
        lut.at<uchar>(i) = saturate_cast<uchar>(pow(static_cast<float>(i) / 255.f, gamma) * 255.f);
    }

    if (image.channels() == 3) {
        cvtColor(image, image, cv::COLOR_BGR2GRAY);
    }

    Mat image_gamma_corrected;
    LUT(image, lut, image_gamma_corrected);

    // 将图像转换回彩色空间
    cvtColor(image_gamma_corrected, image_gamma_corrected, COLOR_GRAY2BGR);

    imwrite("gamma_correction.png", image_gamma_corrected);

    return 0;
}