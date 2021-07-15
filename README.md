# 原神乐谱导出 ![](https://img.shields.io/badge/Author-%E6%9D%8E%E8%8A%92%E6%9E%9C-yellow.svg) ![](https://img.shields.io/badge/PRs-welcome-blue.svg)

[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/0-percent-optimized.svg)](https://forthebadge.com)

从原神弹琴视频中自动扒谱!  
支持Windows, MacOS, Linux (MacOS和Linux用户需要自己编译)

## 使用方法

下载最新Release, 解压.  
把要转换的视频直接拖到可执行文件上即可.  
转换得到的midi文件会输出到和输入文件相同的目录下.

## 编译方法

### 环境配置

Windows:

手动安装msvc, cmake和opencv库.

Linux:

```bash
sudo apt install cmake build-essential libopencv-dev #如果你的系统不是基于debian的Linux, 请自行搜索安装命令
```

### 编译

```bash
git clone --recursive https://github.com/happyme531/GenshinImpactPianoExtract.git #--recursive是必要的, 否则编译会报错
cd GenshinImpactPianoExtract
mkdir build
cd build
cmake ..
cmake --build .
```
