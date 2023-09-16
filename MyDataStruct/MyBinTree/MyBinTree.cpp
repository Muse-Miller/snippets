#include "MyBinTree.h"
#include <iostream>
#include <iomanip>
#include<list>
using namespace std;


static class MyStack
{
public:
	static void Push(BTNode* bt)
	{
		myStack[top] = bt;
		++top;
	}
	static BTNode* GetTop()
	{
		return myStack[top - 1];
	}
	static void Pop()
	{
		if (0 != top) --top;
	}
	static int IsEmpty()
	{
		if (0 == top)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}

private:
	const static int STACKSIZE = 32;
	static BTNode* myStack[STACKSIZE];
	static int top;
};
BTNode* MyStack::myStack[STACKSIZE] = { nullptr };
int MyStack::top = 0;


//根据：括号表示法->（树根（左子树，右子树））：创建二叉树
BTNode* CreateBinTree(const char* expression)
{
	BTNode* root = new BTNode;
	root->data = NULL;
	root->lson = nullptr;
	root->rson = nullptr;
	//MyStack::Push(root);

	BTNode* tmp = root;
	int flag = 0;
	//
	while ('\0' != *expression)
	{
		if ('(' == *expression)
		{
			//根节点后有左括号，说明该根节点包含子树，需要入栈
			MyStack::Push(tmp);
			//改变状态至创建左子树
			flag = 1;
		}
		else if (',' == *expression)
		{
			//改变状态至创建右子树
			flag = 2;
		}
		else if (')' == *expression)
		{
			MyStack::Pop();
		}
		else
		{
			if (0 == flag)
			{
				//pass
			}
			else if (1 == flag)
			{
				//cout <<"L:\t" << *expression << endl;
				//创建栈顶根节点的左子树
				tmp = new BTNode;
				tmp->data = *expression;
				tmp->lson = nullptr;
				tmp->rson = nullptr;

				MyStack::GetTop()->lson = tmp;
				flag = 0;
			}
			else if(2 == flag)
			{
				//cout << "R:\t" << *expression << endl;
				//创建栈顶根节点的右子树
				tmp = new BTNode;
				tmp->data = *expression;
				tmp->lson = nullptr;
				tmp->rson = nullptr;

				MyStack::GetTop()->rson = tmp;
				flag = 0;
			}
			else
			{
				//unarivable
			}
		}
		++expression;
	}
	return root->lson;
}

BTNode* CreateBinTree2(const char* expression)
{
	BTNode* root = nullptr;
	BTNode* tmp = nullptr;
	int flag = 0;
	//
	while ('\0' != *expression)
	{
		if ('(' == *expression)
		{
			MyStack::Push(tmp);
			//改变状态至创建左子树
			flag = 1;
		}
		else if (',' == *expression)
		{
			//改变状态至创建右子树
			flag = 2;
		}
		else if (')' == *expression)
		{
			MyStack::Pop();
		}
		else
		{
			tmp = new BTNode;
			tmp->data = *expression;
			tmp->lson = nullptr;
			tmp->rson = nullptr;

			if (root == nullptr)
			{
				root = tmp;
			}
			else
			{
				if (1 == flag)MyStack::GetTop()->lson = tmp;
				else if (2 == flag)MyStack::GetTop()->rson = tmp;
				else
				{
				}
			}
		}
		++expression;
	}
	return root;
}


//根据：补空法->树根左子树[补空法]右子树[补空法]：创建二叉树
//补空法的特殊字符 #
BTNode* CreateBinTree3(const char* &expression)
{
	BTNode* tmp = nullptr;

	if ('#' == *expression)
	{
		return nullptr;
	}
	tmp = new BTNode();
	tmp->data = *expression;
	tmp->lson = nullptr;
	tmp->rson = nullptr;
	tmp->lson = CreateBinTree3(++expression);
	tmp->rson = CreateBinTree3(++expression);
	return tmp;
}

#define PLEN 20

void PreVisit(BTNode* bt)
{
	if (bt)
	{
		cout << setw(2) << bt->data;
		PreVisit(bt->lson);
		PreVisit(bt->rson);
	}
}
void PreVisit2(BTNode* bt);
void PreVisitBinTree(BTNode* bt,int flag)
{
	if (0 == flag)
	{
		cout << setw(PLEN) << setfill(' ') << "递归实现前序遍历：";
		PreVisit(bt);
	}
	else
	{
		cout << setw(PLEN) << setfill(' ') << "非递归实现前序遍历：";
		//
	}
	cout << endl;
}

void MidVisit(BTNode* bt)
{
	if (bt)
	{
		MidVisit(bt->lson);
		cout << setw(2) << bt->data;
		MidVisit(bt->rson);
	}
}
void MidVisit2(BTNode* bt);
void MidVisitBinTree(BTNode* bt,int flag)
{
	if (0 == flag)
	{
		cout << setw(PLEN) <<setfill(' ') << "递归实现中序遍历：";
		MidVisit(bt);
	}
	else
	{
		cout << setw(PLEN) <<setfill(' ') << "非递归实现中序遍历：";
	}
	cout << endl;
}

void LastVisit(BTNode* bt)
{
	if (bt)
	{
		LastVisit(bt->lson);
		LastVisit(bt->rson);
		cout << setw(2) << bt->data;
	}
}
void LastVisit2(BTNode* bt)
{
	BTNode* p = nullptr;
	while (1 == MyStack::IsEmpty() || nullptr != bt)
	{
		while (bt)
		{
			MyStack::Push(bt);
			bt = bt->lson;
		}

		bt = MyStack::GetTop();
		if (nullptr != bt->rson && p != bt->rson)
		{
			bt = bt->rson;
		}
		else
		{
			cout << setw(2) << bt->data;
			MyStack::Pop();
			p = bt;
			bt = nullptr;
		}
	}
}
void LastVisitBinTree(BTNode* bt,int flag)
{
	if (0 == flag)
	{
		cout << setw(PLEN) << setfill(' ') << "递归实现后序遍历：";
		LastVisit(bt);
	}
	else
	{
		cout << setw(PLEN) << setfill(' ') << "非递归实现后序遍历：";
		LastVisit2(bt);
	}
	
	cout << endl;
}


void LevelVisit(BTNode* bt)
{
	list<BTNode*> rec;
	rec.push_back(bt);
	BTNode* p = nullptr;
	while (!rec.empty())
	{
		p = rec.front();
		cout << p->data;
		if(p->lson)rec.push_back(p->lson);
		if(p->rson)rec.push_back(p->rson);
		rec.pop_front();
	}
}
void LevelVisitBinTree(BTNode* bt, int flag)
{
	cout << "层序遍历:";
	LevelVisit(bt);
}


//三种遍历方式的递归实现
void AllInOne(BTNode* root)
{
	if (root == nullptr)return;
	//前序遍历
	//std::cout << root->data << std::endl;

	AllInOne(root->lson);

	//中序遍历
	//std::cout << root->data << std::endl;

	AllInOne(root->rson);

	//后序遍历
	//std::cout << root->data << std::endl;
}












