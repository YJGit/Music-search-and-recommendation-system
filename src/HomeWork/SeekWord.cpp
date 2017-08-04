#include<iostream>
#include<fstream>
#include<map>
#include"SeekWord.h"
#include"MusicInfo.h"
#include"CharString.h"

using namespace std;

void seekWord(BTree Btree, map<KeyType, INVDIC*> Bmap, set<CharStringSet, Compare>& wordMap)
{
	cout << "���ڲ���..."<<endl;
	//�ؼ��ʲ�ѯ
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

	map<int, int> docOut;    //����

	while(!in.eof())
	{
		string str("");
		//����ÿһ��
		getline(in, str);
	
		cout <<"���ڽ�����ִ�"<<endl;
		//�ִ�
		MusicInfo mus;
		CharStringLink ctrLink;
		CharString cstr;
		cstr.StrAssign(str);
		mus.lyrics = cstr;
		string file = "";
		mus.divideWords(cstr, wordMap, file, ctrLink);

			
		//����һ������
		LinkNode* pLink = ctrLink.head;
		while(pLink != NULL)
		{
			//�ҹؼ���
			KeyType key;
			key.len = pLink ->cStr.length;
			key.asci = new int[key.len];
			for(int j = 0; j < key.len; j++)
				key.asci[j] = pLink->cStr.ch[j];

			BTree pBt = NULL;
			//û�ҵ�
			if(BT_search(Btree, key, pBt) == 0)
				continue;
			else
			{
				INVDOC *p = Bmap[key]->head;
				while(p != NULL)
				{
					//û�ҵ�
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

		//��ӡ
		if(docOut.empty())
			out << "�����ݿ���û���ҵ���Щ��...\n";
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