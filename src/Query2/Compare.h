//set ±È½Ïº¯Êý

#ifndef COMPARE_H
#define COMPARE_H

#include<iostream>
#include"CharString.h"

using namespace std;

class Compare
{
public:
	bool operator()(const CharStringSet& cs1, const CharStringSet& cs2)const{
		if(cs1.length < cs2.length)
			return true;
		else  if(cs1.length == cs2.length)
		{
			if(cs1.oneWordAsci < cs2.oneWordAsci)
				return true;
			else if(cs1.oneWordAsci == cs2.oneWordAsci)
			{
				if(cs1.evenWordAsci < cs2.evenWordAsci)
					return true;
				else if(cs1.evenWordAsci == cs2.evenWordAsci)
				{
					if(cs1.threeWordAsci < cs2.threeWordAsci)
						return true;
					else if(cs1.threeWordAsci == cs2.threeWordAsci)
					{
						if(cs1.fourWordAsci < cs2.fourWordAsci)
							return true;
						else if(cs1.fourWordAsci == cs2.fourWordAsci)
						{
							if(cs1.fiveWordAsci < cs2.fiveWordAsci)
								return true;
							else if(cs1.fiveWordAsci == cs2.fiveWordAsci)
							{
								if(cs1.sixWordAsci < cs2.sixWordAsci)
									return true;
								else if(cs1.sixWordAsci == cs2.sixWordAsci)
								{
									if(cs1.sevenWordAsci < cs2.sevenWordAsci)
										return true;
									else if(cs1.sevenWordAsci == cs2.sevenWordAsci)
									{
										if(cs1.middleWordAsci < cs2.middleWordAsci)
											return true;
										else if(cs1.middleWordAsci == cs2.middleWordAsci)
										{
											if(cs1.lastWordAsci < cs2.lastWordAsci)
												return true;
											else
												return false;
										}
										else
											return false;
									}
									else 
										return false;
								}
								else
									return false;
							}
							else
								return false;
						}
						else
							return false;
					}
					else
						return false;
				}
				else 
					return false;
			}
			else
				return false;
		}
		else
		   return false;
	}
};

#endif