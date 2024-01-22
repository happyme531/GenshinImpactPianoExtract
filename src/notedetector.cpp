#include "notedetector.h"
#include "pulsedetector.h"

#include <opencv2/opencv.hpp>
#include "utils.h"
#include <queue>
#include <thread>
#include <mutex>

static queue<Mat*> frameQueue;

static thread* decoderThread;
static mutex frameQueueMutex;

static bool videoEnded = false;

static void decoderLoop(VideoCapture& video) {
  while (true) {
    while (frameQueue.size() < 10) {
        Mat* frame = new Mat();
        if (!video.read(*frame)) {
          videoEnded = true;
          delete frame;
          return;
        }
        //std::queue 线程不安全
        frameQueueMutex.lock();
        frameQueue.push(frame);
        frameQueueMutex.unlock();
    }
    this_thread::sleep_for(chrono::milliseconds(10));
  }
}

void NoteDetector::begin(string filePath, array<Vec3f, 21> keyPos, int threshold, int startFrame) {
    ui.addTask(L"获取按键数据");
    VideoCapture video(filePath);
    if (!video.isOpened()) {
      throw runtime_error("无法打开视频!");
    }
    double fps = video.get(CAP_PROP_FPS);
    double frameTime = 1.0/fps;
    int videoHeight = video.get(CAP_PROP_FRAME_HEIGHT);
    int videoWidth = video.get(CAP_PROP_FRAME_WIDTH);
    video.set(CAP_PROP_POS_FRAMES, startFrame);
    int curFrame = startFrame;
    //Mat frame;
    Mat resizedFrame;
    if (logLevel == 5)
      namedWindow("frames");
    lastColors.fill(255);
    pressedKeys.fill(false);
    decoderThread = new thread(decoderLoop, ref(video));
    decoderThread->detach();

    vector<PulseDetector<10>*> detectors;
    for (int i = 0; i < 21; i++) {
      detectors.push_back(new PulseDetector<10>(threshold));
    }

    while (!videoEnded){    //处理到视频结尾
        if(video.get(CAP_PROP_POS_FRAMES) <= 2) pressedKeys.fill(false);
        while (frameQueue.size() == 0) {
          this_thread::sleep_for(chrono::milliseconds(5));
          if (videoEnded) break;
        }
        if (videoEnded) break;
        
        frameQueueMutex.lock();
        Mat& frame = *frameQueue.front(); 
        frameQueue.pop();
        frameQueueMutex.unlock();

        // y缩放到480, x等比例 ,和获取位置时一样
        auto targetVideoHeight = 480;
        auto targetVideoWidth = videoWidth * targetVideoHeight / videoHeight;
        resize(frame,resizedFrame,Size(targetVideoWidth,targetVideoHeight));
        resizedFrame = resizedFrame(Rect(Point(0,targetVideoHeight/2),Point(targetVideoWidth-1,targetVideoHeight-1))); //切去上半部分, 保留下半部分
        cvtColor(resizedFrame,resizedFrame,COLOR_BGR2GRAY); //只关心灰度
        
        //分别获取21个按钮区域的平均颜色
        for (int i = 0; i < 21; i++){
            Mat1b mask(resizedFrame.rows,resizedFrame.cols,uchar(0));
            circle(mask,Point(keyPos[i][0],keyPos[i][1]),keyPos[i][2] * 0.95,Scalar(255),FILLED);    // 缩小一些检测范围
            circle(mask,Point(keyPos[i][0],keyPos[i][1]),keyPos[i][2] * 0.75,Scalar(0),FILLED);    // 缩小一些检测范围

            double avgColor = mean(resizedFrame,mask)[0];

            //检测按下
            detectors[i]->addSample(avgColor);
            if(detectors[i]->havePulse()){
              //记录音符
              noteData note = {
                  .key = i,
                  .beginTime = frameTime * curFrame - frameTime * startFrame};
              pressedKeys[i] = true;
              result.push_back(note);
            }else{
                pressedKeys[i] = false;
            }

            if (logLevel == 5) putText(resizedFrame,to_string(avgColor),Point(keyPos[i][0],keyPos[i][1]),FONT_HERSHEY_SIMPLEX,0.25,Scalar(0,0,0));
            lastColors[i] = avgColor;
        }
        if (logLevel == 5) {
          for (int i = 0; i < 21; i++){
            auto c = keyPos[i];
            if (pressedKeys[i]){
              circle(resizedFrame, Point(c[0], c[1]), c[2] * 0.95, Scalar(0, 255, 255), 2);
              circle(resizedFrame, Point(c[0], c[1]), c[2] * 0.75, Scalar(0, 255, 255), 1);
            }
          }
            this_thread::sleep_for(chrono::milliseconds(20));

          imshow("frames", resizedFrame);
          waitKey(1);   //必须加上,否则预览窗口灰屏卡住
        }
        curFrame++;
        ui.updateLastTaskProgress(curFrame,video.get(CAP_PROP_FRAME_COUNT));
        ui.updateMsg(L"音符数量:" + to_wstring(result.size()));
        delete &frame;
    }
    video.release();
    delete decoderThread;
    for (int i = 0; i < 21; i++) {
      delete detectors[i];
    }
    ui.updateLastTaskProgress(curFrame,video.get(CAP_PROP_FRAME_COUNT));
    ui.updateMsg(L"音符数量:" + to_wstring(result.size()));
}