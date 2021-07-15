#pragma once

#include <string>
#include <vector>

using namespace std;

class TermUI{
    protected:
    struct task{
        wstring name;
        wstring desc;
        int currentStep;
        int stepCount;
    };



    struct logMsg {
        //日志等级:5-error, 4-warn,3-info,2-debug,1-verbose
        int lvl;
        //日志内容
        wstring message;
    };
    
    vector<task> tasks;
    vector<logMsg> logMsgs;

    wstring msg;

    void tuiUpdateLoop();
    int curTaskIdx;


    public:
    void begin();
    void addTask(wstring name);
    void updateLastTaskDesc(wstring desc);
    void updateLastTaskProgress(int cur,int all);
    void updateMsg(wstring _msg){ msg = _msg;};
    void logV(wstring _msg);
    void logD(wstring _msg);
    void logI(wstring _msg);
    void logW(wstring _msg);
    void logE(wstring _msg);
};