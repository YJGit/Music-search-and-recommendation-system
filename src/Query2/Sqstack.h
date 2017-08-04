#ifndef SQATACK_H
#define SQATACK_H

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

	Status StackEmpty();
	int StackLength();
	Status GetTop(CharString& e);
	Status Push(CharString& e);
	Status Pop(CharString& e);
};

#endif