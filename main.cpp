#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

using namespace std;
using namespace cv;

int main()
{
    int num=10;
    Size board_size(9,6);
    Mat img, gray, drawn;
    vector<Point2f> each_img_corner;
    vector<vector<Point2f>> point_corner;

    int count=0;
    Size imgsize;
    for(int i=0;i<=num;i++)
    {
        String filename="../chess/"+to_string(i)+".jpg";
        img=imread(filename);
        if(img.empty())
        {
            cout << "fail to find " << filename << endl;
            continue;
        }
        else
        {
            cout<<"get "<<filename<<endl;
        }
        if(i==0)
        {
            imgsize.width=img.cols;
            imgsize.height=img.rows;
        }
        findChessboardCorners(img,board_size,each_img_corner);
        if(each_img_corner.size()==54)
        {
            cvtColor(img,gray,COLOR_BGR2GRAY);
            find4QuadCornerSubpix(gray,each_img_corner,Size(5,5));
            point_corner.push_back(each_img_corner);
            drawn=img.clone();
            drawChessboardCorners(drawn,board_size,each_img_corner,54);
            //string name=to_string(i);
            //imshow(name,drawn);
            each_img_corner.clear();
        }
    }
    cout<<"get corners finished"<<endl;

    Mat cameraMatrix=Mat(3,3,CV_32FC1,Scalar::all(0));
    Mat distCoefficients=Mat(1,5,CV_32FC1,Scalar::all(0));
    vector<Mat>tvecs,rvecs;
    vector<vector<Point3f>> all_corner_points;

    int i,j,k;
    for(k=0;k<=num;k++)
    {
        vector<Point3f> tmpCornerPoints;
        for(i=0;i<board_size.height;i++)
            for(j=0;j<board_size.width;j++)
            {
                Point3f RealPoint;
                RealPoint.x=i*10;
                RealPoint.y=j*10;
                RealPoint.z=0;
                tmpCornerPoints.push_back(RealPoint);
            }
        all_corner_points.push_back(tmpCornerPoints);
    }
    calibrateCamera(all_corner_points,point_corner,imgsize,cameraMatrix,distCoefficients,rvecs,tvecs,0);
    cout<<"calibrate finished"<<endl;

    cout << "内外参数矩阵:" << endl;
    cout << cameraMatrix << endl;
    cout << "畸变系数：" << endl;
    cout << distCoefficients << endl;
    Mat map_x( imgsize, CV_32FC1 );
    Mat map_y( imgsize, CV_32FC1 );
    initUndistortRectifyMap(
            cameraMatrix,
            distCoefficients,
            Mat(),
            cameraMatrix,
            imgsize,
            CV_32FC1,
            map_x,
            map_y
    );

    Mat output_img;
    for (int i = 0; i <=num ; i++ )
    {
        String name = to_string(i);
        string filename="../chess/"+name+".jpg";
        img=imread(filename);
        remap(img, img, map_x, map_y, INTER_LINEAR);
        imshow(name, img);
        waitKey(500);
    }
}
