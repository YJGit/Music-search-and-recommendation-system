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
    CharString title;        //音乐名称
    CharString singer;       //歌手名称
    CharString album;       //专辑名
    CharString publishDate; //发行时间
    CharString lyricist;    //作词
    CharString composer;    //作曲
    CharString lyrics;      //歌词

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

    void extractMusicInfoFromPage(string pageContent);     //解析网页并提取相关信息
    void divideWords(CharString& cStr, set<CharStringSet, Compare>& wordMap, string fileName,
                   CharStringLink& cStrLink);      //中文分词,对象cStr， wordMap为字典， fileName为输出路径，存储在cStrLink中
    void dealMusicTitle();                         //处理音乐标题，去掉（）等符号
};

extern ostream& operator << (ostream& output, CharString& cs);
extern bool Isalpha(char ch);

#endif // MUSICINFO_H
