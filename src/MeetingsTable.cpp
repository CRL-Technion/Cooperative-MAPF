#include "MeetingsTable.h"
#include "CoCBS.h"
#include <map>

MeetingsTable::MeetingsTable(Dijkstra* planner, Task task, CostType cost_type)
: m_planner(planner), m_cost_type(cost_type)
{	
    std::tie(m_task_locations[0], m_task_locations[1], 
        m_agents[0], m_agents_locations[0], 
        m_agents[1], m_agents_locations[1]) = task;   
    calc_meetings();
}

void MeetingsTable::calc_meetings()
{

    meetings.resize(1);
    // TODO Nir these 4 regions can be in a single function with 4 calls, or at least a single for loop

#pragma region Plan from agent A start to all locations
    std::map<int, int> distance_agentA_start_to_all = m_planner->search_l(m_agents_locations[0]);
    // add to distnace table
    distance_table.insert(std::pair<int, std::map<int, int>>(m_agents_locations[0], distance_agentA_start_to_all));
    int task_start_time = distance_agentA_start_to_all[m_task_locations[0]];
#pragma endregion

#pragma region Plan from task start to all locations
    std::map<int, int> distance_task_start_to_all = m_planner->search_l(m_task_locations[0]);
    // add to distnace table
    distance_table.insert(std::pair<int, std::map<int, int>>(m_task_locations[0], distance_task_start_to_all));
#pragma endregion

#pragma region Plan from agentB start to all locations
    std::map<int, int> distance_agentB_start_to_all = m_planner->search_l(m_agents_locations[1]);
    // add to distnace table
    distance_table.insert(std::pair<int, std::map<int, int>>(m_agents_locations[1], distance_agentB_start_to_all));
#pragma endregion

#pragma region Plan from task goal to all locations
    std::map<int, int> distance_goal_to_all = m_planner->search_l(m_task_locations[1]);
    // add to distnace table
    distance_table.insert(std::pair<int, std::map<int, int>>(m_task_locations[1], distance_goal_to_all));
#pragma endregion

#pragma region Construct the meetings table
    int meeting_location, meeting_time, len_to_goal, cost;
    for (std::map<int, int>::iterator itr = distance_task_start_to_all.begin(); itr != distance_task_start_to_all.end(); ++itr) 
    {
        meeting_location = itr->first;
        meeting_time = max(distance_task_start_to_all[meeting_location] + task_start_time, distance_agentB_start_to_all[meeting_location]);
        len_to_goal = distance_goal_to_all[meeting_location];
        if (m_cost_type == SOC)
            cost = (2 * meeting_time + len_to_goal);
        else
            cost = (meeting_time + len_to_goal);

        // <time, location, agentA, agentB, cost>
        Meeting m = std::make_tuple(meeting_location, meeting_time, m_agents[0], m_agents[1], cost);
        meetings[0].insert(std::pair<int, Meeting>(cost, m));
    }
#pragma endregion
}

Meeting MeetingsTable::getNextMeeting(int snapshot_idx)
{
    size_t last_snapshot = meetings.size() - 1;
    Meeting meeting;
    int current_loc, current_time, current_cost, next_time, next_cost;
    if (snapshot_idx > last_snapshot) // this snapshot does not exist, create all snapshots until this
    {
        while (last_snapshot<snapshot_idx)
        {
            current_cost = meetings[last_snapshot].begin()->first;
            meeting = meetings[last_snapshot].begin()->second;
            current_loc = std::get<0>(meeting);
            current_time = std::get<1>(meeting);

            assert(current_cost == std::get<4>(meeting)); // make sure table is consistent

            next_time = current_time + 1;
            next_cost = current_cost + (m_cost_type == MKSP ? 1 : 2);

            // create a new snapshot
            std::multimap<int, Meeting> new_snapshot = meetings[last_snapshot];

            // update the new snapshot (cost and time) to the next meeting in current location
            Meeting new_meeting = meeting;
            std::get<1>(new_meeting) = next_time;
            std::get<4>(new_meeting) = next_cost;
            new_snapshot.erase(new_snapshot.begin());
            new_snapshot.insert(std::make_pair(next_cost, new_meeting));
            meetings.push_back(new_snapshot);
            last_snapshot++;
        }
    }

    // Extract the lowest-cost meeting from table
    return meetings[snapshot_idx].begin()->second;
}

Task MeetingsTable::getTask()
{
    return std::make_tuple(m_task_locations[0], m_task_locations[1], m_agents[0], m_agents_locations[0], m_agents[1], m_agents_locations[1]);
}