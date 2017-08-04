#include<iostream>
#include"FindWord.h"

using namespace std;

//cs1÷–≤È’“cs2
bool findWord(CharString& cs1, CharString& cs2)
{
	int i = 0, j = 0;
	for(i = 0; i < cs1.length; i += 2)
	{
		if(cs1.length - i < cs2.length)
			return false;
		int index = i;
		for(j = 0; j < cs2.length; j++)
		{
			if(cs1.ch[index] == cs2.ch[j])
			{
				index++;
			}
			else
			{
				index = 0;
				break;
			}
		}

		if(j == cs2.length)
			return true;
	}
}