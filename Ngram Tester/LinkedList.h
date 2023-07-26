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

	int GetCount()
	{
		return m_nCount;
	}

    ~LinkedList()
    {
		LinkedListItem<T> * pNext = m_pHead;

		m_pHead = NULL;
		m_pTail = NULL;

		while (pNext)
		{
			LinkedListItem<T> * pTemp = pNext;
			pNext = pTemp->pNext;
			delete pTemp;
		}
    }

    LinkedListItem<T> * AddTail()
    {
        LinkedListItem<T> * pNew = new LinkedListItem<T>();

		m_nCount++;

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

    LinkedListItem<T> * GetHead()
    {
        return m_pHead;
    }

private:
	int					m_nCount;
    LinkedListItem<T> * m_pHead;
    LinkedListItem<T> * m_pTail;
};
