#include<iostream>
#include<fstream>
#include<map>
#include"SeekWord.h"
#include"MusicInfo.h"
#include"CharString.h"

using namespace std;

void seekWord(BTree Btree, map<KeyType, INVDIC*> Bmap, set<CharStringSet, Compare>& wordMap)
{
	cout << "正在查找..."<<endl;
	//关键词查询
	ifstream in("query1.txt");
	ofstream out("result1.txt");
	if(in.fail())
	{
		cout << "error open quety1.txt..." << endl;
		return;
	}
	if(out.fail())
	{
		cout << "error open result1.txt..." << endl;
		return;
	}

	map<int, int> docOut;    //存结果

	while(!in.eof())
	{
		string str("");
		//处理每一行
		getline(in, str);
	
		cout <<"正在将输入分词"<<endl;
		//分词
		MusicInfo mus;
		CharStringLink ctrLink;
		CharString cstr;
		cstr.StrAssign(str);
		mus.lyrics = cstr;
		string file = "";
		mus.divideWords(cstr, wordMap, file, ctrLink);

			
		//处理一个单词
		LinkNode* pLink = ctrLink.head;
		while(pLink != NULL)
		{
			//找关键词
			KeyType key;
			key.len = pLink ->cStr.length;
			key.asci = new int[key.len];
			for(int j = 0; j < key.len; j++)
				key.asci[j] = pLink->cStr.ch[j];

			BTree pBt = NULL;
			//没找到
			if(BT_search(Btree, key, pBt) == 0)
				continue;
			else
			{
				INVDOC *p = Bmap[key]->head;
				while(p != NULL)
				{
					//没找到
					if(docOut.find(p ->DocID) == docOut.end())
					{
						docOut[p ->DocID] = p ->times;
					}
					else
					{
						docOut[p->DocID] += p->times;
					}
					p = p->next;
				}
			}
			pLink = pLink ->next;
		}

		//打印
		if(docOut.empty())
			out << "在数据库中没有找到这些词...\n";
		else
		{
			for(map<int, int>::iterator iter = docOut.begin(); iter != docOut.end(); iter++)
			{
		
				out << "{" << iter ->first << "," << iter ->second << "}" << " ";
			}

			out << '\n';
			docOut.clear();
		}
	}
}