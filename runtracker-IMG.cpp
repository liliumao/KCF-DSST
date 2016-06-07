#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "kcftracker.hpp"

#include <dirent.h>
#include <time.h>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]){

	if (argc > 5) return -1;

	bool HOG = true;
	bool FIXEDWINDOW = false;
	bool MULTISCALE = true;
	bool SILENT = false;
	bool LAB = false;

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

	// Frame readed
	Mat frame;

	// Tracker results
	Rect result;

	string base = "Test/CarDark/";

	// Path to list.txt
	ifstream listFile;
	string fileName = base + "images.txt";
  	listFile.open(fileName);

  	// Read groundtruth for the 1st frame
  	ifstream groundtruthFile;
	string groundtruth = base + "init.txt";
  	groundtruthFile.open(groundtruth);
  	string firstLine;
  	getline(groundtruthFile, firstLine);
	groundtruthFile.close();

  	istringstream ss(firstLine);
		printf("%s\n", groundtruth.c_str());

  	// Read groundtruth like a dumb
  	// float x1, y1, x2, y2;

	float xMin;
	float yMin;
	float width;
	float height;
	ss >> xMin;
	ss >> yMin;
	ss >> width;
	ss >> height;

	// Using min and max of X and Y for groundtruth rectangle


	// printf("%f:%f:%f:%f\n", width, height, xMin, yMin);
	// Read Images
	ifstream listFramesFile;
	string listFrames = base + "images.txt";
	listFramesFile.open(listFrames);
	string frameName;


	// Write Results
	// ofstream resultsFile;
	// string resultsPath = "output.txt";
	// resultsFile.open(resultsPath);

	// Frame counter
	int nFrames = 0;

	clock_t start,end; // typedef long clock_t
	start = clock();

	while ( getline(listFramesFile, frameName) ){
		frameName = frameName;

		// Read each frame from the list
		frame = imread(base + frameName, CV_LOAD_IMAGE_COLOR);

		// First frame, give the groundtruth to the tracker
		if (nFrames == 0) {
			tracker.init( Rect(xMin, yMin, width, height), frame );
			rectangle( frame, Point( xMin, yMin ), Point( xMin+width, yMin+height), Scalar( 0, 255, 255 ), 1, 8 );
			// resultsFile << xMin << "," << yMin << "," << width << "," << height << endl;
		}
		// Update
		else{
			result = tracker.update(frame);
			rectangle( frame, Point( result.x, result.y ), Point( result.x+result.width, result.y+result.height), Scalar( 0, 255, 255 ), 1, 8 );
			// resultsFile << result.x << "," << result.y << "," << result.width << "," << result.height << endl;
		}

		nFrames++;

		if (!SILENT){
			imshow("Image", frame);
			waitKey(1);
		}
	}

	end = clock();

	double duration =(double)(end-start)/CLOCKS_PER_SEC;
  printf("%f\n",nFrames/duration);

	// resultsFile.close();

	listFile.close();

}
