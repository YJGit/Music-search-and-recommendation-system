#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>
#include<fstream>
#include<string>
#include"init.h"
#include"findWord.h"
#include"MusicInfo.h"
#include"Const.h"
#include<QDebug>
#include<QTextCodec>
#include<QMessageBox>
#include<QPoint>
#include<QToolTip>
#include<QPainter>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置当前功能的提示
    setWindowTitle("Music System -- Search");
    setWindowState(Qt::WindowMaximized);

    //初始默认为m_isSearch = true;
    m_isSearch = true;
    m_recommend = false;

    //设置button图像
    QPixmap pixmap;
    pixmap.load(":image/11.jpg");
    ui->searchButton->setFixedSize(pixmap.width(),pixmap.height());
    ui->searchButton->setIcon(pixmap);
    ui->searchButton->setIconSize(QSize(pixmap.width(),pixmap.height()));
    ui->searchButton->setToolTip("Search");
    ui->searchButton->show();

    //点击“SOSO”与显示信息关联
    connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(searchMesShow()));
    connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(recommMesShow()));

    //初始化词典
    initDictionaryInfo(":/dic/dic.txt", m_dicSet);
    m_btree = NULL;
    initMusicInfoMap(m_musicInfoMap, m_Bmap, m_dicSet, m_btree);

    //音乐信息的点击事件，点击查看去全部信息
    ui ->ly1->installEventFilter(this), ui ->ly2->installEventFilter(this),
    ui ->ly3->installEventFilter(this), ui ->ly4->installEventFilter(this),
    ui ->ly5->installEventFilter(this), ui ->ly6->installEventFilter(this);
    ui ->ly7->installEventFilter(this), ui ->ly8->installEventFilter(this);
    ui ->ly9->installEventFilter(this), ui ->ly10->installEventFilter(this);
    //点击上下页翻页
    ui ->next ->installEventFilter(this), ui ->last->installEventFilter(this);

    //初始化存储音乐歌词
    for(int i = 0;i < 300; i++)
        m_ly[i] = "";

    //初始化当前的页数和标志
    m_page = 1;
    m_totalPage = 1;
    m_changPage = false;
    m_searToRec = false;
    //初始化翻页标志为“”,当能翻页时才显示
    ui ->next->setText("");
    ui ->last->setText("");

    //英文字母，后面要判断
    for(int i = 0; i < 26; i++)
    {
        Engs[i] = 'a' + i;
        Engb[i] = 'A' + i;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    m_Bmap.swap(map<KeyType, INVDIC*>());
}

//功能切换
void MainWindow::on_actionSearch_triggered()
{
    //设置当前功能的提示
    setWindowTitle("Music System -- Search");
    //更新当前背景图片
    update();

    if(!m_isSearch)
    {
        //将可能显示在界面上的提示去掉
        ui ->singer1->setText("");
        m_isSearch = true;
        m_recommend = false;
        //如果输入不为空则调用进行查询
        QString Qstr = ui ->lineEdit->text();
        if(Qstr != "")
            searchMesShow();
    }
}

void MainWindow::on_actionRecommend_triggered()
{
    //设置当前功能的提示
    setWindowTitle("Music System -- Recommend");
    //更新当前背景图片
    update();

    if(!m_recommend)
    {
        //将可能显示在界面上的提示去掉
        ui ->singer1->setText("");
        m_isSearch = false;
        m_recommend = true;
        //如果输入不为空则调用进行推荐
        QString Qstr = ui ->lineEdit->text();
        if(Qstr != "")
            recommMesShow();
    }
}

//绘制背景
void MainWindow::paintEvent(QPaintEvent *event)
{
    if(m_isSearch)
    {
        QPainter painter(this);
        painter.drawPixmap(0,0,width(),height(),QPixmap(":/image/b2.jpg"));
    }

    if(m_recommend)
    {
        QPainter painter(this);
        painter.drawPixmap(0,0,width(),height(),QPixmap(":/image/b3.jpg"));
    }
}



/*
 * 实现简单的查寻关键字的音乐查寻功能
 * 查寻规则：
 *查寻，通过找到关键字出现次数，按从高到低进行显示
 * 其中关键字出现在标题，出现次数+60，关键字出现在专辑，出现次数+50，关键字出现在歌手中，出现次数+30，在歌词中+1
 * 在关键字出现在标题和专辑中时，若关键字为“的”等无意义的词，则按+1算
 *  根据出现的多少排序输出结果
 */

