//MusicInfo class...读取内容
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

//网页解析部分
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
	//截取到相应的位置
	charstring.SubString(song_info_area_index, music_list_area_index - song_info_area_index);

	//for(int j = 0; j < charstring.length; j++)
		//cout << charstring.ch[0];

	//解析出信息
	Sqstack  html_tag;
	//标签的内容，栈顶的内容，出栈的内容
	CharString charStringTagContent, strPop, strTop;
	//两个标签之间的内容
	CharString  charStringContent;
	//是否读完一次标签之间的内容
	bool charStringContentRead = false;                                  
	//是否在读标签的内容
	bool tag = false;    
	//是否是需要的信息
	bool IsSingerfind = false, IsAlbumFind = false, IsPublishDateFind = false;
	//是否开始读歌词标志
	bool IsLyricReadFlag = false;    
	bool IsAddLine = false;    //一句歌词是否加过换行符
	CharString CSstr;
	CSstr.StrAssign("收起");   //从“收起”后开始读
	
	string str("");
	string composerCopy(""), lyricsCopy("");
	for(int i = 0; i < charstring.length;i++)
	{
		if((!IsLyricReadFlag && charstring.ch[i] == '\n') || charstring.ch[i] == '\t')
			continue;

		//读歌词
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

				//解析标签及其后的内容
				if(!tag)
				{
					html_tag.GetTop(strTop);

					//找标题
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
					
					//找歌手
                    if(!singerFind)
					{
						if(IsSingerfind)
						{
							IsSingerfind = false;
							singer = charStringContent;
							singerFind = true;
						}	
						if(charStringContent.findString("歌手"))
						{
							IsSingerfind = true;
						}
					}

					//找专辑
                    if(!albumFind)
					{
						if(IsAlbumFind)
						{
				    		IsAlbumFind = false;
							album = charStringContent;
							albumFind = true;
						}	
						if(charStringContent.findString("专辑"))
						{
							IsAlbumFind  = true;
						}
					}

					//找日期
					if(!publishDateFind)
					{
						if(IsPublishDateFind)
						{
							IsPublishDateFind = false;
							publishDate = charStringContent;
							publishDateFind = true;
						}	
						if(charStringContent.findString("发行时间"))
						{
							IsPublishDateFind = true;
						}
					}

					
					//找歌词, 找第二作曲者, 找作曲者, 找做词者
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

		//处理歌词  歌词从“收起”一直读到中文字完
		if(lyricsFind)
		{
			string str1("");//处理每一句歌词
			CharString str1CS;
			string realLyrics("");  //截取的真正歌词

			int index = 0;
			while(lyricsCopy[index] != '\n')
				index++;
			while(index != lyricsCopy.length() - 1)
			{
				index++;
				//处理每一句
				while(lyricsCopy[index] != '\n')
				{
					str1 += lyricsCopy[index];
					index++;
				}
			
				str1CS.StrAssign(str1);
				//找词作者
				if(str1CS.findString("词:") || str1CS.findString("词："))
				{
					//str1 += '\n';
					lyricist.StrAssign(str1);
					if(realLyrics.size() <= 80)
						realLyrics = "";
				}

				//找曲作者
				else if(str1CS.findString("曲:") || str1CS.findString("曲："))
				{
					//str1 += '\n';
					if(!composerCopy.empty())
						composerCopy += '\n' + str1;
					else
						composerCopy += str1;
					if(realLyrics.size() <= 80)
						realLyrics = "";
				}

				//真正歌词
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
		output << "标题: " << title;
	if(singerFind)
	    output << "歌手: " << singer;
	if(albumFind)
		output << "专辑: " <<album;
	if(publishDateFind)
		output << "发行日期: " << publishDate;
	
	output << lyricist << composer;

	output << "歌词：" << endl << lyrics;

	output.close();
	cout << "解析文件结束"<< endl;
}

//中文分词部分

//常见的英文符号
extern char EnglishWordDic[SIGN_SIZE] = {',', '.', ' ', '~', '<', '>', '(', ')', '"', '"', '&', '#', ';', '-', '!', '\'', '\\', '?', '`', 
	                                     ':', '/', '@', '%', '$', '*'};
extern string ChineseWordDic[SIGN_SIZE] = {"～", "（","）", "《", "》", "“", "”", "，", "。", "、", "&", "#", "；", "-", "！", "‘", "’", "？", "、",
	                                 "：", "……", "￥", "%", "--", "@"};

//判是否为字母
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

//判是否为数字
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

//从一句话中递归找词并添进链表,返回找到词包含字的个数
int divideWord(CharString& cStr, set<CharStringSet, Compare>& wordMap, CharStringLink& cStrLink)
{
	CharString aWord;             //词语
	int i = 0;

	//没有词
	if(cStr.length == 0)
	{
		return 0;
	}

	//一个词
	if(cStr.length == 2)
	{
		aWord = cStr;
		cStrLink.createLink(aWord);
		return 1;
	}

	//多词
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
	//处理字符串
	string str("");
	int i = 0;
	bool findEngWord = false;

	//去中文符号
	for(int j = 0; j < SIGN_SIZE; j++)
	{
		cStr.deleteString(ChineseWordDic[j]);
	}

	for(i = 0; i < cStr.length;i++)
	{
		//处理英文字符
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
		//英文字母
		else if(Isalpha(cStr.ch[i]))
			continue;
		//数字
		else if(Isdigit(cStr.ch[i]))
			continue;
		else
		    str += cStr.ch[i];
	}

	cStr.StrAssign(str);

	str = "";
	i = 0;

	//分词
	CharString aCent, aCentCopy;       //句子
	int wordNum = 0;                  //储存找到词包含的字
	
	//先找两个词
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
				//判断数字
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
					//只有一个词
					if(aCent.length == 2)
					{
						cStrLink.createLink(aCent);
						hasWord = aCent.SubString(2, aCent.length -  2);
	
					}

					else
					{
							//先找两个词
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
	//打印链表,销毁在建B树时销毁
	//cStrLink.printListToFile(fileName);
	//cStrLink.destroyLink();
}