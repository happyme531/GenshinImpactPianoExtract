#include "keyposfinder.h"
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <sstream>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include "utils.h"


void KeyPosFinder::begin(string filePath, int houghCirclesThreshold_){
    houghCirclesThreshold = houghCirclesThreshold_;
    ui.addTask(L"检测按键位置");
    VideoCapture video(filePath);
    if(!video.isOpened()){
        throw runtime_error("无法打开视频!");
    }

    double fps = video.get(CAP_PROP_FPS);
    int videoHeight = video.get(CAP_PROP_FRAME_HEIGHT);
    int videoWidth = video.get(CAP_PROP_FRAME_WIDTH);

    wstringstream strBuf;
    strBuf << L"视频高度:" << videoHeight << L", 宽度:" << videoWidth << L", fps:" << fps;
    ui.logI(strBuf.str());
    strBuf.clear();

    Mat frame;
    Mat resizedFrame;
    if (logLevel == 5) namedWindow("frames");
    bool detectedGUIOpen = false;
    //y缩放到480, x等比例
    auto targetVideoHeight = 480;
    auto targetVideoWidth = videoWidth * targetVideoHeight / videoHeight;
    // #pragma omp parallel
    while (minFramesCnt != vaildFramesCnt){
        //针对某些视频一开始根本没有打开界面的问题，在没有检测到界面时每隔5帧读取一帧，节省时间
        if(!video.read(frame)) break;
        if(!detectedGUIOpen) video.set(CAP_PROP_POS_FRAMES, video.get(CAP_PROP_POS_FRAMES) + 4); // 跳过4帧

        resize(frame,resizedFrame,Size(targetVideoWidth,targetVideoHeight));
        resizedFrame = resizedFrame(Rect(Point(0,targetVideoHeight/2),Point(targetVideoWidth-1,targetVideoHeight-1))); //切去上半部分, 保留下半部分
        cvtColor(resizedFrame,resizedFrame,COLOR_BGR2GRAY);
        //equalizeHist(resizedFrame,resizedFrame);
        auto blurredFrame = Mat();
        GaussianBlur(resizedFrame, blurredFrame, Size(31,31), 2);
        //pyrMeanShiftFiltering(resizedFrame,resizedFrame,15*scaleFactor,100*scaleFactor);
        //medianBlur(resizedFrame,resizedFrame,3);
        //Canny(resizedFrame,resizedFrame,80,255);
        vector<Vec3f> circles;
        HoughCircles(blurredFrame,circles, HOUGH_GRADIENT,2,50,200,houghCirclesThreshold,0,60);
        ui.updateMsg(L"找到的按键数量:" + to_wstring(circles.size()));
        if (!detectedGUIOpen && circles.size() > 10 ){
            detectedGUIOpen = true;
            startFrame = video.get(CAP_PROP_POS_FRAMES);
        }   

        if (logLevel == 5) {
          for (auto c : circles)
            circle(blurredFrame, Point(c[0], c[1]), c[2], Scalar(0, 255, 255), 3);
          imshow("frames", blurredFrame);
          waitKey(1);   //必须加上,否则预览窗口灰屏卡住
        }
        //如果有21个圆，保存起来
        if(circles.size() == 21){
            vaildCircles.push_back(circles);
            vaildFramesCnt++;
        }
        ui.updateLastTaskProgress(vaildFramesCnt,minFramesCnt);
    }
    //数据处理

    vector<double> Xs;
    vector<double> Ys;
    vector<double> radiuss;
    for (size_t i = 0; i < minFramesCnt; i++)
    {
        for (size_t j = 0; j < 21; j++){
            Xs.push_back(vaildCircles[i][j][0]);
        }
         for (size_t j = 0; j < 21; j++){
            Ys.push_back(vaildCircles[i][j][1]);
        }
         for (size_t j = 0; j < 21; j++){
            radiuss.push_back(vaildCircles[i][j][2]);
        }
    }
    sort(Xs.begin(), Xs.end());
    sort(Ys.begin(), Ys.end());
    sort(radiuss.begin(), radiuss.end());

    array<double,7> resXs;
    array<double,3> resYs;
    double resRadius;

    //分别取中位数
    //7列
    int groupSize = Xs.size() / 7;
    int j = 0;
    for (size_t i = groupSize / 2; i < Xs.size(); i += groupSize) {
      resXs[j] = Xs[i];
      j++;
    }
    //3行
    groupSize = Ys.size() / 3;
    j = 0;
    for (size_t i = groupSize / 2; i < Ys.size(); i += groupSize) {
      resYs[j] = Ys[i];
      j++;
    }
    
    resRadius = radiuss[radiuss.size() / 2];

    //构造位置矩阵
    for (size_t i = 0; i < 7; i++)
    {
        for (j = 0; j < 3; j++){
            result[i+j*7] = Vec3f(resXs[i],resYs[j],resRadius);
        }
    }
    //完成

}