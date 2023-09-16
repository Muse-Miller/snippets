#pragma 
#include<iostream>
#include<memory>

#include <map>
#include <deque>
#include <stack>
#include<list>

using namespace std;
class B;
#if 0
class A
{
public:
	A(char d) { data = d; e = nullptr; cout << "A(char)" << endl; }
	~A() { cout << "~A()" << endl; }

	void Func(shared_ptr<B> e)
	{
		this->e = e;
		e->Func();
	}
	char data;
	shared_ptr<B> e;
};
class B
{
public:
	B(char d) { data = d; e = nullptr; cout << "B(char)" << endl; }
	~B() { cout << "~B()" << endl; }

	void Func()
	{
		cout << "A->B->Func()" << endl;
	}
	char data;
	shared_ptr<A> e;
};
#endif