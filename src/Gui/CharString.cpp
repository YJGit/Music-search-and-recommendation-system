//definition of CharString
#include<iostream>
#include<string>
#include<fstream>
#include"CharString.h"
#include"MusicInfo.h"

using namespace std;

CharString::CharString()
{
    ch = NULL;
    length = 0;
}

CharString::CharString(const CharString& cStr)
{
        ch = new char[cStr.length + 1];
        int len = cStr.length, i = 0;
        for(i = 0; i < len; i++)
            ch[i] = cStr.ch[i];
        ch[i] = '\0';
        length = cStr.length;
}

CharString::~CharString()
{
    if(ch && length != 0)
    {
        delete[] ch;
        ch = NULL;
        length = 0;
    }
}

sta CharString::StrAssign(string str)
{
    if(ch)
    {
        delete[] ch;
    }

    int size = str.length();
    if(!size)
        return ERROR;

    else
    {
        if(!(ch = new char[size + 1]))
            return ERROR;
        int i = 0;
        for(i = 0; i < size; i++)
            ch[i] = str[i];
        ch[i] = '\0';
        length = size;
    }
    return OK;
}

int CharString::StrLength()
{
    return length;
}

sta CharString::Concat(CharString s1, CharString s2)
{
    if(ch)
    {
        delete[] ch;
    }

    if(!(ch = new char[s1.length+s2.length + 1]))
        return ERROR;
    int i = 0;
    for(i = 0; i < s1.length; i++)
        ch[i] = s1.ch[i];
    length = s1.length + s2.length;
    for(i = s1.length; i < length; i++)
        ch[i] = s2.ch[i - s1.length];
    ch[i] = '\0';
    return OK;
}

sta CharString::SubString(int pos, int len)
{
    if(pos < 0 || pos > length - 1 || len < 0 || len > length - pos + 1)
        return ERROR;
    if(!ch)
        return ERROR;

    CharString charstrCopy;
    charstrCopy.ch = new char[length + 1];
    int i = 0;
    for(i = 0; i < length;i++)
        charstrCopy.ch[i] = ch[i];
    ch[i] = '\0';
    charstrCopy.length = length;

    delete[] ch;

    ch = new char[len + 1];
    for(i = 0; i < len; i++)
        ch[i] = charstrCopy.ch[i + pos];
    ch[i] = '\0';
    length = len;

    return OK;
}

int CharString::indexOf(string str)
{
    int size = str.length();
    if(length < size)
        return ERROR;
    int j = 0;
    int sameChar = 0;
    for(int i = 0; i < length; i++)
    {
        if(ch[i] == str[j])
        {
            sameChar++;
            j++;
        }
        else
        {
            sameChar = 0;
            j = 0;
        }

        if(sameChar == size)
            return (i - size + 1);
    }
    return ERROR;
}

bool CharString::findString(string s)
{
    int len = s.length();
    if(length < len)
        return false;
    int j = 0, sameChar = 0;
    for(int i = 0; i < length; i++)
    {
        if(ch[i] == s[j])
        {
            sameChar++;
            j++;
        }
        else
        {
            sameChar = 0;
            j = 0;
        }
        if(sameChar == len)
            return true;
    }
    if(sameChar == len)
            return true;
    else
        return false;
}

sta CharString::deleteString(string s)
{
    if(!ch)
        return ERROR;
    int pos = indexOf(s);
    if(pos == ERROR)
        return ERROR;

    int len = s.length();
    while(pos != ERROR)
    {
        string str("");
        for(int i = 0; i < length; i++)
        {
          if(i >= pos && i < pos + len)
              continue;
          str.push_back(ch[i]);
        }
        StrAssign(str);
        pos = indexOf(s);
    }
    return OK;
}

string CharString::toString()
{
    string str("");
    for(int i = 0; i < length; i++)
        str += ch[i];
    return str;
}


