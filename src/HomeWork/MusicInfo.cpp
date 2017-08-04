//MusicInfo class...��ȡ����
#include<fstream>
#include<set>
#include"MusicInfo.h"
#include"CharString.h"
#include"Sqstack.h"

#define SIGN_SIZE 25
using namespace std;

ostream& operator << (ostream& output, CharString& cs)
{
	for(int i = 0; i < cs.length;i++)
		output << cs.ch[i];
	output << endl;
	return output;
}

//��ҳ��������
void MusicInfo::extractMusicInfoFromPage(string pageContent)
{
	CharString charstring;
	charstring.StrAssign(pageContent);

	int song_info_area_index = charstring.indexOf("<div class=\"song_info_area\"");
	if(song_info_area_index == ERROR)
		return;
	int music_list_area_index = charstring.indexOf("<div class=\"music_list_area\"");
	if(music_list_area_index == ERROR)
		return;
	//��ȡ����Ӧ��λ��
	charstring.SubString(song_info_area_index, music_list_area_index - song_info_area_index);

	//for(int j = 0; j < charstring.length; j++)
		//cout << charstring.ch[0];

	//��������Ϣ
	Sqstack  html_tag;
	//��ǩ�����ݣ�ջ�������ݣ���ջ������
	CharString charStringTagContent, strPop, strTop;
	//������ǩ֮�������
	CharString  charStringContent;
	//�Ƿ����һ�α�ǩ֮�������
	bool charStringContentRead = false;                                  
	//�Ƿ��ڶ���ǩ������
	bool tag = false;    
	//�Ƿ�����Ҫ����Ϣ
	bool IsSingerfind = false, IsAlbumFind = false, IsPublishDateFind = false;
	//�Ƿ�ʼ����ʱ�־
	bool IsLyricReadFlag = false;    
	bool IsAddLine = false;    //һ�����Ƿ�ӹ����з�
	CharString CSstr;
	CSstr.StrAssign("����");   //�ӡ����𡱺�ʼ��
	
	string str("");
	string composerCopy(""), lyricsCopy("");
	for(int i = 0; i < charstring.length;i++)
	{
		if((!IsLyricReadFlag && charstring.ch[i] == '\n') || charstring.ch[i] == '\t')
			continue;

		//�����
		if(IsLyricReadFlag && charstring.ch[i] == '\n' && !IsAddLine)
		{
			str.push_back('\n');
			IsAddLine = true;
			continue;
		}

		if(charstring.ch[i] == '<' || tag)
		{
			if(charStringContentRead)
			{
				charStringContentRead = false;
				charStringContent.StrAssign(str);

				//������ǩ����������
				if(!tag)
				{
					html_tag.GetTop(strTop);

					//�ұ���
					if(!titleFind)
					{
					    if(strTop.findString("title"))
						{
							string titleCopy = "";
							int pos = strTop.indexOf("title");
							while(strTop.ch[pos] != '\"')
							{
								pos++;
							}

							pos++;
							while(strTop.ch[pos] != '\"')
						    {
								titleCopy.push_back(strTop.ch[pos]);
								pos++;
							}
					
							titleFind = true;
							title.StrAssign(titleCopy);
						}
					}
					
					//�Ҹ���
                    if(!singerFind)
					{
						if(IsSingerfind)
						{
							IsSingerfind = false;
							singer = charStringContent;
							singerFind = true;
						}	
						if(charStringContent.findString("����"))
						{
							IsSingerfind = true;
						}
					}

					//��ר��
                    if(!albumFind)
					{
						if(IsAlbumFind)
						{
				    		IsAlbumFind = false;
							album = charStringContent;
							albumFind = true;
						}	
						if(charStringContent.findString("ר��"))
						{
							IsAlbumFind  = true;
						}
					}

					//������
					if(!publishDateFind)
					{
						if(IsPublishDateFind)
						{
							IsPublishDateFind = false;
							publishDate = charStringContent;
							publishDateFind = true;
						}	
						if(charStringContent.findString("����ʱ��"))
						{
							IsPublishDateFind = true;
						}
					}

					
					//�Ҹ��, �ҵڶ�������, ��������, ��������
					if(!lyricsFind)
					{
						if(IsLyricReadFlag)
						{
							lyricsFind = true;
							IsLyricReadFlag = false;
							lyricsCopy = str;
						}

						if(!lyricsFind && charStringContent == CSstr)
						{
							IsLyricReadFlag = true;
						}	
					}
				
					str = "";
		       }
			}

			tag = true;
			str.push_back(charstring.ch[i]);

			if(charstring.ch[i] == '>')
			{
				tag = false;
				charStringTagContent.StrAssign(str);
				if(!charStringTagContent.findString("img"))
				{
					if(charStringTagContent.findString("/"))
					{
						html_tag.GetTop(strTop);
						str = "";
						int Tpos = charStringTagContent.indexOf("/");
						Tpos++;
						while(charStringTagContent.ch[Tpos] != '>')
						{
							str += charStringTagContent.ch[Tpos];
							Tpos++;
						}

						if(strTop.findString(str))
						{
							html_tag.Pop(strPop);
						}
						else
							html_tag.Push(charStringTagContent);
					}
					else
						html_tag.Push(charStringTagContent);
				}

				str = "";
			}
		}

		else
		{
			str.push_back(charstring.ch[i]);
			charStringContentRead = true; 
			IsAddLine = false;
		}

		//������  ��ʴӡ�����һֱ������������
		if(lyricsFind)
		{
			string str1("");//����ÿһ����
			CharString str1CS;
			string realLyrics("");  //��ȡ���������

			int index = 0;
			while(lyricsCopy[index] != '\n')
				index++;
			while(index != lyricsCopy.length() - 1)
			{
				index++;
				//����ÿһ��
				while(lyricsCopy[index] != '\n')
				{
					str1 += lyricsCopy[index];
					index++;
				}
			
				str1CS.StrAssign(str1);
				//�Ҵ�����
				if(str1CS.findString("��:") || str1CS.findString("�ʣ�"))
				{
					//str1 += '\n';
					lyricist.StrAssign(str1);
					if(realLyrics.size() <= 80)
						realLyrics = "";
				}

				//��������
				else if(str1CS.findString("��:") || str1CS.findString("����"))
				{
					//str1 += '\n';
					if(!composerCopy.empty())
						composerCopy += '\n' + str1;
					else
						composerCopy += str1;
					if(realLyrics.size() <= 80)
						realLyrics = "";
				}

				//�������
				else
				{
					realLyrics += str1 + '\n';
				}
				str1 = "";
			}

			lyrics.StrAssign(realLyrics);
			composer.StrAssign(composerCopy);
			composerFind = true;
			lyricistFind = true;
		}

		if(titleFind && singerFind && albumFind && publishDateFind
			&& lyricistFind && composerFind && lyricsFind)
			break;
			
	}    
}

