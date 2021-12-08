#pragma once

#include <string>
#include <vector>
#include <opencv2/core/mat.hpp>
#include "termui.h"
#include <fstream>
#include <sstream>

using namespace std;
using namespace cv;

struct noteData
{   
    //按键:左下角1,右下角7,右上角21
    int key;
    //按下时的时间(秒)
    double beginTime;
};

class NoteDetector {
private:
  TermUI& ui;
  array<bool,21> pressedKeys;
  array<double,21> lastColors;
  vector<noteData> result;

public:

    void begin(string filePath, array<Vec3f,21> keyPos, int threshold, int startFrame = 0);
    vector<noteData>&& getResult(){
      return move(result);
    }
    NoteDetector(TermUI& _ui): ui(_ui){};
    ~NoteDetector(){};

    //serialize the result array
    void serialize(string filePath){
      ofstream ofs(filePath);
      for(auto& note : result){
        ofs << note.key << " " << note.beginTime << endl;
      }
    }
    // deserialize the result array
    void deserialize(string filePath) {
      ifstream ifs(filePath);
      string line;
      while (getline(ifs, line)) {
        istringstream iss(line);
        int key;
        double beginTime;
        iss >> key >> beginTime;
        result.push_back(noteData{key, beginTime});
      }
    }
};
