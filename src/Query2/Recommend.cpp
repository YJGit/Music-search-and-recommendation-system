/*
*实现简单的音乐的推荐
*推荐规则：
*        标题中含同样的词汇最先(除去“的”这样的词汇，标题很好表达了歌的内容等等)
*        歌手，曲词作者，专辑都相同的第二
*        歌手，曲词作者相同的次于前面
*        仅歌手相同的最后(后面三者都是为了保证风格大致一样)
*        比较直到得到10首歌为止，多于10首的输出10首歌曲，少于10首的不补
*/

#include<iostream>
#include<fstream>
#include<vector>

#include"Recommend.h"
#include"FindWord.h"

using namespace std;

//用于输出的信息结构
struct recoMess
{
	int docNum;
	CharString title;
};

void Recommend(map<KeyType, MusicInfo>& Bmap, set<CharStringSet, Compare>& wordMap)
{
	ifstream in("query2.txt");
	ofstream ou("result2.txt");

	if(in.fail())
	{
		cout << "error opening query2.." << endl;
		return;
	}

	if(ou.fail())
	{
		cout  << "error opening results2.."<<endl;
		return;
	}

	while(!in.eof())
	{
		bool HaveFound = false;             //记录是否找到相关音乐
		//一行行处理
		string str("");
		getline(in, str);

		//制作key
		KeyType key;
		key.len = str.length();
		key.asci = new int[key.len];
		for(int k = 0; k < key.len; k++)
			key.asci[k] = str[k];

		//推荐
		vector<recoMess> ve;     //记录找到的歌曲
		map<KeyType, MusicInfo>::iterator iter = Bmap.find(key);
		if(iter != Bmap.end())
		{
			ve.clear();
			//是否已经在ve里了
			bool haveIn = false;
			//找到记录信息
			CharString title = iter ->second.title;
			CharString singer = iter->second.singer;
			CharString composer = iter->second.composer;
			CharString lyricist = iter ->second.lyricist;
			CharString abulm = iter ->second.album;

			bool compareTime[4] = {false, false, false, false};     //记录比较次数，共三次
			while(ve.size() < 10)
			{
				if(compareTime[2])
				{
					compareTime[3] = true;
					compareTime[2] = false;
				}

				if(compareTime[1])
				{
					compareTime[2] = true;
					compareTime[1] = false;
				}

				if(compareTime[0])
				{
					compareTime[1] = true;
					compareTime[0] = false;
				}

				if(!compareTime[0] && !compareTime[1] && !compareTime[2] && !compareTime[3])
					compareTime[0] = true;

				//第一次比较
				if(compareTime[0])
				{
					//利用歌词的分词函数对歌名进行分词
					CharStringLink cStrLink;
					MusicInfo musi;
					musi.lyrics = title;
					string file = "1.txt";
					musi.divideWords(title,wordMap, file, cStrLink);

					//查找
					LinkNode* p = cStrLink.head;
					while(p != NULL)
					{
						//去掉一些无意义的词，即利用这些词查出的结果一般无意义
						CharString cstrLinkstr;
						cstrLinkstr.StrAssign("的");
						if(p ->cStr == cstrLinkstr)
						{
							p = p->next;
						}
						if(p == NULL)
							break;

						for(map<KeyType, MusicInfo>::iterator iter0 = Bmap.begin(); iter0 != Bmap.end(); iter0++)
						{
							if(iter0 ->second.title == title)
								continue;
							if(findWord(iter0->second.title,p ->cStr))
							{
								recoMess reco;
								reco.docNum = iter0->second.docNum;
								reco.title = iter0->second.title;
								ve.push_back(reco);
								HaveFound = true;
								if(ve.size() >= 10)
									break;
							}
						}

						p = p ->next;
					}
				}

				//第二次比较
				else if(compareTime[1])
				{
					for(map<KeyType, MusicInfo>::iterator iter1 = Bmap.begin(); iter1 != Bmap.end(); iter1++)
					{
						if(iter1 ->second.title == title)
							continue;
						if(iter1 ->second.singer == singer && iter1 ->second.album == abulm 
							&& iter1 ->second.composer == composer && iter1 ->second.lyricist == lyricist)
						{
							//ve中是否存在
							for(int ve2 = 0; ve2 < ve.size(); ve2++)
							{
								if(ve[ve2].docNum == iter1->second.docNum)
								{
									haveIn = true;
									break;
								}
							}
							if(haveIn)
							{
								haveIn = false;
								continue;
							}

							recoMess reco;
							reco.docNum = iter1->second.docNum;
							reco.title = iter1->second.title;
							ve.push_back(reco);
							HaveFound = true;
							if(ve.size() >= 10)
								break;
						}
					}
				}

				//第三次比较
				else if(compareTime[2])
				{
					for(map<KeyType, MusicInfo>::iterator iter2 = Bmap.begin(); iter2 != Bmap.end(); iter2++)
					{
						if(iter2 ->second.title == title)
							continue;
						if(iter2 ->second.singer == singer && iter2 ->second.composer == composer && iter2->second.lyricist == lyricist)
						{
							//ve中是否存在
							for(int ve1 = 0; ve1 < ve.size(); ve1++)
							{
								if(ve[ve1].docNum == iter2->second.docNum)
								{
									haveIn = true;
									break;
								}
							}
							if(haveIn)
							{
								haveIn = false;
								continue;
							}

							else
							{
								recoMess reco;
								reco.docNum = iter2->second.docNum;
								reco.title = iter2->second.title;
								ve.push_back(reco);
								HaveFound = true;
								if(ve.size() >= 10)
									break;
							}
						}
					}
				}

				//第四次比较
				else if(compareTime[3])
				{
					for(map<KeyType, MusicInfo>::iterator iter3 = Bmap.begin(); iter3 != Bmap.end(); iter3++)
					{
						if(iter3 ->second.title == title)
							continue;
						if(iter3 ->second.singer == singer)
						{
							//ve中是否存在
							for(int ve2 = 0; ve2 < ve.size(); ve2++)
							{
								if(ve[ve2].docNum == iter3->second.docNum)
								{
									haveIn = true;
									break;
								}
							}
							if(haveIn)
							{
								haveIn = false;
								continue;
							}

							recoMess reco;
							reco.docNum = iter3->second.docNum;
							reco.title = iter3->second.title;
							ve.push_back(reco);
							HaveFound = true;
							if(ve.size() >= 10)
								break;
						}
					}
				}

				if(compareTime[3])
					break;
		    }

			//没找到
			if(!HaveFound)
				ou << "数据库中没有找到类似歌\n";
			else
			{
				for(int veI = 0; veI < ve.size(); veI++)
				{
					ou << "("<<ve[veI].docNum << ",";
					//防止输出‘\n'
					for(int i = 0; i < ve[veI].title.length;i++)
					{
						ou << ve[veI].title.ch[i];
					}
					ou << ")";
				}
				ou << '\n';
			}
		}
		else
			ou << "数据库中没有找到类似歌\n";
	}

	ou.close();
	in.close();
}