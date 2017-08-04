#include<queue>
#include<fstream>
#include"InvertedDocument.h"
#include"LinkSort.h"

using namespace std;

//�ĵ�������,�鲢����
void  INVDIC::sortDoc()
{
	MergeSort(&head);
}


//KeyType���������
bool KeyType:: operator <(KeyType key)const
{
	if(len < key.len)
		return true;
	else if(len == key.len)
	{
		for(int i = 0; i < len; i++)
		{
			if(asci[i] > key.asci[i])
				return false;
			else if(asci[i] < key.asci[i])
				return true;
			else
				continue;
		}
		return false;
	}
	else 
		return false;
}

bool KeyType::operator >(KeyType key)
{
	if(len > key.len)
		return true;
	else if(len == key.len)
	{
		for(int i = 0; i < len; i++)
		{
			if(asci[i] > key.asci[i])
				return true;
			else if(asci[i] < key.asci[i])
				return false;
			else
				continue;
		}
		return false;
	}
	else
		return false;
}

bool KeyType::operator ==(KeyType key)
{
	if(len == key.len)
	{
		for(int i = 0; i < len; i++)
		{
			if(asci[i] != key.asci[i])
				return false;
		}
		return true;
	}
	else
		return false;
}

//B-tree ����������
// ����T �� B-Tree�ĸ��ڵ㣬 K ��Ҫ���ҵ���Ϣ
//p ������ҵ� K���� p Ϊ���ڵĽڵ㣬���û���ң��� p Ϊ���һ�β��ҵĽڵ�
//p ������ָ�������
// ����ֵΪ 0��Ϊδ�ҵ���Ϊ����ֵ��Ϊ�ڽڵ��е�λ��
int BT_search(BTree T, KeyType K, BTree &p)
{
        BTNode *q;
        int i;
        p=q=T;
        
         while (q!=NULL)
        {
               p=q;
               
                // �ڽڵ��ڲ��ң��˴������ö��ַ�����
                for (i=1; i<=q->keynum; i++)
               {
				   if (K == q ->key[i])
                           return i;              
                   else if (K > q->key[i])
                            continue;
                   else
                            break ;         
               }

               q=q->ptr[i-1];

        }
         return 0;
}

// ���Ѻ�������һ���ڵ�����Ϣ������
// ��Ҫ���ѽڵ�
// ����p Ϊ������
// ��ֺ󣬷�Ϊ p�ͷ���ֵ q �����ڵ�
BTNode *split(BTree &p)
{
        BTNode *q;
        int mid;
		int j, k;

        q=new BTNode;
        mid=(M+1)/2;
        q->ptr[0]=p->ptr[mid];
         if (q->ptr[0] != NULL)
               q->ptr[0]->parent=q;

         // ��mid λ�ò�֣� mid ֵ�ϵ������ڵ�
         // mid ��Ϊһ���֣� midǰΪһ����
         for (j=1, k=mid+1; k<=M; k++)
        {
               q->key[j]=p->key[k];
               q->ptr[j]=p->ptr[k];
               if (q->ptr[j] != NULL)
                      q->ptr[j]->parent=q; // ע��˴���ֺ�Ҫ�޸ĸ��ڵ���Ϣ��
               j++;
        }

        q->keynum=M-mid;
        p->keynum=mid-1;
        q->parent=p->parent;

         return q;
}


// ����ڵ㣬���� TΪ���ڵ㣬 K Ϊ������Ϣ
bool insert_BTree(BTree &T, KeyType K)
{
	KeyType KMid = K;   //��¼�м�ı���
    int i;
    BTNode *p=NULL, *s1=NULL, *s2=NULL;
    if (!BT_search(T, KMid, p))
    {
         while (p!=NULL)
         {
                p->key[0]=KMid;
                for (i=p->keynum; KMid < p->key[i]; i--)
                {
                      p->key[i+1]=p->key[i];
                      p->ptr[i+1]=p->ptr[i];
                }
                i+=1;
                p->key[i]=KMid;
                p->ptr[i]=s2;
                p->ptr[i-1]=s1;
                       
                if (++(p->keynum) < M)
                     break ;
               else
               {
                     s2=split(p);
                     s1=p;
                     KMid=p->key[p->keynum+1];                        
                     p=p->parent;           
               }

               if (p==NULL)
               {
					  p=new BTNode;
                      p->keynum=1;
                      p->key[1]=KMid;
                      p->ptr[0]=s1;
                      s1->parent=p;
                      p->ptr[1]=s2;
                      s2->parent=p;
                      p->parent=NULL;
                      T=p;
                      break ;         
                }
       }
	   return false;
   }
   else
	   return true;
}

//���ݷִʽ������,������tree��
void createB_Tree(CharStringLink& cstrLink, BTree& tree, string filename, map<KeyType, INVDIC*>& Bmap)
{
	filename = filename.substr(1);
	//ofstream out("1.txt");

	LinkNode* p = cstrLink.head;

	//��B��
	while(p != NULL)
	{
		int sum = 0;
		KeyType key;
		key.len = p ->cStr.length;
		key.asci = new int[key.len];
		for(int i = 0; i < key.len; i++)
		{
			key.asci[i] = p->cStr.ch[i];
			sum += key.asci[i];
		}

		if(tree == NULL)
		{
			tree = new BTNode;
			tree ->keynum = 1;
			tree ->key[1] = key;
			tree ->parent = NULL;
			tree->ptr[0] = NULL;
			tree ->ptr[1] = NULL;
			
			//���ĵ�����
			INVDOC* doc = new INVDOC;
			doc ->DocID = atoi(filename.c_str());
			doc ->times = 1;
			doc ->next = NULL;

			//�ʵ�Ľ���
			INVDIC* dic = new INVDIC;
			dic->DF++;
			dic->head = doc;
			dic->occur += doc ->times;
			dic->term = p ->cStr;

			Bmap[key] = dic;
		}
		
		else
		{
			bool exsi = insert_BTree(tree, key);
			
			//�Ѿ�����
			if(exsi)
			{
				INVDOC *q = Bmap[key]->head;
				//�ҵ���ǰ�ĵ�
				while(q ->next != NULL)
					q = q ->next;

				//����ͬһ�ĵ�
				if(q ->DocID != atoi(filename.c_str()))
				{
					//���ĵ�����
					INVDOC* doc = new INVDOC;
					doc ->DocID = atoi(filename.c_str());
					doc ->times = 1;
					doc ->next = NULL;
					Bmap[key]->DF++;

					q ->next = doc;
				}

				else
					q->times++;
				Bmap[key]->occur++;
			}

			else
			{
				//���ĵ�����
				INVDOC* doc = new INVDOC;
				doc ->DocID = atoi(filename.c_str());
				doc ->times = 1;
				doc ->next = NULL;

				//�ʵ�Ľ���
				INVDIC* dic = new INVDIC;
				dic ->DF++;
				dic->head = doc;
				dic->occur += doc ->times;
				dic->term = p ->cStr;

				Bmap[key] = dic;
			}
		}

		p = p->next;
	}
}

