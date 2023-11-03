#include <opencv2/opencv.hpp>
#include<iostream>
#include<opencv2/highgui/highgui_c.h>
#include<vector>
#include <opencv2/imgproc.hpp >
 
using namespace cv;
using namespace std;

int main() {
    // ��ȡͼƬ
    Mat image = imread("chepai5.jpg");
    if (image.empty()) {
        std::cout << "�޷���ȡͼ���ļ�" << std::endl;
        return -1;
    }

    // ��ͼ��תΪHSV��ɫ�ռ�
    Mat hsvImage;
    cvtColor(image, hsvImage, COLOR_BGR2HSV);

    // ������ɫ���Ƶ���ɫ��Χ
    Scalar lowerBlue(100, 50, 50);  // HSV����ɫ���½�
    Scalar upperBlue(130, 255, 255);  // HSV����ɫ���Ͻ�

    // ��ɫ��ֵ��
    Mat colorMask;
    inRange(hsvImage, lowerBlue, upperBlue, colorMask);

    // ��̬ѧ�������ղ�����ʴȥ������
    Mat kernel = getStructuringElement(MORPH_RECT, Size(20, 20));
    morphologyEx(colorMask, colorMask, MORPH_CLOSE, kernel);

    // ��������
    vector<vector<Point>> contours;
    findContours(colorMask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // ��ȡ�������ĸ������
    vector<Point> contour = contours[0];
    RotatedRect minRect = minAreaRect(contour);
    Point2f vertices[4];
    minRect.points(vertices);

    for (int i = 0; i < 4; i++)
    {
        cout << vertices[i] << endl;
    }
    // ���徭������任��Ķ�����������
    Point2f correctedVertices[4];

    // ����У�����Ŀ�궥������
    correctedVertices[0] = Point2f(0, 0);
    correctedVertices[1] = Point2f(image.size().width, 0);
    correctedVertices[2] = Point2f(image.size().width, image.size().height);
    correctedVertices[3] = Point2f(0, image.size().height);

    // ԭʼ���������Ŀ�궥�����������
    vector<Point2f> srcPoints;
    vector<Point2f> dstPoints;

    for (int i = 0; i < 3; i++) {
        srcPoints.push_back(vertices[i]);
        dstPoints.push_back(correctedVertices[i]);
    }

    // �������任����
    Mat affineTransform = getAffineTransform(srcPoints, dstPoints);
    // ���з���任
    Mat correctedImage;
    warpAffine(image, correctedImage, affineTransform, image.size());

    // ��ʾԭʼͼ����������ͼ��
    imshow("ԭʼͼ��", image);
    imshow("�������ͼ��", correctedImage);
    waitKey(0);

    return 0;
}
