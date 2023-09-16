#include "undigrap.h"

void Node::DelEdge(std::shared_ptr<UndiEdge> edge)
{
	for (auto it = m_edges.begin(); it != m_edges.end(); ++it)
	{
		if ((*edge).GetAnother() == (*it)->GetAnother())
		{
			m_edges.erase(it);
		}
	}
}
#if 1
std::list<std::shared_ptr<Node>> Node::GetAllNodes()
{
	std::list<std::shared_ptr<Node> > ret;
	for (auto ch : m_edges)
	{
		ret.push_back(ch->GetAnother());
	}
	return ret;
}
#endif

void Node::ShowEdges()
{
	for (auto ch : m_edges)
	{
		ch->Show();
	}
}