void MainWindow::searchMesShow()
{
    //不查询，则直接返回
    if(!m_isSearch)
        return;
    //将状态调为只为查寻状态
    m_searToRec = false;

    //查寻前和显示前均须先置空
    ui ->label_2->setText(""), ui ->label_3->setText(""), ui ->label_4->setText(""), ui ->label_5->setText(""),
            ui ->label_6->setText("");
    ui ->title1->setText(""), ui ->title2->setText(""), ui ->title3->setText(""), ui ->title4->setText(""),
    ui ->title5->setText(""), ui ->title6->setText(""), ui ->title7->setText(""), ui ->title8->setText(""),
            ui ->title9->setText(""), ui ->title10->setText("");

    ui ->singer1->setText(""), ui ->singer2->setText(""), ui ->singer3->setText(""), ui ->singer4->setText(""),
    ui ->singer5->setText(""), ui ->singer6->setText(""), ui ->singer7->setText(""), ui ->singer8->setText(""),
            ui ->singer9->setText(""), ui ->singer10->setText("");

    ui ->album1->setText(""), ui ->album2->setText(""), ui ->album3->setText(""), ui ->album4->setText(""),
    ui ->album5->setText(""), ui ->album6->setText(""), ui ->album7->setText(""), ui ->album8->setText(""),
            ui ->album9->setText(""), ui ->album10->setText("");

    ui ->time1->setText(""), ui ->time2->setText(""), ui ->time3->setText(""), ui ->time4->setText(""),
    ui ->time5->setText(""), ui ->time6->setText(""), ui ->time7->setText(""), ui ->time8->setText(""),
            ui ->time9->setText(""), ui ->time10->setText("");

    ui ->ly1->setText(""), ui ->ly2->setText(""), ui ->ly3->setText(""), ui ->ly4->setText(""),
    ui ->ly5->setText(""), ui ->ly6->setText(""), ui ->ly7->setText(""), ui ->ly8->setText(""),
            ui ->ly9->setText(""), ui ->ly10->setText("");

    //不换页
    if(!m_changPage)
    {
        //查寻前将记录歌词清零
        for(int i = 0;i < 300; i++)
            m_ly[i] = "";

        //将当前的页面和总页面置1
        m_page = 1;
        m_totalPage = 1;
        //将翻页的标志置空
        ui ->next->setText("");
        ui ->last->setText("");

        //将显示结果先清空
        m_resultSort.clear();

        //获取输入信息内容
        QString Qstr = ui ->lineEdit->text();
        //qDebug() << Qstr;
        string str = Qstr.toStdString();
        QTextCodec *code = QTextCodec::codecForName("gb18030");
        if(code)
            str = code ->fromUnicode(Qstr).data();
        //cout << str << endl;
        //return;

        //输入为空
        if(str.empty())
        {
            //设置字号
            QFont ft;
            ft.setPointSize(24);
            ui->singer1->setFont(ft);
            //设置颜色
            QPalette pa;
            pa.setColor(QPalette::WindowText,Qt::red);
            ui->singer1->setPalette(pa);
            ui ->singer1->setText("No Results have found!");

            return;
        }

        //如果输入的全是英文，即搜索英文歌曲
        bool isWholeEng = true;
        int j = 0;

        for(int iE = 0; iE < str.size(); iE++)
        {
            if(str[iE] == ' ')
            {
               continue;
            }

            for(j = 0; j < 26; j++)
            {
                if(str[iE] == Engs[j] || str[iE] == Engb[j])
                {
                    break;
                }
            }

            if(j == 26)
            {
                isWholeEng = false;
                break;
            }
        }

        if(isWholeEng)
        {
           m_searToRec = true;
           recommMesShow();
           m_searToRec = false;

           return;
        }

        else
        {
            //利用音乐歌词分词函数分词
            MusicInfo mus;
            CharString Cstr;
            Cstr.StrAssign(str);
            mus.lyrics = Cstr;
            //cout <<mus.lyrics;
            //cStrlink记录分词结果
            CharStringLink cStrLink;
            mus.divideWords(Cstr, m_dicSet, "", cStrLink);

            //return;
            //一个个词查找
            LinkNode* pLink = cStrLink.head;

            //记录找到的文档id及其对应关键字的次数,在标题找到+60，在专辑找到+50,在歌手中找到+30，
            map<int, int> resultId;
            while(pLink != NULL)
            {
                //做关键字
                KeyType key;
                key.len = pLink ->cStr.length;
                key.asci = new int[key.len];
                for(int klen = 0; klen < key.len; klen++)
                    key.asci[klen] = pLink ->cStr.ch[klen];
                BTNode* btp = NULL;

                //将“的”字排除
                CharString cs;
                cs.StrAssign("的");

                /*查找是否有这些关键词，但是由于BT_search(m_btree, key, btp)这个函数是存储歌词的结果，故若是查歌名
                有可能查不到此歌名的歌曲，故用BT_search(m_btree, key, btp)前先看查寻的词是否在标题和专辑，歌名中出现
                */

                for(int iMus = 0; iMus < m_musicInfoMap.size(); iMus++)
                {
                    if(findWord(m_musicInfoMap[iMus].title, pLink->cStr))
                    {
                        if(pLink->cStr == cs)
                        {
                            //do nothing
                        }
                        else
                        {
                            //已经有记录
                            if(resultId.find(iMus) != resultId.end())
                                resultId[iMus] += 60;
                            else
                                resultId[iMus] = 60;
                        }
                    }

                    if(findWord(m_musicInfoMap[iMus].album, pLink ->cStr))
                    {
                        if(pLink->cStr == cs)
                        {
                            //do nothing
                        }
                        else
                        {
                            //已经有记录
                            if(resultId.find(iMus) != resultId.end())
                                resultId[iMus] += 50;
                            else
                                resultId[iMus] = 50;
                        }
                    }

                    if(findWord(m_musicInfoMap[iMus].singer, pLink ->cStr))
                    {
                        if(pLink->cStr == cs)
                        {
                            //do nothing
                        }
                        else
                        {
                            //已经有记录
                            if(resultId.find(iMus) != resultId.end())
                                resultId[iMus] += 30;
                            else
                                resultId[iMus] = 30;
                        }
                    }
                }

                if(BT_search(m_btree, key, btp))
                {
                    INVDOC *pdoc = m_Bmap[key]->head;
                    //找其标题和专辑，歌名是否含关键字并将相应doc的,其中若一些无意义的词（例如“的”）在标题，专辑中出现，不加
                    while(pdoc != NULL)
                    {
                       //已经有记录
                       if(resultId.find(pdoc ->DocID) != resultId.end())
                           resultId[pdoc ->DocID] += pdoc ->times;
                       else
                           resultId[pdoc->DocID] = pdoc ->times;

                       pdoc = pdoc ->next;
                    }
                }

                pLink = pLink->next;
            }

            //for(map<int, int>::iterator iter = resultId.begin(); iter != resultId.end(); iter++)
                //cout << iter ->first << " "<< iter ->second << endl;
            // return;

            for(map<int, int>::iterator iter = resultId.begin(); iter != resultId.end(); iter++)
            {
                veOut ve;
                ve.docId = iter ->first;
                ve.time = iter ->second;
                m_resultSort.push_back(ve);
            }

            m_sort(m_resultSort);
            //初始化m_iterSort
            m_iterSort = m_resultSort.end();
        }
    }

    //显示结果到界面
    //并没有查到结果
    if(m_resultSort.size() == 0)
    {
        //设置字号
        QFont ft;
        ft.setPointSize(24);
        ui->singer1->setFont(ft);
        //设置颜色
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::red);
        ui->singer1->setPalette(pa);

        ui ->singer1->setText("No Results have found!");
    }

    else
    {
        if(!m_changPage)
        {
            m_totalPage = (m_resultSort.size() % 10 == 0)? m_resultSort.size() / 10:(m_resultSort.size() / 10 + 1);
            //cout << "共元素"<<m_resultSort.size()<<endl;
            if(m_totalPage > 1)
                ui ->next->setText("-->");
        }
        //将singer1的字体颜色改回
        QFont ft;
        ft.setPointSize(12);
        ui->singer1->setFont(ft);
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::black);
        ui->singer1->setPalette(pa);
        ui ->singer1->setText("");   //因为前面用到singer1打印过“no result”，故须先清空

        int outSize = 0; //计数
