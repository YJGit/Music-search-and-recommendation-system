#ifndef FINDWORD_H
#define FINDWORD_H

#include<iostream>
#include"CharString.h"

using namespace std;

//从cs1中找cs2，找到返回true（两个changstring类型）
extern bool findWord(CharString& cs1, CharString& cs2);

#endif // FINDWORD_H
