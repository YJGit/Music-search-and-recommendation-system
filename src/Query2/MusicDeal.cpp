/*
*对音乐信息的进一步处理
*一个是对歌名的处理，一个是将音乐信息对应到文件名
*因为实验二利用实验一的dll，故这里的一些函数相当于实验一的补丁
*/

#include<iostream>
#include"MusicDeal.h"

using namespace std;

void dealMusicTitle(MusicInfo& musicInfo)
{
	//处理歌名
	string str("");
	int index = 0, k = 0;
	if(musicInfo.title.findString("（"))
	{
		index = musicInfo.title.indexOf("（");
		while(k != index)
		{
			str += musicInfo.title.ch[k];
			k++;
		}
	}
	else
	{
		while(musicInfo.title.ch[k] != '(' && k < musicInfo.title.length)
		{
			str += musicInfo.title.ch[k];
			k++;
		}
	}
	musicInfo.title.StrAssign(str);
}

void addMusicDoc(MusicInfo& musicInfo, string fileName)
{
	fileName = fileName.substr(1);
	musicInfo.docNum = atoi(fileName.c_str());
}