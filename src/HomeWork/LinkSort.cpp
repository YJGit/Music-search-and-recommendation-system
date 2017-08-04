#include<iostream>
#include"LinkSort.h"

using namespace std;

/* ͨ���ı�ָ�������򣬶��������� */
void MergeSort(struct INVDOC** headRef)
{
     struct INVDOC* head = *headRef;
	 struct INVDOC* a;
	 struct INVDOC* b;
	 if ((head == NULL) || (head->next == NULL))
	 {
		 return;
	 }

	 
	 /* �ָ�����Ϊ 'a' ��  'b' ���������� */
	FrontBackSplit(head, &a, &b);
	MergeSort(&a);
	MergeSort(&b);
	 /* �ǵø���ͷָ�� */
	*headRef = SortedMerge(a, b);

}

/* �ϲ��������������� */
struct INVDOC* SortedMerge(struct INVDOC* a, struct INVDOC* b)
{
	  struct INVDOC* result = NULL;
	  if (a == NULL)
	     return(b);
	  else if (b==NULL)
	     return(a);
	  /* ʹ�õݹ���õķ��� */
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

/* ���ú��� */
/* �ָ�һ������Ϊ�����֣�ͨ��ָ�뷵�ؽ����ʹ�ÿ���ָ��  */
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
	    /* 'fast'�ƶ������ڵ�,  'slow' �ƶ�һ���ڵ� */
		 while (fast != NULL)
	    {
	      fast = fast->next;
	      if (fast != NULL)
	      {
	        slow = slow->next;
	        fast = fast->next;
	      }
	    }
	 
	    /* 'slow' ���м�λ�õ�ǰ�� */
	    *frontRef = source;
	    *backRef = slow->next;
	    slow->next = NULL;
	  }
}
