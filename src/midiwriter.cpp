#include "midiwriter.h"
#include <MidiFile.h>

using namespace smf;

MidiWriter::MidiWriter(){};
MidiWriter::~MidiWriter(){};

/**
 * @brief Convert a key to midi pitch
 * 
 */
static int keyToPitch(int key){
    int octave = 6 - key / 7; //左上角是第0个按键
    key++;
    int pitch = key % 7;
    int midiPitch;
    //["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"];
    switch (pitch) {
    case 1:
      midiPitch = 0;
      break;
    case 2:
      midiPitch = 2;
      break;
    case 3:
      midiPitch = 4;
      break;
    case 4:
      midiPitch = 5;
      break;
    case 5:
      midiPitch = 7;
      break;
    case 6:
      midiPitch = 9;
      break;
    case 0:
      midiPitch = 11;
      break;
    default:
      break;
    }
    return midiPitch + (octave * 12) ;
}
void MidiWriter::toMidiFile(vector<noteData> data, string fileName){
    MidiFile midiFile;
    midiFile.addTimbre(0,0,0,0);
    midiFile.setTPQ(120);
    midiFile.absoluteTicks();
    midiFile.addTempo(0, 0, 60);
    for (unsigned int i = 0; i < data.size(); i++) {
      if(data[i].beginTime < 1) continue; //跳过无效的音符
      // int startTick = data[i].beginTime * 1000;
      // int endTick = startTick + 200;
      int startTick = data[i].beginTime * 120;
      int endTick = startTick + 30;
      midiFile.addNoteOn(0, startTick, 0, keyToPitch(data[i].key), 100);
      midiFile.addNoteOff(0, endTick, 0, keyToPitch(data[i].key));
    }
    midiFile.sortTracks();
    midiFile.write(fileName);
    return;
}
