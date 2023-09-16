#include"undigrap.h"
#include"test.h"


#if 0
class A
{
public:
	virtual ~A() = 0;
	virtual inline void Show()
	{
		std::cout <<"A" << std::endl;
	}
};
class B :public A
{
public:
	void Show()
	{
		std::cout << "B" << std::endl;
	}
	~B() override{}
};
class A {
public:
	A(A& src)
	{
		//
	}
	A()
	{
		std::cout << "A()" << std::endl;
	}
	int a;
};
struct B {
	int m;
	operator int()
	{
		return m;
	}
};
#endif

#if 0
int func(int x)
{
	int y = 0;
	while (x)
	{
		y++;
		x = x & (x - 1);
	}
	return y;
}

#endif
int main()
{
#if 0
	//A a('a');
	//B b('b');

	shared_ptr<A> a2(new A('a'));
	shared_ptr<B> b2(new B('b'));

	//cout << "a2.use_count() = " << a2.use_count() << endl;
	//cout << "b2.use_count() = " << b2.use_count() << endl;

	a2->e = b2;
	//b2->e = a2;

	//cout << "a2.use_count() = " << a2.use_count() << endl;
	//cout << "b2.use_count() = " << b2.use_count() << endl;
	{
		//shared_ptr<A> a3(new A('a3'));
		//a3->e = b2;
		//cout << "b2.use_count() = " << b2.use_count() << endl;
	}
	//cout << "b2.use_count() = " << b2.use_count() << endl;
	a2->Func(b2);
#endif

#if 0
	std::cout << func(0) << std::endl;
	std::cout << func(12) << std::endl;
	std::cout << func(23) << std::endl;
	std::cout << func(100) << std::endl;
	std::cout << func(101) << std::endl;
	std::cout << func(9999999) << std::endl;


	//char c = 'A';
	//char lst[] = "hello";
	//int i = 0;
	//int lst2[] = { 0,1,2,3 };

	//std::cout << sizeof(lst) << std::endl;
	//std::cout << strlen(lst) << std::endl;

	//std::cout << sizeof(i) << std::endl;
	//std::cout << strlen() << std::endl;

	//std::cout << sizeof(lst2) << std::endl;
	//std::cout << strlen(lst2) << std::endl;
	//B b;
	//b.Show();
	//B* bp = new B();
	//std::shared_ptr<A> ptr(bp);
	//ptr = new B();
	//ptr->Show();
	//A a;
	//a.Show();
#endif
#if 0
	UndiGraph grap;
	grap.PushNode('A');
	grap.PushNode('B');
	grap.PushNode('C');
	grap.PushNode('D');
	grap.PushNode('E');
	/*
	*				A	-	B	-	F
	*				   \ /	|
	*				   / \	|
	*		E	-	C	-	D		G
	*/
	grap.PushEdge('A', 'B');
	grap.PushEdge('A', 'D');
	//grap.PushEdge('A', 'E');
	
	grap.PushEdge('B', 'D');
	grap.PushEdge('B', 'C');
	
	grap.PushEdge('C', 'D');
	grap.PushEdge('C', 'E');

	grap.PushNode('F');
	grap.PushEdge('F', 'B');

	grap.PushNode('G');
	grap.PushEdge('F', 'C');

	//grap.ShowNodes();
	//grap.ShowEdges();
	std::cout << "BFS:";
	grap.BFS();
	std::cout << std::endl;
	std::cout << "DFS:";
	grap.DFS();
#endif
}