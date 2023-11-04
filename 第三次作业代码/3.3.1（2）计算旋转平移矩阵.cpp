#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main() {
    // 定义世界坐标系下的四个点
    Mat world_points = (Mat_<float>(4, 3) << -0.05, 0.05, 0,
        0.05, 0.05, 0,
        0.05, -0.05, 0,
        -0.05, -0.05, 0);

    // 定义像素坐标系下的四个点
    Mat image_points = (Mat_<float>(4, 2) << 982, 815,
        1066, 807,
        1073, 898,
        990, 905);

    // 定义相机内参
    Mat camera_matrix = (Mat_<float>(3, 3) << 1900, 0, 960,
        0, 1900, 540,
        0, 0, 1);
    Mat distortion_coeffs = Mat::zeros(5, 1, CV_32F);

    // 计算旋转和平移向量
    Mat rotation_vector, translation_vector;
    bool success = solvePnP(world_points, image_points, camera_matrix, distortion_coeffs, rotation_vector, translation_vector);

    if (success) {
        cout << "计算成功！" << endl;

        // 计算旋转矩阵
        Mat rotation_matrix;
        Rodrigues(rotation_vector, rotation_matrix);

        cout << "旋转向量：" << rotation_vector << endl;
        cout << "平移向量：" << translation_vector << endl;
        cout << "旋转矩阵：" << rotation_matrix << endl;
    }
    else {
        cout << "计算失败！" << endl;
    }

    return 0;
}