#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;
int main()
{
    Mat taxi = imread("../taxi.jpg");
    if (taxi.empty())return -1;

    vector<Point2f> ori(4);
    /*ori[0]=Point2f(597,332);
    ori[1]=Point2f(764,334);
    ori[2]=Point2f(605,384);
    ori[3]=Point2f(772,386);*/
    ori[0].x=597,ori[1].x=764,ori[2].x=605,ori[3].x=772;
    ori[0].y=332,ori[1].y=334,ori[2].y=384,ori[3].y=386;
    vector<Point2f> ret(4);
    /*ori[0]=Point2f(0,0);
    ori[1]=Point2f(167,0);
    ori[2]=Point2f(0,52);
    ori[3]=Point2f(167,52);*/
    ret[0].x=0,ret[1].x=334,ret[2].x=0,ret[3].x=334;
    ret[0].y=0,ret[1].y=0,ret[2].y=104,ret[3].y=104;

    Mat transform = getPerspectiveTransform(ori,ret);
    Mat license;
    warpPerspective(taxi,license,transform,Size(334,104));
    imshow("ori",taxi);
    imshow("return1",license);
    Mat Gray;
    cvtColor(license,Gray,COLOR_BGR2GRAY);
    threshold(Gray,Gray,125,255,cv::THRESH_BINARY);
    imshow("return2",Gray);
    waitKey(0);
}