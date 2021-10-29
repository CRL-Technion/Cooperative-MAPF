#include "CBSNode.h"


void CBSNode::clear()
{
	conflicts.clear();
	unknownConf.clear();
	conflictGraph.clear();
}

void CBSNode::printConflictGraph(int num_of_agents) const
{
	if (conflictGraph.empty())
		return;
	cout << "	Build conflict graph in " << *this << ": ";
	for (auto e : conflictGraph)
	{
		if (e.second == 0)
			continue;
		int i = e.first / num_of_agents;
		int j = e.first % num_of_agents;
		std::cout << "(" << i << "," << j << ")=" << e.second << ",";
	}
	cout << endl;
}

std::ostream& operator<<(std::ostream& os, const CBSNode& node)
{
	os << (node.root ? "root" : "regular") << " node " << node.time_generated << " (" << node.g_val + node.h_val << " = " << node.g_val << " + " <<
	   node.h_val << " ) with " << node.constraints.size() << " constraints, " << node.conflicts.size() + node.unknownConf.size() << " conflicts and " <<
	   node.paths.size() << " new paths, meetings set: ";
	for (int i = 0; i < node.meetings_set.size() - 1; ++i)
  		os << node.meetings_set[i] << ",";
	os << node.meetings_set[node.meetings_set.size() - 1];
	os << ", assignment index: " << node.assignment_idx;
	os << ", tie_breaking: " << node.tie_breaking;
	os << ", meetings_tb: " << node.meetings_tb;
	return os;
}

void CBSNode::printConstraints(int agent) const // print constraints on the given agent
{
	auto curr = this;
	while (curr->parent != nullptr)
	{
		int a, x, y, t;
		constraint_type type;
		for (auto constraint : curr->constraints)
		{
			tie(a, x, y, t, type) = curr->constraints.front();
			switch (type)
			{
			case constraint_type::LEQLENGTH:
			case constraint_type::POSITIVE_VERTEX:
			case constraint_type::POSITIVE_EDGE:
				cout << constraint << ",";
				break;
			case constraint_type::GLENGTH:
			case constraint_type::VERTEX:
			case  constraint_type::EDGE:
			case constraint_type::BARRIER:
			case constraint_type::POSITIVE_BARRIER:
			case constraint_type::RANGE:
			case constraint_type::POSITIVE_RANGE:
				if (a == agent)
					cout << constraint << ",";
				break;
			}
		}
		curr = curr->parent;
	}
}