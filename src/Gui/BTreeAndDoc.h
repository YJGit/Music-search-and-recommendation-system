#ifndef BTREEANDDOC_H
#define BTREEANDDOC_H

#include<iostream>
#include<string>
#include<map>
#include"CharString.h"
#define M 31

using namespace std;
//InvDoc struct
struct INVDOC
{
    int DocID;    //文档ID
    int times;    //单词出现次数
    struct INVDOC* next;
};

//InvDic class
class INVDIC
{
public:
    CharString term; //单词
    int DF;      //单词出现的文档数
    int occur;   //单词出现的总次数
    struct INVDOC* head;  //文档链表

public:
    INVDIC():DF(0), occur(0), head(NULL){term.StrAssign("");}
    ~INVDIC()
    {
        delete head;
    }
    void sortDoc();           //根据文档中的关键词出现次数排序
    //文档的创建，添加，查找均在createB_Tree这个函数内实现
};

//B 树

//自定义B树搜索的关键字
class KeyType
{
public:
    int len;
    int *asci;

public:
    KeyType():len(0), asci(NULL){};

    bool operator <(KeyType key)const;
    bool operator >(KeyType key);
    //KeyType& operator =(KeyType key);
    bool operator ==(KeyType key);

};

//B树结构
typedef struct BTNode{
    int keynum;             //关键字个数
    struct BTNode* parent; //指向双亲
    KeyType key[M+1];      //关键字，0号不用
    struct BTNode* ptr[M+1]; //子树指针
}BTNode, *BTree;


//B树的操作,搜索、分裂、插入


//B-tree 的搜索函数
extern int BT_search(BTree T, KeyType K, BTree &p);
// 分裂函数，当一个节点中信息数过多
BTNode *split(BTree& p);
// 插入节点，参数 T为父节点， K 为插入信息,存在true,不存在返回false
extern bool insert_BTree(BTree& T, KeyType K);
//深度遍历函数，用于检验 b-tree 正确性。
extern void mid_order(BTree T);

//根据分词建B树
extern void createB_Tree(CharStringLink& cstrLink, BTree& tree, int docId, map<KeyType, INVDIC*>& Bmap);

#endif // BTREEANDDOC_H
