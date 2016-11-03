//opencv libraries
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

//C++ standard libraries
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>

#define gif false

using namespace cv;
using namespace std;

void send_jpg(Mat &poster, int &image_cnt);
int main()
{
    VideoCapture cap(0);

    // if not successful, exit program
    if (!cap.isOpened())
    {
        cout << "Cannot open the video cam" << endl;
        return -1;
    }

    // read a new frame from video
    Mat frame;
    bool bSuccess0 = cap.read(frame);

    //if not successful, break loop
    if (!bSuccess0)
    {
        cout << "Cannot read a frame from video stream" << endl;
    }

    Mat poster = imread("wanted.jpg", IMREAD_COLOR);
    Mat poster_disp = Mat::ones(800, 1280, CV_8UC1);
    Mat tmp;
    resize(poster, poster, Size(640,800));
    cvtColor(poster,poster, CV_BGR2GRAY);
    poster.copyTo(poster_disp.rowRange(0, 0 + poster.rows).colRange(320, 320 + poster.cols));

    //show the frame in "MyVideo" window
    namedWindow("WANTED", WINDOW_NORMAL);
    int x = 210;
    int y = 75;
    int image_cnt = 52;
    int tmp_image_cnt = 0;
    char filename[128];
    char cmd[128];
    bool spacedown = false;

    while (1)
    {
        // read a new frame from video
        bool bSuccess = cap.read(frame);
        resize(frame, frame, Size(500,450));
        cvtColor(frame, frame, CV_BGR2GRAY);

        //if not successful, break loop
        if (!bSuccess)
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }

        frame.copyTo(poster.rowRange(x, x + frame.rows).colRange(y, y + frame.cols));
        frame.copyTo(poster_disp.rowRange(x, x + frame.rows).colRange(y + 320, y + 320+ frame.cols));
        imshow("WANTED", poster_disp);

        int k = waitKey(30);
        if (k == 27)
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }
        else if (k == 32 && gif) {
          spacedown = true;
          sprintf(filename, "tmp%i.jpg", tmp_image_cnt++);
          cout << "Writing " << filename << endl;
          resize(poster, tmp, Size(448,560));
          imwrite(filename, tmp);
          usleep(500000);
        }
        else if (spacedown && gif) {
          spacedown = false;
          cout << "count: " << tmp_image_cnt << endl;
          sprintf(cmd, "ruby make_gif.rb %i %i", tmp_image_cnt, image_cnt);
          system(cmd);

          sprintf(filename, "WANTED%i.gif", image_cnt++);
          sprintf(cmd, "ruby send.rb %s", filename);
          system(cmd);
          tmp_image_cnt = 0;
        }
        else if (!gif && k == 32) {
          send_jpg(poster, image_cnt);
        }

    }
    cap.release();
    return 0;
}

void send_jpg(Mat &poster, int &image_cnt) {
  char filename[128];
  char cmd[128];

  sprintf(filename, "WANTED%i.jpg", image_cnt++);
  imwrite(filename, poster);

  sprintf(cmd, "ruby send.rb %s 2> /dev/null", filename);
  system(cmd);
}
