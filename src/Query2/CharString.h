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
	CharString(const CharString& cStr);                        //���

	~CharString();

	Status StrAssign(string str);                      //����str������
	int indexOf(string str);                           //��λstr
	bool findString(string s);                             
	Status deleteString(string s);                 //ɾ���ַ���

	int StrLength();                                   //���س���
	Status Concat(CharString s1, CharString s2);       //����s1, s2�����ݲ�����״̬
	Status SubString(int pos, int len);               //�ضϵõ�s�Ĵ�pos��ĳ�len���ַ�     
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

	~CharStringLink(){};

	void createLink(CharString& cstr);                               //�����ﴴ����Ϊ���
	void destroyLink();                                              //����
	void printListToFile(string fileName);                           //��ӡ
	Status seekLink(CharString cstr);                                //����
	void  deleteLink(CharString cstr);                               //ɾ��
};


//class set ��set�������
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

#endif