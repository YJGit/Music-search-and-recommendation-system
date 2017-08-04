/*
 *初始化操作
 * 初始查寻和推荐用的词典，音乐信息的存储词典等等
*/

#ifndef INIT_H
#define INIT_H

#include<iostream>
#include<string>
#include<set>
#include<vector>
#include"compare.h"
#include"MusicInfo.h"
#include"BTreeAndDoc.h"

using namespace std;

//初始化词典
extern void initDictionaryInfo(string fileName, set<CharStringSet, Compare>& dicSet);
/*
 * 初始化音乐词典map<int, MusicInfo>和分词词典map<KeyType, INVDIC*>
 * 音乐词典map<int, MusicInfo>储存音乐信息，便于可视化显示
 * 分词词典map<KeyType, INVDIC*>存的是分词和文档的关系，通过B树进行索引
 */
extern void initMusicInfoMap(map<int, MusicInfo>& MusicInfoMap, map<KeyType, INVDIC*>& Bmap,
                             set<CharStringSet, Compare>& dicSet, BTree& Btree);

//存储最后打印结果
struct veOut
{
    int docId;
    int time;
};

//将vector按顺序排
extern void m_sort(vector<veOut>& ve);

#endif // INIT_H
