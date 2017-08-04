#ifndef RECOMMEND_H
#define RECOMMEND_H

#include<iostream>
#include<map>
#include"MusicInfo.h"
#include"InvertedDocument.h"

using namespace std;

//ÍÆ¼öÒôÀÖµÄº¯Êı
extern void Recommend(map<KeyType, MusicInfo>& Bmap, set<CharStringSet, Compare>& wordMap);

#endif