CharString& CharString:: operator =(const CharString& cs)
{
    if(ch)
    {
        delete[] ch;
    }

    ch = new char[cs.length + 1];
    int i = 0;
    for(i = 0; i < cs.length;i++)
        ch[i] = cs.ch[i];
    ch[i] = '\0';
    length = cs.length;
    return *this;
}

bool CharString::operator ==(const CharString& cs)
{
    if(cs.length == length)
    {
        for(int i = 0; i < length; i++)
        {
            if(ch[i] != cs.ch[i])
                return false;
        }
        return true;
    }
    return false;
}

//definition of CharStringLink Class
void CharStringLink::createLink(CharString& cStr)
{
    if(head == NULL)
    {
        head = new LinkNode;
        head->cStr = cStr;
        current = head;
        current ->next = NULL;
    }

    else
    {
        LinkNode *node = new LinkNode;
        if(!node)
            return;
        node ->cStr = cStr;
        current ->next = node;
        current = node;
        current ->next = NULL;
    }
}

void CharStringLink::destroyLink()
{
    LinkNode *p = head, *q = p;
    while(p != NULL)
    {
        p = p ->next;
        delete q;
        q = p;
    }
}

void CharStringLink::printListToFile(string fileName)
{
    ofstream output(fileName);
    if(output.fail())
        return;
    LinkNode *p = head;
    while(p != NULL)
    {
        output << p ->cStr;
        p = p ->next;
    }
}


sta CharStringLink::seekLink(CharString cstr)
{
    LinkNode *p = head;
    while(p != NULL)
    {
        if(p ->cStr == cstr)
        {
            return OK;
        }
        p = p->next;
    }
    return ERROR;
}

void  CharStringLink::deleteLink(CharString cstr)
{
    if(!seekLink(cstr))
        return;
    LinkNode *p = head, *q = p;
    if(head ->cStr == cstr)
    {
        head = p ->next;
        delete p;
        return;
    }

    while(p != NULL)
    {
        if(p ->cStr == cstr)
        {
            q ->next = p ->next;
            delete p;
        }
        q = p;
        p = p->next;
    }
}

//class set definition
bool CharStringSet::operator <(const CharStringSet& cs)const
{
    if(length < cs.length)
            return true;
    else  if(length == cs.length)
        {
            if(oneWordAsci < cs.oneWordAsci)
                return true;
            else if(oneWordAsci == cs.oneWordAsci)
            {
                if(evenWordAsci < cs.evenWordAsci)
                    return true;
                else if(evenWordAsci == cs.evenWordAsci)
                {
                    if(threeWordAsci < cs.threeWordAsci)
                        return true;
                    else if(threeWordAsci == cs.threeWordAsci)
                    {
                        if(fourWordAsci < cs.fourWordAsci)
                            return true;
                        else if(fourWordAsci == cs.fourWordAsci)
                        {
                            if(fiveWordAsci < cs.fiveWordAsci)
                                return true;
                            else if(fiveWordAsci == cs.fiveWordAsci)
                            {
                                if(sixWordAsci < cs.sixWordAsci)
                                    return true;
                                else if(sixWordAsci == cs.sixWordAsci)
                                {
                                    if(sevenWordAsci < cs.sevenWordAsci)
                                        return true;
                                    else if(sevenWordAsci == cs.sevenWordAsci)
                                    {
                                        if(middleWordAsci < cs.middleWordAsci)
                                            return true;
                                        else if(middleWordAsci == cs.middleWordAsci)
                                        {
                                            if(lastWordAsci < cs.lastWordAsci)
                                                return true;
                                            else
                                                return false;
                                        }
                                        else
                                            return false;
                                    }
                                    else
                                        return false;
                                }
                                else
                                    return false;
                            }
                            else
                                return false;
                        }
                        else
                            return false;
                    }
                    else
                        return false;
                }
                else
                    return false;
            }
            else
                return false;
        }
        else
           return false;
}
