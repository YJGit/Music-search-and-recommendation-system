/*
*��������Ϣ�Ľ�һ������
*һ���ǶԸ����Ĵ���һ���ǽ�������Ϣ��Ӧ���ļ���
*��Ϊʵ�������ʵ��һ��dll���������һЩ�����൱��ʵ��һ�Ĳ���
*/

#include<iostream>
#include"MusicDeal.h"

using namespace std;

void dealMusicTitle(MusicInfo& musicInfo)
{
	//�������
	string str("");
	int index = 0, k = 0;
	if(musicInfo.title.findString("��"))
	{
		index = musicInfo.title.indexOf("��");
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