\
        for(; ;)
        {
            m_iterSort--;
            //设置字号
            QFont ft;
            ft.setPointSize(12);
            ui->label_2->setFont(ft);
            ui->label_3->setFont(ft);
            ui->label_4->setFont(ft);
            ui->label_5->setFont(ft);
            ui->label_6->setFont(ft);

            //显示标题信息
            QString str;
            str = str.fromLocal8Bit("歌名");
            ui ->label_2->setText(str);
            str = str.fromLocal8Bit("歌手");
            ui ->label_3->setText(str);
            str = str.fromLocal8Bit("专辑");
            ui ->label_4->setText(str);
            str = str.fromLocal8Bit("发行时间");
            ui ->label_5->setText(str);
            str = str.fromLocal8Bit("歌词(长按不放查看全部)");
            ui ->label_6->setText(str);

            QString title, singer,album, publishDate, lyrics;
            const char* c_s;         //string 转 const char*
            //处理歌词，先展现出一行，其他在点击是再显示
            string st;

            switch (outSize % 10) {
            case 0:
                st = m_musicInfoMap[m_iterSort ->docId].title.toString();
                c_s = st.c_str();
                //cout << c_s;
                title = title.fromLocal8Bit(c_s);
                ui ->title1->setText(title);

                st = m_musicInfoMap[m_iterSort->docId].singer.toString();
                c_s = st.c_str();
                singer = singer.fromLocal8Bit(c_s);
                ui ->singer1->setText(singer);

                st = m_musicInfoMap[m_iterSort->docId].album.toString();
                c_s = st.c_str();
                album = album.fromLocal8Bit(c_s);
                ui ->album1->setText(album);

                st = m_musicInfoMap[m_iterSort->docId].publishDate.toString();
                c_s = st.c_str();
                publishDate = publishDate.fromLocal8Bit(c_s);
                ui ->time1 ->setText(publishDate);


                st = m_musicInfoMap[m_iterSort->docId].lyrics.toString();

                //存储完整的音乐信息
                m_ly[(m_page - 1) * 10] = QString::fromLocal8Bit(st.c_str());

                st = st.substr(0, st.find("\n"));
                c_s = st.c_str();
                lyrics = lyrics.fromLocal8Bit(c_s);
                ui ->ly1 ->setText(lyrics);
                break;

            case 1:
                st = m_musicInfoMap[m_iterSort->docId].title.toString();
                c_s = st.c_str();
                //cout << c_s;
                title = title.fromLocal8Bit(c_s);
                ui ->title2->setText(title);

                st = m_musicInfoMap[m_iterSort->docId].singer.toString();
                c_s = st.c_str();
                singer = singer.fromLocal8Bit(c_s);
                ui ->singer2->setText(singer);

                st = m_musicInfoMap[m_iterSort->docId].album.toString();
                c_s = st.c_str();
                album = album.fromLocal8Bit(c_s);
                ui ->album2->setText(album);

                st = m_musicInfoMap[m_iterSort->docId].publishDate.toString();
                c_s = st.c_str();
                publishDate = publishDate.fromLocal8Bit(c_s);
                ui ->time2->setText(publishDate);


                st = m_musicInfoMap[m_iterSort->docId].lyrics.toString();

                //存储完整的音乐信息
                m_ly[(m_page - 1) * 10 + 1] = QString::fromLocal8Bit(st.c_str());

                st = st.substr(0, st.find("\n"));
                c_s = st.c_str();
                lyrics = lyrics.fromLocal8Bit(c_s);
                ui ->ly2 ->setText(lyrics);

                break;

            case 2:
                st = m_musicInfoMap[m_iterSort->docId].title.toString();
                c_s = st.c_str();
                //cout << c_s;
                title = title.fromLocal8Bit(c_s);
                ui ->title3->setText(title);

                st = m_musicInfoMap[m_iterSort->docId].singer.toString();
                c_s = st.c_str();
                singer = singer.fromLocal8Bit(c_s);
                ui ->singer3->setText(singer);

                st = m_musicInfoMap[m_iterSort->docId].album.toString();
                c_s = st.c_str();
                album = album.fromLocal8Bit(c_s);
                ui ->album3->setText(album);

                st = m_musicInfoMap[m_iterSort->docId].publishDate.toString();
                c_s = st.c_str();
                publishDate = publishDate.fromLocal8Bit(c_s);
                ui ->time3 ->setText(publishDate);


                st = m_musicInfoMap[m_iterSort->docId].lyrics.toString();

                //存储完整的音乐信息
                m_ly[(m_page - 1) * 10 + 2] = QString::fromLocal8Bit(st.c_str());

                st = st.substr(0, st.find("\n"));
                c_s = st.c_str();
                lyrics = lyrics.fromLocal8Bit(c_s);
                ui ->ly3 ->setText(lyrics);

                break;

            case 3:
                st = m_musicInfoMap[m_iterSort->docId].title.toString();
                c_s = st.c_str();
                //cout << c_s;
                title = title.fromLocal8Bit(c_s);
                ui ->title4->setText(title);

                st = m_musicInfoMap[m_iterSort->docId].singer.toString();
                c_s = st.c_str();
                singer = singer.fromLocal8Bit(c_s);
                ui ->singer4->setText(singer);

                st = m_musicInfoMap[m_iterSort->docId].album.toString();
                c_s = st.c_str();
                album = album.fromLocal8Bit(c_s);
                ui ->album4->setText(album);

                st = m_musicInfoMap[m_iterSort->docId].publishDate.toString();
                c_s = st.c_str();
                publishDate = publishDate.fromLocal8Bit(c_s);
                ui ->time4 ->setText(publishDate);


                st = m_musicInfoMap[m_iterSort->docId].lyrics.toString();

                //存储完整的音乐信息
                m_ly[(m_page - 1) * 10 + 3] = QString::fromLocal8Bit(st.c_str());

                st = st.substr(0, st.find("\n"));
                c_s = st.c_str();
                lyrics = lyrics.fromLocal8Bit(c_s);
                ui ->ly4 ->setText(lyrics);

                break;

            case 4:
                st = m_musicInfoMap[m_iterSort->docId].title.toString();
                c_s = st.c_str();
                //cout << c_s;
                title = title.fromLocal8Bit(c_s);
                ui ->title5->setText(title);

                st = m_musicInfoMap[m_iterSort->docId].singer.toString();
                c_s = st.c_str();
                singer = singer.fromLocal8Bit(c_s);
                ui ->singer5->setText(singer);

                st = m_musicInfoMap[m_iterSort->docId].album.toString();
                c_s = st.c_str();
                album = album.fromLocal8Bit(c_s);
                ui ->album5->setText(album);

                st = m_musicInfoMap[m_iterSort->docId].publishDate.toString();
                c_s = st.c_str();
                publishDate = publishDate.fromLocal8Bit(c_s);
                ui ->time5 ->setText(publishDate);


                st = m_musicInfoMap[m_iterSort->docId].lyrics.toString();

                //存储完整的音乐信息
                m_ly[(m_page - 1) * 10+ 4] = QString::fromLocal8Bit(st.c_str());

                st = st.substr(0, st.find("\n"));
                c_s = st.c_str();
                lyrics = lyrics.fromLocal8Bit(c_s);
                ui ->ly5 ->setText(lyrics);

                break;

            case 5:
                st = m_musicInfoMap[m_iterSort->docId].title.toString();
                c_s = st.c_str();
                //cout << c_s;
                title = title.fromLocal8Bit(c_s);
                ui ->title6->setText(title);

                st = m_musicInfoMap[m_iterSort->docId].singer.toString();
                c_s = st.c_str();
                singer = singer.fromLocal8Bit(c_s);
                ui ->singer6->setText(singer);

                st = m_musicInfoMap[m_iterSort->docId].album.toString();
                c_s = st.c_str();
                album = album.fromLocal8Bit(c_s);
                ui ->album6->setText(album);

                st = m_musicInfoMap[m_iterSort->docId].publishDate.toString();
                c_s = st.c_str();
                publishDate = publishDate.fromLocal8Bit(c_s);
                ui ->time6 ->setText(publishDate);


                st = m_musicInfoMap[m_iterSort->docId].lyrics.toString();

                //存储完整的音乐信息
                m_ly[(m_page - 1) * 10 + 5] = QString::fromLocal8Bit(st.c_str());

                st = st.substr(0, st.find("\n"));
                c_s = st.c_str();
                lyrics = lyrics.fromLocal8Bit(c_s);
                ui ->ly6 ->setText(lyrics);

                break;

            case 6:
                st = m_musicInfoMap[m_iterSort->docId].title.toString();
                c_s = st.c_str();
                //cout << c_s;
                title = title.fromLocal8Bit(c_s);
                ui ->title7->setText(title);

                st = m_musicInfoMap[m_iterSort->docId].singer.toString();
                c_s = st.c_str();
                singer = singer.fromLocal8Bit(c_s);
                ui ->singer7->setText(singer);

                st = m_musicInfoMap[m_iterSort->docId].album.toString();
                c_s = st.c_str();
                album = album.fromLocal8Bit(c_s);
                ui ->album7->setText(album);

                st = m_musicInfoMap[m_iterSort->docId].publishDate.toString();
                c_s = st.c_str();
                publishDate = publishDate.fromLocal8Bit(c_s);
                ui ->time7 ->setText(publishDate);


                st = m_musicInfoMap[m_iterSort->docId].lyrics.toString();

                //存储完整的音乐信息
                m_ly[(m_page - 1) * 10 + 6] = QString::fromLocal8Bit(st.c_str());

                st = st.substr(0, st.find("\n"));
                c_s = st.c_str();
                lyrics = lyrics.fromLocal8Bit(c_s);
                ui ->ly7 ->setText(lyrics);

                break;

            case 7:
                st = m_musicInfoMap[m_iterSort->docId].title.toString();
                c_s = st.c_str();
                //cout << c_s;
                title = title.fromLocal8Bit(c_s);
                ui ->title8->setText(title);

                st = m_musicInfoMap[m_iterSort->docId].singer.toString();
                c_s = st.c_str();
                singer = singer.fromLocal8Bit(c_s);
                ui ->singer8->setText(singer);

                st = m_musicInfoMap[m_iterSort->docId].album.toString();
                c_s = st.c_str();
                album = album.fromLocal8Bit(c_s);
                ui ->album8->setText(album);

                st = m_musicInfoMap[m_iterSort->docId].publishDate.toString();
                c_s = st.c_str();
                publishDate = publishDate.fromLocal8Bit(c_s);
                ui ->time8 ->setText(publishDate);


                st = m_musicInfoMap[m_iterSort->docId].lyrics.toString();

                //存储完整的音乐信息
                m_ly[(m_page - 1) * 10 + 7] = QString::fromLocal8Bit(st.c_str());

                st = st.substr(0, st.find("\n"));
                c_s = st.c_str();
                lyrics = lyrics.fromLocal8Bit(c_s);
                ui ->ly8 ->setText(lyrics);

                break;

            case 8:
                st = m_musicInfoMap[m_iterSort->docId].title.toString();
                c_s = st.c_str();
                //cout << c_s;
                title = title.fromLocal8Bit(c_s);
                ui ->title9->setText(title);

                st = m_musicInfoMap[m_iterSort->docId].singer.toString();
                c_s = st.c_str();
                singer = singer.fromLocal8Bit(c_s);
                ui ->singer9->setText(singer);

                st = m_musicInfoMap[m_iterSort->docId].album.toString();
                c_s = st.c_str();
                album = album.fromLocal8Bit(c_s);
                ui ->album9->setText(album);

                st = m_musicInfoMap[m_iterSort->docId].publishDate.toString();
                c_s = st.c_str();
                publishDate = publishDate.fromLocal8Bit(c_s);
                ui ->time9 ->setText(publishDate);


                st = m_musicInfoMap[m_iterSort->docId].lyrics.toString();

                //存储完整的音乐信息
                m_ly[(m_page - 1) * 10 + 8] = QString::fromLocal8Bit(st.c_str());

                st = st.substr(0, st.find("\n"));
                c_s = st.c_str();
                lyrics = lyrics.fromLocal8Bit(c_s);
                ui ->ly9->setText(lyrics);

                break;

            case 9:
                st = m_musicInfoMap[m_iterSort->docId].title.toString();
                c_s = st.c_str();
                //cout << c_s;
                title = title.fromLocal8Bit(c_s);
                ui ->title10->setText(title);

                st = m_musicInfoMap[m_iterSort->docId].singer.toString();
                c_s = st.c_str();
                singer = singer.fromLocal8Bit(c_s);
                ui ->singer10->setText(singer);

                st = m_musicInfoMap[m_iterSort->docId].album.toString();
                c_s = st.c_str();
                album = album.fromLocal8Bit(c_s);
                ui ->album10->setText(album);

                st = m_musicInfoMap[m_iterSort->docId].publishDate.toString();
                c_s = st.c_str();
                publishDate = publishDate.fromLocal8Bit(c_s);
                ui ->time10->setText(publishDate);


                st = m_musicInfoMap[m_iterSort->docId].lyrics.toString();

                //存储完整的音乐信息
                m_ly[(m_page - 1) * 10 + 9] = QString::fromLocal8Bit(st.c_str());

                st = st.substr(0, st.find("\n"));
                c_s = st.c_str();
                lyrics = lyrics.fromLocal8Bit(c_s);
                ui ->ly10->setText(lyrics);

                break;
            default:
                break;
            }

            outSize++;
            if(m_iterSort == m_resultSort.begin() || outSize == 10)
                break;
        }
    }

    if(m_changPage)
    {
        //cout << "total" <<m_totalPage << endl;
        //cout << m_page << endl;
        m_changPage = false;
        //若是第一页则消除上一页的标准
        if(m_page == 1)
        {
            ui ->last->setText("");
        }

        //当翻下一页时，产生上一页的标志
        if(m_page > 1)
            ui ->last ->setText("<--");

        //添加下一页的标志
        if(m_page < m_totalPage)
            ui ->next->setText("-->");
        //当达最后一页，消除下一页标志
        if(m_page == m_totalPage)
            ui ->next->setText("");
    }
}


