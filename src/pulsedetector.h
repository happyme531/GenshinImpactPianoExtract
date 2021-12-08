#include <deque>
#include <algorithm>
#include <numeric>

template<int BUFFER_SIZE>
class PulseDetector{
private:
    //触发状态, 防止连续触发
    bool triggered = false;
    bool pulse = false;

    float threshold = 0.0f;
    float last_sample = 0;

public:
    PulseDetector(float threshold_){
        threshold = threshold_;
    }
    ~PulseDetector(){};
    void addSample(float sample){
        float delta = -(sample - last_sample);
        last_sample = sample;
        if(delta > threshold){
            if(!triggered){
                triggered = true;
            }
            return;
        }
        //复位
        if(delta < 0){
            if(triggered){
                triggered = false;
                pulse = true;
            }
            return;
        }

    }

    bool havePulse(){
        if(pulse){
            pulse = false;
            return true;
        }
        return false;
    }
    void reset(){
    }
};