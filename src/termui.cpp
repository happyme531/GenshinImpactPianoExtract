#include "termui.h"

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cinttypes>

#include "buildconfig.h"

using namespace ftxui;

static auto to_text = [](int number) {
  return text(to_wstring(number));
};

thread* tuiUpdateThread = nullptr;

static uint64_t tuiSpinnerIdx = 0;

void TermUI::tuiUpdateLoop(){
     auto render = [&](){
            vector<Element> taskEntries;
            for(auto& i: tasks){
              bool taskCompleted = i.currentStep == i.stepCount;
              taskEntries.push_back(
                  hbox({
                      text(i.name) | bold | size(WIDTH, EQUAL, 12),
                      separator(),
                      to_text(i.currentStep) | size(WIDTH, EQUAL, 4),
                      text(L"/"),
                      to_text(i.stepCount) | size(WIDTH, EQUAL, 4),
                      separator(),
                      taskCompleted ? spinner(2,1): spinner(2, tuiSpinnerIdx),
                      i.currentStep == 0? spinner(16,tuiSpinnerIdx): gauge(i.currentStep / float(i.stepCount)),
                  }) |
                  color(taskCompleted ? Color::Green : Color::White));
            }
            vector<Element> logs;
            Element lastlog;
            for (size_t i = 0; i < logMsgs.size(); i++) {
              auto& msg = logMsgs[i];
              Color col;
              switch (msg.lvl) {
              case 5:
                col = Color::Red;
                break;
              case 4:
                col = Color::Orange1;
                break;
              case 3:
                col = Color::Green;
                break;
              case 2:
                col = Color::Blue;
              case 1:
                col = Color::Cyan;
              };
              if (i != logMsgs.size() - 2) {
                logs.push_back(hbox(text(msg.message)) | flex| color(col));
              } else {
                logs.push_back(focus(hbox(text(msg.message)) | color(col)));
              }
            }

            return window(
                text(wstring(L"  原神乐谱导出 v") + LversionStr + L" ") | center,
                vbox({window(text(L" 任务 "), vbox(std::move(taskEntries))),
                      window(text(L" 消息 "), text(msg)),
                      window(text(L" 日志 "), yframe(vbox(std::move(logs))))}));
        };
       std::string reset_position;
    while (1){
       auto content = render();

       auto screen = Screen::Create(Dimension::Full(), Dimension::Full());
       Render(screen, content);
       // screen.Print();
       std::cout << reset_position << screen.ToString() << std::flush;
       reset_position = screen.ResetPosition();
       tuiSpinnerIdx++;
       this_thread::sleep_for(chrono::milliseconds(100));
    }
}


void TermUI::begin(){
    tuiUpdateThread = new thread(&TermUI::tuiUpdateLoop,this);
}

void TermUI::addTask(wstring name){
  curTaskIdx = tasks.size();
  task tmp = {.name = name, .desc = L"", .currentStep = 0, .stepCount = 1};
  tasks.push_back(tmp);
}

void TermUI::updateLastTaskProgress(int cur,int all){
    task tmp = tasks[curTaskIdx];
    tmp.currentStep = cur;
    tmp.stepCount = all;
    tasks[curTaskIdx] = tmp;
}

void TermUI::logV(wstring _msg){
  logMsg tmp = {
    .lvl = 1,
    .message = _msg
  };
  logMsgs.push_back(tmp);
}

void TermUI::logD(wstring _msg){
  logMsg tmp = {
    .lvl = 2,
    .message = _msg
  };
  logMsgs.push_back(tmp);
}

void TermUI::logI(wstring _msg){
  logMsg tmp = {
    .lvl = 3,
    .message = _msg
  };
  logMsgs.push_back(tmp);
}
void TermUI::logW(wstring _msg){
  logMsg tmp = {
    .lvl = 4,
    .message = _msg
  };
  logMsgs.push_back(tmp);
}
void TermUI::logE(wstring _msg){
  logMsg tmp = {
    .lvl = 5,
    .message = _msg
  };
  logMsgs.push_back(tmp);
}
