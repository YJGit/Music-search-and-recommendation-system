#include"init.h"
#include<iostream>
#include<string>
#include<vector>
#include<io.h>
#include<QDebug>
#include<fstream>
#include<QFile>
#include<QString>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include"BTreeAndDoc.h"
#include"MusicInfo.h"
#include<QTextCodec>
#include<QFileDialog>
#include<QDir>

using namespace std;

/*
 * 读入词典
 * 将准备的词典读入到set<CharStringSet, Compare>& wordMap中
 */

void initDictionaryInfo(string fileName, set<CharStringSet, Compare>& dicSet)
{     
    string str("");

    QString finame = QString::fromStdString(fileName);
    QFile file(finame);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the file!"<<endl;
    }

    QTextStream txtInput(&file);
    QString lineStr;

    //建set
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
        str = lineStr.toStdString();
        QTextCodec *code = QTextCodec::codecForName("gb18030");
        if(code)
            str = code ->fromUnicode(lineStr).data();

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

        dicSet.insert(cStrSet);
    }
}

/*
 *  初始化音乐词典map<int, MusicInfo>
 * 首先读入到一个路径下所有的文件
 * 然后一个个读入html文件
 * 最后分词并生成记录音乐信息的词典
*/

//读取路径下的文件
void getAllFiles(string spath, vector<string>& files)
{
    QString qspath = QString::fromStdString(spath);
    QDir dir(qspath);
    //dir.setFilter(QDir::Files|QDir::Hidden|QDir::NoSymLinks);
    //dir.setSorting(QDir::Size|QDir::Reversed);  //这两个函数设置进入list的顺序和文件属性，这种设置下就应该是文件隐藏文件 不列出连接符
    QFileInfoList list=dir.entryInfoList();
    QString str;
    for(int i=0;i < list.count(); i++)
    {
        QFileInfo fileInfo = list.at(i);
        str = fileInfo.fileName();

        files.push_back(str.toStdString());
        //qDebug() << str;
    }
}

//读取一个html文件整个的内容
string readWholeHtmlContent(string fileName)
{
    string str("");
    string buffer("");
    QString fiName = QString::fromStdString(fileName);
    QFile input(fiName);

    if(!input.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the file!"<<endl;
        return str;
    }

    QTextStream txtInput(&input);
    QString lineStr;

    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
        buffer = lineStr.toStdString();
        QTextCodec *code = QTextCodec::codecForName("gb18030");
        if(code)
            buffer = code ->fromUnicode(lineStr).data();

        if(!buffer.empty())
            str = str + buffer + '\n';
    }
    return str;
}


/*
 * 初始化音乐词典map<int, MusicInfo>和分词词典map<KeyType, INVDIC*>
 * 音乐词典map<int, MusicInfo>储存音乐信息，便于可视化显示
 * 分词词典map<KeyType, INVDIC*>存的是分词和文档的关系，通过B树进行索引
 */

void initMusicInfoMap(map<int, MusicInfo>& MusicInfoMap, map<KeyType, INVDIC*>& Bmap,
                             set<CharStringSet, Compare>& dicSet, BTree& Btree)
{
    string arg2 = ":/html/pages_300";

    //读入测试路径
    vector<string> files;
    getAllFiles(arg2, files);
    //cout << files.size()<<endl;

    int size = files.size();

    for(int i = 0; i < size; i++)
    {
        string filePath = arg2 + '/' + files[i];
        //cout << filePath;
        //return;
        string pageContent = readWholeHtmlContent(filePath);
        MusicInfo musicInfo;
        //解析文件,获取信息
        musicInfo.extractMusicInfoFromPage(pageContent);
        //处理一下歌名
        musicInfo.dealMusicTitle();

        //加入音乐信息词典
        MusicInfoMap[i] = musicInfo;

        //中文分词
        CharStringLink cStrLink;
        musicInfo.divideWords(musicInfo.lyrics, dicSet, "", cStrLink); //""本来为文件名，用于打印分词结果，此处不用，故置空
        /*
        LinkNode *p = cStrLink.head;
        while(p != NULL)
        {
            cout << p ->cStr << '\n';
            p = p->next;
        }
        */
        createB_Tree(cStrLink , Btree, i, Bmap);
        cStrLink.destroyLink();


    }
}


void m_sort(vector<veOut>& ve)
{
    veOut temp;
    for(int i = 0; i < ve.size(); i++)
        for(int j = i + 1; j < ve.size(); j++)
        {
            if(ve[i].time > ve[j].time)
            {
                temp.docId = ve[i].docId;
                temp.time = ve[i].time;

                ve[i].docId = ve[j].docId;
                ve[i].time = ve[j].time;


                ve[j].docId = temp.docId;
                ve[j].time = temp.time;
            }
        }
}