void MusicInfo::printInfoToFile(string fileName)
{
	ofstream output(fileName);
	if(output.fail())
	{
		cout << "open error output.."; 
		return;
	}

	if(titleFind)
		output << "����: " << title;
	if(singerFind)
	    output << "����: " << singer;
	if(albumFind)
		output << "ר��: " <<album;
	if(publishDateFind)
		output << "��������: " << publishDate;
	
	output << lyricist << composer;

	output << "��ʣ�" << endl << lyrics;

	output.close();
	cout << "�����ļ�����"<< endl;
}

//���ķִʲ���

//������Ӣ�ķ���
extern char EnglishWordDic[SIGN_SIZE] = {',', '.', ' ', '~', '<', '>', '(', ')', '"', '"', '&', '#', ';', '-', '!', '\'', '\\', '?', '`', 
	                                     ':', '/', '@', '%', '$', '*'};
extern string ChineseWordDic[SIGN_SIZE] = {"��", "��","��", "��", "��", "��", "��", "��", "��", "��", "&", "#", "��", "-", "��", "��", "��", "��", "��",
	                                 "��", "����", "��", "%", "--", "@"};

//���Ƿ�Ϊ��ĸ
bool Isalpha(char ch)
{
	for(char chr = 'a'; ; chr++)
	{
		if(ch == chr || ch == chr - 32)    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			return true;
		if(chr == 'z')
			break;
	}

	return false;
}

