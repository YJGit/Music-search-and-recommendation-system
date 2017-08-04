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
#include"SeekWord.h"

using namespace std;

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
	
	//B树索引
	BTree Btree = NULL;
	//map存储词典
	map<KeyType, INVDIC*> Bmap;

	for(int i = 0; i < size; i++)
	{
		outPath = argv[3];
		string pageContent = readWholeHtmlContent(files[i]);
		MusicInfo musicInfo;
		//解析文件
		musicInfo.extractMusicInfoFromPage(pageContent);
		//打印info
		string filename = files[i].substr(files[i].find_last_of('\\'));
		filename = filename.substr(0, filename.find(".html"));                               //改过
		string infoPath = outPath + filename + ".info";
		//musicInfo.printInfoToFile(infoPath);
		cout << "正在对音乐歌词进行分词"<<endl;
		//中文分词
		string txtPath = outPath + filename + ".txt";
		CharStringLink cStrLink;
		musicInfo.divideWords(musicInfo.lyrics, wordMap, txtPath, cStrLink);
		createB_Tree(cStrLink , Btree, filename, Bmap);
		cStrLink.destroyLink();
	}

	//文档排序
	for(map<KeyType, INVDIC*>::iterator iter = Bmap.begin(); iter != Bmap.end(); iter++)
	{
		iter ->second ->sortDoc();
	}

	//查找关键词
	seekWord(Btree, Bmap, wordMap);
	cout << "Accomplish!" << endl;

	system("pause");
	return 0;
}
