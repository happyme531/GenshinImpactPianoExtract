# 原神乐谱导出 ![](https://img.shields.io/badge/Author-%E6%9D%8E%E8%8A%92%E6%9E%9C-yellow.svg) ![](https://img.shields.io/badge/PRs-welcome-blue.svg)

[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/0-percent-optimized.svg)](https://forthebadge.com)

从原神弹琴视频中自动扒谱!  
支持Windows, MacOS, Linux (MacOS和Linux用户需要自己编译)

## 使用方法


从[这里](https://github.com/happyme531/GenshinImpactPianoExtract/releases/latest)下载最新Release(下面的.7z压缩包文件), 解压.  
(如果下载太慢或者无法下载, 可以右键获得链接，之后打开[这里](https://gh.api.99988866.xyz/)，将链接粘贴进去即可高速下载)
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
