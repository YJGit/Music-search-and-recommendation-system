#ifndef CHARSTRING_H
#define CHARSTRING_H

#include<iostream>
#include<string>
#include"Const.h"

using namespace std;

//CharString class
class CharString
{
public:
    char* ch;
    int length;

public:
    CharString();
    CharString(const CharString& cStr);                        //深拷贝

    ~CharString();

    sta StrAssign(string str);                      //复制str的内容
    int indexOf(string str);                           //定位str
    bool findString(string s);
    sta deleteString(string s);                 //删掉字符串

    int StrLength();                                   //返回长度
    sta Concat(CharString s1, CharString s2);       //连接s1, s2的内容并返回状态
    sta SubString(int pos, int len);               //截断得到s的从pos起的长len的字符
    string toString();                             //转成string
    CharString& operator =(const CharString& cs);
    bool  operator ==(const CharString& cs);
};


//CharStringLink class
class LinkNode
{
public:
    CharString cStr;
    LinkNode* next;
};


class CharStringLink
{
public:
    LinkNode* head;
    LinkNode* current;
    int LinkLen;

public:
    CharStringLink()
    {
        head = NULL;
        current = NULL;
        LinkLen = 0;
    };

    void createLink(CharString& cstr);                               //在这里创建即为添加
    void destroyLink();                                              //销毁
    void printListToFile(string fileName);                           //打印
    sta seekLink(CharString cstr);                                //查找
    void  deleteLink(CharString cstr);                               //删除
};


//class set 存set里的数据
class CharStringSet
{
public:
    CharString cStr;
    int length;
    int oneWordAsci;
    int evenWordAsci;
    int threeWordAsci;
    int fourWordAsci;
    int fiveWordAsci;
    int sixWordAsci;
    int sevenWordAsci;
    int middleWordAsci;
    int lastWordAsci;

public:
    CharStringSet()
    {
        length = 0;
        oneWordAsci = 0;
        evenWordAsci = 0;
        threeWordAsci = 0;
        fourWordAsci = 0;
        fiveWordAsci = 0;
        sixWordAsci = 0;
        sevenWordAsci = 0;
        middleWordAsci = 0;
        lastWordAsci = 0;
    }

    bool operator <(const CharStringSet& cs)const;
};

#endif // CHARSTRING_H
