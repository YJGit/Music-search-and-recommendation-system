#ifndef SQSTACK_H
#define SQSTACK_H

#include<iostream>
#include<string>
#include"CharString.h"
#include"Const.h"

using namespace std;

//Sqstack class
class Sqstack
{
public:
    CharString* base;
    CharString* top;
    int stacksize;

public:
    Sqstack();
    ~Sqstack();

    sta StackEmpty();
    int StackLength();
    sta GetTop(CharString& e);
    sta Push(CharString& e);
    sta Pop(CharString& e);
};

#endif // SQSTACK_H
