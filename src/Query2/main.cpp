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

//��ȡ�ļ�����
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


//����ʵ�
void initDictionaryInfo(string fileName, set<CharStringSet, Compare>& wordMap)
{
	string str("");
	ifstream input(fileName, ios::in);
	if(input.fail())
	{
		cout << "error open dic file" << endl;
		return;
	}

	//��set
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

//��ȡ·���µ��ļ�
void getAllFiles( string path, vector<string>& files)  
{  
     //�ļ����  
     long   hFile   =   0;  
     //�ļ���Ϣ  
     struct _finddata_t fileinfo;  //�����õ��ļ���Ϣ��ȡ�ṹ
     string p;  //string�������˼��һ����ֵ����:assign()���кܶ����ذ汾
     if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)  
     {  
         do  
         {   
             if((fileinfo.attrib &  _A_SUBDIR))  //�Ƚ��ļ������Ƿ����ļ���
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
         }while(_findnext(hFile, &fileinfo)  == 0);  //Ѱ����һ�����ɹ�����0������-1
         _findclose(hFile); 
    } 
}  


int main(int argc, char* argv[])
{
	cout << "Waiting..." << endl;

	string dicPath = "..\\src\\�ʿ�.dic";
	argv[2] = "pages_300";
	argv[3] = "outPut";

	set<CharStringSet, Compare> wordMap;
	initDictionaryInfo(dicPath, wordMap);

	//�������·��
	vector<string> files;
	getAllFiles(argv[2], files);
	string outPath(argv[3]);

	int size = files.size();
	
	//map�洢�ʵ�
	map<KeyType, MusicInfo> Bmap;

	for(int i = 0; i < size; i++)
	{
		outPath = argv[3];
		string pageContent = readWholeHtmlContent(files[i]);
		MusicInfo musicInfo;
		//�����ļ�
		musicInfo.extractMusicInfoFromPage(pageContent);
		//�������
		dealMusicTitle(musicInfo);

		//��ӡinfo
		string filename = files[i].substr(files[i].find_last_of('\\'));
		filename = filename.substr(0, filename.find(".html"));                               //�Ĺ�
		string infoPath = outPath + filename + ".info";
		//musicInfo.printInfoToFile(infoPath);
		cout << "���ڶ����ָ�ʽ��зִ�" << endl;
		//���ķִ�
		string txtPath = outPath + filename + ".txt";
		CharStringLink cStrLink;
		musicInfo.divideWords(musicInfo.lyrics, wordMap, txtPath, cStrLink);
		cStrLink.destroyLink();

		//����������Ϣ�ʵ�
		addMusicDoc(musicInfo, filename);                //Ϊ������Ϣ����ļ���
		KeyType key;                          //��info�ĸ�������key,�������ʵ�
		key.len = musicInfo.title.length;
		key.asci = new int[key.len];
		for(int k = 0; k < key.len; k++)
			key.asci[k] = musicInfo.title.ch[k];
		Bmap[key] = musicInfo;
	}
	
	//�Ƽ�
	cout << "���ڽ����Ƽ�"<< endl;
	Recommend(Bmap, wordMap);

	cout << "Accomplish!" << endl;
	system("pause");
	return 0;
}
