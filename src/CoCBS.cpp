#include <algorithm>    // std::shuffle
#include <random>      // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include "CoCBS.h"
#include "SIPP.h"
#include "SpaceTimeAStar.h"


// takes the paths_found_initially and UPDATE all (constrained) paths found for agents from curr to start
// also, do the same for ll_min_f_vals and paths_costs (since its already "on the way").
inline void CoCBS::updatePaths(CBSNode* curr)
{
	for (int i = 0; i < num_of_agents; i++)
		paths[i] = &paths_found_initially[curr->assignment_idx][i];
	vector<bool> updated(num_of_agents, false);  // initialized for false

	while (curr != nullptr)
	{
		for (auto & path : curr->paths)
		{
			if (!updated[path.first])
			{
				paths[path.first] = &(path.second);
				updated[path.first] = true;
			}
		}
		curr = curr->parent;
	}
}


void CoCBS::copyConflicts(const list<shared_ptr<Conflict >>& conflicts,
	list<shared_ptr<Conflict>>& copy, const list<int>& excluded_agents)
{
	for (const auto& conflict : conflicts)
	{
		bool found = false;
		for (auto a : excluded_agents)
		{
			if (conflict->a1 == a || conflict->a2 == a)
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			assert(!conflict->constraint1.empty());
			assert(!conflict->constraint2.empty());
			copy.push_back(conflict);
		}
	}
}


void CoCBS::findConflicts(CBSNode& curr, int a1, int a2)
{
	size_t min_path_length = paths[a1]->size() < paths[a2]->size() ? paths[a1]->size() : paths[a2]->size();
	int task_idx = getAgentTask(curr.assignment_idx, a1);
	Meeting m = curr.meetings[task_idx];
	int meeting_location, meeting_time, a, b, cost;
	tie(meeting_location, meeting_time, a, b, cost) = m;
	for (size_t timestep = 0; timestep < min_path_length; timestep++)
	{
		int loc1 = paths[a1]->at(timestep).location;
		int loc2 = paths[a2]->at(timestep).location;
		if (loc1 == loc2)
		{
			// check if this conflict is actually a meeting
			if (loc1 == meeting_location && meeting_time == timestep && (a1 == a && a2 == b || a1 == b && a2 == a))
				break; // This is a meeting, not a real conflict
			
			shared_ptr<Conflict> conflict(new Conflict());
			if (target_reasoning && paths[a1]->size() == timestep + 1)
			{
				conflict->targetConflict(a1, a2, loc1, timestep);
			}
			else if (target_reasoning && paths[a2]->size() == timestep + 1)
			{
				conflict->targetConflict(a2, a1, loc1, timestep);
			}
			else
			{
				conflict->vertexConflict(a1, a2, loc1, timestep);
			}
			assert(!conflict->constraint1.empty());
			assert(!conflict->constraint2.empty());
			curr.unknownConf.push_back(conflict);
		}
		else if (timestep < min_path_length - 1
				 && loc1 == paths[a2]->at(timestep + 1).location
				 && loc2 == paths[a1]->at(timestep + 1).location)
		{
			shared_ptr<Conflict> conflict(new Conflict());
			conflict->edgeConflict(a1, a2, loc1, loc2, (int)timestep + 1);
			assert(!conflict->constraint1.empty());
			assert(!conflict->constraint2.empty());
			curr.unknownConf.push_back(conflict); // edge conflict
		}
	}
	// TODO Nir don't check target conflicts or vertex conflicts on final location since we assume disappear-at-target
	return;
	if (paths[a1]->size() != paths[a2]->size())
	{
		int a1_ = paths[a1]->size() < paths[a2]->size() ? a1 : a2;
		int a2_ = paths[a1]->size() < paths[a2]->size() ? a2 : a1;
		int loc1 = paths[a1_]->back().location;
		for (size_t timestep = min_path_length; timestep < paths[a2_]->size(); timestep++)
		{
			int loc2 = paths[a2_]->at(timestep).location;
			if (loc1 == loc2)
			{
				shared_ptr<Conflict> conflict(new Conflict());
				if (target_reasoning)
					conflict->targetConflict(a1_, a2_, loc1, timestep);
				else
					conflict->vertexConflict(a1_, a2_, loc1, timestep);
				assert(!conflict->constraint1.empty());
				assert(!conflict->constraint2.empty());
				curr.unknownConf.push_front(conflict); // It's at least a semi conflict			
			}
		}
	}
}


void CoCBS::findConflicts(CBSNode& curr)
{
	clock_t t = clock();
	if (curr.parent != nullptr)
	{
		// Copy from parent、
		list<int> new_agents;
		for (const auto& p : curr.paths)
		{
			new_agents.push_back(p.first);
		}
		copyConflicts(curr.parent->conflicts, curr.conflicts, new_agents);
		copyConflicts(curr.parent->unknownConf, curr.unknownConf, new_agents);

		// detect new conflicts
		for (auto it = new_agents.begin(); it != new_agents.end(); ++it)
		{
			int a1 = *it;
			for (int a2 = 0; a2 < num_of_agents; a2++)
			{
				if (a1 == a2)
					continue;
				bool skip = false;
				for (auto it2 = new_agents.begin(); it2 != it; ++it2)
				{
					if (*it2 == a2)
					{
						skip = true;
						break;
					}
				}
				findConflicts(curr, a1, a2);
			}
		}
	}
	else
	{
		for (int a1 = 0; a1 < num_of_agents; a1++)
		{
			for (int a2 = a1 + 1; a2 < num_of_agents; a2++)
			{
				findConflicts(curr, a1, a2);
			}
		}
	}
	// curr.tie_breaking = (int)(curr.unknownConf.size() + curr.conflicts.size());
	runtime_detect_conflicts += (double) (clock() - t) / CLOCKS_PER_SEC;
}


shared_ptr<Conflict> CoCBS::chooseConflict(const CBSNode& node) const
{
	if (screen == 3)
		printConflicts(node);
	shared_ptr<Conflict> choose;
	if (node.conflicts.empty() && node.unknownConf.empty())
		return nullptr;
	else if (!node.conflicts.empty())
	{
		choose = node.conflicts.back();
		for (const auto& conflict : node.conflicts)
		{
			if (*choose < *conflict)
				choose = conflict;
		}
	}
	else
	{
		choose = node.unknownConf.back();
		for (const auto& conflict : node.unknownConf)
		{
			if (*choose < *conflict)
				choose = conflict;
		}
	}
	return choose;
}


void CoCBS::computePriorityForConflict(Conflict& conflict, const CBSNode& node)
{
    conflict.secondary_priority = 0; // random
    /*switch (conflict.type) // Earliest
    {
        case conflict_type::STANDARD:
        case conflict_type::RECTANGLE:
        case conflict_type::TARGET:
        case conflict_type::MUTEX:
            conflict.secondary_priority = get<3>(conflict.constraint1.front());
            break;
        case conflict_type::CORRIDOR:
            conflict.secondary_priority = min(get<2>(conflict.constraint1.front()),
                                              get<3>(conflict.constraint1.front()));
            break;
    }*/
}


void CoCBS::classifyConflicts(CBSNode& node)
{
	// Classify all conflicts in unknownConf
	while (!node.unknownConf.empty())
	{
		shared_ptr<Conflict> con = node.unknownConf.front();
		int a1 = con->a1, a2 = con->a2;
		int a, loc1, loc2, timestep;
		constraint_type type;
		tie(a, loc1, loc2, timestep, type) = con->constraint1.back();
		node.unknownConf.pop_front();


		bool cardinal1 = false, cardinal2 = false;
		int task_idx;
		Task task;
		Meeting m;
		if (timestep >= (int) paths[a1]->size())
			cardinal1 = true;
		else //if (!paths[a1]->at(0).is_single())
		{
			// update the initial constraints to match the actual goal location of this agent, 
			// with this specific task assignment
			task_idx = getAgentTask(node.assignment_idx, a1);
			m = node.meetings[task_idx];
			task = m_meeting_tables[node.assignment_idx][task_idx]->getTask();
			if (a1 % 2 == 0) // agent A // TODO Nir this should more generic
				initial_constraints[a1].goal_location = std::get<0>(m); // meeting location
			else // agent B // TODO Nir this should more generic
				initial_constraints[a1].goal_location = std::get<1>(task); // task goal
			mdd_helper.findSingletons(node, a1, *paths[a1], &task);
		}
		if (timestep >= (int) paths[a2]->size())
			cardinal2 = true;
		else //if (!paths[a2]->at(0).is_single())
		{
			// update the initial constraints to match the actual goal location of this agent, 
			// with this specific task assignment
			task_idx = getAgentTask(node.assignment_idx, a2);
			m = node.meetings[task_idx];
			task = m_meeting_tables[node.assignment_idx][task_idx]->getTask();
			if (a2 % 2 == 0) // agent A // TODO Nir this should more generic
				initial_constraints[a2].goal_location = std::get<0>(m); // meeting location
			else // agent B // TODO Nir this should more generic
				initial_constraints[a2].goal_location = std::get<1>(task); // task goal
			mdd_helper.findSingletons(node, a2, *paths[a2], &task);
		}

		if (type == constraint_type::EDGE) // Edge conflict
		{
			cardinal1 = paths[a1]->at(timestep).is_single() && paths[a1]->at(timestep - 1).is_single();
			cardinal2 = paths[a2]->at(timestep).is_single() && paths[a2]->at(timestep - 1).is_single();
		}
		else // vertex conflict or target conflict
		{
			if (!cardinal1)
				cardinal1 = paths[a1]->at(timestep).is_single();
			if (!cardinal2)
				cardinal2 = paths[a2]->at(timestep).is_single();
		}

		/*int width_1 = 1, width_2 = 1;

		if (paths[a1]->size() > timestep){
		  width_1 = paths[a1]->at(timestep).mdd_width;
		}

		if (paths[a2]->size() > timestep){
		  width_2 = paths[a2]->at(timestep).mdd_width;
		}
		con -> mdd_width = width_1 * width_2;*/

		if (cardinal1 && cardinal2)
		{
			con->priority = conflict_priority::CARDINAL;
		}
		else if (cardinal1 || cardinal2)
		{
			con->priority = conflict_priority::SEMI;
		}
		else
		{
			con->priority = conflict_priority::NON;
		}

		// Corridor reasoning
		auto corridor = corridor_helper.run(con, paths, node);
		if (corridor != nullptr)
		{
			corridor->priority = con->priority;
			computePriorityForConflict(*corridor, node);
			node.conflicts.push_back(corridor);
			continue;
		}

		// Target Reasoning
		if (con->type == conflict_type::TARGET)
		{
			computePriorityForConflict(*con, node);
			node.conflicts.push_back(con);
			continue;
		}

		// Rectangle reasoning
		if (rectangle_helper.strategy != rectangle_strategy::NR &&
			(int) paths[con->a1]->size() > timestep &&
			(int) paths[con->a2]->size() > timestep && //conflict happens before both agents reach their goal locations
			type == constraint_type::VERTEX && // vertex conflict
			con->priority != conflict_priority::CARDINAL) // not caridnal vertex conflict
		{
			auto mdd1 = mdd_helper.getMDD(node, a1, paths[a1]->size());
			auto mdd2 = mdd_helper.getMDD(node, a2, paths[a2]->size());
			auto rectangle = rectangle_helper.run(paths, timestep, a1, a2, mdd1, mdd2);
			if (rectangle != nullptr)
			{
				computePriorityForConflict(*rectangle, node);
				node.conflicts.push_back(rectangle);
				continue;
			}
		}

		// Mutex reasoning
		if (mutex_reasoning)
		{
			// TODO mutex reasoning is per agent pair, don't do duplicated work...
			auto mdd1 = mdd_helper.getMDD(node, a1, paths[a1]->size());
			auto mdd2 = mdd_helper.getMDD(node, a2, paths[a2]->size());

			auto mutex_conflict = mutex_helper.run(paths, a1, a2, node, mdd1, mdd2);

			if (mutex_conflict != nullptr && (*mutex_conflict != *con)) // ignore the cases when mutex finds the same vertex constraints
			{
				computePriorityForConflict(*mutex_conflict, node);
				node.conflicts.push_back(mutex_conflict);
				continue;
			}
		}

		computePriorityForConflict(*con, node);
		node.conflicts.push_back(con);
	}


	// remove conflicts that cannot be chosen, to save some memory
	removeLowPriorityConflicts(node.conflicts);
}


