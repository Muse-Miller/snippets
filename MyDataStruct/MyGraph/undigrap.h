#ifndef __MY_UNDIGRAPH__
#define __MY_UNDIGRAPH__


/*
* 类型		：无向图
* 存储方式	：邻接表
*/
#include<iostream>
#include<list>
#include<memory>
#include <map>
#include <deque>
#include <stack>

class Node;
class UndiEdge;

//节点对象
//节点包含数据、无向边的集合
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

//边对象
//包含两个节点、权重
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

//无向图
class UndiGraph
{
public:
	UndiGraph() {}
	~UndiGraph() {}
	//=====================================================================
	//基础功能
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
	//检测图中是否存在某个节点，并在ptr中返回其指针
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
	//图的遍历
	//深度优先遍历
	void DFS()
	{
		//初始化各种数据结构
		std::map<std::shared_ptr<Node>, bool> rec;
		std::stack<std::shared_ptr<Node> > stk;
		for (auto ch : m_nodes)
		{
			rec.insert(std::pair<std::shared_ptr<Node>, bool>(ch, false));
		}
		//
		for (auto ch : m_nodes)
		{
			//先找一个节点加入栈中
			auto ptr = rec.find(ch);
			if (ptr == rec.end())continue;
			if (ptr->second) continue;

			//将节点入栈，并标记已访问
			stk.push(ch);
			ptr->second = true;
			do
			{
				std::cout << stk.top()->GetData() << " ";
				//查找其子节点
				std::list<std::shared_ptr<Node> > children
					= stk.top()->GetAllNodes();
				//弹出父节点
				auto ptr2 = rec.find(stk.top());
				ptr2->second = true;
				stk.pop();
				//将未访问的子节点全部入栈,节点一旦入栈，就立即标记为已访问
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
			//	//访问本节点
			//	std::cout << stk.top()->GetData() << " ";
			//	auto ptr2 = rec.find(stk.top());
			//	ptr2->second = true;
			//	//并将子节点压入栈中
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
	//广度优先遍历
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
			
			//将该结点加入队列
			deq.push_back(ch);
			ptr->second = true;

			while (!deq.empty())
			{
				auto node = deq.front();
				std::cout << node->GetData() << " ";
				auto ptr2 = rec.find(node);

				//访问子节点
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
	//最短路径
	//Dijkstra算法（单源最短路径）
	//Ford算法
	//Floyed算法（……）

	//=====================================================================
	//最小生成树
	//prim算法
	//kruskal算法

private:
	std::list<std::shared_ptr<Node> > m_nodes;
};





#endif//__MY_UNDIGRAPH__


#if 0


#endif