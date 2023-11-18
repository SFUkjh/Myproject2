#include <iostream>
#include <opencv2/opencv.hpp>

// KalmanFilter 类的代码与之前保持不变
class KalmanFilter {
public:
    cv::KalmanFilter kf_;

    KalmanFilter() {
        // 初始化卡尔曼滤波器参数
        kf_ = cv::KalmanFilter(4, 2, 0);

        // 设置状态转移矩阵 A
        kf_.transitionMatrix = (cv::Mat_<float>(4, 4) << 1, 0, 1, 0,
            0, 1, 0, 2,
            0, 0, 1, 0,
            0, 0, 0, 1);

        // 设置测量矩阵 H
        kf_.measurementMatrix = (cv::Mat_<float>(2, 4) << 1, 0, 0, 0,
            0, 1, 0, 0);

        // 设置过程噪声协方差矩阵 Q
        kf_.processNoiseCov = (cv::Mat_<float>(4, 4) << 1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1) * 0.1;

        // 设置测量噪声协方差矩阵 R
        kf_.measurementNoiseCov = (cv::Mat_<float>(2, 2) << 1, 0,
            0, 1) * 5;

        // 设置初始状态协方差矩阵 P
        kf_.errorCovPost = (cv::Mat_<float>(4, 4) << 1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);
        state_ = cv::Mat(4, 1, CV_32F);
    }

    cv::Mat predict() {
        state_ = kf_.predict();
        return state_;
    }

    cv::Mat correct(const cv::Mat& measurement) {
        return kf_.correct(measurement);
    }

private:
    cv::Mat state_;
};

int main() {
    cv::Mat measurement = cv::Mat::zeros(2, 1, CV_32F); // 初始化测量值
    KalmanFilter kf; // 创建卡尔曼滤波器对象

    std::vector<float> stateX, stateY, measurementX, measurementY; // 用于存储状态和测量值的数据

    float vx = 1.0; // x 方向的匀速运动初速度
    float vy = 0.2; // y 方向的匀速运动初速度

    // 创建画布
    cv::Mat canvas(800, 800, CV_8UC3, cv::Scalar(255, 255, 255));

    // 绘制坐标系


    int numPointsToDraw = 200; // 要绘制的点的数量
    int skipFactor = 2; // 跳过的点数

    while (true) {
        // 生成随机运动状态
        cv::Mat state = kf.predict();

        // 更新状态变量
        state.at<float>(0) += vx; // 更新 x 位置
        state.at<float>(1) += vy; // 更新 y 位置

        // 生成测量值
        cv::randn(measurement, cv::Scalar::all(0), cv::Scalar::all(std::sqrt(kf.kf_.measurementNoiseCov.at<float>(0, 0))));
        measurement += kf.kf_.measurementMatrix * state;

        // 储存状态和测量值数据
        stateX.push_back(state.at<float>(0));
        stateY.push_back(state.at<float>(1));
        measurementX.push_back(measurement.at<float>(0));
        measurementY.push_back(measurement.at<float>(1));


        // 更新卡尔曼滤波器
        kf.correct(measurement);

        // 清空画布
        canvas.setTo(cv::Scalar(255, 255, 255));

        cv::line(canvas, cv::Point(0, 400), cv::Point(800, 400), cv::Scalar(0, 0, 0));
        cv::line(canvas, cv::Point(400, 0), cv::Point(400, 800), cv::Scalar(0, 0, 0));

        cv::putText(canvas, "X", cv::Point(780, 380), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1, cv::LINE_AA);
        cv::putText(canvas, "Y", cv::Point(380, 780), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1, cv::LINE_AA);
        // 绘制 X 轴刻度
        for (int x = -10; x <= 10; ++x) {
            cv::line(canvas, cv::Point((x + 10) * 40, 395), cv::Point((x + 10) * 40, 405), cv::Scalar(0, 0, 0), 1, cv::LINE_AA);
            if (x != 0) {
                cv::putText(canvas, std::to_string(x), cv::Point((x + 10) * 40 - 5, 420), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1, cv::LINE_AA);
            }
        }

        // 绘制 Y 轴刻度
        for (int y = -10; y <= 10; ++y) {
            cv::line(canvas, cv::Point(395, (y + 10) * 40), cv::Point(405, (y + 10) * 40), cv::Scalar(0, 0, 0), 1, cv::LINE_AA);
            if (y != 0) {
                cv::putText(canvas, std::to_string(y), cv::Point(415, (y + 10) * 40 + 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1, cv::LINE_AA);
            }
        }
        // 绘制状态曲线
        for (int i = 1 + skipFactor; i < stateX.size(); i += skipFactor) {
            cv::line(canvas, cv::Point(stateX[i - skipFactor] * 2 + 400, 400 - stateY[i - skipFactor] * 2),
                cv::Point(stateX[i] * 2 + 400, 400 - stateY[i] * 2), cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
        }

        // 绘制测量曲线
        for (int i = 1 + skipFactor; i < measurementX.size(); i += skipFactor) {
            cv::line(canvas, cv::Point(measurementX[i - skipFactor] * 2 + 400, 400 - measurementY[i - skipFactor] * 2),
                cv::Point(measurementX[i] * 2 + 400, 400 - measurementY[i] * 2), cv::Scalar(255, 0, 0), 1, cv::LINE_AA);
        }

        // 显示画布
        cv::imshow("Canvas", canvas);

        // 退出条件
        char c = cv::waitKey(30);
        if (c == 27)
            break;
    }

    return 0;
}