void CoCBS::removeLowPriorityConflicts(list<shared_ptr<Conflict>>& conflicts) const
{
	if (conflicts.empty())
		return;
	unordered_map<int, shared_ptr<Conflict> > keep;
	list<shared_ptr<Conflict>> to_delete;
	for (const auto& conflict : conflicts)
	{
		int a1 = min(conflict->a1, conflict->a2), a2 = max(conflict->a1, conflict->a2);
		int key = a1 * num_of_agents + a2;
		auto p = keep.find(key);
		if (p == keep.end())
		{
			keep[key] = conflict;
		}
		else if (*(p->second) < *conflict)
		{
			to_delete.push_back(p->second);
			keep[key] = conflict;
		}
		else
		{
			to_delete.push_back(conflict);
		}
	}

	for (const auto& conflict : to_delete)
	{
		conflicts.remove(conflict);
	}
}


bool CoCBS::findPathForSingleAgent(CBSNode* node, int ag, int lowerbound)
{
	clock_t t = clock();
	// build reservation table
	// CAT cat(node->makespan + 1);  // initialized to false
	// updateReservationTable(cat, ag, *node);
	// find a path
	// TODO Nir there's a lot of duplicated code (also from generateRoot())
	int task_idx = getAgentTask(node->assignment_idx, ag);
	Meeting m = node->meetings[task_idx];
	Task task = m_meeting_tables[node->assignment_idx][task_idx]->getTask();
	assert(ag == std::get<2>(task) || ag == std::get<4>(task));
	int	meeting_location = std::get<0>(m);
	int	meeting_time = std::get<1>(m); 
	int next_agentA_start_location, next_agentA_start_time;
	Path fullPath, agentAtoTaskStart, agentAtoMeeting, agentB_to_meeting, agentB_to_goal;

	if (ag % 2 == 0) // agent A
	{
		// Plan agent A to task start
		search_engines[ag]->setStartLocation(std::get<3>(task)); // TODO Nir should this be done inside the solver?
		search_engines[ag]->setGoalLocation(std::get<0>(task)); // TODO Nir should this be done inside the solver?
		initial_constraints[ag].goal_location = search_engines[ag]->goal_location; // TODO Nir IMPORTANT should initial_constraints be per assignment index???
		agentAtoTaskStart = search_engines[ag]->findPath(*node, initial_constraints[ag], paths, ag, 0);

		num_LL_expanded += search_engines[ag]->num_expanded;
		num_LL_generated += search_engines[ag]->num_generated;
		runtime_build_CT += search_engines[ag]->runtime_build_CT;
		runtime_build_CAT += search_engines[ag]->runtime_build_CAT;

		if (agentAtoTaskStart.size() == 0)
		{
			if (screen > 1)
				std::cout << "No solution agent A (" << ag << ") to task start" << endl;
			return false;
		}
		next_agentA_start_location = agentAtoTaskStart.back().location;
		next_agentA_start_time = agentAtoTaskStart.size() - 1;

		// Plan agent A to meeting
		search_engines[ag]->setStartLocation(next_agentA_start_location); // TODO Nir should this be done inside the solver?
		search_engines[ag]->setGoalLocation(meeting_location); // TODO Nir should this be done inside the solver?
		initial_constraints[ag].goal_location = search_engines[ag]->goal_location;
		agentAtoMeeting = search_engines[ag]->findPath(*node, initial_constraints[ag], paths, ag, 0, meeting_time, next_agentA_start_time);
		
		num_LL_expanded += search_engines[ag]->num_expanded;
		num_LL_generated += search_engines[ag]->num_generated;
		runtime_build_CT += search_engines[ag]->runtime_build_CT;
		runtime_build_CAT += search_engines[ag]->runtime_build_CAT;

		if (agentAtoMeeting.size() == 0)
		{
			if (screen > 1)
				std::cout << "No solution agent A (" << ag << ") to meeting" << endl;
			return false;
		}
		// put wait actions at meeting location
		while((int)agentAtoTaskStart.size() + agentAtoMeeting.size() - 1 <= meeting_time)
			agentAtoMeeting.emplace_back(meeting_location);
		//CAT cat(dummy_start->makespan + 1);  // initialized to false
		//updateReservationTable(cat, i, *dummy_start);
		fullPath = agentAtoTaskStart;
		fullPath.insert(fullPath.end(), agentAtoMeeting.begin() + 1, agentAtoMeeting.end());
	} else { // agentB
		// Plan agent B to meeting
		search_engines[ag]->setStartLocation(std::get<5>(task));
		search_engines[ag]->setGoalLocation(meeting_location);
		initial_constraints[ag].goal_location = search_engines[ag]->goal_location;
		agentB_to_meeting = search_engines[ag]->findPath(*node, initial_constraints[ag], paths, ag, 0, meeting_time);
		
		num_LL_expanded += search_engines[ag]->num_expanded;
		num_LL_generated += search_engines[ag]->num_generated;
		runtime_build_CT += search_engines[ag]->runtime_build_CT;
		runtime_build_CAT += search_engines[ag]->runtime_build_CAT;
		
		if (agentB_to_meeting.size() == 0)
		{
			if (screen > 1)
				std::cout << "No solution agent B (" << ag << ") to meeting" << endl;
			return false;
		}
		// put wait actions at meeting location
		while((int)agentB_to_meeting.size() <= meeting_time)
			agentB_to_meeting.emplace_back(meeting_location);

		// Plan agent B to goal
		search_engines[ag]->setStartLocation(meeting_location);
		search_engines[ag]->setGoalLocation(std::get<1>(task));
		initial_constraints[ag].goal_location = search_engines[ag]->goal_location;
		agentB_to_goal = search_engines[ag]->findPath(*node, initial_constraints[ag], paths, ag, 0, -1, meeting_time);
		
		num_LL_expanded += search_engines[ag]->num_expanded;
		num_LL_generated += search_engines[ag]->num_generated;
		runtime_build_CT += search_engines[ag]->runtime_build_CT;
		runtime_build_CAT += search_engines[ag]->runtime_build_CAT;
		
		if (agentB_to_goal.size() == 0)
		{
			if (screen > 1)
				std::cout << "No solution agent B (" << ag << ") to goal" << endl;
			return false;
		}
		// fullPath.resize(agentB_to_meeting.size());
		fullPath = agentB_to_meeting;
		fullPath.insert(fullPath.end(), agentB_to_goal.begin() + 1, agentB_to_goal.end());
	}

	// Path new_path = search_engines[ag]->findPath(*node, initial_constraints[ag], paths, ag, lowerbound);
	Path new_path = fullPath;
	num_LL_expanded += search_engines[ag]->num_expanded;
	num_LL_generated += search_engines[ag]->num_generated;
	runtime_build_CT += search_engines[ag]->runtime_build_CT;
	runtime_build_CAT += search_engines[ag]->runtime_build_CAT;
	runtime_path_finding += (double) (clock() - t) / CLOCKS_PER_SEC;
	if (!new_path.empty())
	{
		// assert(!isSamePath(*paths[ag], new_path)); // TODO Nir even though a meeting has changed, the path may remain the same
		node->paths.emplace_back(ag, new_path);
		node->g_val = node->g_val - (int) paths[ag]->size() + (int) new_path.size();
		node->soc = node->soc - (int) paths[ag]->size() + (int) new_path.size();
		paths[ag] = &node->paths.back().second;
		node->makespan = max(node->makespan, new_path.size() - 1);
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * @brief Generates a child node based on a parent. Attributes are copied from parent, and paths are re-planned.
 * 
 * @param node child node, constructed with CBSNode(), with possibly added constraint due to conflicts in parent.
 * @param parent parent node.
 * @return true if child has valid solution.
 * @return false otherwise.
 */
bool CoCBS::generateChild(CBSNode* node, CBSNode* parent)
{
	clock_t t1 = clock();
	node->parent = parent;
	node->g_val = parent->g_val;
	node->soc = parent->soc;
	node->makespan = parent->makespan;
	node->depth = parent->depth + 1;
	// copy meetings from parent
	node->meetings = parent->meetings;
	node->meetings_set = parent->meetings_set;
	node->meetings_tb = parent->meetings_tb; // regular nodes meetings tie breaker is always the same as the parent
	node->assignment_idx = parent->assignment_idx;
	node->assignment = parent->assignment;
	int agent, x, y, t;
	constraint_type type;
	assert(node->constraints.size() > 0);
	tie(agent, x, y, t, type) = node->constraints.front();

	if (type == constraint_type::LEQLENGTH)
	{
		assert(node->constraints.size() <= 2);
		if ((int)node->constraints.size() == 2) // generated by corridor-target conflict
		{
			int a = std::get<0>(node->constraints.back()); // it is a G-length constraint or a range constraint on this agent
			int lowerbound = (int)paths[a]->size() - 1;
			if (!findPathForSingleAgent(node, a, lowerbound))
			{
				runtime_generate_child += (double)(clock() - t1) / CLOCKS_PER_SEC;
				return false;
			}
		}
		for (int ag = 0; ag < num_of_agents; ag++)
		{
			if (ag == agent)
			{
				continue;
			}
			for (int i = t; i < (int) paths[ag]->size(); i++)
			{
				if (paths[ag]->at(i).location == x)
				{
					int lowerbound = (int) paths[ag]->size() - 1;
					if (!findPathForSingleAgent(node, ag, lowerbound))
					{
						runtime_generate_child += (double) (clock() - t1) / CLOCKS_PER_SEC;
						return false;
					}
					break;
				}
			}
		}
	}
	else if (type == constraint_type::POSITIVE_VERTEX)
	{
		assert(node->constraints.size() == 1);
		for (const auto& constraint : node->constraints)
		{
			tie(agent, x, y, t, type) = constraint;
			for (int ag = 0; ag < num_of_agents; ag++)
			{
				if (ag == agent)
				{
					continue;
				}
				if (getAgentLocation(ag, t) == x)
				{
					if (!findPathForSingleAgent(node, ag, (int) paths[ag]->size() - 1))
					{
						runtime_generate_child += (double) (clock() - t1) / CLOCKS_PER_SEC;
						return false;
					}
				}
			}
		}

	}
	else if (type == constraint_type::POSITIVE_EDGE)
	{
		assert(node->constraints.size() == 1);
		for (int ag = 0; ag < num_of_agents; ag++)
		{
			if (ag == agent)
			{
				continue;
			}
			int curr = getAgentLocation(ag, t);
			int prev = getAgentLocation(ag, t - 1);
			if (prev == x || curr == y ||
				(prev == y && curr == x))
			{
				if (!findPathForSingleAgent(node, ag, (int) paths[ag]->size() - 1))
				{
					runtime_generate_child += (double) (clock() - t1) / CLOCKS_PER_SEC;
					return false;
				}
			}
		}

	}
	else
	{
		int lowerbound = (int) paths[agent]->size() - 1;
		if (!findPathForSingleAgent(node, agent, lowerbound))
		{
			runtime_generate_child += (double) (clock() - t1) / CLOCKS_PER_SEC;
			return false;
		}
	}

	assert(!node->paths.empty());
	findConflicts(*node);
	heuristic_helper.computeQuickHeuristics(*node);
	runtime_generate_child += (double) (clock() - t1) / CLOCKS_PER_SEC;
	return true;
}


bool CoCBS::generateRootNode(CBSNode* node, CBSNode* parent, int task_idx=-1)
{
	clock_t t1 = clock();
	if (task_idx != -1) // only set parent for root nodes that are not new assignment nodes
		node->parent = parent;
	node->g_val = parent->g_val;
	node->soc = parent->soc;
	node->makespan = parent->makespan;
	node->depth = 0;
	node->changed_task_idx = task_idx;
	node->meetings_tb = parent->meetings_tb;
	// node->tie_breaking = 1;

	assert(node->constraints.size() == 0);

	if (lazy_expansion)
	{
		if (task_idx == -1)
		{
			// this is the case where we changed the assignment and need to re-plan all agents
			vector<MeetingsTable*> m_tables = m_meeting_tables[node->assignment_idx];
			int node_cost = 0;
			for (int task_idx = 0; task_idx < num_of_tasks; ++task_idx)
			{
				node_cost += std::get<4>(m_tables[task_idx]->getNextMeeting());
			}

			// update the cost according to the new assignment
			node->g_val = node_cost;
			node->soc = node_cost;
			// node->makespan = max(node->makespan, new_path.size() - 1); // TODO Nir can we discard this?
			node->solved = false;
			node->tie_breaking = parent->tie_breaking; //std::numeric_limits<int>::max(); // TODO Nir take the parent tie_breaking as an approximation?
			runtime_generate_child += (double) (clock() - t1) / CLOCKS_PER_SEC;
			runtime_generate_root += (double) (clock() - t1) / CLOCKS_PER_SEC;
			node->meetings_tb = heuristic_helper.computeMeetingsTieBreaker(*node);
			return true;
		} else 
		{
			int meeting_idx = node->meetings_set[task_idx];
			int prev_meeting_idx = meeting_idx - 1;
			int new_cost = std::get<4>(m_meeting_tables[node->assignment_idx][task_idx]->getNextMeeting(meeting_idx));
			int prev_cost = std::get<4>(m_meeting_tables[node->assignment_idx][task_idx]->getNextMeeting(prev_meeting_idx));

			// update the cost according to new meeting cost
			node->g_val = node->g_val - prev_cost + new_cost;
			node->soc = node->soc - prev_cost + new_cost;
			// node->makespan = max(node->makespan, new_path.size() - 1); // TODO Nir can we discard this?
			node->solved = false;
			node->tie_breaking = parent->tie_breaking; //std::numeric_limits<int>::max(); // TODO Nir take the parent tie_breaking as an approximation?
			runtime_generate_child += (double) (clock() - t1) / CLOCKS_PER_SEC;
			runtime_generate_root += (double) (clock() - t1) / CLOCKS_PER_SEC;
			node->meetings_tb = heuristic_helper.computeMeetingsTieBreaker(*node);
			return true;
		}
		
	}

	if (task_idx == -1)
		// this is the case where we changed the assignment and need to re-plan all agents
		return planAssignmentNode(node);
	else
		return planRootNode(node);
}

inline void CoCBS::pushNode(CBSNode* node)
{
	// update handles
	node->open_handle = open_list.push(node);
	num_HL_generated++;
	node->time_generated = num_HL_generated;
	if (node->g_val + node->h_val <= focal_list_threshold)
	{
		if (node->root)
			node->focal_handle = root_focal_list.push(node);
		else
			node->focal_handle = focal_list.push(node);
	}
	allNodes_table.push_back(node);
}


void CoCBS::printPaths() const
{
	for (int i = 0; i < num_of_agents; i++)
	{
		cout << "Agent " << i << " (" << paths_found_initially[0][i].size() - 1 << " -->" << // TODO Nir paths_found_initially index
			 paths[i]->size() - 1 << "): ";
		for (const auto& t : *paths[i])
			cout << t.location << "->";
		cout << endl;
	}
}


// adding new nodes to FOCAL (those with min-f-val*f_weight between the old and new LB)
void CoCBS::updateFocalList()
{
	CBSNode* open_head = open_list.top();
	if (open_head->g_val + open_head->h_val > min_f_val)
	{
		if (screen == 3)
		{
			cout << "  Note -- FOCAL UPDATE!! from |FOCAL|=" << focal_list.size() << " with |OPEN|=" << open_list.size() << " to |FOCAL|=";
		}
		min_f_val = open_head->g_val + open_head->h_val;
		double new_focal_list_threshold = min_f_val * focal_w;
		for (CBSNode* n : open_list)
		{
			if (n->g_val + n->h_val > focal_list_threshold &&
				n->g_val + n->h_val <= new_focal_list_threshold)
				if (n->root)
					n->focal_handle = root_focal_list.push(n);
				else
					n->focal_handle = focal_list.push(n);
		}
		focal_list_threshold = new_focal_list_threshold;
		if (screen == 3)
		{
			cout << focal_list.size() << endl;
		}
	}
}


void CoCBS::printResults() const
{
	if (solution_cost >= 0) // solved
		cout << (m_run_baseline ? "Baseline," : "Optimal,");
	else if (solution_cost == -1) // time_out
		cout << "Timeout,";
	else if (solution_cost == -2) // no solution
		cout << "No solutions,";
	else if (solution_cost == -3) // nodes out
		cout << "Nodesout,";

	cout << solution_cost << "," << runtime << "," <<
		 num_HL_expanded << "," << num_LL_expanded << "," << // HL_num_generated << "," << LL_num_generated << "," <<
		 min_f_val << "," << dummy_start->g_val << "," << dummy_start->g_val + dummy_start->h_val << "," <<
		 endl;
}

void CoCBS::saveResults(const string& fileName, const string& instanceName) const
{
	std::ifstream infile(fileName);
	bool exist = infile.good();
	infile.close();
	if (!exist)
	{
		ofstream addHeads(fileName);
		addHeads << "instance name," << 
			"solver name," << 
			"number of tasks," << 
			"runtime," << 
			"#iterations," <<
			"#high-level expanded," << 
			"#high-level generated," << 
			"#root expanded," << 
			"#root generated," << 
			"#low-level expanded," << 
			"#low-level generated," <<
			"solution cost," << 
			"meetings set," <<
			"assignment index," <<
			"min f value," << 
			"root g value," << 
			"root f value," <<
			"#adopt bypasses," <<
			"standard conflicts," << 
			"rectangle conflicts," << 
			"corridor conflicts," << 
			"target conflicts," << 
			"mutex conflicts," <<
			"#merge MDDs," << 
			"#solve 2 agents," << 
			"#memoization," <<
			"runtime of building heuristic graph," << 
			"runtime of solving MVC," <<
			"runtime of detecting conflicts," <<
			"runtime of rectangle conflicts," << 
			"runtime of corridor conflicts," << 
			"runtime of mutex conflicts," <<
			"runtime of building MDDs," << 
			"runtime of building constraint tables," << 
			"runtime of building CATs," <<
			"runtime of path finding," << 
			"runtime of generating child nodes," <<
			"preprocessing runtime," << 
			"runtime of generating roots," << 
			"runtime of global cost matrix," << 
			"runtime of task assignment" << 
			endl;
		addHeads.close();
	}
	int meetings_set = 0;
	int assignment_idx = -1;
	if (solution_cost < 0) 
		meetings_set = -1;
	else {
		assignment_idx = goal_node->assignment_idx;
		for (int i = 0; i < num_of_tasks; ++i)
			meetings_set += goal_node->meetings_set[i];
	}

	ofstream stats(fileName, std::ios::app);
	stats << instanceName << "," << 
	getSolverName() << "," <<
		num_of_tasks << "," <<
		runtime << "," <<
		num_iterations  << "," <<
		num_HL_expanded << "," << 
		num_HL_generated << "," <<
		num_root_expanded << "," <<
		num_root_generated  << "," <<
		num_LL_expanded << "," << 
		num_LL_generated << "," <<
		solution_cost << "," << 
		meetings_set << "," <<
		assignment_idx << "," <<
		min_f_val << "," << 
		dummy_start->g_val << "," << 
		dummy_start->g_val + dummy_start->h_val << "," <<
		num_adopt_bypass << "," <<
		num_standard_conflicts << "," << 
		num_rectangle_conflicts << "," << 
		num_corridor_conflicts << "," <<
		num_target_conflicts << "," << 
		num_mutex_conflicts << "," <<
		heuristic_helper.num_merge_MDDs << "," <<
		heuristic_helper.num_solve_2agent_problems << "," <<
		heuristic_helper.num_memoization << "," <<
		heuristic_helper.runtime_build_dependency_graph << "," <<
		heuristic_helper.runtime_solve_MVC << "," <<
		runtime_detect_conflicts << "," <<
		rectangle_helper.accumulated_runtime << "," << 
		corridor_helper.accumulated_runtime << "," << 
		mutex_helper.accumulated_runtime << "," <<
		mdd_helper.accumulated_runtime << "," << 
		runtime_build_CT << "," << 
		runtime_build_CAT << "," <<
		runtime_path_finding << "," << 
		runtime_generate_child << "," <<
		runtime_preprocessing << "," <<
		runtime_generate_root << "," <<
		runtime_cost_matrix << "," <<
		runtime_assignments << 
		endl;
	stats.close();
}

void CoCBS::saveStats(const string &fileName, const string &instanceName) const
{
	ofstream stats(fileName + ".txt", std::ios::app);
	stats << instanceName << endl;
	stats << "agent 1,agent 2,node id,#expanded nodes, h" << endl;
	for (auto ins : heuristic_helper.sub_instances)
	{
		stats << std::get<0>(ins) << "," << std::get<1>(ins) << "," << std::get<2>(ins)->time_generated << "," << std::get<3>(ins) << "," << std::get<4>(ins) << endl;
	}
	stats.close();
	cout << "Write " << heuristic_helper.sub_instances.size() << " samples to files" << endl;
}


void CoCBS::saveCT(const string &fileName) const // write the CT to a file
{
	std::ofstream output;
	output.open(fileName, std::ios::out);
	output << "digraph G {" << endl;
	output << "size = \"5,5\";" << endl;
	output << "center = true;" << endl;
	for (auto node : allNodes_table)
	{
		output << node->time_generated << " [label=\"#" << node->time_generated 
					<< "\ng+h="<< node->g_val << "+" << node->h_val 
					<< "\nd=" << node->tie_breaking << "\"]" << endl;
		if (node == dummy_start)
			continue;
		output << node->parent->time_generated << " -> " << node->time_generated << " [label=\"";
		for (const auto &constraint : node->constraints)
			output << constraint;
		output << "\nAgents ";
        for (const auto &path : node->paths)
            output << path.first << "(+" << path.second.size() - paths_found_initially[node->assignment_idx][path.first].size() << ") ";
        output << "\"]" << endl;
	}
	auto node = goal_node;
	while (node != nullptr)
	{
		output << node->time_generated << " [color=red]" << endl;
		node = node->parent;
	}
	output << "}" << endl;
	output.close();
}


void CoCBS::printConflicts(const CBSNode &curr)
{
	for (const auto& conflict : curr.conflicts)
	{
		cout << *conflict << endl;
	}
	for (const auto& conflict : curr.unknownConf)
	{
		cout << *conflict << endl;
	}
}


string CoCBS::getSolverName() const
{
	if (m_run_baseline)
		return "Baseline";

	string name;
	if (disjoint_splitting)
		name += "Disjoint ";
	switch (heuristic_helper.type)
	{
	case heuristics_type::ZERO:
		if (PC)
			name += "ICBS";
		else
			name += "CBS";
		break;
	case heuristics_type::CG:
		name += "CG";
		break;
	case heuristics_type::DG:
		name += "DG";
		break;
	case heuristics_type::WDG:
		name += "WDG";
		break;
	case STRATEGY_COUNT:
		break;
	}
	
	if (rectangle_helper.getName() != "NR")
		name += "+" + rectangle_helper.getName();
	if (corridor_helper.getName() != "NC")
    	name += "+" + corridor_helper.getName();
	if (target_reasoning)
		name += "+T";
	if (mutex_reasoning)
		name += "+M";
	if (bypass)
		name += "+BP";
	if (lazy_expansion)
		name += "+LE";

	switch (heuristic_helper.root_tb_type)
	{
	case root_tie_breaker_type::NONE:
		name += "";
		break;
	case root_tie_breaker_type::SUM:
		name += "+TB_SUM";
		break;
	case root_tie_breaker_type::MIN:
		name += "+TB_MIN";
		break;
	}

	name += " with " + search_engines[0]->getName();

	switch (ta_type)
	{
	case search_assignments::NOTA:
		name += "";
		break;
	case search_assignments::GREEDY:
		name += "+Greedy Assignment";
		break;
	case search_assignments::SINGLE:
		name += "+Single Assignment (m=" + std::to_string(num_assignments) + ")";
		break;
	case search_assignments::FULL:
		name += "+Full Assignments (m=" + std::to_string(num_assignments) + ")";
		break;
	}

	return name;
}

bool CoCBS::solve(double _time_limit, int _cost_lowerbound, int _cost_upperbound)
{
	this->min_f_val = _cost_lowerbound;
	this->cost_upperbound = _cost_upperbound;
	this->time_limit = _time_limit;

	if (screen > 0) // 1 or 2
	{
		string name = getSolverName();
		name.resize(45, ' ');
		cout << name << ": ";
	}
	// set timer
	start = clock();

    task_assignment();
	// if (ta_type == search_assignments::SEARCH)
	// 	this->time_limit += runtime_assignments; // add the time it took the calculate all assignments, since it is done "offline"
	computeMeetingTables(0);

	CBSNode* curr;
	int cbs_iter = 1;

	if (m_run_baseline)
	{
		// Greedy Baseline
		curr = new CBSNode();
		if (!solve_baseline(curr))
			return false;
	} else 
	{
		// Co-CBS
		generateRoot();
		if (screen == 3)
			printConflicts(*dummy_start);		
	}

	while (!open_list.empty() && !solution_found)
	{
		if (screen > 1)
			std::cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% CBS iteration: " << cbs_iter++ << " %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;

		// the focal list holds all lowest cost nodes
		updateFocalList();
		if (min_f_val >= cost_upperbound)
		{
			solution_cost = (int) min_f_val;
			solution_found = false;
			break;
		}
		runtime = (double) (clock() - start) / CLOCKS_PER_SEC;
		if (runtime > time_limit || num_HL_expanded > node_limit
		    || heuristic_helper.sub_instances.size() >= MAX_NUM_STATS)
		{  // time/node out
			solution_cost = -1;
			solution_found = false;
			break;
		}

		if (screen > 2)
			printFocalList();
		curr = extractNode();

		if (screen > 1)
			cout << endl << "Pop " << *curr << endl;

		if (!curr->solved)
		{
			assert(curr->root);
			if (screen > 1)
				std::cout << "Planning root node due to lazy expansion" << std::endl;
			bool solved;
			if (curr->changed_task_idx == -1)
			{ // new assignment node, need to plan all agents
				// reset the node values, since it's going to be calculated from scratch
				curr->g_val = 0;
				curr->soc = 0;
				curr->makespan = 0;
				solved = planAssignmentNode(curr);
			} else {
				updatePaths(curr->parent);
				// solve nodes that haven't been planned due to lazy expansion
				// bring back values from parent // TODO Nir handle this in generateRootNode
				curr->g_val = curr->parent->g_val;
				curr->soc = curr->parent->soc;
				curr->makespan = curr->parent->makespan;

			 	solved = planRootNode(curr);
			}
			if (!solved)
			{
				if (screen > 1)
				std::cout << "No solution in lazy expansion, continue..." << std::endl;
				curr->clear(); // TODO Nir make sure it's okay to clear and delete
				delete curr;
				continue;
			}
		}
		// takes the paths_found_initially and UPDATE all constrained paths found for agents from curr to dummy_start (and lower-bounds)
		updatePaths(curr);

		if (curr->unknownConf.size() + curr->conflicts.size() == 0) //no conflicts
		{// found a solution (and finish the while look)
			solution_found = true;
			solution_cost = curr->g_val;
			goal_node = curr;
			break;
		}

		// debug
		/*if (curr->time_generated == 15 && num_of_agents > 2)
		{
			int a1 = 51, a2 = 54;
			auto mdd1 = mdd_helper.getMDD(*curr, a1, paths[a1]->size());
			cout << "The mdd of agent " << a1 << endl;
			mdd1->printNodes();
			auto mdd2 = mdd_helper.getMDD(*curr, a2, paths[a2]->size());
			cout << "The mdd of agent " << a2 << endl;
			mdd2->printNodes();
			for (int t = 0; t < min(paths[a1]->size(), paths[a2]->size()); t++)
			{
				if (paths[a1]->at(t).location == paths[a2]->at(t).location)
					rectangle_helper.printOverlapArea(paths, t, a1, a2, mdd1, mdd2);
			}
			cout << "The constraints " << endl;
			curr->printConstraints(a1);
			curr->printConstraints(a2);
		}*/
		/*if (curr->time_generated == 1 && num_of_agents > 2)
		{
			int a1 = 12, a2 = 23;
			auto mdd1 = mdd_helper.getMDD(*curr, a1, paths[a1]->size());
			cout << "The mdd of agent " << a1 << endl;
			mdd1->printNodes();
			auto mdd2 = mdd_helper.getMDD(*curr, a2, paths[a2]->size());
			cout << "The mdd of agent " << a2 << endl;
			mdd2->printNodes();
			for (int t = 0; t < min(paths[a1]->size(), paths[a2]->size()); t++)
			{
				if (paths[a1]->at(t).location == paths[a2]->at(t).location)
					rectangle_helper.printOverlapArea(paths, t, a1, a2, mdd1, mdd2);
			}
			cout << "The constraints " << endl;
			curr->printConstraints(a1);
			curr->printConstraints(a2);
		}*/

		if (!curr->h_computed) // heuristics has not been computed yet
		{
			if (PC) // prioritize conflicts
				classifyConflicts(*curr);
			runtime = (double) (clock() - start) / CLOCKS_PER_SEC;
			bool succ = heuristic_helper.computeInformedHeuristics(*curr, time_limit - runtime);
			runtime = (double) (clock() - start) / CLOCKS_PER_SEC;
			if (runtime > time_limit)
			{  // timeout
				solution_cost = -1;
				solution_found = false;
				break;
			}
			if (!succ) // no solution, so prune this node
			{
				curr->clear();
				continue;
			}

			// reinsert the node
			curr->open_handle = open_list.push(curr);
			if (curr->g_val + curr->h_val <= focal_list_threshold)
			{
				if (curr->root)
					curr->focal_handle = root_focal_list.push(curr);
				else
					curr->focal_handle = focal_list.push(curr);
			}
			if (screen >= 2)
			{
				cout << "	Reinsert " << *curr << endl;
			}
			continue;
		} 

		//Expand the node
		num_HL_expanded++;
		curr->time_expanded = num_HL_expanded;
		bool foundBypass = true;
		while (foundBypass)
		{
			if (curr->unknownConf.size() + curr->conflicts.size() == 0) //no conflicts
			{// found a solution (and finish the while look)
				solution_found = true;
				solution_cost = curr->g_val;
				goal_node = curr;
				break;
			}

			if (curr->root)
			{
				num_root_expanded++;
				expandRoot(curr);
				updatePaths(curr);
			}

			foundBypass = false;
			CBSNode* child[2] = { new CBSNode(), new CBSNode() };


#pragma region Select a conflict and add corresponding constraints to childs
			curr->conflict = chooseConflict(*curr);
			// for (const auto& conflict : curr->unknownConf)
			// {
			// 	if (conflict.get()->a1 == 1) {
			// 		curr->conflict = conflict;
			// 		break;
			// 	}
			// }

			if (disjoint_splitting && curr->conflict->type == conflict_type::STANDARD)
			{
				int first = (bool) (rand() % 2);
				if (first) // disjoint splitting on the first agent
				{
					child[0]->constraints = curr->conflict->constraint1;
					int a, x, y, t;
					constraint_type type;
					tie(a, x, y, t, type) = curr->conflict->constraint1.back();
					if (type == constraint_type::VERTEX)
					{
						child[1]->constraints.emplace_back(a, x, y, t, constraint_type::POSITIVE_VERTEX);
					}
					else
					{
						assert(type == constraint_type::EDGE);
						child[1]->constraints.emplace_back(a, x, y, t, constraint_type::POSITIVE_EDGE);
					}
				}
				else // disjoint splitting on the second agent
				{
					child[1]->constraints = curr->conflict->constraint2;
					int a, x, y, t;
					constraint_type type;
					tie(a, x, y, t, type) = curr->conflict->constraint2.back();
					if (type == constraint_type::VERTEX)
					{
						child[0]->constraints.emplace_back(a, x, y, t, constraint_type::POSITIVE_VERTEX);
					}
					else
					{
						assert(type == constraint_type::EDGE);
						child[0]->constraints.emplace_back(a, x, y, t, constraint_type::POSITIVE_EDGE);
					}
				}
			}
			else
			{
				child[0]->constraints = curr->conflict->constraint1;
				child[1]->constraints = curr->conflict->constraint2;
				if (curr->conflict->type == conflict_type::RECTANGLE && rectangle_helper.strategy == rectangle_strategy::DR)
				{
					int i = (bool)(rand() % 2);
					for (const auto constraint : child[1 - i]->constraints)
					{
						child[i]->constraints.emplace_back(std::get<0>(constraint), std::get<1>(constraint), std::get<2>(constraint), std::get<3>(constraint), 
																							constraint_type::POSITIVE_BARRIER);
					}
				}
				else if (curr->conflict->type == conflict_type::CORRIDOR && corridor_helper.getStrategy() == corridor_strategy::DC)
				{
					int i = (bool)(rand() % 2);
					assert(child[1 - i]->constraints.size() == 1);
					auto constraint = child[1 - i]->constraints.front();
					child[i]->constraints.emplace_back(std::get<0>(constraint), std::get<1>(constraint), std::get<2>(constraint), std::get<3>(constraint),
						constraint_type::POSITIVE_RANGE);
				}
			}
#pragma endregion

			if (screen > 1)
				cout << "	Expand " << *curr << endl <<
					 "	on " << *(curr->conflict) << endl;

			bool solved[2] = { false, false };
			vector<vector<PathEntry>*> copy(paths);

			for (int i = 0; i < 2; i++)
			{
				if (i > 0)
					paths = copy; // TODO Nir IMPORTANT! make sure every time we use paths, it's the right one
				solved[i] = generateChild(child[i], curr);
				if (!solved[i])
				{
					delete child[i];
					continue;
				}
				if (child[i]->g_val + child[i]->h_val == min_f_val && curr->unknownConf.size() + curr->conflicts.size() == 0) //no conflicts
				{// found a solution (and finish the while look)
					break;
				}
				else if (bypass && child[i]->g_val == curr->g_val && child[i]->tie_breaking < curr->tie_breaking) // Bypass1
				{
					if (i == 1 && !solved[0])
						continue;
					foundBypass = true;
					num_adopt_bypass++;
					curr->conflicts = child[i]->conflicts;
					curr->unknownConf = child[i]->unknownConf;
					curr->tie_breaking = child[i]->tie_breaking;
					curr->conflict = nullptr;
					for (const auto& path : child[i]->paths) // update paths
					{
						auto p = curr->paths.begin();
						while (p != curr->paths.end())
						{
							if (path.first == p->first)
							{
								p->second = path.second;
								paths[p->first] = &p->second;
								break;
							}
							++p;
						}
						if (p == curr->paths.end())
						{
							curr->paths.emplace_back(path);
							paths[path.first] = &curr->paths.back().second;
						}
					}
					if (screen > 1)
					{
						cout << "	Update " << *curr << endl;
					}
					break;
				}
			}
			if (foundBypass)
			{
				for (auto & i : child)
				{
					delete i;
					i = nullptr;
				}
                if (PC) // prioritize conflicts
                    classifyConflicts(*curr); // classify the new-detected conflicts
			}
			else
			{
				for (int i = 0; i < 2; i++)
				{
					if (solved[i])
					{
						pushNode(child[i]);
						if (screen > 1)
						{
							cout << "		Generate " << *child[i] << endl;
						}
					}
				}
			}
		}
		if (curr->conflict != nullptr)
		{
			switch (curr->conflict->type)
			{
			case conflict_type::RECTANGLE:
				num_rectangle_conflicts++;
				break;
			case conflict_type::CORRIDOR:
				num_corridor_conflicts++;
				break;
			case conflict_type::TARGET:
				num_target_conflicts++;
				break;
			case conflict_type::STANDARD:
				num_standard_conflicts++;
				break;
			case conflict_type::MUTEX:
				num_mutex_conflicts++;
				break;
			}
		}
		// if we're not with lazy expansion, we can clear the conflict lists from this since we've done with it
		// TODO Nir with lazy expansion, we need to clear this node after all of its children have been expanded
		if (!lazy_expansion)
			curr->clear();
	}  // end of while loop

	runtime = (double) (clock() - start) / CLOCKS_PER_SEC;
	if (solution_found && !validateSolution(*curr))
	{
		cout << "Solution invalid!!!" << endl;
		printPaths();
		exit(-1);
	}
	if (screen == 2)
        printPaths();
	if (screen > 0) // 1 or 2
	{
		if (solution_cost >= 0)
			std::cout << "Solution found: " << *goal_node << std::endl;
		printResults();
	}

	return solution_found;
}

bool CoCBS::solve_baseline(CBSNode* curr)
{
	solution_found = false;
	solution_cost = -1;
	paths_found_initially.resize(1);
	paths_found_initially[0].resize(num_of_agents);
	paths.resize(num_of_agents, nullptr);

	curr->meetings.resize(num_of_tasks);
	curr->h_val = 0;
	curr->meetings_set.resize(num_of_tasks);
	curr->assignment_idx = 0;
	ConstraintTable constraints;
	Meeting m;
	MeetingsTable* mt;
	Task task;
	int agA, agB, meeting_location, meeting_time, next_agentA_start_location, next_agentA_start_time;

	Path fullPathA, fullPathB, agentAtoTaskStart, agentAtoMeeting, agentB_to_meeting, agentB_to_goal;
	// loop over tasks (meeting tables)
	for (int ti = 0; ti < num_of_tasks; ++ti)
	{
		runtime = (double) (clock() - start) / CLOCKS_PER_SEC;
		if (runtime > time_limit)
		{  // time out
			solution_cost = -1;
			solution_found = false;
			goal_node = curr;
			dummy_start = curr;
			return false;
		}

		mt = m_meeting_tables[0][ti];
		fullPathA.clear(); fullPathB.clear(); agentAtoTaskStart.clear(); agentAtoMeeting.clear(); agentB_to_meeting.clear(); agentB_to_goal.clear();

		// take the optimal meeting (location only)
		m = mt->getNextMeeting();
		task = mt->getTask();
		meeting_location = std::get<0>(m);
		agA = std::get<2>(m);
		agB = std::get<3>(m);
		// plan agent A from start to task start then to meeting location
		// Plan agent A to task start
		search_engines[agA]->setStartLocation(std::get<3>(task)); // TODO Nir should this be done inside the solver?
		search_engines[agA]->setGoalLocation(std::get<0>(task)); // TODO Nir should this be done inside the solver?
		constraints.goal_location = search_engines[agA]->goal_location; // TODO Nir IMPORTANT should initial_constraints be per assignment index???
		agentAtoTaskStart = search_engines[agA]->findPath(*curr, constraints, paths, agA, 0);
		if (agentAtoTaskStart.size() == 0)
		{
			solution_cost = -2;
			solution_found = false;
			if (screen > 1)
				std::cout << "No solution agent A (" << agA << ") to task start" << endl;
			goal_node = curr;
			dummy_start = curr;
			return false;
		}

		runtime = (double) (clock() - start) / CLOCKS_PER_SEC;
		if (runtime > time_limit)
		{  // time out
			solution_cost = -1;
			solution_found = false;
			goal_node = curr;
			dummy_start = curr;
			return false;
		}

		next_agentA_start_location = agentAtoTaskStart.back().location;
		next_agentA_start_time = agentAtoTaskStart.size() - 1;

		// Plan agent A to meeting
		search_engines[agA]->setStartLocation(next_agentA_start_location); // TODO Nir should this be done inside the solver?
		search_engines[agA]->setGoalLocation(meeting_location); // TODO Nir should this be done inside the solver?
		constraints.goal_location = meeting_location;
		agentAtoMeeting = search_engines[agA]->findPath(*curr, constraints, paths, agA, 0, -1, next_agentA_start_time);
		if (agentAtoMeeting.size() == 0)
		{
			solution_cost = -2;
			solution_found = false;
			if (screen > 1)
				std::cout << "No solution agent A (" << agA << ") to meeting" << endl;
			goal_node = curr;
			dummy_start = curr;
			return false;
		}

		runtime = (double) (clock() - start) / CLOCKS_PER_SEC;
		if (runtime > time_limit)
		{  // time out
			solution_cost = -1;
			solution_found = false;
			goal_node = curr;
			dummy_start = curr;
			return false;
		}

		fullPathA = agentAtoTaskStart;
		fullPathA.insert(fullPathA.end(), agentAtoMeeting.begin() + 1, agentAtoMeeting.end());

		// insert the path the paths
		paths_found_initially[0][agA] = fullPathA;
		paths[agA] = &paths_found_initially[0][agA];

		curr->g_val += (int) fullPathA.size();
		curr->soc += (int) fullPathA.size();
		curr->makespan = max(curr->makespan, fullPathA.size() - 1);

		// add agent A steps as constraints
		for (int t = 0; t < fullPathA.size() - 1; ++t) // don't add the meeting as a constraint, so that agentB can arrive there
			constraints.insert2CT(fullPathA[t].location, t, t + 1);

		// plan agent B from start to meeting location then to goal
		// Plan agent B to meeting
		search_engines[agB]->setStartLocation(std::get<5>(task));
		search_engines[agB]->setGoalLocation(meeting_location);
		constraints.goal_location = meeting_location;
		agentB_to_meeting = search_engines[agB]->findPath(*curr, constraints, paths, agB, 0);

		if (agentB_to_meeting.size() == 0)
		{
			solution_cost = -2;
			solution_found = false;
			if (screen > 1)
				std::cout << "No solution agent B (" << agB << ") to meeting" << endl;
			goal_node = curr;
			dummy_start = curr;
			return false;
		}

		runtime = (double) (clock() - start) / CLOCKS_PER_SEC;
		if (runtime > time_limit)
		{  // time out
			solution_cost = -1;
			solution_found = false;
			goal_node = curr;
			dummy_start = curr;		
			return false;
		}

		// determine the meeting time as the earliest both agents arrive
		meeting_time = max(fullPathA.size() - 1, agentB_to_meeting.size() - 1);

		// put wait actions at meeting location
		while((int)fullPathA.size() <= meeting_time - 1) // not including the meeting itself
		{
			fullPathA.emplace_back(meeting_location);
			constraints.insert2CT(meeting_location, fullPathA.size() - 1, fullPathA.size());
		}
			

		// Plan agent B to goal
		search_engines[agB]->setStartLocation(meeting_location);
		search_engines[agB]->setGoalLocation(std::get<1>(task));
		constraints.goal_location = search_engines[agB]->goal_location;
		agentB_to_goal = search_engines[agB]->findPath(*curr, constraints, paths, agB, 0, -1, meeting_time);

		if (agentB_to_goal.size() == 0)
		{
			solution_cost = -2;
			solution_found = false;
			if (screen > 1)
				std::cout << "No solution agent B (" << agB << ") to goal" << endl;
			goal_node = curr;
			dummy_start = curr;
			return false;
		}

		// put wait actions at meeting location
		while((int)agentB_to_meeting.size() <= meeting_time)
		{
			agentB_to_meeting.emplace_back(meeting_location);
			constraints.insert2CT(meeting_location, agentB_to_meeting.size() - 1, agentB_to_meeting.size());
		}

		runtime = (double) (clock() - start) / CLOCKS_PER_SEC;
		if (runtime > time_limit)
		{  // time out
			solution_cost = -1;
			solution_found = false;
			goal_node = curr;
			dummy_start = curr;
			return false;
		}

		// fullPath.resize(agentB_to_meeting.size());
		fullPathB = agentB_to_meeting;
		fullPathB.insert(fullPathB.end(), agentB_to_goal.begin() + 1, agentB_to_goal.end());
	
		// insert the path the paths
		paths_found_initially[0][agB] = fullPathB;
		paths[agB] = &paths_found_initially[0][agB];

		// set the meeting
		curr->meetings[ti] = Meeting(meeting_location, meeting_time, agA, agB, fullPathA.size() + fullPathB.size() - 2);
		curr->meetings_set[ti] = 0;

		curr->g_val += (int) fullPathB.size();
		curr->soc += (int) fullPathB.size();
		curr->makespan = max(curr->makespan, fullPathB.size() - 1);

		// add agent B steps as constraints
		for (int t = 0; t < fullPathB.size(); ++t)
			constraints.insert2CT(fullPathB[t].location, t, t + 1);
	}
	solution_found = true;
	solution_cost = curr->g_val;
	goal_node = curr;
	dummy_start = curr;
	return true;
}

void CoCBS::task_assignment()
{
	if (ta_type == search_assignments::NOTA) // No task assignment
	{
		m_assignments.resize(1);
		m_assignments[0].resize(num_of_tasks);
		for (int ti = 0; ti < num_of_tasks; ++ti)
			m_assignments[0][ti] = Assignment(ti, ti*2, ti*2 + 1);
		return;
	}

	clock_t t = clock();

	if (ta_type == search_assignments::GREEDY)
	{
		CBSNode dummy_node;
		dummy_node.root = true;
		vector<int> a_agents(num_of_tasks), b_agents(num_of_tasks);
		for (int i = 0; i < num_of_tasks; ++i)
		{
			a_agents[i] = i;
			b_agents[i] = i;
		}
		int closest_a, closest_b, dist_to_a, dist_to_b;
		int min_dist_to_a;
		int min_dist_to_b;
		
		m_assignments.resize(1);
		m_assignments[0].resize(num_of_tasks);

		for (int ti = 0; ti < num_of_tasks; ++ti)
		{
			min_dist_to_a = std::numeric_limits<int>::max();
			min_dist_to_b = std::numeric_limits<int>::max();
			closest_a = -1;
			closest_b = -1;
			// loop over tasks, and select the closest agents
			for (int ai : a_agents)
			{
				search_engines[ai*2]->setStartLocation(search_engines[ai*2]->instance.start_locations[ai*2]);
				search_engines[ai*2]->setGoalLocation(search_engines[ai*2]->instance.task_start_locations[ti]);
				initial_constraints[ai*2].goal_location = search_engines[ai*2]->instance.task_start_locations[ti];
				dist_to_a = search_engines[ai*2]->findPath(dummy_node, initial_constraints[ai*2], paths, ai*2, 0).size() - 1;

				if (dist_to_a < min_dist_to_a)
				{
					min_dist_to_a = dist_to_a;
					closest_a = ai;
				}
			}
			for (int bi : b_agents)
			{
				search_engines[bi*2+1]->setStartLocation(search_engines[bi*2+1]->instance.start_locations[bi*2+1]);
				search_engines[bi*2+1]->setGoalLocation(search_engines[bi*2+1]->instance.task_start_locations[ti]);
				initial_constraints[bi*2+1].goal_location = search_engines[bi*2+1]->instance.task_start_locations[ti];
				dist_to_b = search_engines[bi*2+1]->findPath(dummy_node, initial_constraints[bi*2+1], paths, bi*2+1, 0).size() - 1;
				
				if (dist_to_b < min_dist_to_b)
				{
					min_dist_to_b = dist_to_b;
					closest_b = bi;
				}
			}
			// update the remaining agent vectors
			a_agents.erase(std::remove(a_agents.begin(), a_agents.end(), closest_a), a_agents.end());
			b_agents.erase(std::remove(b_agents.begin(), b_agents.end(), closest_b), b_agents.end());

			// set the assignment
			m_assignments[0][ti] = Assignment(ti, closest_a*2, closest_b*2+1);

		}

		runtime_assignments = (double) (clock() - t) / CLOCKS_PER_SEC;
		std::cout << "Runtime Global Cooperation Cost Matrix: " << runtime_cost_matrix << ", Assignments: " << runtime_assignments << std::endl;
		return;
	}

	coder::array<cell_wrap_0, 2U> assignments;
	coder::array<double, 1U> costs;
	coder::array<double, 1U> solutionGap;

	t = clock();
	coder::array<double, 3U> costMatrix = computeGlobalCostMatrix();
	runtime_cost_matrix = (double) (clock() - t) / CLOCKS_PER_SEC;

	t = clock();

	// num_assignments = ta_type == search_assignments::SINGLE ? 1 : 100; // TODO Nir deal with the 100
	ITaskAssignment* task_assignment = new ITaskAssignment();

	m_assignments.resize(num_assignments);
	assignment_times.resize(num_assignments);
	double runtime_all = task_assignment->get_best_assignments(costMatrix, num_assignments, assignments, costs, solutionGap, assignment_times);
	
	// for (int i = 0; i < num_assignments; ++i)
	// 	std::cout << costs.data_.data_[i] << std::endl;
	
	delete task_assignment;
	for (int i = 0; i < assignments.numel(); ++i) {
		cell_wrap_0 assignment = assignments[i];
		m_assignments[i].resize(num_of_tasks);
		for (int ti = 0; ti < num_of_tasks; ++ti)
			m_assignments[i][ti] = Assignment(assignment.f1[ti]-2, (assignment.f1[ti+num_of_tasks]-2)*2, (assignment.f1[ti+2*num_of_tasks]-2)*2+1);
	}

	runtime_assignments = (double) (clock() - t) / CLOCKS_PER_SEC;

	std::cout << "Runtime Global Cooperation Cost Matrix: " << runtime_cost_matrix << ", Assignments: " << runtime_assignments << std::endl;
}


coder::array<double, 3U> CoCBS::computeGlobalCostMatrix() const
{
	coder::array<double, 3U> costMatrix;

  	// Set the size of the array.
  	// Change this size to the value that the application requires.
	costMatrix.set_size(num_of_tasks, num_of_tasks, num_of_tasks);

	Task task;
	MeetingsTable* pMt;
	Meeting m;
	float curr, tot = costMatrix.numel();
	if (screen > 0)
		std::cout << std::endl << "Calculating global cost matrix..." << std::endl;

	// Initialize the costMatrix array
	for (int ti = 0; ti < costMatrix.size(0); ti++) {
		for (int ai = 0; ai < costMatrix.size(1); ai++) {
			for (int bi = 0; bi < costMatrix.size(2); bi++) {
				curr++;
				// Set the value of the array element.
				// Change this value to the value that the application requires.
				costMatrix[(ti + costMatrix.size(0) * ai) + costMatrix.size(0) * costMatrix.size(1) * bi] = 0.0;

				// Get the cost of best meeting from the corresponding meetings table
				task = std::make_tuple(
						search_engines[0]->instance.task_start_locations[ti], // task start location
						search_engines[0]->instance.task_goal_locations[ti],  // task goal location
						ai*2,  // agent A index (0, 2, 4, ...)
						search_engines[0]->instance.start_locations[ai*2],  // agent A start location
						bi*2+1,  // agent B index (1, 3, 5, ...)
						search_engines[0]->instance.start_locations[bi*2+1]   // agent B start location
						);
				// std::cout << std::get<0>(task) << ", " << std::get<1>(task) << ", " << std::get<2>(task) << ", " << std::get<3>(task) << ", " << std::get<4>(task) << ", " << std::get<5>(task) << std::endl;
				// create a MeetingsTable
				pMt = new MeetingsTable(dijkstra, task, m_cost_type);
				// get the best meeting
				m = pMt->getNextMeeting(0);
				delete pMt;
				// assign the meeting's cost
				costMatrix[(ti + costMatrix.size(0) * ai) + costMatrix.size(0) * costMatrix.size(1) * bi] = std::get<4>(m);
				// std::cout << std::get<4>(m) << std::endl;
				if (screen > 0 && int(curr) % 10 == 0)
					std::cout << (curr/tot)*100 << "%" << std::endl;
			}
		}
	}
	std::cout << "100%" << std::endl;
	return costMatrix;
}


CoCBS::CoCBS(vector<SingleAgentSolver*>& search_engines, // TODO Nir when is this constructor used?
		 const vector<ConstraintTable>& initial_constraints,
         vector<vector<Path>>& paths_found_initially, int screen) :
		screen(screen), focal_w(1),
		initial_constraints(initial_constraints), paths_found_initially(paths_found_initially),
		search_engines(search_engines),
		mdd_helper(initial_constraints, search_engines),
		rectangle_helper(search_engines[0]->instance),
		mutex_helper(search_engines[0]->instance, initial_constraints),
		corridor_helper(search_engines, initial_constraints),
		heuristic_helper(search_engines.size(), paths, search_engines, initial_constraints, mdd_helper, m_meeting_tables)
{
	num_of_agents = (int) search_engines.size();
	num_of_tasks = (int) num_of_agents / 2;
	mutex_helper.search_engines = search_engines;
}

CoCBS::CoCBS(const Instance& instance, bool sipp, int screen) :
		screen(screen), focal_w(1),
		num_of_agents(instance.getDefaultNumberOfAgents()),
		num_of_tasks(instance.getDefaultNumberOfAgents() / 2),
		mdd_helper(initial_constraints, search_engines),
		rectangle_helper(instance),
		mutex_helper(instance, initial_constraints),
		corridor_helper(search_engines, initial_constraints),
		heuristic_helper(instance.getDefaultNumberOfAgents(), paths, search_engines, initial_constraints, mdd_helper, m_meeting_tables)
{
	clock_t t = clock();
	initial_constraints.resize(num_of_agents,
							   ConstraintTable(instance.num_of_cols, instance.map_size));

	search_engines.resize(num_of_agents);
	for (int i = 0; i < num_of_agents; i++)
	{
		if (sipp)
			search_engines[i] = new SIPP(instance, i);
		else {
			search_engines[i] = new SpaceTimeAStar(instance, i);
		}

		initial_constraints[i].goal_location = search_engines[i]->goal_location;
	}
	runtime_preprocessing = (double) (clock() - t) / CLOCKS_PER_SEC;

	mutex_helper.search_engines = search_engines;

	if (screen >= 2) // print start and goals
	{
		instance.printAgents();
	}

	// init Dijkstra planner
	dijkstra = new Dijkstra(instance);
	// m_meeting_tables.resize(num_of_tasks);
}

bool CoCBS::generateRoot()
{
	const int FIRST_MEETING_INDEX = 0;
	dummy_start = new CBSNode();
	// dummy_start->tie_breaking = 1;
	dummy_start->g_val = 0;
	dummy_start->root = true;
	paths.resize(num_of_agents, nullptr);
	dummy_start->meetings.resize(num_of_tasks); // TODO Nir hold meetings in CBS (same as paths)?
	dummy_start->meetings_set.resize(num_of_tasks, FIRST_MEETING_INDEX);
	dummy_start->assignment_idx = 0;
	dummy_start->assignment = &m_assignments[0];

	// add the first assignment time
	// if (ta_type == search_assignments::SEARCH) // TODO Nir
	// 	tot_assignment_time += assignment_times[0];

	// add to meetings record
	vector<int> record(dummy_start->meetings_set);
	record.push_back(dummy_start->assignment_idx); // In the meetings record, the assignment index is at the end
	m_meetings_record.emplace(record);

	mdd_helper.init(num_of_agents);
	heuristic_helper.init();
	vector<Path> init_paths;
	paths_found_initially.emplace_back(init_paths);

	// initialize paths_found_initially
	if (paths_found_initially[0].empty())
	{
		paths_found_initially[0].resize(num_of_agents);

		// generate a random permutation of agent indices
		vector<int> agents(num_of_agents);
		for (int i = 0; i < num_of_agents; i++)
		{
			agents[i] = i;
		}

		if (randomRoot)
		{
			std::random_device rd;
			std::mt19937 g(rd());
			std::shuffle(std::begin(agents), std::end(agents), g);
		}

		Path fullPath, agentAtoTaskStart, agentAtoMeeting, agentB_to_meeting, agentB_to_goal;
		Meeting m;
		Task task;
		int meeting_location, meeting_time, next_agentA_start_location, next_agentA_start_time, task_idx;

		for (auto i : agents)
		{
			task_idx = getAgentTask(dummy_start->assignment_idx, i);
			m = m_meeting_tables[0][task_idx]->getNextMeeting(FIRST_MEETING_INDEX);
			task = m_meeting_tables[0][task_idx]->getTask();
			meeting_location = std::get<0>(m);
			meeting_time = std::get<1>(m);
			fullPath.clear(); agentAtoTaskStart.clear(); agentAtoMeeting.clear(); agentB_to_meeting.clear(); agentB_to_goal.clear();

			if (i % 2 == 0) // agent A
			{
				// put the meeting into the node
				dummy_start->meetings[task_idx] = m;
				// Plan agent A to task start
				search_engines[i]->setStartLocation(std::get<3>(task)); // TODO Nir should this be done inside the solver?
				search_engines[i]->setGoalLocation(std::get<0>(task)); // TODO Nir should this be done inside the solver?
				initial_constraints[i].goal_location = search_engines[i]->goal_location;
				agentAtoTaskStart = search_engines[i]->findPath(*dummy_start, initial_constraints[i], paths, i, 0);
				
				num_LL_expanded += search_engines[i]->num_expanded;
				num_LL_generated += search_engines[i]->num_generated;
				runtime_build_CT += search_engines[i]->runtime_build_CT;
				runtime_build_CAT += search_engines[i]->runtime_build_CAT;

				if (agentAtoTaskStart.size() == 0)
				{
					if (screen > 1)
						std::cout << "No solution agent A (" << i << ") to task start" << endl;
					return false;
				}
				next_agentA_start_location = agentAtoTaskStart.back().location;
				next_agentA_start_time = agentAtoTaskStart.size() - 1;

				// Plan agent A to meeting
				search_engines[i]->setStartLocation(next_agentA_start_location); // TODO Nir should this be done inside the solver?
				search_engines[i]->setGoalLocation(meeting_location); // TODO Nir should this be done inside the solver?
				initial_constraints[i].goal_location = search_engines[i]->goal_location;
				agentAtoMeeting = search_engines[i]->findPath(*dummy_start, initial_constraints[i], paths, i, 0, meeting_time, next_agentA_start_time);
				
				num_LL_expanded += search_engines[i]->num_expanded;
				num_LL_generated += search_engines[i]->num_generated;
				runtime_build_CT += search_engines[i]->runtime_build_CT;
				runtime_build_CAT += search_engines[i]->runtime_build_CAT;

				if (agentAtoMeeting.size() == 0)
				{
					if (screen > 1)
						std::cout << "No solution agent A (" << i << ") to meeting" << endl;
					return false;
				}
				// put wait actions at meeting location
				while((int)agentAtoTaskStart.size() + agentAtoMeeting.size() - 1 <= meeting_time)
                    agentAtoMeeting.emplace_back(meeting_location);
				//CAT cat(dummy_start->makespan + 1);  // initialized to false
				//updateReservationTable(cat, i, *dummy_start);
				fullPath = agentAtoTaskStart;
				fullPath.insert(fullPath.end(), agentAtoMeeting.begin() + 1, agentAtoMeeting.end());
			} else { // agentB
				// Plan agent B to meeting
				search_engines[i]->setStartLocation(std::get<5>(task));
				search_engines[i]->setGoalLocation(meeting_location);
				initial_constraints[i].goal_location = search_engines[i]->goal_location;
				agentB_to_meeting = search_engines[i]->findPath(*dummy_start, initial_constraints[i], paths, i, 0, meeting_time);
				
				num_LL_expanded += search_engines[i]->num_expanded;
				num_LL_generated += search_engines[i]->num_generated;
				runtime_build_CT += search_engines[i]->runtime_build_CT;
				runtime_build_CAT += search_engines[i]->runtime_build_CAT;
				
				if (agentB_to_meeting.size() == 0)
				{
					if (screen > 1)
						std::cout << "No solution agent B (" << i << ") to meeting" << endl;
					return false;
				}
				// put wait actions at meeting location
				while((int)agentB_to_meeting.size() <= meeting_time)
                    agentB_to_meeting.emplace_back(meeting_location);

				// Plan agent B to goal
				search_engines[i]->setStartLocation(meeting_location);
				search_engines[i]->setGoalLocation(std::get<1>(task));
				initial_constraints[i].goal_location = search_engines[i]->goal_location;
				agentB_to_goal = search_engines[i]->findPath(*dummy_start, initial_constraints[i], paths, i, 0, -1, meeting_time);
				
				num_LL_expanded += search_engines[i]->num_expanded;
				num_LL_generated += search_engines[i]->num_generated;
				runtime_build_CT += search_engines[i]->runtime_build_CT;
				runtime_build_CAT += search_engines[i]->runtime_build_CAT;

				if (agentB_to_goal.size() == 0)
				{
					if (screen > 1)
						std::cout << "No solution agent B (" << i << ") to goal" << endl;
					return false;
				}
				// fullPath.resize(agentB_to_meeting.size());
				fullPath = agentB_to_meeting;
				fullPath.insert(fullPath.end(), agentB_to_goal.begin() + 1, agentB_to_goal.end());
			}

			// paths_found_initially[i] = search_engines[i]->findPath(*dummy_start, initial_constraints[i], paths, i, 0);
			paths_found_initially[0][i] = fullPath;
			// if (paths_found_initially[i].empty())
			// {
			// 	cout << "No path exists for agent " << i << endl;
			// 	return false;
			// }
			paths[i] = &paths_found_initially[0][i];
			dummy_start->makespan = max(dummy_start->makespan, paths_found_initially[0][i].size() - 1);
			dummy_start->soc += paths_found_initially[0][i].size() - 1;
			dummy_start->g_val += (int) paths_found_initially[0][i].size() - 1;

			// num_LL_expanded += search_engines[i]->num_expanded;
			// num_LL_generated += search_engines[i]->num_generated;
		}
	}
	else
	{
		for (int i = 0; i < num_of_agents; i++)
		{
			paths[i] = &paths_found_initially[0][i];
			dummy_start->makespan = max(dummy_start->makespan, paths_found_initially[0][i].size() - 1);
			dummy_start->g_val += (int) paths_found_initially[0][i].size() - 1;
		}
	}

	// generate dummy start and update data structures		
	dummy_start->h_val = 0;
	dummy_start->depth = 0;
	dummy_start->open_handle = open_list.push(dummy_start);
	dummy_start->focal_handle = root_focal_list.push(dummy_start);

	num_HL_generated++;
	dummy_start->time_generated = num_HL_generated;
	allNodes_table.push_back(dummy_start);
	findConflicts(*dummy_start);
	// We didn't compute the node-selection tie-breaking value for the root node
	// since it does not need it.
	min_f_val = max(min_f_val, (double) dummy_start->g_val);
	focal_list_threshold = min_f_val * focal_w;

	if (screen >= 2) // print start and goals
	{
		printPaths();
	}

	dummy_start->solved = true;
	dummy_start->meetings_tb = heuristic_helper.computeMeetingsTieBreaker(*dummy_start);

	return true;
}

inline void CoCBS::releaseNodes()
{
	open_list.clear();
	focal_list.clear();
	root_focal_list.clear();
	for (auto node : allNodes_table)
		delete node;
	allNodes_table.clear();
}



/*inline void CoCBS::releaseOpenListNodes()
{
	while (!open_list.empty())
	{
		CBSNode* curr = open_list.top();
		open_list.pop();
		delete curr;
	}
}*/

CoCBS::~CoCBS()
{
	releaseNodes();
	mdd_helper.clear();
}

void CoCBS::clearSearchEngines()
{
	for (auto s : search_engines)
		delete s;
	search_engines.clear();

	delete dijkstra;
	for (auto mt : m_meeting_tables) {
		for (auto t : mt)
			delete t;
		mt.clear();
	}
	m_meeting_tables.clear();
}


bool CoCBS::validateSolution(CBSNode& node) const
{
	Meeting m;
	int meeting_location, meeting_time, a, b, cost, task_idx;
	for (int a1 = 0; a1 < num_of_agents; a1++)
	{
		task_idx = getAgentTask(node.assignment_idx, a1);
		m = node.meetings[task_idx];
		meeting_location, meeting_time, a, b, cost;
		tie(meeting_location, meeting_time, a, b, cost) = m;
	
		for (int a2 = a1 + 1; a2 < num_of_agents; a2++)
		{
			size_t min_path_length = paths[a1]->size() < paths[a2]->size() ? paths[a1]->size() : paths[a2]->size();
			for (size_t timestep = 0; timestep < min_path_length; timestep++)
			{
				int loc1 = paths[a1]->at(timestep).location;
				int loc2 = paths[a2]->at(timestep).location;
				if (loc1 == loc2)
				{
					if (loc1 == meeting_location && meeting_time == timestep && (a1 == a && a2 == b || a1 == b && a2 == a))
						continue; // This is a meeting, not a real conflict
			
					cout << "Agents " << a1 << " and " << a2 << " collides at " << loc1 << " at timestep " << timestep << endl;
					return false;
				}
				else if (timestep < min_path_length - 1
						 && loc1 == paths[a2]->at(timestep + 1).location
						 && loc2 == paths[a1]->at(timestep + 1).location)
				{
					cout << "Agents " << a1 << " and " << a2 << " collides at (" <<
						 loc1 << "-->" << loc2 << ") at timestep " << timestep << endl;
					return false;
				}
			}
			// TODO Nir don't check target conflicts or vertex conflicts on final location since we assume disappear-at-target
			return true;
			if (paths[a1]->size() != paths[a2]->size())
			{
				int a1_ = paths[a1]->size() < paths[a2]->size() ? a1 : a2;
				int a2_ = paths[a1]->size() < paths[a2]->size() ? a2 : a1;
				int loc1 = paths[a1_]->back().location;
				for (size_t timestep = min_path_length; timestep < paths[a2_]->size(); timestep++)
				{
					int loc2 = paths[a2_]->at(timestep).location;
					if (loc1 == loc2)
					{
						cout << "Agents " << a1 << " and " << a2 << " collides at " << loc1 << " at timestep " << timestep << endl;
						return false; // It's at least a semi conflict			
					}
				}
			}
		}
	}
	return true;
}

inline int CoCBS::getAgentLocation(int agent_id, size_t timestep) const
{
	size_t t = max(min(timestep, paths[agent_id]->size() - 1), (size_t) 0);
	return paths[agent_id]->at(t).location;
}


// used for rapid random  restart
void CoCBS::clear()
{
	mdd_helper.clear();
	heuristic_helper.clear();
	releaseNodes();
	paths.clear();
	for (auto p : paths_found_initially)
		p.clear();
	paths_found_initially.clear();
	dummy_start = nullptr;
	goal_node = nullptr;
	solution_found = false;
	solution_cost = -2;
}


// Co-CBS methods
void CoCBS::computeMeetingTables(int assignment_idx) 
{
	if (assignment_idx < m_meeting_tables.size())
		return;


	if (screen > 1)
		cout << endl << "Calculating meeting tables for all tasks for assignment index " << assignment_idx << "..." << endl;

	Task task;
	vector<MeetingsTable*> tables;
	tables.resize(num_of_tasks);
	Assignment assignment;

	for (int task_idx = 0; task_idx < num_of_tasks; ++task_idx)
	{
		assignment = getAssignedAgents(assignment_idx, task_idx);
		assert(std::get<0>(assignment) == task_idx);
		task = std::make_tuple(
			search_engines[0]->instance.task_start_locations[task_idx], // task start location
			search_engines[0]->instance.task_goal_locations[task_idx],  // task goal location
			std::get<1>(assignment),  // agent A index
			search_engines[0]->instance.start_locations[std::get<1>(assignment)],  // agent A start location
			std::get<2>(assignment),  // agent B index
			search_engines[0]->instance.start_locations[std::get<2>(assignment)]   // agent B start location
			);
		tables[task_idx] = new MeetingsTable(dijkstra, task, m_cost_type);
		std::cout << task_idx + 1 << "/" << num_of_tasks << std::endl;
	}	
	m_meeting_tables.emplace_back(tables);
	assert(m_meeting_tables.size() == assignment_idx + 1);
}

/**
 * @brief Extracts the lowest-cost node from the open lists.
 * 
 * @return CBSNode* - lowest-cost node.
 */
CBSNode* CoCBS::extractNode()
{
	CBSNode* node;
	if (root_focal_list.size() > 0 && focal_list.size() > 0)
	{
		// we have both types of nodes, extract random by p_root
		if (rand() % 100 < m_p_root * 100)
		{
			node = root_focal_list.top();
			root_focal_list.pop();
		} else 
		{
			node = focal_list.top();
			focal_list.pop();
		}
	} else if (root_focal_list.size() > 0)
	{
		// we have only root nodes
		node = root_focal_list.top();
		root_focal_list.pop();
	} else 
	{
		// we have only regular nodes
		node = focal_list.top();
		focal_list.pop();
	}

	open_list.erase(node->open_handle);
	return node;
}

void CoCBS::expandRoot(CBSNode * root)
{
	CBSNode* new_root;
	vector<vector<PathEntry>*> copy(paths);
	vector<int> record;

	// create a new root node for every task, by changing its meeting
	for (int ti = 0; ti < num_of_tasks; ++ti)
	{
		if (ti > 0)
			paths = copy; // paths is changed by the planner, so before we move to the next new node, return to original paths
		vector<int> new_meetings_set = root->meetings_set;
		new_meetings_set[ti]++;
		record = new_meetings_set;
		record.push_back(root->assignment_idx); // In the meetings record, the assignment index is at the end
		// check if we already had this set of meetings
		if (m_meetings_record.count(record) > 0)
		{
			if (screen > 1)
				cout << "Meeting skipped..." << endl;
			continue;
		}

		m_meetings_record.emplace(record);

		new_root = new CBSNode(); // TODO Nir check it's released
		new_root->meetings = root->meetings;
		new_root->root = true;
		new_root->meetings_set = new_meetings_set;
		new_root->assignment_idx = root->assignment_idx;
		new_root->assignment = &m_assignments[root->assignment_idx];

		// update ti meeting only to create the next root node
		new_root->meetings[ti] = m_meeting_tables[root->assignment_idx][ti]->getNextMeeting(new_meetings_set[ti]);

		// generate a "brother" root node
		bool solved = generateRootNode(new_root, root, ti);

		if (!solved)
		{
			delete new_root;
			continue;
		}

		num_root_generated++;
		pushNode(new_root);

		// TODO Nir here we need to handle solution found, bypass, PC etc. as in lines 1039-1079 for regular nodes
		if (screen > 1)
		{
			cout << "		Generate " << *new_root << endl;
		}

		// TODO Nir counting conflicts if using bypass (line 1104)
	}

	if (ta_type == search_assignments::FULL)
	{
		// search over the assignments space, by generating more root nodes with different assignments
		int current_assignment_idx = root->assignment_idx;
		int next_assignment_idx = current_assignment_idx + 1;
		if (next_assignment_idx >= num_assignments) {
			if (screen > 0)
				std::cout << "Maximum number of assignments reached (" << num_assignments << "), not expanding more" << std::endl;
			return;
		}
		int last_assignment_idx = m_meeting_tables.size() - 1;
		if (last_assignment_idx < next_assignment_idx) {
			computeMeetingTables(next_assignment_idx);
			// // add the time of the next assignment
			// tot_assignment_time += assignment_times[next_assignment_idx];
		}

		paths = copy;

		record = root->meetings_set;
		record.push_back(next_assignment_idx);
		if (m_meetings_record.count(record) > 0)
		{
			if (screen > 1)
				cout << "Assignment skipped..." << endl;
		} else 
		{ // create new root node with next best assignment
			bool expand_assignment = true;
			for (int s : root->meetings_set)
				if (s > 0) {
					expand_assignment = false;
					break;
				}
			if (expand_assignment) 
			{ // Create new assignments nodes only from root nodes with the first set of meetings
				m_meetings_record.emplace(record);
				new_root = new CBSNode(); // TODO Nir check it's released
				new_root->meetings = root->meetings;
				new_root->root = true;
				new_root->meetings_set = root->meetings_set;
				new_root->assignment_idx = next_assignment_idx;
				new_root->assignment = &m_assignments[next_assignment_idx];

				// generate a new assignment root node
				bool solved = generateRootNode(new_root, root);

				if (solved)
				{
					num_root_generated++;
					pushNode(new_root);

					// TODO Nir here we need to handle solution found, bypass, PC etc. as in lines 1039-1079 for regular nodes
					if (screen > 1)
					{
						cout << "		Generate " << *new_root << endl;
					}

					// TODO Nir counting conflicts if using bypass (line 1104)
				} else
					delete new_root;
			}
		}
	}
}

bool CoCBS::planRootNode(CBSNode* node)
{
	clock_t t1 = clock();

	int task_idx = node->changed_task_idx;
	Task task = m_meeting_tables[node->assignment_idx][task_idx]->getTask();

	// re-plan agent A
	int agA = std::get<2>(task);
	int lowerbound = (int) paths[agA]->size() - 1;
	if (!findPathForSingleAgent(node, agA, lowerbound))
	{
		runtime_generate_child += (double) (clock() - t1) / CLOCKS_PER_SEC;
		runtime_generate_root += (double) (clock() - t1) / CLOCKS_PER_SEC;
		return false;
	}
	// re-plan agent B
	int agB = std::get<4>(task);
	lowerbound = (int) paths[agB]->size() - 1;
	if (!findPathForSingleAgent(node, agB, lowerbound))
	{
		runtime_generate_child += (double) (clock() - t1) / CLOCKS_PER_SEC;
		runtime_generate_root += (double) (clock() - t1) / CLOCKS_PER_SEC;
		return false;
	}

	node->solved = true;

	assert(!node->paths.empty());
	findConflicts(*node);
	heuristic_helper.computeQuickHeuristics(*node); // TODO Nir IMPORTANT!!! do we need this with lazy expansion? because the node was already extracted from the focal list
	runtime_generate_child += (double) (clock() - t1) / CLOCKS_PER_SEC;
	runtime_generate_root += (double) (clock() - t1) / CLOCKS_PER_SEC;
	return true;
}

bool CoCBS::planAssignmentNode(CBSNode* node)
{
	clock_t t1 = clock();
	paths.clear();
	paths.resize(num_of_agents, nullptr); // reset the paths

	// TODO Nir code from generateRoot, put into function
	vector<Path> init_paths(num_of_agents);
	paths_found_initially.emplace_back(init_paths);

	// generate a random permutation of agent indices
	vector<int> agents(num_of_agents);
	for (int i = 0; i < num_of_agents; i++)
	{
		agents[i] = i;
	}

	Path fullPath, agentAtoTaskStart, agentAtoMeeting, agentB_to_meeting, agentB_to_goal;
	Meeting m;
	Task task;
	int meeting_location, meeting_time, next_agentA_start_location, next_agentA_start_time, task_idx;

	for (auto i : agents)
	{
		task_idx = getAgentTask(node->assignment_idx, i);
		m = m_meeting_tables[node->assignment_idx][task_idx]->getNextMeeting(0); // TODO Nir FIRST_MEETING_INDEX
		task = m_meeting_tables[node->assignment_idx][task_idx]->getTask();
		meeting_location = std::get<0>(m);
		meeting_time = std::get<1>(m);
		fullPath.clear(); agentAtoTaskStart.clear(); agentAtoMeeting.clear(); agentB_to_meeting.clear(); agentB_to_goal.clear();

		if (i % 2 == 0) // agent A
		{
			// put the meeting into the node
			node->meetings[task_idx] = m;
			// Plan agent A to task start
			search_engines[i]->setStartLocation(std::get<3>(task)); // TODO Nir should this be done inside the solver?
			search_engines[i]->setGoalLocation(std::get<0>(task)); // TODO Nir should this be done inside the solver?
			initial_constraints[i].goal_location = search_engines[i]->goal_location;
			agentAtoTaskStart = search_engines[i]->findPath(*node, initial_constraints[i], paths, i, 0);
			
			num_LL_expanded += search_engines[i]->num_expanded;
			num_LL_generated += search_engines[i]->num_generated;
			runtime_build_CT += search_engines[i]->runtime_build_CT;
			runtime_build_CAT += search_engines[i]->runtime_build_CAT;

			if (agentAtoTaskStart.size() == 0)
			{
				if (screen > 1)
					std::cout << "No solution agent A (" << i << ") to task start" << endl;
				return false;
			}
			next_agentA_start_location = agentAtoTaskStart.back().location;
			next_agentA_start_time = agentAtoTaskStart.size() - 1;

			// Plan agent A to meeting
			search_engines[i]->setStartLocation(next_agentA_start_location); // TODO Nir should this be done inside the solver?
			search_engines[i]->setGoalLocation(meeting_location); // TODO Nir should this be done inside the solver?
			initial_constraints[i].goal_location = search_engines[i]->goal_location;
			agentAtoMeeting = search_engines[i]->findPath(*node, initial_constraints[i], paths, i, 0, meeting_time, next_agentA_start_time);
			
			num_LL_expanded += search_engines[i]->num_expanded;
			num_LL_generated += search_engines[i]->num_generated;
			runtime_build_CT += search_engines[i]->runtime_build_CT;
			runtime_build_CAT += search_engines[i]->runtime_build_CAT;

			if (agentAtoMeeting.size() == 0)
			{
				if (screen > 1)
					std::cout << "No solution agent A (" << i << ") to meeting" << endl;
				return false;
			}
			// put wait actions at meeting location
			while((int)agentAtoTaskStart.size() + agentAtoMeeting.size() - 1 <= meeting_time)
				agentAtoMeeting.emplace_back(meeting_location);
			//CAT cat(node->makespan + 1);  // initialized to false
			//updateReservationTable(cat, i, *node);
			fullPath = agentAtoTaskStart;
			fullPath.insert(fullPath.end(), agentAtoMeeting.begin() + 1, agentAtoMeeting.end());
		} else { // agentB
			// Plan agent B to meeting
			search_engines[i]->setStartLocation(std::get<5>(task));
			search_engines[i]->setGoalLocation(meeting_location);
			initial_constraints[i].goal_location = search_engines[i]->goal_location;
			agentB_to_meeting = search_engines[i]->findPath(*node, initial_constraints[i], paths, i, 0, meeting_time);
			
			num_LL_expanded += search_engines[i]->num_expanded;
			num_LL_generated += search_engines[i]->num_generated;
			runtime_build_CT += search_engines[i]->runtime_build_CT;
			runtime_build_CAT += search_engines[i]->runtime_build_CAT;
			
			if (agentB_to_meeting.size() == 0)
			{
				if (screen > 1)
					std::cout << "No solution agent B (" << i << ") to meeting" << endl;
				return false;
			}
			// put wait actions at meeting location
			while((int)agentB_to_meeting.size() <= meeting_time)
				agentB_to_meeting.emplace_back(meeting_location);

			// Plan agent B to goal
			search_engines[i]->setStartLocation(meeting_location);
			search_engines[i]->setGoalLocation(std::get<1>(task));
			initial_constraints[i].goal_location = search_engines[i]->goal_location;
			agentB_to_goal = search_engines[i]->findPath(*node, initial_constraints[i], paths, i, 0, -1, meeting_time);
			
			num_LL_expanded += search_engines[i]->num_expanded;
			num_LL_generated += search_engines[i]->num_generated;
			runtime_build_CT += search_engines[i]->runtime_build_CT;
			runtime_build_CAT += search_engines[i]->runtime_build_CAT;

			if (agentB_to_goal.size() == 0)
			{
				if (screen > 1)
					std::cout << "No solution agent B (" << i << ") to goal" << endl;
				return false;
			}
			// fullPath.resize(agentB_to_meeting.size());
			fullPath = agentB_to_meeting;
			fullPath.insert(fullPath.end(), agentB_to_goal.begin() + 1, agentB_to_goal.end());
		}

		// paths_found_initially[i] = search_engines[i]->findPath(*node, initial_constraints[i], paths, i, 0);
		paths_found_initially[node->assignment_idx][i] = fullPath;
		// if (paths_found_initially[i].empty())
		// {
		// 	cout << "No path exists for agent " << i << endl;
		// 	return false;
		// }
		paths[i] = &paths_found_initially[node->assignment_idx][i];
		node->paths.emplace_back(i, fullPath);
		node->makespan = max(node->makespan, paths_found_initially[node->assignment_idx][i].size() - 1);
		node->soc += paths_found_initially[node->assignment_idx][i].size() - 1;
		node->g_val += (int) paths_found_initially[node->assignment_idx][i].size() - 1;
		// num_LL_expanded += search_engines[i]->num_expanded;
		// num_LL_generated += search_engines[i]->num_generated;
	}
	node->solved = true;

	// assert(!node->paths.empty());
	findConflicts(*node);
	// heuristic_helper.computeQuickHeuristics(*node); // TODO Nir IMPORTANT!!! do we need this with lazy expansion? because the node was already extracted from the focal list
	runtime_generate_child += (double) (clock() - t1) / CLOCKS_PER_SEC;
	runtime_generate_root += (double) (clock() - t1) / CLOCKS_PER_SEC;

	return true;
}


Assignment CoCBS::getAssignedAgents(int assignment_idx, int task_idx)
{
	AssignmentSet assignment = m_assignments[assignment_idx];
	Assignment a = assignment[task_idx];
	assert(std::get<0>(a) == task_idx);
	return a;
}

int CoCBS::getAgentTask(int assignment_idx, int agent_idx) const
{
	AssignmentSet assignment = m_assignments[assignment_idx];
	for (Assignment a : assignment)
		if (std::get<1>(a) == agent_idx || std::get<2>(a) == agent_idx)
			return std::get<0>(a);
	assert(false);
}

void CoCBS::printFocalList() const
{
	std::cout << "Focal list" << std::endl;
	for (CBSNode* n : focal_list)
	{
		std::cout << *n << std::endl;
	}
	std::cout << "===============================================================================================" << std::endl;
	std::cout << "Root focal list" << std::endl;
	for (CBSNode* n : root_focal_list)
	{
		std::cout << *n << std::endl;
	}
	std::cout << "===============================================================================================" << std::endl;
}

void CoCBS::printOpenList() const
{
	std::cout << "Open list" << std::endl;
	for (CBSNode* n : open_list)
	{
		std::cout << *n << std::endl;
	}
	std::cout << "===============================================================================================" << std::endl;
}