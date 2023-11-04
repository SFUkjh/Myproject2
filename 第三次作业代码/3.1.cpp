#include <iostream>
#include <Eigen/Dense>
#include <math.h>
#include <opencv2/viz.hpp>
#define M_PI 3.1415926
using namespace Eigen;
using namespace cv;

int main()
{
    double alpha, beta, gamma;
    std::cout << "请输入欧拉角(单位：度)：" << std::endl;
    std::cin >> alpha >> beta >> gamma;

    alpha = alpha * M_PI / 180.0;
    beta = beta * M_PI / 180.0;
    gamma = gamma * M_PI / 180.0;

    Matrix3d R;
    R = AngleAxisd(alpha, Vector3d::UnitX())
        * AngleAxisd(beta, Vector3d::UnitY())
        * AngleAxisd(gamma, Vector3d::UnitZ());

    std::vector<cv::Point3d> points;
    points.push_back(cv::Point3d(0, 0, 0));
    points.push_back(cv::Point3d(R(0, 0), R(0, 1), R(0, 2)));
    points.push_back(cv::Point3d(0, 0, 0));
    points.push_back(cv::Point3d(R(1, 0), R(1, 1), R(1, 2)));
    points.push_back(cv::Point3d(0, 0, 0));
    points.push_back(cv::Point3d(R(2, 0), R(2, 1), R(2, 2)));

    viz::WCloud cloud(points);
    cloud.setRenderingProperty(viz::POINT_SIZE, 5);

    viz::Viz3d window("旋转矩阵可视化");
    window.showWidget("坐标系", viz::WCoordinateSystem());
    window.showWidget("旋转矩阵", cloud);
    window.spin();

    return 0;
}
