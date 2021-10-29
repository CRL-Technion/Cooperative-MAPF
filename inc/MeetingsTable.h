#pragma once

#include "common.h"
#include "Dijkstra.h"
#include <map>
class CoCBS;


class MeetingsTable
{
private:
    int m_task_locations[2]; // task start and goal locations
    int m_agents_locations[2]; // agents start locations
    int m_agents[2]; // agents indices
    Dijkstra* m_planner;
    std::vector<std::multimap<int, Meeting>> meetings;
    CostType m_cost_type;

    void calc_meetings();

public:
    std::map<int, std::map<int, int>> distance_table;

    // Constructor
    MeetingsTable(Dijkstra* planner, Task task, CostType cost_type);

    Meeting getNextMeeting(int snapshot_idx=0);
    Task getTask();
};