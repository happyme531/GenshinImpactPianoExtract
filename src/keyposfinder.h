#pragma once

#include <string>
#include <vector>
#include <opencv2/core/mat.hpp>
#include "termui.h"

using namespace std;
using namespace cv;

class KeyPosFinder {
private:
  int vaildFramesCnt = 0;
  int minFramesCnt = 20;  
  TermUI& ui;
  vector<vector<Vec3f>> vaildCircles; //3x7 = 21个按键
  array<Vec3f,21> result;
  
  //视频中界面被打开时的帧数
  int startFrame = 0;
  

public:

    void begin(string filePath);
    array<Vec3f,21>&& getResult(){
      return move(result);
    }
    int getStartFrame(){
      return startFrame;
    }
    KeyPosFinder(TermUI& _ui): ui(_ui){};
    ~KeyPosFinder(){};
};
