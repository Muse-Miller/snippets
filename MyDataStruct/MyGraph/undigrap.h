#ifndef __MY_UNDIGRAPH__
#define __MY_UNDIGRAPH__


/*
* ����		������ͼ
* �洢��ʽ	���ڽӱ�
*/
#include<iostream>
#include<list>
#include<memory>
#include <map>
#include <deque>
#include <stack>

class Node;
class UndiEdge;

//�ڵ����
//�ڵ�������ݡ�����ߵļ���
class Node
{
public:
	Node(char data):m_data(data){}
	void SetData(char data){
		m_data = data;
	}
	char GetData() { return m_data; }
	void AddEdge(std::shared_ptr<UndiEdge> edge)
	{
		m_edges.push_back(edge);
	}
	void DelEdge(std::shared_ptr<UndiEdge> edge);
#if 1
	std::list<std::shared_ptr<Node> > GetAllNodes();
#endif
#if 0
	std::list<std::shared_ptr<Node>> GetAllNodes()
	{
		std::list<std::shared_ptr<Node> > ret;
		for (auto ch : m_edges)
		{
			ret.push_back(ch->GetAnother());
		}
		return ret;
	}
#endif
	void ShowEdges();
private:
	char m_data;
	std::list<std::shared_ptr<UndiEdge> >m_edges;
};

//�߶���
//���������ڵ㡢Ȩ��
class UndiEdge
{
public:
	UndiEdge(std::shared_ptr<Node> one, std::shared_ptr<Node> another,int weight=1)
		:m_one(one),m_another(another),m_weight(weight)
	{}
	void Show()
	{
		std::cout << m_one->GetData();
		std::cout << "<-->";
		std::cout << m_another->GetData();
		std::cout << std::endl;
	}
	std::shared_ptr<Node> GetAnother() { return m_another; }
private:
	std::shared_ptr<Node> m_one;
	std::shared_ptr<Node> m_another;
	int m_weight;
};

//����ͼ
class UndiGraph
{
public:
	UndiGraph() {}
	~UndiGraph() {}
	//=====================================================================
	//��������
	void PushNode(char data)
	{
		m_nodes.push_back(std::make_shared<Node>(data));
	}
	void PushEdge(char data_1, char data_2, int weight = 0)
	{
		std::shared_ptr<Node> one_ptr;
		std::shared_ptr<Node> another_ptr;
		if (!Exist(data_1,one_ptr) || !Exist(data_2,another_ptr)) { return; }
		one_ptr->AddEdge(std::make_shared<UndiEdge>(one_ptr, another_ptr));
		another_ptr->AddEdge(std::make_shared<UndiEdge>(another_ptr, one_ptr));
	}
	//���ͼ���Ƿ����ĳ���ڵ㣬����ptr�з�����ָ��
	bool Exist(char data,std::shared_ptr<Node>& ptr)
	{
		bool ret = false;
		for (auto& ch : m_nodes)
		{
			if (data == ch->GetData())
			{
				ptr = ch;
				ret = true;
			}
		}
		return ret;
	}

	void ShowNodes()
	{
		std::cout << "nodes:";
		for (auto& ch : m_nodes)
		{
			std::cout << ch->GetData() << " ";
		}
		std::cout << std::endl;
	}
	void ShowEdges()
	{
		for (auto& ch : m_nodes)
		{
			ch->ShowEdges();
		}
	}
	
	//=====================================================================
	//ͼ�ı���
	//������ȱ���
	void DFS()
	{
		//��ʼ���������ݽṹ
		std::map<std::shared_ptr<Node>, bool> rec;
		std::stack<std::shared_ptr<Node> > stk;
		for (auto ch : m_nodes)
		{
			rec.insert(std::pair<std::shared_ptr<Node>, bool>(ch, false));
		}
		//
		for (auto ch : m_nodes)
		{
			//����һ���ڵ����ջ��
			auto ptr = rec.find(ch);
			if (ptr == rec.end())continue;
			if (ptr->second) continue;

			//���ڵ���ջ��������ѷ���
			stk.push(ch);
			ptr->second = true;
			do
			{
				std::cout << stk.top()->GetData() << " ";
				//�������ӽڵ�
				std::list<std::shared_ptr<Node> > children
					= stk.top()->GetAllNodes();
				//�������ڵ�
				auto ptr2 = rec.find(stk.top());
				ptr2->second = true;
				stk.pop();
				//��δ���ʵ��ӽڵ�ȫ����ջ,�ڵ�һ����ջ�����������Ϊ�ѷ���
				for (auto ch : children)
				{
					auto ptr3 = rec.find(ch);
					if (ptr3->second)continue;
					//std::cout << ch->GetData() << " ";
					stk.push(ptr3->first);
					ptr3->second = true;
				}
				//std::cout << std::endl;
			} while (!stk.empty());

			//stk.push(ptr->first);
			//ptr->second = true;

			//while (!stk.empty())
			//{
			//	//���ʱ��ڵ�
			//	std::cout << stk.top()->GetData() << " ";
			//	auto ptr2 = rec.find(stk.top());
			//	ptr2->second = true;
			//	//�����ӽڵ�ѹ��ջ��
			//	std::list<std::shared_ptr<Node> > children = stk.top()->GetAllNodes();
			//	stk.pop();
			//	for (auto ch : children)
			//	{
			//		auto ptr3 = rec.find(ch);
			//		if (ptr3->second)continue;
			//		std::cout << ch->GetData() << " ";
			//		stk.push(ptr3->first);
			//		//ptr3->second = true;
			//	}
			//	std::cout << std::endl;
			//}
		}
	}
	//������ȱ���
	void BFS()
	{
		std::map<std::shared_ptr<Node>, bool> rec;
		std::deque<std::shared_ptr<Node> > deq;

		for (auto ch : m_nodes)
		{
			rec.insert(std::pair<std::shared_ptr<Node>,bool>(ch, false));
		}
		for (auto ch : m_nodes)
		{
			auto ptr = rec.find(ch);
			if (ptr == rec.end())continue;
			if (ptr->second) continue;
			
			//���ý��������
			deq.push_back(ch);
			ptr->second = true;

			while (!deq.empty())
			{
				auto node = deq.front();
				std::cout << node->GetData() << " ";
				auto ptr2 = rec.find(node);

				//�����ӽڵ�
				std::list<std::shared_ptr<Node> > children = node->GetAllNodes();
				for (auto ch : children)
				{
					auto ptr3 = rec.find(ch);
					if (ptr3->second)continue;
					deq.push_back(ptr3->first);
					ptr3->second = true;
				}
				deq.pop_front();
			}
		}
	}

	//=====================================================================
	//���·��
	//Dijkstra�㷨����Դ���·����
	//Ford�㷨
	//Floyed�㷨��������

	//=====================================================================
	//��С������
	//prim�㷨
	//kruskal�㷨

private:
	std::list<std::shared_ptr<Node> > m_nodes;
};





#endif//__MY_UNDIGRAPH__


#if 0


#endif