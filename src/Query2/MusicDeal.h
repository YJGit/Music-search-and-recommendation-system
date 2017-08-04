#ifndef MUSICDEAL_H
#define MUSICDEAL_H

#include<iostream>
#include<string>
#include"MusicInfo.h"

using namespace std;

//处理歌词
extern void dealMusicTitle(MusicInfo& musicInfo);
//为音乐信息添加文件名
extern void addMusicDoc(MusicInfo& musicInfo, string fileName);

#endif