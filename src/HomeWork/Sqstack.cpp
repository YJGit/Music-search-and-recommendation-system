//definition of Sqstack

#include"Sqstack.h"

using namespace std;

Sqstack::Sqstack()
{
	//an empty stack
	base = (CharString*)malloc(STACK_INIT_SIZE * sizeof(CharString));
	if(!base) return;
	top = base;
	base->ch = NULL;
	base ->length = 0;
	top ->ch = NULL;
	top ->length = 0;
	stacksize = STACK_INIT_SIZE;
}

Sqstack::~Sqstack()
{
	//destroy stack
	if(!StackEmpty())
	{
		free(base);
		base = top = NULL;
		stacksize = 0;
	}
}

Status Sqstack::StackEmpty()
{
	// if empty return true
	if(top == base)
		return OK;
	else
		return ERROR;
}

int Sqstack::StackLength()
{
	if(StackEmpty())
		return ERROR;
	else
	{
		int i = 0;
		CharString* p = base;
		while(p != top)
		{
			i++;
			p++;
		}
		return i;
	}
}

Status Sqstack::GetTop(CharString& e)
{
	if(StackEmpty())
		return ERROR;
	CharString* p = top;
	p--;
	e = *p;
	return OK;
}

Status Sqstack::Push(CharString& e)
{
	if(top - base >= stacksize)
	{
		base = (CharString*)realloc(base, (stacksize + STACKINCREMENT)* sizeof(CharString));
		if(!base) return ERROR;
		top = base + stacksize;
		stacksize += STACKINCREMENT;
	}
	*top = e;
	top++;
	top ->ch = NULL;
	top ->length = 0;
	return OK;
}

Status Sqstack::Pop(CharString& e)
{
	if(StackEmpty())
		return ERROR;
	e = *--top;
	return OK;
}