#pragma once

template <class T>
class Array
{
public:
	T * List;
	int ArrayLen;
	int Count;

	Array()
	{
		List = new T[100];
		XTRACK(List);
		ArrayLen = 100;
		Count = 0;
	}

	void Clean()
	{
		XUNTRACK(List);
		List = NULL;
		ArrayLen = 0;
		Count = 0;
	}

	~Array()
	{
		Clean();
	}

	void Add(T value)
	{
		if (ArrayLen == Count + 1)
		{
			T * temp = new T[ArrayLen * 2];
			XTRACK(temp);
			memcpy(temp, List, sizeof(T) * ArrayLen);
			XUNTRACK(List);
			delete[] List;
			List = temp;
			ArrayLen *= 2;
		}
		List[Count] = value;
		Count++;
	}
};

