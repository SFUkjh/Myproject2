#include <opencv2/opencv.hpp>
#include<iostream>
#include<opencv2/highgui/highgui_c.h>
#include<vector>
#include <opencv2/imgproc.hpp >
 
using namespace cv;
using namespace std;

int main() {
    // 读取图片
    Mat image = imread("chepai5.jpg");
    if (image.empty()) {
        std::cout << "无法读取图像文件" << std::endl;
        return -1;
    }

    // 将图像转为HSV颜色空间
    Mat hsvImage;
    cvtColor(image, hsvImage, COLOR_BGR2HSV);

    // 设置蓝色车牌的颜色范围
    Scalar lowerBlue(100, 50, 50);  // HSV中蓝色的下界
    Scalar upperBlue(130, 255, 255);  // HSV中蓝色的上界

    // 颜色阈值化
    Mat colorMask;
    inRange(hsvImage, lowerBlue, upperBlue, colorMask);

    // 形态学操作，闭操作腐蚀去除噪声
    Mat kernel = getStructuringElement(MORPH_RECT, Size(20, 20));
    morphologyEx(colorMask, colorMask, MORPH_CLOSE, kernel);

    // 查找轮廓
    vector<vector<Point>> contours;
    findContours(colorMask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // 获取轮廓的四个顶点角
    vector<Point> contour = contours[0];
    RotatedRect minRect = minAreaRect(contour);
    Point2f vertices[4];
    minRect.points(vertices);

    for (int i = 0; i < 4; i++)
    {
        cout << vertices[i] << endl;
    }
    // 定义经过仿射变换后的顶点坐标数组
    Point2f correctedVertices[4];

    // 计算校正后的目标顶点坐标
    correctedVertices[0] = Point2f(0, 0);
    correctedVertices[1] = Point2f(image.size().width, 0);
    correctedVertices[2] = Point2f(image.size().width, image.size().height);
    correctedVertices[3] = Point2f(0, image.size().height);

    // 原始顶点坐标和目标顶点坐标的向量
    vector<Point2f> srcPoints;
    vector<Point2f> dstPoints;

    for (int i = 0; i < 3; i++) {
        srcPoints.push_back(vertices[i]);
        dstPoints.push_back(correctedVertices[i]);
    }

    // 计算仿射变换矩阵
    Mat affineTransform = getAffineTransform(srcPoints, dstPoints);
    // 进行仿射变换
    Mat correctedImage;
    warpAffine(image, correctedImage, affineTransform, image.size());

    // 显示原始图像和修正后的图像
    imshow("原始图像", image);
    imshow("修正后的图像", correctedImage);
    waitKey(0);

    return 0;
}
