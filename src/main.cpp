#include <iostream>
#include <omp.h>
#include <argparse/argparse.hpp>

#ifdef WIN32
#include <Windows.h>
#include <commctrl.h>
#endif

#include "termui.h"
#include "utils.h"
#include "keyposfinder.h"
#include "notedetector.h"
#include "midiwriter.h"
#include "buildconfig.h"

int logLevel = DEFAULT_LOG_LEVEL;

int main(int argc, char* argv[]) {
  // int argc =3; 
  // const char* argv[] = {R"(D:\Documents\GenshinImpactPianoExtract\build\Debug\GenshinImpactPianoExtract.exe)", R"123(c:\Users\user\Desktop\[WPF]JJDown\Download\原神 弹琴的正确姿势！超难《千 本樱》纯手弹 烛子演奏 - 1.成片-2021-3-30 214421(Av417358900,P1).mp4)123","--verbose"};
  using namespace std;
  using namespace argparse;
#ifdef WIN32
  //辣鸡Windows
  SetConsoleOutputCP(65001);
#endif
  //ui.logD(StringToWString(getBuildInformation()));
  ArgumentParser parser("GenshinImpactPianoExtract",versionStr);
  parser.add_argument("-d", "--debug")
      .help("设置日志等级为Debug")
      .default_value(false)
      .implicit_value(true);
  parser.add_argument("-v", "--verbose")
      .help("设置日志等级为Verbose, 开启输出预览")
      .default_value(false)
      .implicit_value(true);
  parser.add_argument("-o", "--output")
      .help("输出文件路径")
      .default_value(string("output.mid")); //存储const char*的std::any被any_cast到std::string时会抛异常
  parser.add_argument("input")
      .required()
      .help("输入文件路径");
  //get a parameter which represent the threshold of the note finder
  parser.add_argument("-t", "--note-threshold")
      .help("设置音符检测的阈值, 如果输出的文件中有大量不存在的音符则增大这个值, 反之减少")
      .default_value(DEFAULT_KEY_THRESHOLD);

  parser.add_epilog(R"123(
提示: 在Windows系统中, 可以直接把待转换的视频拖动到此exe文件上.

建议使用1080p/720p,30帧的视频. 480p/360p,60帧的视频可能会导致音符缺失.
由于使用了较为特殊的midi文件格式, 使用某些播放器时播放速度可能会异常的略微变慢.
如果想在原神或者其它游戏里演奏提取到的乐谱, 可以看一下这个项目中的"楚留香音乐盒"部分: https://github.com/happyme531/clxTools

这是一个开源项目, 如果你觉得这个项目对你有帮助, 请给一个star: https://github.com/happyme531/GenshinImpactPianoExtract
作者: 楚留香(一梦江湖)::声声慢::心慕流霞::李芒果
ps:我不玩原神)123");

  try {
    parser.parse_args(argc, argv);
  } catch (exception &e) {
    cout << parser.help().str() << endl;
    fflush(stdin);
    getchar();
    return 1;
  }
  if(parser["-v"] == true){
    logLevel = 5;
  }else if(parser["-d"] == true){
    logLevel = 4;
  }
  TermUI ui;
  ui.begin();


  //Windows - 拖放获取文件路径
  //TODO
  #ifdef WIN32
  //auto window = GetConsoleWindow();
  //Linux - 文件选择
  //TODO
  //parser.parse_args(1, (char**)&filePath);
  //ui.logD("filePath: " + StringToWString(filePath));
  //end of Windows - 文件选择
  // setwindow
  #endif

  omp_set_num_threads(omp_get_num_procs());

  string filepath = parser.get<string>("input");
  KeyPosFinder finder(ui); 
  finder.begin(filepath);
  NoteDetector detector(ui);
  detector.begin(filepath,finder.getResult(),parser.get<int>("--note-threshold"));
  MidiWriter writer;

  //let the output file to be the input file except the extension changed to mid 
  string output = parser.get<string>("input");
  string output_path = output.substr(0,output.find_last_of("."));
  output_path += ".mid";

  writer.toMidiFile(detector.getResult(),output_path);
  try {
    ui.logI(L"转换完成,文件已输出至" + StringToWString(output_path) + L"请按任意键退出");
  } catch (...) {
    ui.logI(L"转换完成,请按任意键退出");
  }

  // pause the program until the user press any key
  fflush(stdin);
  getchar();

  return 0;
}