//推荐时用的显示函数
void MainWindow::recommendOut(int num, MusicInfo musicInfo)
{
    QString title, singer, album,  publishDate, lyrics;
    string st;
    const char* c_s;
    switch (num) {
    case 0:
        st = musicInfo.title.toString();
        c_s = st.c_str();
        //cout << c_s;
        title = title.fromLocal8Bit(c_s);
        ui ->title1->setText(title);

        st = musicInfo.singer.toString();
        c_s = st.c_str();
        singer = singer.fromLocal8Bit(c_s);
        ui ->singer1->setText(singer);

        st = musicInfo.album.toString();
        c_s = st.c_str();
        album = album.fromLocal8Bit(c_s);
        ui ->album1->setText(album);

        st = musicInfo.publishDate.toString();
        c_s = st.c_str();
        publishDate = publishDate.fromLocal8Bit(c_s);
        ui ->time1 ->setText(publishDate);


        st = musicInfo.lyrics.toString();

        //存储完整的音乐信息
        m_ly[num] = QString::fromLocal8Bit(st.c_str());

        st = st.substr(0, st.find("\n"));
        c_s = st.c_str();
        lyrics = lyrics.fromLocal8Bit(c_s);
        ui ->ly1 ->setText(lyrics);
        break;

    case 1:
        st = musicInfo.title.toString();
        c_s = st.c_str();
        //cout << c_s;
        title = title.fromLocal8Bit(c_s);
        ui ->title2->setText(title);

        st = musicInfo.singer.toString();
        c_s = st.c_str();
        singer = singer.fromLocal8Bit(c_s);
        ui ->singer2->setText(singer);

        st = musicInfo.album.toString();
        c_s = st.c_str();
        album = album.fromLocal8Bit(c_s);
        ui ->album2->setText(album);

        st = musicInfo.publishDate.toString();
        c_s = st.c_str();
        publishDate = publishDate.fromLocal8Bit(c_s);
        ui ->time2 ->setText(publishDate);


        st = musicInfo.lyrics.toString();

        //存储完整的音乐信息
        m_ly[num] = QString::fromLocal8Bit(st.c_str());

        st = st.substr(0, st.find("\n"));
        c_s = st.c_str();
        lyrics = lyrics.fromLocal8Bit(c_s);
        ui ->ly2 ->setText(lyrics);
        break;

    case 2:
        st = musicInfo.title.toString();
        c_s = st.c_str();
        //cout << c_s;
        title = title.fromLocal8Bit(c_s);
        ui ->title3->setText(title);

        st = musicInfo.singer.toString();
        c_s = st.c_str();
        singer = singer.fromLocal8Bit(c_s);
        ui ->singer3->setText(singer);

        st = musicInfo.album.toString();
        c_s = st.c_str();
        album = album.fromLocal8Bit(c_s);
        ui ->album3->setText(album);

        st = musicInfo.publishDate.toString();
        c_s = st.c_str();
        publishDate = publishDate.fromLocal8Bit(c_s);
        ui ->time3 ->setText(publishDate);


        st = musicInfo.lyrics.toString();

        //存储完整的音乐信息
        m_ly[num] = QString::fromLocal8Bit(st.c_str());

        st = st.substr(0, st.find("\n"));
        c_s = st.c_str();
        lyrics = lyrics.fromLocal8Bit(c_s);
        ui ->ly3 ->setText(lyrics);
        break;

    case 3:
        st = musicInfo.title.toString();
        c_s = st.c_str();
        //cout << c_s;
        title = title.fromLocal8Bit(c_s);
        ui ->title4->setText(title);

        st = musicInfo.singer.toString();
        c_s = st.c_str();
        singer = singer.fromLocal8Bit(c_s);
        ui ->singer4->setText(singer);

        st = musicInfo.album.toString();
        c_s = st.c_str();
        album = album.fromLocal8Bit(c_s);
        ui ->album4->setText(album);

        st = musicInfo.publishDate.toString();
        c_s = st.c_str();
        publishDate = publishDate.fromLocal8Bit(c_s);
        ui ->time4 ->setText(publishDate);


        st = musicInfo.lyrics.toString();

        //存储完整的音乐信息
        m_ly[num] = QString::fromLocal8Bit(st.c_str());

        st = st.substr(0, st.find("\n"));
        c_s = st.c_str();
        lyrics = lyrics.fromLocal8Bit(c_s);
        ui ->ly4 ->setText(lyrics);
        break;

    case 4:
        st = musicInfo.title.toString();
        c_s = st.c_str();
        //cout << c_s;
        title = title.fromLocal8Bit(c_s);
        ui ->title5->setText(title);

        st = musicInfo.singer.toString();
        c_s = st.c_str();
        singer = singer.fromLocal8Bit(c_s);
        ui ->singer5->setText(singer);

        st = musicInfo.album.toString();
        c_s = st.c_str();
        album = album.fromLocal8Bit(c_s);
        ui ->album5->setText(album);

        st = musicInfo.publishDate.toString();
        c_s = st.c_str();
        publishDate = publishDate.fromLocal8Bit(c_s);
        ui ->time5 ->setText(publishDate);


        st = musicInfo.lyrics.toString();

        //存储完整的音乐信息
        m_ly[num] = QString::fromLocal8Bit(st.c_str());

        st = st.substr(0, st.find("\n"));
        c_s = st.c_str();
        lyrics = lyrics.fromLocal8Bit(c_s);
        ui ->ly5 ->setText(lyrics);
        break;

    case 5:
        st = musicInfo.title.toString();
        c_s = st.c_str();
        //cout << c_s;
        title = title.fromLocal8Bit(c_s);
        ui ->title6->setText(title);

        st = musicInfo.singer.toString();
        c_s = st.c_str();
        singer = singer.fromLocal8Bit(c_s);
        ui ->singer6->setText(singer);

        st = musicInfo.album.toString();
        c_s = st.c_str();
        album = album.fromLocal8Bit(c_s);
        ui ->album6->setText(album);

        st = musicInfo.publishDate.toString();
        c_s = st.c_str();
        publishDate = publishDate.fromLocal8Bit(c_s);
        ui ->time6->setText(publishDate);


        st = musicInfo.lyrics.toString();

        //存储完整的音乐信息
        m_ly[num] = QString::fromLocal8Bit(st.c_str());

        st = st.substr(0, st.find("\n"));
        c_s = st.c_str();
        lyrics = lyrics.fromLocal8Bit(c_s);
        ui ->ly6 ->setText(lyrics);
        break;

    case 6:
        st = musicInfo.title.toString();
        c_s = st.c_str();
        //cout << c_s;
        title = title.fromLocal8Bit(c_s);
        ui ->title7->setText(title);

        st = musicInfo.singer.toString();
        c_s = st.c_str();
        singer = singer.fromLocal8Bit(c_s);
        ui ->singer7->setText(singer);

        st = musicInfo.album.toString();
        c_s = st.c_str();
        album = album.fromLocal8Bit(c_s);
        ui ->album7->setText(album);

        st = musicInfo.publishDate.toString();
        c_s = st.c_str();
        publishDate = publishDate.fromLocal8Bit(c_s);
        ui ->time7 ->setText(publishDate);


        st = musicInfo.lyrics.toString();

        //存储完整的音乐信息
        m_ly[num] = QString::fromLocal8Bit(st.c_str());

        st = st.substr(0, st.find("\n"));
        c_s = st.c_str();
        lyrics = lyrics.fromLocal8Bit(c_s);
        ui ->ly7 ->setText(lyrics);
        break;

    case 7:
        st = musicInfo.title.toString();
        c_s = st.c_str();
        //cout << c_s;
        title = title.fromLocal8Bit(c_s);
        ui ->title8->setText(title);

        st = musicInfo.singer.toString();
        c_s = st.c_str();
        singer = singer.fromLocal8Bit(c_s);
        ui ->singer8->setText(singer);

        st = musicInfo.album.toString();
        c_s = st.c_str();
        album = album.fromLocal8Bit(c_s);
        ui ->album8->setText(album);

        st = musicInfo.publishDate.toString();
        c_s = st.c_str();
        publishDate = publishDate.fromLocal8Bit(c_s);
        ui ->time8 ->setText(publishDate);


        st = musicInfo.lyrics.toString();

        //存储完整的音乐信息
        m_ly[num] = QString::fromLocal8Bit(st.c_str());

        st = st.substr(0, st.find("\n"));
        c_s = st.c_str();
        lyrics = lyrics.fromLocal8Bit(c_s);
        ui ->ly8 ->setText(lyrics);
        break;

    case 8:
        st = musicInfo.title.toString();
        c_s = st.c_str();
        //cout << c_s;
        title = title.fromLocal8Bit(c_s);
        ui ->title9->setText(title);

        st = musicInfo.singer.toString();
        c_s = st.c_str();
        singer = singer.fromLocal8Bit(c_s);
        ui ->singer9->setText(singer);

        st = musicInfo.album.toString();
        c_s = st.c_str();
        album = album.fromLocal8Bit(c_s);
        ui ->album9->setText(album);

        st = musicInfo.publishDate.toString();
        c_s = st.c_str();
        publishDate = publishDate.fromLocal8Bit(c_s);
        ui ->time9 ->setText(publishDate);


        st = musicInfo.lyrics.toString();

        //存储完整的音乐信息
        m_ly[num] = QString::fromLocal8Bit(st.c_str());

        st = st.substr(0, st.find("\n"));
        c_s = st.c_str();
        lyrics = lyrics.fromLocal8Bit(c_s);
        ui ->ly9 ->setText(lyrics);
        break;

    case 9:
        st = musicInfo.title.toString();
        c_s = st.c_str();
        //cout << c_s;
        title = title.fromLocal8Bit(c_s);
        ui ->title10->setText(title);

        st = musicInfo.singer.toString();
        c_s = st.c_str();
        singer = singer.fromLocal8Bit(c_s);
        ui ->singer10->setText(singer);

        st = musicInfo.album.toString();
        c_s = st.c_str();
        album = album.fromLocal8Bit(c_s);
        ui ->album10->setText(album);

        st = musicInfo.publishDate.toString();
        c_s = st.c_str();
        publishDate = publishDate.fromLocal8Bit(c_s);
        ui ->time10 ->setText(publishDate);


        st = musicInfo.lyrics.toString();

        //存储完整的音乐信息
        m_ly[num] = QString::fromLocal8Bit(st.c_str());

        st = st.substr(0, st.find("\n"));
        c_s = st.c_str();
        lyrics = lyrics.fromLocal8Bit(c_s);
        ui ->ly10 ->setText(lyrics);
        break;
    default:
        break;
    }
}


