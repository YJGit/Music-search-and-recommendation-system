#ifndef MUSICDEAL_H
#define MUSICDEAL_H

#include<iostream>
#include<string>
#include"MusicInfo.h"

using namespace std;

//������
extern void dealMusicTitle(MusicInfo& musicInfo);
//Ϊ������Ϣ����ļ���
extern void addMusicDoc(MusicInfo& musicInfo, string fileName);

#endif