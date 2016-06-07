#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#include "kcftracker.hpp"

#include <dirent.h>

using namespace std;
using namespace cv;

// Frame readed
Mat frame;
// Tracker results
Rect result;
bool select_flag=false;
bool selected = false;
Point origin;

void onMouse(int event,int x,int y,int flags, void* userdata)
{
    //Point origin;//不能在这个地方进行定义，因为这是基于消息响应的函数，执行完后origin就释放了，所以达不到效果。
    if(select_flag)
    {
        result.x=MIN(origin.x,x);//不一定要等鼠标弹起才计算矩形框，而应该在鼠标按下开始到弹起这段时间实时计算所选矩形框
        result.y=MIN(origin.y,y);
        result.width=abs(x-origin.x);//算矩形宽度和高度
        result.height=abs(y-origin.y);
        result&=Rect(0,0,frame.cols,frame.rows);//保证所选矩形框在视频显示区域之内
    }
    if(event==EVENT_LBUTTONDOWN)
    {
        select_flag=true;//鼠标按下的标志赋真值
        origin=Point(x,y);//保存下来单击是捕捉到的点
        result=Rect(x,y,0,0);//这里一定要初始化，宽和高为(0,0)是因为在opencv中Rect矩形框类内的点是包含左上角那个点的，但是不含右下角那个点
    }
    else if(event==EVENT_LBUTTONUP)
    {
        select_flag=false;
        selected = true;
    }
}

int main(int argc, char* argv[]){

	if (argc > 5) return -1;

	bool HOG = true;
	bool FIXEDWINDOW = false;
	bool MULTISCALE = true;
	bool SILENT = false;
	bool LAB = false;
	VideoCapture cam(0); //webcam

	for(int i = 0; i < argc; i++){
		if ( strcmp (argv[i], "hog") == 0 )
			HOG = true;
		if ( strcmp (argv[i], "fixed_window") == 0 )
			FIXEDWINDOW = true;
		if ( strcmp (argv[i], "singlescale") == 0 )
			MULTISCALE = false;
		if ( strcmp (argv[i], "show") == 0 )
			SILENT = false;
		if ( strcmp (argv[i], "lab") == 0 ){
			LAB = true;
			HOG = true;
		}
		if ( strcmp (argv[i], "gray") == 0 )
			HOG = false;
	}

	// Create KCFTracker object
	KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);



	//New window
	string window_name = "video | q or esc to quit";
	namedWindow(window_name, 1);

	//set Rect
	// setMouseCallback(window_name,onMouse,0);

	// Using min and max of X and Y for groundtruth rectangle
	// float xMin =  result.x;
	// float yMin =  result.y;
	// float width = result.width;
	// float height = result.height;

	// Write Results
	ofstream resultsFile;
	string resultsPath = "output.txt";
	resultsFile.open(resultsPath);

	// Frame counter
	int nFrames = 0;

	while ( 1 ){
		cam >> frame;

    float xMin =  result.x;
  	float yMin =  result.y;
  	float width = result.width;
  	float height = result.height;
    setMouseCallback(window_name,onMouse,0);

    if (selected){
      // First frame, give the groundtruth to the tracker
      if (nFrames == 0) {
        tracker.init( Rect(xMin, yMin, width, height), frame );
        // rectangle( frame, Point( xMin, yMin ), Point( xMin+width, yMin+height), Scalar( 0, 255, 255 ), 1, 8 );
        // resultsFile << xMin << "," << yMin << "," << width << "," << height << endl;
      }
    // 	// Update
      else{
    		result = tracker.update(frame);
        rectangle( frame, Point( result.x, result.y ), Point( result.x+result.width, result.y+result.height), Scalar( 0, 255, 255 ), 1, 8 );
    // 		resultsFile << result.x << "," << result.y << "," << result.width << "," << result.height << endl;
      }
      // printf("%s\n", "a");
      nFrames++;
    }else{
      rectangle( frame, Point( xMin, yMin ), Point( xMin+width, yMin+height), Scalar( 0, 255, 255 ), 1, 8 );
    }


		if (!SILENT){
			imshow(window_name, frame);
		}

		char key = (char)waitKey(30); //delay N millis, usually long enough to display and capture input

    switch (key) {
    case 'q':
    case 'Q':
    case 27: //escape key
        return 0;
		default:
        break;
    }
    // if(key == 27)
    //   break;

	}
	// resultsFile.close();

  return 0;

}
