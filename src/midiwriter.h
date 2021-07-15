#pragma once
#include <string>
#include "notedetector.h"

using namespace std;

class MidiWriter{
private:
    /* data */
public:
    MidiWriter(/* args */);
    ~MidiWriter();
    void toMidiFile(vector<noteData> data, string fileName);
};
