#ifndef SEEKWORD_H
#define SEEKWORD_H

#include<iostream>
#include<map>
#include<set>
#include"Compare.h"
#include"CharString.h"
#include"InvertedDocument.h"

extern void seekWord(BTree Btree, map<KeyType, INVDIC*> Bmap, set<CharStringSet, Compare>& wordMap);

#endif