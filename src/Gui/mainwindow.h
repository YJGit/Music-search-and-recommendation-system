#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<iostream>
#include<string>
#include<map>
#include<set>
#include<QEvent>
#include<QPaintEvent>
#include"init.h"
#include"CharString.h"
#include"compare.h"
#include"MusicInfo.h"
#include"BTreeAndDoc.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //diferent function，查寻和推荐
    void on_actionSearch_triggered();
    void on_actionRecommend_triggered();

    //查寻信息显示函数
    void searchMesShow();
    void recommMesShow();

private:
    Ui::MainWindow *ui;

    //查寻和推荐的两个标志位
    bool m_isSearch;
    bool m_recommend;
    //词典
    set<CharStringSet, Compare> m_dicSet;
    //存储音乐信息
    map<int, MusicInfo> m_musicInfoMap;
    //存储分词和文档的关系
    map<KeyType, INVDIC*> m_Bmap;

    //B树索引
    BTree m_btree;
    //记录打印到哪里
    vector<veOut>::iterator m_iterSort;
    //当前的页数和总页数，当超过10时可翻页
    int m_page;
    int m_totalPage;
    //改页数的标志
    bool m_changPage;
    //存储完整的音乐歌词, 最多300条
    QString m_ly[300];
    //存储搜索要打印的音乐文档
    vector<veOut> m_resultSort;

    //查找英文歌时，显示推荐结果
    bool m_searToRec;
    //推荐时用的显示函数
    void recommendOut(int num, MusicInfo musicInfo);
    //记录推荐过的歌的id，避免重复打印
    vector<int> m_Haverec;
    //英文字母，用来判断
    char Engs[26];
    char Engb[26];

protected:
    //音乐信息的点击事件，点击查看去全部信息
    bool eventFilter(QObject *obj, QEvent *ev);
    //绘制界面图片
    void paintEvent(QPaintEvent *event);
    //添加键盘事件，当按下回车，可以行驶功能
    void keyPressEvent(QKeyEvent* ev);
};

#endif // MAINWINDOW_H
