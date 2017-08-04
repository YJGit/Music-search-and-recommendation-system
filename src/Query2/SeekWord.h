#ifndef SEEKWORD_H
#define SEEKWORD_H

#include<iostream>
#include<map>
#include"CharString.h"
#include"InvertedDocument.h"

extern void seekWord(BTree Btree, map<KeyType, INVDIC*> Bmap);

#endif