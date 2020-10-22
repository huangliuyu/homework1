#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;
int main()
{
    Mat ori=imread("../apple.png");
    vector<Mat> detects;
    Mat blue,red,green;
    split(ori,detects);
    blue=detects[0];
    green=detects[1];
    red=detects[2];
    //Mat newchannel=green.clone();
    //newchannel.setTo(0);
    //Mat newchannels[3]={blue,newchannel,red};
    //Mat detect;
    //merge(newchannels,3,detect);
    //imshow("1",detect);
    //waitKey(0);

    threshold(red,red,200,255,cv::THRESH_BINARY);
    //imshow("2",red);
    //waitKey(0);
    Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(18, 18));
    morphologyEx(red,red,cv::MORPH_OPEN,element);
    //imshow("3",red);
    for(int i=0;i<15;i++)dilate(red,red,Mat());

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(red,contours,hierarchy,RETR_LIST,CHAIN_APPROX_NONE,Point(0,40));
    //drawContours(apple,contours,-1,Scalar(255,255,255,0));
    imshow("red2",red);
    waitKey(0);
    for(int i=0;i<contours.size();++i)
    {
        if (contours[i][0].x < 10 || contours[i].size() < 50)continue;
        RotatedRect bound = minAreaRect(contours[i]);
        if (bound.size.width/bound.size.height<0.5)continue;
        if (bound.size.height < 200)continue;
        rectangle(ori, Point2f(bound.center.x - bound.size.width / 2, bound.center.y - bound.size.height / 2),
                  Point2f(bound.center.x + bound.size.width / 2, bound.center.y + bound.size.height / 2),
                  Scalar(0, 255, 255), 2
        );
    }
    imshow("result",ori);
    imwrite("result.jpg",ori);
    waitKey(0);
    destroyAllWindows();
    return 0;
}