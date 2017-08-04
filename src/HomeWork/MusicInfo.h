#ifndef MUSICINFO_H
#define MUSICINFO_H

#include<iostream>
#include<string>
#include<set>
#include"CharString.h"
#include"Compare.h"

using namespace std;

class MusicInfo
{
public:
	CharString title;        //��������
	CharString singer;       //��������
	CharString album;       //ר����
	CharString publishDate; //����ʱ��
	CharString lyricist;    //����
	CharString composer;    //����
	CharString lyrics;      //���
	int docNum;             //��¼�ļ���

	bool titleFind;
	bool singerFind;
	bool albumFind;
	bool publishDateFind;
	bool lyricistFind;
	bool composerFind;
	bool lyricsFind;

public:
	MusicInfo()
	{
		titleFind = false;
		singerFind = false;
		albumFind = false;
		publishDateFind =false;
		lyricistFind = false;
		composerFind = false;
		lyricsFind = false;
	}

	~MusicInfo(){};
	void extractMusicInfoFromPage(string pageContent);     //������ҳ����ȡ�����Ϣ           
	void printInfoToFile(string fileName);         //��ӡ���ļ���info
	void divideWords(CharString& cStr, set<CharStringSet, Compare>& wordMap, string fileName, 
		           CharStringLink& cStrLink);      //���ķִ�,����cStr�� wordMapΪ�ֵ䣬 fileNameΪ���·�����洢��cStrLink��
};

extern ostream& operator << (ostream& output, CharString& cs);

#endif
