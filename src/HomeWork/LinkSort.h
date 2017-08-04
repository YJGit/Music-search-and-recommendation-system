#ifndef LINKSORT_H
#define LINKSORT_H

#include<iostream>
#include"InvertedDocument.h"

using namespace std;
	 
/* 
*�鲢�������� 
*/
extern struct INVDOC* SortedMerge(struct INVDOC* a, struct INVDOC* b);
extern void FrontBackSplit(struct INVDOC* source,
	          struct INVDOC** frontRef, struct INVDOC** backRef);
extern void MergeSort(struct INVDOC** headRef);

#endif