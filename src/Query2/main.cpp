#include<iostream>
#include<string>
#include<fstream>
#include<set>
#include<io.h>
#include<vector>
#include<map>
#include"Const.h"
#include"MusicInfo.h"
#include"CharString.h"
#include"Compare.h"
#include"InvertedDocument.h"
#include"MusicDeal.h"
#include"Recommend.h"
#include"FindWord.h"

using namespace std;
#pragma comment(lib, "Query1.lib")

//读取文件内容
string readWholeHtmlContent(string fileName)
{
	string str("");
	string buffer("");
	ifstream input(fileName, ios::in);
	if(input.fail())
	{
		cout << "error open html file" << endl;
		return str;
	}
	while(!input.eof())
	{
		getline(input, buffer);
		if(!buffer.empty())
			str = str + buffer + '\n';
	}
	input.close();
	return str;
}


//读入词典
void initDictionaryInfo(string fileName, set<CharStringSet, Compare>& wordMap)
{
	string str("");
	ifstream input(fileName, ios::in);
	if(input.fail())
	{
		cout << "error open dic file" << endl;
		return;
	}

	//建set
	while(!input.eof())
	{
		getline(input, str);
		CharString  cStr;
		CharStringSet cStrSet;
		cStr.StrAssign(str);
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
				if(j == cStr.length - 3 || j == cStr.length - 2)
					cStrSet.lastWordAsci += cStr.ch[j];
				cStrSet.sevenWordAsci += cStr.ch[j];
			}
		}

		wordMap.insert(cStrSet);
	}

	input.close();
}

//读取路径下的文件
void getAllFiles( string path, vector<string>& files)  
{  
     //文件句柄  
     long   hFile   =   0;  
     //文件信息  
     struct _finddata_t fileinfo;  //很少用的文件信息读取结构
     string p;  //string类很有意思的一个赋值函数:assign()，有很多重载版本
     if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)  
     {  
         do  
         {   
             if((fileinfo.attrib &  _A_SUBDIR))  //比较文件类型是否是文件夹
             {  
                 if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)  
                 {
                     files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
                     getAllFiles( p.assign(path).append("\\").append(fileinfo.name), files ); 
                 }
             }  
             else  
             {  
                 files.push_back(p.assign(path).append("\\").append(fileinfo.name) );  
             }  
         }while(_findnext(hFile, &fileinfo)  == 0);  //寻找下一个，成功返回0，否则-1
         _findclose(hFile); 
    } 
}  


int main(int argc, char* argv[])
{
	cout << "Waiting..." << endl;

	string dicPath = "..\\src\\词库.dic";
	argv[2] = "pages_300";
	argv[3] = "outPut";

	set<CharStringSet, Compare> wordMap;
	initDictionaryInfo(dicPath, wordMap);

	//读入测试路径
	vector<string> files;
	getAllFiles(argv[2], files);
	string outPath(argv[3]);

	int size = files.size();
	
	//map存储词典
	map<KeyType, MusicInfo> Bmap;

	for(int i = 0; i < size; i++)
	{
		outPath = argv[3];
		string pageContent = readWholeHtmlContent(files[i]);
		MusicInfo musicInfo;
		//解析文件
		musicInfo.extractMusicInfoFromPage(pageContent);
		//处理歌名
		dealMusicTitle(musicInfo);

		//打印info
		string filename = files[i].substr(files[i].find_last_of('\\'));
		filename = filename.substr(0, filename.find(".html"));                               //改过
		string infoPath = outPath + filename + ".info";
		//musicInfo.printInfoToFile(infoPath);
		cout << "正在对音乐歌词进行分词" << endl;
		//中文分词
		string txtPath = outPath + filename + ".txt";
		CharStringLink cStrLink;
		musicInfo.divideWords(musicInfo.lyrics, wordMap, txtPath, cStrLink);
		cStrLink.destroyLink();

		//制作音乐信息词典
		addMusicDoc(musicInfo, filename);                //为音乐信息添加文件名
		KeyType key;                          //以info的歌名制作key,并制作词典
		key.len = musicInfo.title.length;
		key.asci = new int[key.len];
		for(int k = 0; k < key.len; k++)
			key.asci[k] = musicInfo.title.ch[k];
		Bmap[key] = musicInfo;
	}
	
	//推荐
	cout << "正在进行推荐"<< endl;
	Recommend(Bmap, wordMap);

	cout << "Accomplish!" << endl;
	system("pause");
	return 0;
}
