#include<queue>
#include<fstream>
#include"BTreeAndDoc.h"

using namespace std;

//文档的排序,归并排序
void  INVDIC::sortDoc()
{
    //MergeSort(&head);
}


//KeyType运算符重载
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

//B-tree 的搜索函数
// 参数T ： B-Tree的根节点， K ：要查找的信息
//p ：如果找到 K，则 p 为所在的节点，如果没有找，则 p 为最后一次查找的节点
//p 传的是指针的引用
// 返回值为 0，为未找到；为其他值，为在节点中的位置
int BT_search(BTree T, KeyType K, BTree &p)
{
        BTNode *q;
        int i;
        p=q=T;

         while (q!=NULL)
        {
               p=q;

                // 在节点内查找，此处可以用二分法查找
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

// 分裂函数，当一个节点中信息数过多
// 需要分裂节点
// 参数p 为传引用
// 拆分后，分为 p和返回值 q 两个节点
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

         // 从mid 位置拆分， mid 值上调到父节点
         // mid 后为一部分， mid前为一部分
         for (j=1, k=mid+1; k<=M; k++)
        {
               q->key[j]=p->key[k];
               q->ptr[j]=p->ptr[k];
               if (q->ptr[j] != NULL)
                      q->ptr[j]->parent=q; // 注意此处拆分后，要修改父节点信息。
               j++;
        }

        q->keynum=M-mid;
        p->keynum=mid-1;
        q->parent=p->parent;

         return q;
}


// 插入节点，参数 T为父节点， K 为插入信息
bool insert_BTree(BTree &T, KeyType K)
{
    KeyType KMid = K;   //记录中间的变量
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

//根据分词结果建树,保存在tree中
void createB_Tree(CharStringLink& cstrLink, BTree& tree, int docId, map<KeyType, INVDIC*>& Bmap)
{
    LinkNode* p = cstrLink.head;

    //建B树
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

            //建文档链表
            INVDOC* doc = new INVDOC;
            doc ->DocID = docId;
            doc ->times = 1;
            doc ->next = NULL;

            //词典的建立
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

            //已经存在
            if(exsi)
            {
                INVDOC *q = Bmap[key]->head;
                //找到当前文档
                while(q ->next != NULL)
                    q = q ->next;

                //不是同一文档
                if(q ->DocID != docId)
                {
                    //建文档链表
                    INVDOC* doc = new INVDOC;
                    doc ->DocID = docId;
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
                //建文档链表
                INVDOC* doc = new INVDOC;
                doc ->DocID = docId;
                doc ->times = 1;
                doc ->next = NULL;

                //词典的建立
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


