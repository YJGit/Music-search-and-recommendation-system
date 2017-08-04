#ifndef INVERTEDDOCUMENT_H
#define INVERTEDDOCUMENT_H

#include<iostream>
#include<string>
#include<map>
#include"CharString.h"
#define M 31

using namespace std;
//InvDoc struct 
struct INVDOC
{
	int DocID;    //�ĵ�ID
	int times;    //���ʳ��ִ���
	struct INVDOC* next;
};

//InvDic class
class INVDIC
{
public:
	CharString term; //����
	int DF;      //���ʳ��ֵ��ĵ���
	int occur;   //���ʳ��ֵ��ܴ���
	struct INVDOC* head;  //�ĵ�����

public:
	INVDIC():DF(0), occur(0), head(NULL){term.StrAssign("");}
	~INVDIC()
	{
		delete head;
	}
	void sortDoc();           //�����ĵ��еĹؼ��ʳ��ִ�������
};

//B ��

//�Զ���ؼ���
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

//B���ṹ
typedef struct BTNode{
	int keynum;             //�ؼ��ָ���
	struct BTNode* parent; //ָ��˫��
	KeyType key[M+1];      //�ؼ��֣�0�Ų���
	struct BTNode* ptr[M+1]; //����ָ��
}BTNode, *BTree;


//B���Ĳ���,���������ѡ�����


//B-tree ����������
extern int BT_search(BTree T, KeyType K, BTree &p);
// ���Ѻ�������һ���ڵ�����Ϣ������
BTNode *split(BTree& p);
// ����ڵ㣬���� TΪ���ڵ㣬 K Ϊ������Ϣ,����true,�����ڷ���false
extern bool insert_BTree(BTree& T, KeyType K);
//��ȱ������������ڼ��� b-tree ��ȷ�ԡ�
extern void mid_order(BTree T);

//���ݷִʽ�B��
extern void createB_Tree(CharStringLink& cstrLink, BTree& tree, string filename, map<KeyType, INVDIC*>& Bmap);

#endif
