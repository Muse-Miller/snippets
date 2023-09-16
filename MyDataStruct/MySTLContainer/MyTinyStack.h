#pragma once
#include<iostream>

#ifndef __MY_TINY_STACK__
#define __MY_TINY_STACK__


#define DEBUG 1
template<typename ELEMTYPE,int capacity = 8>
class MyTinyStack
{
public:
	MyTinyStack()
	{
		m_capacity = capacity > 0 ? capacity : 8;
		m_elem_pool = new ELEMTYPE[m_capacity];
	}
	~MyTinyStack()
	{
		m_top = 0;
		m_size = 0;
		m_capacity = 0;
		while(m_elem_pool)
		{
			delete[] m_elem_pool;
			m_elem_pool = nullptr;
		}
	}

	ELEMTYPE& top()
	{
		return m_elem_pool[m_top];
	}
	void pop()
	{
		--m_top;
		--m_size;
	}
	void push(ELEMTYPE&& src)
	{
		if(m_size < m_capacity)
		{
			++m_size;
#ifdef DEBUG
			std::cout <<m_top<<":" << m_size << ":" << m_capacity << std::endl;
#endif
			m_elem_pool[m_top] = src;
			++m_top;
		}
		else
		{
			//
			std::cout << "stack is filled" << std::endl;
		}
		
	}
private:
	ELEMTYPE* m_elem_pool{nullptr};
	
	int m_top{ 0 };
	int m_size{ 0 };
	int m_capacity;
	const int m_default_capacity{ 8 };
};


#endif //__MY_TINY_STACK__