/*
*实现简单的音乐的推荐
*推荐规则：
*   若输入的是歌名：
*        标题中含同样的词汇最先(除去“的”这样的词汇，标题很好表达了歌的内容等等)
*        歌手，曲词作者，专辑都相同的第二
*        歌手，曲词作者相同的次于前面
*        仅歌手相同的最后(后面三者都是为了保证风格大致一样)
*        比较直到得到10首歌为止，多于10首的输出10首歌曲，少于10首的不补
*   否则：
*       给出提示”请输入歌名“
*   且一次仅检索一个词，也就是只能输入一个歌名
*/

void MainWindow::recommMesShow()
{
    //将page置为1
    m_page = 1;
    //如果不是推荐或查寻英文歌曲，直接返回
    if(!m_recommend && !m_searToRec)
        return;
    //显示前均须置空
    ui ->label_2->setText(""), ui ->label_3->setText(""), ui ->label_4->setText(""), ui ->label_5->setText(""),
            ui ->label_6->setText("");
    ui ->title1->setText(""), ui ->title2->setText(""), ui ->title3->setText(""), ui ->title4->setText(""),
    ui ->title5->setText(""), ui ->title6->setText(""), ui ->title7->setText(""), ui ->title8->setText(""),
            ui ->title9->setText(""), ui ->title10->setText("");

    ui ->singer1->setText(""), ui ->singer2->setText(""), ui ->singer3->setText(""), ui ->singer4->setText(""),
    ui ->singer5->setText(""), ui ->singer6->setText(""), ui ->singer7->setText(""), ui ->singer8->setText(""),
            ui ->singer9->setText(""), ui ->singer10->setText("");

    ui ->album1->setText(""), ui ->album2->setText(""), ui ->album3->setText(""), ui ->album4->setText(""),
    ui ->album5->setText(""), ui ->album6->setText(""), ui ->album7->setText(""), ui ->album8->setText(""),
            ui ->album9->setText(""), ui ->album10->setText("");

    ui ->time1->setText(""), ui ->time2->setText(""), ui ->time3->setText(""), ui ->time4->setText(""),
    ui ->time5->setText(""), ui ->time6->setText(""), ui ->time7->setText(""), ui ->time8->setText(""),
            ui ->time9->setText(""), ui ->time10->setText("");

    ui ->ly1->setText(""), ui ->ly2->setText(""), ui ->ly3->setText(""), ui ->ly4->setText(""),
    ui ->ly5->setText(""), ui ->ly6->setText(""), ui ->ly7->setText(""), ui ->ly8->setText(""),
            ui ->ly9->setText(""), ui ->ly10->setText("");
    ui ->next->setText(""), ui->last->setText("");

    //查寻前将记录歌词清零
    for(int i = 0;i < 300; i++)
        m_ly[i] = "";
    //获取输入信息内容
    QString Qstr = ui ->lineEdit->text();
    //qDebug() << Qstr;
    string str = Qstr.toStdString();
    QTextCodec *code = QTextCodec::codecForName("gb18030");
    if(code)
        str = code ->fromUnicode(Qstr).data();

    //输入为空
    if(str.empty())
    {
        //设置字号
        QFont ft;
        ft.setPointSize(24);
        ui->singer1->setFont(ft);
        //设置颜色
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::red);
        ui->singer1->setPalette(pa);
        ui ->singer1->setText("Please input a music name!");

        return;
    }
    else
    {
       //将存储已推荐的歌的数据清空
       m_Haverec.clear();
       bool isAMusicName = false;     //记录输入是否为歌名
       //记录找到的歌的信息
       CharString title, singer, album, composer, lyricist;
       //记录输入歌的Id
       int id = 0;
       //判断是否被推荐
       bool IsRec = false;
       //显示标题信息
        int findNum = 0;

        //将singer标签的字体颜色改回
        QFont ft;
        ft.setPointSize(12);
        ui->singer1->setFont(ft);
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::black);
        ui->singer1->setPalette(pa);
        ui ->singer1->setText("");   //因为前面用到singer1打印过“no result”，故须先清空


       //判断是否为一首歌的歌名
       for(map<int, MusicInfo>::iterator iter = m_musicInfoMap.begin(); iter != m_musicInfoMap.end(); iter++)
       {
           //string 转 Charstring
           CharString cs;
           cs.StrAssign(str);
           if((m_recommend &&iter->second.title == cs) || (m_searToRec && findWord(iter ->second.title, cs))
                   || (m_searToRec && findWord(iter ->second.album, cs)) ||
                       (m_searToRec && findWord(iter ->second.singer, cs)))
           {
               if(m_recommend)
               {
                   isAMusicName = true;
                   title = iter->second.title;
                   singer = iter ->second.singer;
                   album = iter ->second.album;
                   composer = iter ->second.composer;
                   lyricist = iter ->second.lyricist;
                   id = iter ->first;

                   break;
               }

               //如果是在搜索，则要输出这首歌
               if(m_searToRec)
               {
                   isAMusicName = true;
                   title = iter->second.title;
                   singer = iter ->second.singer;
                   album = iter ->second.album;
                   composer = iter ->second.composer;
                   lyricist = iter ->second.lyricist;
                   id = iter ->first;

                   recommendOut(findNum, iter ->second);
                   findNum++;
                   if(findNum == 10)
                       break;
               }
           }
       }

       //不是一首歌的歌名
       if(!isAMusicName)
       {
           //设置字号
           QFont ft;
           ft.setPointSize(24);
           ui->singer1->setFont(ft);
           //设置颜色
           QPalette pa;
           pa.setColor(QPalette::WindowText,Qt::red);
           ui->singer1->setPalette(pa);
           if(m_recommend)
                ui ->singer1->setText("Please input a music name!");
           else if(m_searToRec)
               ui ->singer1->setText("No Results have found!");

           return;
       }

       //是一首歌的歌名
        //设置字号
        ft.setPointSize(12);
        ui->label_2->setFont(ft);
        ui->label_3->setFont(ft);
        ui->label_4->setFont(ft);
        ui->label_5->setFont(ft);
        ui->label_6->setFont(ft);

        QString str;
        str = str.fromLocal8Bit("歌名");
        ui ->label_2->setText(str);
        str = str.fromLocal8Bit("歌手");
        ui ->label_3->setText(str);
        str = str.fromLocal8Bit("专辑");
        ui ->label_4->setText(str);
        str = str.fromLocal8Bit("发行时间");
        ui ->label_5->setText(str);
        str = str.fromLocal8Bit("歌词(长按不放查看全部)");
        ui ->label_6->setText(str);

        //开始查寻
        //标题中有
        if(findNum != 10)
        {
            for(map<int, MusicInfo>::iterator iter1 = m_musicInfoMap.begin(); iter1 != m_musicInfoMap.end(); iter1++)
            {
                if(iter1 ->first == id)
                {
                    continue;
                }
                //标题中出现
                if(findWord(iter1 ->second.title, title))
                {
                     recommendOut(findNum, iter1->second);
                     findNum++;
                     if(findNum == 10)
                         break;
                }
            }
        }

        //歌手，曲词作者，专辑都相同的第二
        if(findNum != 10)
        {
            for(map<int, MusicInfo>::iterator iter1 = m_musicInfoMap.begin(); iter1 != m_musicInfoMap.end(); iter1++)
            {
                if(iter1 ->first == id)
                    continue;
                //标题中出现
                if(iter1->second.singer == singer && iter1 ->second.composer == composer
                        && iter1 ->second.lyricist == lyricist && iter1 ->second.album == album)
                {
                     //判断是否被推荐过
                     for(int i = 0;i < m_Haverec.size(); i++)
                     {
                         if(iter1->first == m_Haverec[i])
                         {
                             IsRec = true;
                             break;
                         }
                     }

                     if(!IsRec)
                     {
                         recommendOut(findNum, iter1->second);
                         findNum++;
                         m_Haverec.push_back(iter1->first);
                         if(findNum == 10)
                             break;
                     }
                     else
                         IsRec = false;
                }
            }
        }

         //歌手，曲词作者相同的次于前面
         if(findNum != 10)
         {
             for(map<int, MusicInfo>::iterator iter1 = m_musicInfoMap.begin(); iter1 != m_musicInfoMap.end(); iter1++)
             {
                 if(iter1 ->first == id)
                     continue;
                 //标题中出现
                 if(iter1->second.singer == singer && iter1 ->second.composer == composer
                         && iter1 ->second.lyricist == lyricist)
                 {
                     //判断是否被推荐过
                     for(int i = 0;i < m_Haverec.size(); i++)
                     {
                         if(iter1->first == m_Haverec[i])
                         {
                             IsRec = true;
                             break;
                         }
                     }
                     if(!IsRec)
                     {
                         recommendOut(findNum, iter1->second);
                         findNum++;
                         m_Haverec.push_back(iter1->first);
                         if(findNum == 10)
                             break;
                     }
                     else
                         IsRec = false;

                 }
             }
         }


          //仅歌手相同的最后
          if(findNum != 10)
          {
              for(map<int, MusicInfo>::iterator iter1 = m_musicInfoMap.begin(); iter1 != m_musicInfoMap.end(); iter1++)
              {
                  if(iter1 ->first == id)
                      continue;
                  //标题中出现
                  if(iter1->second.singer == singer)
                  {
                      //判断是否被推荐过
                      for(int i = 0;i < m_Haverec.size(); i++)
                      {
                          if(iter1->first == m_Haverec[i])
                          {
                              IsRec = true;
                              break;
                          }
                      }

                      if(!IsRec)
                      {
                          recommendOut(findNum, iter1 ->second);
                          findNum++;
                          m_Haverec.push_back(iter1->first);
                          if(findNum == 10)
                              break;
                      }
                      else
                          IsRec = false;
                  }
              }
          }

          //没有推荐内容
          if(findNum == 0)
              ui ->singer1->setText("No Results have found!");
    }
}