//���Ƿ�Ϊ����
bool Isdigit(char ch)
{
	int c = ch - '1' + 1;
	for(int i = 0; i < 10; i++)
	{
		if(c == i)
			return true;
	}
	return false;
}

//��һ�仰�еݹ��Ҵʲ��������,�����ҵ��ʰ����ֵĸ���
int divideWord(CharString& cStr, set<CharStringSet, Compare>& wordMap, CharStringLink& cStrLink)
{
	CharString aWord;             //����
	int i = 0;

	//û�д�
	if(cStr.length == 0)
	{
		return 0;
	}

	//һ����
	if(cStr.length == 2)
	{
		aWord = cStr;
		cStrLink.createLink(aWord);
		return 1;
	}

	//���
	else
	{
		set<CharStringSet>::iterator iter;
		CharStringSet cStrSet;
		cStrSet.cStr = cStr;
		cStrSet.length = cStr.length;
		
		for(int j = 0; j < cStr.length; j++)
		{
			if(j < 2)
			{
				if(j % 2 == 0)
					cStrSet.evenWordAsci += cStr.ch[j];
				 if(j == cStr.length / 2 || j == cStr.length / 2 + 1)
					cStrSet.middleWordAsci += cStr.ch[j];
				 if(j == cStr.length - 2 || j == cStr.length - 3)
					cStrSet.lastWordAsci += cStr.ch[j];
				cStrSet.oneWordAsci += cStr.ch[j];
				cStrSet.threeWordAsci += cStr.ch[j];
				cStrSet.fourWordAsci += cStr.ch[j];
				cStrSet.fiveWordAsci += cStr.ch[j];
				cStrSet.sixWordAsci += cStr.ch[j];
				cStrSet.sevenWordAsci += cStr.ch[j];
			}

			else if(j < 6)
			{
				if(j % 2 == 0)
					cStrSet.evenWordAsci += cStr.ch[j];
				 if(j == cStr.length / 2 || j == cStr.length / 2 + 1)
					cStrSet.middleWordAsci += cStr.ch[j];
				 if(j == cStr.length - 2 || j == cStr.length - 3)
					cStrSet.lastWordAsci += cStr.ch[j];
				cStrSet.threeWordAsci += cStr.ch[j];
				cStrSet.fourWordAsci += cStr.ch[j];
				cStrSet.fiveWordAsci += cStr.ch[j];
				cStrSet.sixWordAsci += cStr.ch[j];
				cStrSet.sevenWordAsci += cStr.ch[j];
			}

			else if(j < 8)
			{
				if(j % 2 == 0)
					cStrSet.evenWordAsci += cStr.ch[j];
				 if(j == cStr.length / 2 || j == cStr.length / 2 + 1)
					cStrSet.middleWordAsci += cStr.ch[j];
				 if(j == cStr.length - 2 || j == cStr.length - 3)
					cStrSet.lastWordAsci += cStr.ch[j];
				cStrSet.fourWordAsci += cStr.ch[j];
				cStrSet.fiveWordAsci += cStr.ch[j];
				cStrSet.sixWordAsci += cStr.ch[j];
				cStrSet.sevenWordAsci += cStr.ch[j];
			}

			else if(j < 10)
			{
				if(j % 2 == 0)
					cStrSet.evenWordAsci += cStr.ch[j];
				 if(j == cStr.length / 2 || j == cStr.length / 2 + 1)
					cStrSet.middleWordAsci += cStr.ch[j];
				 if(j == cStr.length - 2 || j == cStr.length - 3)
					cStrSet.lastWordAsci += cStr.ch[j];
				cStrSet.fiveWordAsci += cStr.ch[j];
				cStrSet.sixWordAsci += cStr.ch[j];
				cStrSet.sevenWordAsci += cStr.ch[j];
			}

			else if(j < 12)
			{
				if(j % 2 == 0)
					cStrSet.evenWordAsci += cStr.ch[j];
				 if(j == cStr.length / 2 || j == cStr.length / 2 + 1)
					cStrSet.middleWordAsci += cStr.ch[j];
				 if(j == cStr.length - 2 || j == cStr.length - 3)
					cStrSet.lastWordAsci += cStr.ch[j];
				cStrSet.sixWordAsci += cStr.ch[j];
				cStrSet.sevenWordAsci += cStr.ch[j];
			}

			else if(j < 14)
			{
				if(j % 2 == 0)
					cStrSet.evenWordAsci += cStr.ch[j];
				 if(j == cStr.length / 2 || j == cStr.length / 2 + 1)
					cStrSet.middleWordAsci += cStr.ch[j];
				 if(j == cStr.length - 2 || j == cStr.length - 3)
					cStrSet.lastWordAsci += cStr.ch[j];
				cStrSet.sevenWordAsci += cStr.ch[j];
			}
		}

		if((iter = wordMap.find(cStrSet)) != wordMap.end())
		{
			aWord = cStr;
			cStrLink.createLink(aWord);
			return (cStr.length / 2);
		}
		else
		{
			cStr.SubString(0, cStr.length - 2);
			int num = divideWord(cStr, wordMap, cStrLink);
			return num;
		}
	}
}

