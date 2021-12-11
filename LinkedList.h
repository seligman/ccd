#pragma once

template <class T>
class LinkedListItem
{
public:
	T Data;
    LinkedListItem<T>* pNext;

    LinkedListItem()
    {
		Data.Init();
        pNext = NULL;
    }

    ~LinkedListItem()
    {
		Data.Clean();
    }
};

template <class T>
class LinkedList
{
public:
    LinkedList()
    {
        m_pHead = NULL;
        m_pTail = NULL;
		m_nCount = 0;
    }

	void Clear()
	{
		LinkedListItem<T> * pNext = m_pHead;

		m_pHead = NULL;
		m_pTail = NULL;

		while (pNext)
		{
			LinkedListItem<T> * pTemp = pNext;
			pNext = pTemp->pNext;
			XUNTRACK(pTemp);
			delete pTemp;
		}
	}

    ~LinkedList()
    {
		Clear();
    }

    LinkedListItem<T> * AddTail()
    {
		m_nCount++;

        LinkedListItem<T> * pNew = new LinkedListItem<T>();
		XTRACK(pNew);

        if (m_pTail)
        {
            m_pTail->pNext = pNew;
            m_pTail = pNew;
        }
        else
        {
            m_pHead = pNew;
            m_pTail = pNew;
        }
        
		return pNew;
    }

	int GetCount()
	{
		return m_nCount;
	}

    LinkedListItem<T> * GetHead()
    {
        return m_pHead;
    }

private:
    LinkedListItem<T> * m_pHead;
    LinkedListItem<T> * m_pTail;
	int					m_nCount;
};

class Result
{
public:
    bool            bHidden;
    bool            bOnlyDir;
    TCHAR*          szFullName;
    TCHAR*          szShortcut;

	void Init()
	{
        bOnlyDir = false;
        szFullName = NULL;
        szShortcut = NULL;
	}

	void Clean()
	{
		XUNTRACK(szFullName);
        delete[] szFullName;
		XUNTRACK(szShortcut);
        delete[] szShortcut;
        szFullName = NULL;
        szShortcut = NULL;
	}
};