//相应label高中点击事件，比如显示全部歌词和翻页
bool MainWindow::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj == ui->ly1) {
        if (ev->type() == QEvent::MouseButtonPress) {
            QPoint pont = cursor().pos();
            QToolTip::showText(pont, m_ly[(m_page - 1) * 10]);
            return true;
        } else {
            return false;
        }
    }

    else if (obj == ui->ly2) {
        if (ev->type() == QEvent::MouseButtonPress) {
            QPoint pont = cursor().pos();
            QToolTip::showText(pont, m_ly[(m_page - 1) * 10 + 1]);
            return true;
        } else {
            return false;
        }
    }
    else if (obj == ui->ly3) {
        if (ev->type() == QEvent::MouseButtonPress) {
            QPoint pont = cursor().pos();
            QToolTip::showText(pont, m_ly[(m_page - 1) * 10 + 2]);
            return true;
        } else {
            return false;
        }
    }
    else if (obj == ui->ly4) {
        if (ev->type() == QEvent::MouseButtonPress) {
            QPoint pont = cursor().pos();
            QToolTip::showText(pont, m_ly[(m_page - 1) * 10 + 3]);
            return true;
        } else {
            return false;
        }
    }
    else if (obj == ui->ly5) {
        if (ev->type() == QEvent::MouseButtonPress) {
            QPoint pont = cursor().pos();
            QToolTip::showText(pont, m_ly[(m_page - 1) * 10 + 4]);
            return true;
        } else {
            return false;
        }
    }
    else if (obj == ui->ly6) {
        if (ev->type() == QEvent::MouseButtonPress) {
            QPoint pont = cursor().pos();
            QToolTip::showText(pont, m_ly[(m_page - 1) * 10 + 5]);
            return true;
        } else {
            return false;
        }
    }
    else if (obj == ui->ly7) {
        if (ev->type() == QEvent::MouseButtonPress) {
            QPoint pont = cursor().pos();
            QToolTip::showText(pont, m_ly[(m_page - 1) * 10 + 6]);
            return true;
        } else {
            return false;
        }
    }
    else if (obj == ui->ly8) {
        if (ev->type() == QEvent::MouseButtonPress) {
            QPoint pont = cursor().pos();
            QToolTip::showText(pont, m_ly[(m_page - 1) * 10 + 7]);
            return true;
        } else {
            return false;
        }
    }
    if (obj == ui->ly9) {
        if (ev->type() == QEvent::MouseButtonPress) {
            QPoint pont = cursor().pos();
            QToolTip::showText(pont, m_ly[(m_page - 1) * 10 + 8]);
            return true;
        } else {
            return false;
        }
    }
    else if (obj == ui->ly10) {
        if (ev->type() == QEvent::MouseButtonPress) {
            QPoint pont = cursor().pos();
            QToolTip::showText(pont, m_ly[(m_page - 1) * 10 + 9]);
            return true;
        } else {
            return false;
        }
    }

    //翻下一页
    else if (obj == ui->next) {
        if (ev->type() == QEvent::MouseButtonPress && m_totalPage > 1 && m_page < m_totalPage) {
            m_changPage = true;
            m_page += 1;

            searchMesShow();
            return true;
        } else {
            return false;
        }
    }

    //翻上一页
    else if (obj == ui->last) {
        if (ev->type() == QEvent::MouseButtonPress && m_page > 1) {
            m_changPage = true;
            //将m_resultSort的迭代器加回到上一值
            //如果不是最后一页，直接将迭代器回退20，回到上一页
            if(m_page != m_totalPage)
            {
                for(int i = 0; i < 20; i++)
                    m_iterSort++;
            }
            else
            {
                //找到最后一页的文件数目，回退
                int back = m_resultSort.size() % 10;
                for(int i = 0; i < back + 10; i++)
                    m_iterSort++;
            }

            m_page -= 1;
            searchMesShow();
            return true;
        } else {
            return false;
        }
    }


    else {
        // pass the event on to the parent class
        return QMainWindow::eventFilter(obj, ev);
    }
}

//添加键盘事件，当按下回车，可以行驶功能
void MainWindow::keyPressEvent(QKeyEvent* ev)
{
    switch(ev->key())
    {
    //回车键
    case Qt::Key_Enter:
    case Qt::Key_Return:
         //当前在查寻
         if(m_isSearch)
         {
            searchMesShow();
         }
         //当前在推荐
         if(m_recommend)
        {
          recommMesShow();
        }
        break;
    default:
        QWidget::keyReleaseEvent(ev);
    }
}
