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


//���ݣ����ű�ʾ��->��������������������������������������
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
			//���ڵ���������ţ�˵���ø��ڵ������������Ҫ��ջ
			MyStack::Push(tmp);
			//�ı�״̬������������
			flag = 1;
		}
		else if (',' == *expression)
		{
			//�ı�״̬������������
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
				//����ջ�����ڵ��������
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
				//����ջ�����ڵ��������
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
			//�ı�״̬������������
			flag = 1;
		}
		else if (',' == *expression)
		{
			//�ı�״̬������������
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


//���ݣ����շ�->����������[���շ�]������[���շ�]������������
//���շ��������ַ� #
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
		cout << setw(PLEN) << setfill(' ') << "�ݹ�ʵ��ǰ�������";
		PreVisit(bt);
	}
	else
	{
		cout << setw(PLEN) << setfill(' ') << "�ǵݹ�ʵ��ǰ�������";
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
		cout << setw(PLEN) <<setfill(' ') << "�ݹ�ʵ�����������";
		MidVisit(bt);
	}
	else
	{
		cout << setw(PLEN) <<setfill(' ') << "�ǵݹ�ʵ�����������";
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
		cout << setw(PLEN) << setfill(' ') << "�ݹ�ʵ�ֺ��������";
		LastVisit(bt);
	}
	else
	{
		cout << setw(PLEN) << setfill(' ') << "�ǵݹ�ʵ�ֺ��������";
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
	cout << "�������:";
	LevelVisit(bt);
}


//���ֱ�����ʽ�ĵݹ�ʵ��
void AllInOne(BTNode* root)
{
	if (root == nullptr)return;
	//ǰ�����
	//std::cout << root->data << std::endl;

	AllInOne(root->lson);

	//�������
	//std::cout << root->data << std::endl;

	AllInOne(root->rson);

	//�������
	//std::cout << root->data << std::endl;
}












