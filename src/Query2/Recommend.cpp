/*
*ʵ�ּ򵥵����ֵ��Ƽ�
*�Ƽ�����
*        �����к�ͬ���Ĵʻ�����(��ȥ���ġ������Ĵʻ㣬����ܺñ���˸�����ݵȵ�)
*        ���֣��������ߣ�ר������ͬ�ĵڶ�
*        ���֣�����������ͬ�Ĵ���ǰ��
*        ��������ͬ�����(�������߶���Ϊ�˱�֤������һ��)
*        �Ƚ�ֱ���õ�10�׸�Ϊֹ������10�׵����10�׸���������10�׵Ĳ���
*/

#include<iostream>
#include<fstream>
#include<vector>

#include"Recommend.h"
#include"FindWord.h"

using namespace std;

//�����������Ϣ�ṹ
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
		bool HaveFound = false;             //��¼�Ƿ��ҵ��������
		//һ���д���
		string str("");
		getline(in, str);

		//����key
		KeyType key;
		key.len = str.length();
		key.asci = new int[key.len];
		for(int k = 0; k < key.len; k++)
			key.asci[k] = str[k];

		//�Ƽ�
		vector<recoMess> ve;     //��¼�ҵ��ĸ���
		map<KeyType, MusicInfo>::iterator iter = Bmap.find(key);
		if(iter != Bmap.end())
		{
			ve.clear();
			//�Ƿ��Ѿ���ve����
			bool haveIn = false;
			//�ҵ���¼��Ϣ
			CharString title = iter ->second.title;
			CharString singer = iter->second.singer;
			CharString composer = iter->second.composer;
			CharString lyricist = iter ->second.lyricist;
			CharString abulm = iter ->second.album;

			bool compareTime[4] = {false, false, false, false};     //��¼�Ƚϴ�����������
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

				//��һ�αȽ�
				if(compareTime[0])
				{
					//���ø�ʵķִʺ����Ը������зִ�
					CharStringLink cStrLink;
					MusicInfo musi;
					musi.lyrics = title;
					string file = "1.txt";
					musi.divideWords(title,wordMap, file, cStrLink);

					//����
					LinkNode* p = cStrLink.head;
					while(p != NULL)
					{
						//ȥ��һЩ������Ĵʣ���������Щ�ʲ���Ľ��һ��������
						CharString cstrLinkstr;
						cstrLinkstr.StrAssign("��");
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

				//�ڶ��αȽ�
				else if(compareTime[1])
				{
					for(map<KeyType, MusicInfo>::iterator iter1 = Bmap.begin(); iter1 != Bmap.end(); iter1++)
					{
						if(iter1 ->second.title == title)
							continue;
						if(iter1 ->second.singer == singer && iter1 ->second.album == abulm 
							&& iter1 ->second.composer == composer && iter1 ->second.lyricist == lyricist)
						{
							//ve���Ƿ����
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

				//�����αȽ�
				else if(compareTime[2])
				{
					for(map<KeyType, MusicInfo>::iterator iter2 = Bmap.begin(); iter2 != Bmap.end(); iter2++)
					{
						if(iter2 ->second.title == title)
							continue;
						if(iter2 ->second.singer == singer && iter2 ->second.composer == composer && iter2->second.lyricist == lyricist)
						{
							//ve���Ƿ����
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

				//���ĴαȽ�
				else if(compareTime[3])
				{
					for(map<KeyType, MusicInfo>::iterator iter3 = Bmap.begin(); iter3 != Bmap.end(); iter3++)
					{
						if(iter3 ->second.title == title)
							continue;
						if(iter3 ->second.singer == singer)
						{
							//ve���Ƿ����
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

			//û�ҵ�
			if(!HaveFound)
				ou << "���ݿ���û���ҵ����Ƹ�\n";
			else
			{
				for(int veI = 0; veI < ve.size(); veI++)
				{
					ou << "("<<ve[veI].docNum << ",";
					//��ֹ�����\n'
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
			ou << "���ݿ���û���ҵ����Ƹ�\n";
	}

	ou.close();
	in.close();
}