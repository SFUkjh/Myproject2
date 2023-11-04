#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

const int MAX_BINARY_VALUE = 255;

int main()
{
    string filename = "1_whole.png";

    Mat img = imread(filename);

    Mat hsv;
    cvtColor(img, hsv, COLOR_BGR2HSV);

    Scalar lower_green = Scalar(35, 43, 46);
    Scalar upper_green = Scalar(77, MAX_BINARY_VALUE, MAX_BINARY_VALUE);

    Mat mask;
    inRange(hsv, lower_green, upper_green, mask);

    vector<vector<Point>> contours;
    findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    float maxMinRatio = 0.6f;
    float minMaxRatio = 1.7f;

    for (int i = 0; i < contours.size(); i++)
    {
        Rect rect = cv::boundingRect(contours[i]);

        float aspectRatio = static_cast<float>(rect.width) / rect.height;

        if (aspectRatio >= maxMinRatio && aspectRatio <= minMaxRatio)
        {
            Point2f vertices[4];
            RotatedRect rect = minAreaRect(contours[i]);
            rect.points(vertices);

            Point2f pixelCoordinates[4];
            for (int j = 0; j < 4; j++)
            {
                pixelCoordinates[j] = vertices[j];
                cout << "Contour " << i << ", Vertex " << j
                    << ": (" << pixelCoordinates[j].x << ", " << pixelCoordinates[j].y << ")" << endl;
            }

           
        }
    }

    return 0;
}