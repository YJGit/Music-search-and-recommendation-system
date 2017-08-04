#include<iostream>
#include"LinkSort.h"

using namespace std;

/* 通过改变指针来排序，而不是数据 */
void MergeSort(struct INVDOC** headRef)
{
     struct INVDOC* head = *headRef;
	 struct INVDOC* a;
	 struct INVDOC* b;
	 if ((head == NULL) || (head->next == NULL))
	 {
		 return;
	 }

	 
	 /* 分割链表为 'a' 和  'b' 两个子链表 */
	FrontBackSplit(head, &a, &b);
	MergeSort(&a);
	MergeSort(&b);
	 /* 记得跟新头指针 */
	*headRef = SortedMerge(a, b);

}

/* 合并两个已排序链表 */
struct INVDOC* SortedMerge(struct INVDOC* a, struct INVDOC* b)
{
	  struct INVDOC* result = NULL;
	  if (a == NULL)
	     return(b);
	  else if (b==NULL)
	     return(a);
	  /* 使用递归调用的方法 */
	  if (a->times <= b->times)
	  {
	     result = a;
	     result->next = SortedMerge(a->next, b);
	  }

	  else
	  {
	     result = b;
	     result->next = SortedMerge(a, b->next);
	  }
	  return(result);
}

/* 公用函数 */
/* 分割一个链表为两部分，通过指针返回结果，使用快慢指针  */
void FrontBackSplit(struct INVDOC* source,
	          struct INVDOC** frontRef, struct INVDOC** backRef)
{
	  struct INVDOC* fast;
	  struct INVDOC* slow;
	  if (source==NULL || source->next==NULL)
	  {
	    /* length < 2 */
	    *frontRef = source;
	    *backRef = NULL;
	  }
	  else
	  {
	    slow = source;
	    fast = source->next;
	    /* 'fast'移动两个节点,  'slow' 移动一个节点 */
		 while (fast != NULL)
	    {
	      fast = fast->next;
	      if (fast != NULL)
	      {
	        slow = slow->next;
	        fast = fast->next;
	      }
	    }
	 
	    /* 'slow' 在中间位置的前面 */
	    *frontRef = source;
	    *backRef = slow->next;
	    slow->next = NULL;
	  }
}
