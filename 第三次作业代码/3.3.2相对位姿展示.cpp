#include <iostream>
#include <opencv2/viz.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {

    viz::Viz3d myWindow("Coordinate Frame");
    //----------------------读入相机位姿----------------------
    Vec3d cam1_pose(0, 0, 0), cam1_focalPoint(0, 0, 1), cam1_y_dir(0, -1, 0); // 设置相机的朝向（光轴朝向）
    Affine3d cam_3_pose = viz::makeCameraPose(cam1_pose, cam1_focalPoint, cam1_y_dir); // 设置相机位置与朝向

    myWindow.showWidget("World_coordinate", viz::WCoordinateSystem(), cam_3_pose); // 创建3号相机位于世界坐标系的原点
    // 创建R\T
    Matx33d PoseR_0; // 旋转矩阵
    Vec3d PoseT_0; // 平移向量
    PoseR_0 = Matx33d(0.9196649454643102, - 0.07740041278793452, - 0.3850007327062954, -0.1440154416358656, - 0.9785530848666946,- 0.1472868380687909,
    0.365343592600929,0.1909005924509743, - 0.911087824058314);
    PoseT_0 = Vec3d(0.07524284121419732, 0.3529977288625165, 2.12192829013572);
    Affine3d Transpose03(PoseR_0, PoseT_0); // 03相机变换矩阵
    
    // ----------------------设置坐标系----------------------
    myWindow.showWidget("Cam0", viz::WCoordinateSystem(), Transpose03);
    
    // ----------------------显示----------------------
    myWindow.spin();
    return 0;
}

