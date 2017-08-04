#ifndef LINKSORT_H
#define LINKSORT_H

#include<iostream>
#include"InvertedDocument.h"

using namespace std;
	 
/* 
*归并函数定义 
*/
extern struct INVDOC* SortedMerge(struct INVDOC* a, struct INVDOC* b);
extern void FrontBackSplit(struct INVDOC* source,
	          struct INVDOC** frontRef, struct INVDOC** backRef);
extern void MergeSort(struct INVDOC** headRef);

#endif