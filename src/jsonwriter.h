#pragma once
#include <string>
#include "notedetector.h"

using namespace std;

class JSONWriter{
private:
    /* data */
public:
    JSONWriter(/* args */);
    ~JSONWriter();
    void toToneJSON(vector<noteData> &data, string fileName);
};