void MusicInfo::divideWords(CharString& cStr, set<CharStringSet, Compare>& wordMap, string fileName, 
						   CharStringLink& cStrLink)
{
	//�����ַ���
	string str("");
	int i = 0;
	bool findEngWord = false;

	//ȥ���ķ���
	for(int j = 0; j < SIGN_SIZE; j++)
	{
		cStr.deleteString(ChineseWordDic[j]);
	}

	for(i = 0; i < cStr.length;i++)
	{
		//����Ӣ���ַ�
		for(int j = 0; j < SIGN_SIZE; j++)
	    {
			if(cStr.ch[i] == EnglishWordDic[j])
			{
				findEngWord = true;
				break;
			}
	    }
		 if(findEngWord)
		 {
			 findEngWord = false;
			 continue;
		 }
		//Ӣ����ĸ
		else if(Isalpha(cStr.ch[i]))
			continue;
		//����
		else if(Isdigit(cStr.ch[i]))
			continue;
		else
		    str += cStr.ch[i];
	}

	cStr.StrAssign(str);

	str = "";
	i = 0;

	//�ִ�
	CharString aCent, aCentCopy;       //����
	int wordNum = 0;                  //�����ҵ��ʰ�������
	
	//����������
	CharString aCentCopyFind2;

	while(i < cStr.length)
	{
		while(cStr.ch[i] != '\n' && i != cStr.length)
		{
			str.push_back(cStr.ch[i]);
			i++;
		}

		if(!str.empty())
		{
			aCent.StrAssign(str);

			str = "";
		
			int hasWord = OK;
			while(hasWord)
			{
				//�ж�����
				int k = 0;
				if(Isdigit(aCent.ch[k]))
				{
					string strDigit("");
					CharString cStrDigit;
					while(Isdigit(aCent.ch[k]))
					{
						strDigit.push_back(aCent.ch[k]);
						k++;
					}
					cStrDigit.StrAssign(strDigit);
					cStrLink.createLink(cStrDigit);
					hasWord = aCent.SubString(k, aCent.length - k);
				}

				else
				{
					//ֻ��һ����
					if(aCent.length == 2)
					{
						cStrLink.createLink(aCent);
						hasWord = aCent.SubString(2, aCent.length -  2);
	
					}

					else
					{
							//����������
							string strFind2("");
							for(int i = 0; i < 4; i++)
							{
								strFind2.push_back(aCent.ch[i]);
							}
							aCentCopyFind2.StrAssign(strFind2);
							set<CharStringSet>::iterator iter;
						    CharStringSet cStrSet;
					        cStrSet.cStr = aCentCopyFind2;
						    cStrSet.length = aCentCopyFind2.length;
							
							for(int j = 0; j < aCentCopyFind2.length; j++)
							{
								if(j < 2)
								{
									if(j % 2 == 0)
					                     cStrSet.evenWordAsci += cStr.ch[j];
									if(j == cStr.length / 2 || j == cStr.length / 2 + 1)
										cStrSet.middleWordAsci += cStr.ch[j];
									if(j == cStr.length - 2 || j == cStr.length - 3)
										cStrSet.lastWordAsci += cStr.ch[j];
									cStrSet.oneWordAsci += cStr.ch[j];
									cStrSet.threeWordAsci += cStr.ch[j];
									cStrSet.fourWordAsci += cStr.ch[j];
									cStrSet.fiveWordAsci += cStr.ch[j];
									cStrSet.sixWordAsci += cStr.ch[j];
									cStrSet.sevenWordAsci += cStr.ch[j];
								}

								else if(j < 6)
								{
									if(j % 2 == 0)
										cStrSet.evenWordAsci += cStr.ch[j];
									if(j == cStr.length / 2 || j == cStr.length / 2 + 1)
										cStrSet.middleWordAsci += cStr.ch[j];
									if(j == cStr.length - 2 || j == cStr.length - 3)
										cStrSet.lastWordAsci += cStr.ch[j];
									cStrSet.threeWordAsci += cStr.ch[j];
									cStrSet.fourWordAsci += cStr.ch[j];
									cStrSet.fiveWordAsci += cStr.ch[j];
									cStrSet.sixWordAsci += cStr.ch[j];
									cStrSet.sevenWordAsci += cStr.ch[j];
								}

								else if(j < 8)
								{
									if(j % 2 == 0)
										cStrSet.evenWordAsci += cStr.ch[j];
									if(j == cStr.length / 2 || j == cStr.length / 2 + 1)
										cStrSet.middleWordAsci += cStr.ch[j];
									if(j == cStr.length - 2 || j == cStr.length - 3)
										cStrSet.lastWordAsci += cStr.ch[j];
									cStrSet.fourWordAsci += cStr.ch[j];
									cStrSet.fiveWordAsci += cStr.ch[j];
									cStrSet.sixWordAsci += cStr.ch[j];
									cStrSet.sevenWordAsci += cStr.ch[j];
								}

								else if(j < 10)
								{
									if(j % 2 == 0)
										cStrSet.evenWordAsci += cStr.ch[j];	
									if(j == cStr.length / 2 || j == cStr.length / 2 + 1)
										cStrSet.middleWordAsci += cStr.ch[j];
									if(j == cStr.length - 2 || j == cStr.length - 3)
										cStrSet.lastWordAsci += cStr.ch[j];
									cStrSet.fiveWordAsci += cStr.ch[j];
									cStrSet.sixWordAsci += cStr.ch[j];
									cStrSet.sevenWordAsci += cStr.ch[j];
								}

								else if(j < 12)
								{
									if(j % 2 == 0)
										cStrSet.evenWordAsci += cStr.ch[j];
									if(j == cStr.length / 2 || j == cStr.length / 2 + 1)
										cStrSet.middleWordAsci += cStr.ch[j];
									if(j == cStr.length - 2 || j == cStr.length - 3)
										cStrSet.lastWordAsci += cStr.ch[j];
									cStrSet.sixWordAsci += cStr.ch[j];
									cStrSet.sevenWordAsci += cStr.ch[j];
								}

								else if(j < 14)
								{
									if(j % 2 == 0)
										cStrSet.evenWordAsci += cStr.ch[j];
									if(j == cStr.length / 2 || j == cStr.length / 2 + 1)
										cStrSet.middleWordAsci += cStr.ch[j];
									if(j == cStr.length - 2 || j == cStr.length - 3)
										cStrSet.lastWordAsci += cStr.ch[j];
									cStrSet.sevenWordAsci += cStr.ch[j];
								}
							}
						
							if((iter = wordMap.find(cStrSet)) != wordMap.end())
							{
								cStrLink.createLink(aCentCopyFind2);
								hasWord = aCent.SubString(4, aCent.length - 4);
							}
							else
							{
								aCentCopy = aCent;
								wordNum = divideWord(aCentCopy, wordMap, cStrLink);
								hasWord = aCent.SubString(wordNum * 2, aCent.length - (wordNum * 2));
							}
					}
				}
			}
		}
		i++;
	}
	//��ӡ����,�����ڽ�B��ʱ����
	//cStrLink.printListToFile(fileName);
	//cStrLink.destroyLink();
}