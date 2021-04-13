from copy import deepcopy
import numpy as np
import time

from environment import Warehouse
from constraints import *
from classes import MeetingsTable, CostType
from sortedcontainers import SortedSet

from config import verbose
from itertools import combinations
from classes import Location, State, Meeting


class CTNode(object):
    def __init__(self, root=False):
        self.solution = {}
        self.constraint_dict = {}
        self.meetings_dict = {}
        self.root = root
        self.cost = 0
        self.meetings_set = []

    def init_constraints(self, agent_names):
        # initialize node empty constraints
        for agent in agent_names:
            self.constraint_dict[agent] = Constraints()

    def clone(self):
        return deepcopy(self)

    def __eq__(self, other):
        if not isinstance(other, type(self)):
            return NotImplemented
        if self.root:
            # for root nodes, compare only the meetings
            return self.meetings_dict == other.meetings_dict
        else:
            return self.meetings_dict == other.meetings_dict \
                   and self.solution == other.solution and self.cost == other.cost

    def __hash__(self):
        return hash(self.cost + sum([x.__hash__() for x in self.meetings_dict.values()]))

    def __lt__(self, other):
        return self.cost < other.cost

    def __str__(self):
        return ('root' if self.root else 'regular') + \
               f' node, cost: {self.cost}, ' + \
               f'num_constraints: {sum([x.num_constraints() for x in self.constraint_dict.values()])}, ' + \
               f'meetings_set: {"".join([str(x) + "," for x in self.meetings_set])[:-1]}'  # + \
        # f'meetings: {self.meetings_dict}'


class CoCBS(object):
    def __init__(self, environment, cost_type=CostType.SOC, timeout=10, p_root=0.0):
        """
        Initializes a Co-CBS object with the environment, cost_type, timeout and p_root value.

        :param environment: environment to plan on
        :param cost_type: either MKSP or SOC
        :param timeout: Co-CBS timeout in minutes
        :param p_root: the probability to extract a root node in case of a tie
        """
        self.env = environment
        self.cost_type = cost_type
        self.timeout = timeout
        self.p_root = p_root

        # Initialize open and closed sets
        self.open_set = SortedSet()
        self.closed_set = SortedSet()
        self.root_open_set = SortedSet()
        self.root_closed_set = SortedSet()

        # Init meeting tables
        self.meeting_tables = {}
        self.meetings_record = []  # a record of all meeting sets we've tried

        # Init the statistics dictionary
        self.statistics = {}

    def search(self):
        """
        Co-CBS main search procedure.

        :return: a solution the corresponding CT node if found, otherwise returns False.
                    Also returns the run statistics.
        """

        started = time.time()
        self.statistics = {
            'iterations': 0,
            'expanded_roots': 0,
            'extracted_root': 0,
            'extracted_regular': 0,
            'meeting_tables_time': 0
        }

        # initialize a meetings table for each task
        t = time.time()
        self.compute_meeting_tables()
        self.statistics["meeting_tables_time"] = time.time() - t

        # Create and initialize the first (root) node
        start = CTNode(root=True)
        start.init_constraints(self.env.agent_dict.keys())
        # set the first set of meetings (lowest-cost meeting for each task)
        for task in self.env.task_dict.keys():
            start.meetings_dict[task] = self.meeting_tables[task].get_next_meeting()

        start.meetings_set = [0 for x in start.meetings_dict.keys()]
        # keep record of sets of meetings we've tried
        self.meetings_record.append(tuple(start.meetings_set))

        # compute paths using low-level planning
        start.solution = self.plan_paths(start.meetings_dict, start.constraint_dict)

        if not start.solution:
            return False, [], self.statistics
        start.cost = self.compute_solution_cost(start.solution)

        # add the first node to root open
        self.root_open_set.add(start)

        iter_num = 1

        # main Co-CBS's loop
        while self.open_set or self.root_open_set:
            self.statistics["iterations"] = iter_num
            if (time.time() - started) / 60 > self.timeout:
                print(f'Reached timeout of {self.timeout} minutes. Stopping.')
                return False, [], self.statistics

            P = self.extract_node()

            if P.root:
                self.statistics["extracted_root"] += 1
            else:
                self.statistics["extracted_regular"] += 1

            my_print(f'iter {iter_num}: ' + str(P), 1)

            iter_num += 1
            # add extracted node to closed list
            self.closed_set.add(P) if not P.root else self.root_closed_set.add(P)

            # get the first conflict in solution
            conflict_dict = self.get_first_conflict(P)
            if not conflict_dict:
                print("solution found:")
                my_print(str(P), 0)
                return self.generate_plan(P.solution), P, self.statistics

            start_time = time.time()
            if P.root:
                my_print('Expanding a root node', 2)
                self.expand_root(P)
            else:
                my_print('Expanding a regular node', 2)

            constraint_dict = self.create_constraints_from_conflict(conflict_dict)

            # split the tree
            for agent in constraint_dict.keys():
                new_node = P.clone()
                # add the constraint to the relevant agent
                new_node.constraint_dict[agent].add_constraint(constraint_dict[agent])
                new_node.root = False

                # compute paths using low-level planning
                new_node.solution = self.plan_paths(new_node.meetings_dict,
                                                    new_node.constraint_dict,
                                                    changed_agents=[agent],
                                                    previous_solution=new_node.solution)

                if not new_node.solution:
                    continue
                new_node.cost = self.compute_solution_cost(new_node.solution)

                if new_node not in self.closed_set:
                    # new node is always regular here
                    self.open_set.add(new_node)

            end_time = time.time()
            my_print('Expansion time: ' +
                     str(round((end_time - start_time) * 1000)) +
                     ' [ms]', 2)  # expansion time in milliseconds

        return False, [], self.statistics

    def compute_meeting_tables(self):
        """
        Calculates the meeting tables - one for each task.
        Uses the planner provided by the environment.

        """
        my_print('Calculating meeting tables for all tasks...', 1)
        for task in self.env.task_dict.keys():
            task_id = self.env.task_dict[task]['id']

            mt = MeetingsTable(self.env, self.env.task_dict[task], self.cost_type)

            self.meeting_tables['task' + str(task_id)] = mt
            my_print(f'{task_id + 1}/{len(self.env.task_dict)}', 1)

    def extract_node(self):
        """
        Extracts the lowest-cost node from the open lists.

        :return: lowest-cost node
        """
        if len(self.root_open_set) and len(self.open_set):
            # extract minimum from both sets
            root_node = self.root_open_set[0]
            regular_node = self.open_set[0]
            # return lowest-cost node
            if regular_node.cost < root_node.cost:
                node = self.open_set.pop(0)
            elif regular_node.cost > root_node.cost:
                node = self.root_open_set.pop(0)
            else:
                # in case we have a root and non-root nodes with the same cost, choose randomly
                if np.random.rand() < self.p_root:
                    node = self.root_open_set.pop(0)
                else:
                    node = self.open_set.pop(0)
        # we have nodes only in one of the lists, return the lowest-cost node
        # from that list
        elif len(self.root_open_set):
            node = self.root_open_set.pop(0)
        else:
            node = self.open_set.pop(0)

        return node

    def expand_root(self, root):
        """
        Expands root node 'root' and adds all new root nodes to the root open list.

        :param root: root node to expand
        """
        # create a new root node for every task, by changing its meeting
        for ti, task in enumerate(self.env.task_dict.keys()):
            new_meetings_set = root.meetings_set.copy()
            new_meetings_set[ti] = new_meetings_set[ti] + 1
            # check if we already had this set of meetings
            if tuple(new_meetings_set) in self.meetings_record:
                my_print("Meeting skipped...", 2)
                continue

            self.statistics["expanded_roots"] += 1

            self.meetings_record.append(tuple(new_meetings_set))

            new_root = root.clone()
            new_root.root = True
            new_root.init_constraints(self.env.agent_dict.keys())

            new_root.meetings_set = new_meetings_set

            # update ti meeting only to create the next root node
            new_root.meetings_dict[task] = self.meeting_tables[task]. \
                get_next_meeting(new_meetings_set[ti])

            # the two agents changed (the ones in 'task')
            changed_agents = [a['name'] for a in self.meeting_tables[task].agents]
            # update the solution for the changed agents
            new_root.solution = self.plan_paths(new_root.meetings_dict,
                                                new_root.constraint_dict,
                                                changed_agents=changed_agents,
                                                previous_solution=new_root.solution)

            # no solution found
            if not new_root.solution:
                continue

            new_root.cost = self.compute_solution_cost(new_root.solution)
            self.root_open_set.add(new_root)

    def plan_paths(self, meetings, constraints, changed_agents=None, previous_solution=None):
        """
        Performs the low-level search of Co-CBS, given a set of meetings and constraints.
        Uses the planner provided by the environment.

        :param meetings: a dictionary of meetings, one for each task
        :param constraints: a dictionary of constraints per agent
        :param changed_agents: which agents changed, for whom we need to re-plan
        :param previous_solution: the current node solution, before the changes
        :return: solution - a path for each agent
        """

        solution = previous_solution if previous_solution is not None else {}
        tasks = self.env.task_dict

        # TODO assuming one task per pair of agents, where 'task0' is assigned to agentA0 and agentB0
        for task in tasks.keys():
            task_start = tasks[task]['start']
            task_goal = tasks[task]['goal']

            # get the agents assigned to this task
            agentA, agentB = self.meeting_tables[task].agents

            # get the meeting between the agents
            meeting = meetings[task]

            # region 1) plan agent A to task start and then to meeting
            if changed_agents is None or agentA['name'] in changed_agents:
                # re-plan agent A, due to meetings or constraints change

                # plan a path for agent A to the task start
                agentA_to_task_start = self.env.plan(agentA['start'],
                                                     task_start,
                                                     constraints[agentA['name']])
                if not agentA_to_task_start:
                    my_print(f'no solution {agentA["name"]} to task start', 1)
                    return False

                # set agent A start as the task start time and location
                next_agentA_start = agentA_to_task_start[-1]

                # given a meeting, plan the rest
                # plan agent A to the meeting
                agentA_to_meeting = self.env.plan(next_agentA_start,
                                                  meeting.state(),
                                                  constraints[agentA['name']],
                                                  goal_time=True)
                if not agentA_to_meeting:
                    my_print(f'no solution {agentA["name"]} to meeting', 1)
                    return False
                agentA_path_to_meeting = agentA_to_task_start + agentA_to_meeting[1:]
                solution.update({agentA['name']: agentA_path_to_meeting})
            # endregion

            # region 2) plan agent B to meeting and then to task goal
            if changed_agents is None or agentB['name'] in changed_agents:
                # re-plan agent B, due to meetings or constraints change

                # plan agent B to the meeting
                agentB_to_meeting = self.env.plan(agentB['start'],
                                                  meeting.state(),
                                                  constraints[agentB['name']],
                                                  goal_time=True)
                if not agentB_to_meeting:
                    my_print(f'no solution {agentB["name"]} to meeting', 1)
                    return False

                # plan agent B to the goal
                agentB_to_goal = self.env.plan(meeting.state(),
                                               task_goal,
                                               constraints[agentB['name']])
                if not agentB_to_goal:
                    my_print(f'no solution {agentB["name"]} to goal', 1)
                    return False
                agentB_path = agentB_to_meeting + agentB_to_goal[1:]
                solution.update({agentB['name']: agentB_path})
            # endregion
        return solution

    def compute_solution_cost(self, solution):
        """
        Computes a solution cost.

        :param solution: dictionary of a path per agent
        :return: cost according to the cost type
        """
        if self.cost_type == CostType.SOC:
            # sum of cost
            return sum([len(path) - 1 for path in solution.values()])  # -1 for compatibility with meetings cost
            # (since here we have also the start location)
        elif self.cost_type == CostType.MKSP:
            # makespan
            return max([len(path) - 1 for path in solution.values()])
        assert False

    def create_constraints_from_conflict(self, conflict):
        """
        Creates a Constraints() object from a Conflict object.

        :param conflict: a Conflict object
        :return: a Constraints object
        """
        constraint_dict = {}
        if conflict.type == Conflict.VERTEX:
            v_constraint = VertexConstraint(conflict.time, conflict.location_1)
            constraint = Constraints()
            constraint.vertex_constraints |= {v_constraint}
            constraint_dict[conflict.agent_1] = constraint
            constraint_dict[conflict.agent_2] = constraint

        elif conflict.type == Conflict.EDGE:
            constraint1 = Constraints()
            constraint2 = Constraints()

            e_constraint1 = EdgeConstraint(conflict.time, conflict.location_1, conflict.location_2)
            e_constraint2 = EdgeConstraint(conflict.time, conflict.location_2, conflict.location_1)

            constraint1.edge_constraints |= {e_constraint1}
            constraint2.edge_constraints |= {e_constraint2}

            constraint_dict[conflict.agent_1] = constraint1
            constraint_dict[conflict.agent_2] = constraint2

        return constraint_dict

    def generate_plan(self, solution):
        plan = {}
        for agent, path in solution.items():
            path_dict_list = [{'t': state.time, 'x': state.location.x, 'y': state.location.y} for state in path]
            plan[agent] = path_dict_list
        return plan

    def get_first_conflict(self, node):
        """
        Returns the first conflict found in the solution of node

        :param node: node to check conflicts
        :return: a Conflict object if a conflict found, otherwise False
        """
        solution = node.solution
        max_t = max([len(plan) for plan in solution.values()])
        result = Conflict()
        for t in range(max_t):
            # check vertex conflicts
            for agent_1, agent_2 in combinations(solution.keys(), 2):
                state_1 = self.get_state(agent_1, solution, t)
                state_2 = self.get_state(agent_2, solution, t)
                if state_1.is_equal_except_time(state_2) and not state_1.location.x == -1:
                    meeting1 = Meeting(t, state_1.location, [agent_1, agent_2], 0)
                    meeting2 = Meeting(t, state_1.location, [agent_2, agent_1], 0)
                    if meeting1 in node.meetings_dict.values() or meeting2 in node.meetings_dict.values():
                        continue
                    result.time = t
                    result.type = Conflict.VERTEX
                    result.location_1 = state_1.location
                    result.agent_1 = agent_1
                    result.agent_2 = agent_2
                    return result

            # check edge (swapping) conflicts
            for agent_1, agent_2 in combinations(solution.keys(), 2):
                state_1a = self.get_state(agent_1, solution, t)
                state_1b = self.get_state(agent_1, solution, t + 1)

                state_2a = self.get_state(agent_2, solution, t)
                state_2b = self.get_state(agent_2, solution, t + 1)

                if state_1a.is_equal_except_time(state_2b) and state_1b.is_equal_except_time(state_2a) \
                        and not state_1a.location.x == -1:
                    result.time = t
                    result.type = Conflict.EDGE
                    result.agent_1 = agent_1
                    result.agent_2 = agent_2
                    result.location_1 = state_1a.location
                    result.location_2 = state_1b.location
                    return result
        return False

    def get_state(self, agent_name, solution, t):
        """
        Returns an agent state at time t in the given solution.

        :param agent_name: agent to query in solution
        :param solution: list of paths of all agents
        :param t: time to query
        :return: the state of agent_name at time t
        """
        if t < len(solution[agent_name]):
            return solution[agent_name][t]
        else:
            # TODO when an agent rests at its location, return as if it's outside the board so it won't collide.
            #   We probably should do something more elegant.
            return State(solution[agent_name][-1].time, Location(-1, -1))

    # for debug
    def show_open_set(self, root=False):
        """
        DEBUG method. Shows the open list (root or regular) and costs.

        :param root: whether to show root open list
        """
        print('root | cost')
        print('===========')
        for s in (self.open_set if not root else self.root_open_set):
            print(('  x  | ' if s.root else '     | ') + str(s.cost))

    def show_closed_set(self, root=False):
        """
        DEBUG method. Shows the closed list (root or regular) and costs.

        :param root: whether to show root closed list
        """
        print('root | cost')
        print('===========')
        for s in (self.closed_set if not root else self.root_closed_set):
            print(('  x  | ' if s.root else '     | ') + str(s.cost))


def my_print(s, level):
    if verbose >= level:
        print(s)


def run_and_show(warehouse_env):
    cost_type = CostType.SOC
    co_cbs = CoCBS(warehouse_env, cost_type, timeout=10, p_root=0.5)

    t = time.time()
    paths, N, statistics = co_cbs.search()

    if not paths:
        print('No solution')
        exit()

    meetings, cost = N.meetings_dict, N.cost

    print(f'Co-CBS elapsed: {np.round(time.time() - t, 2)} [sec], cost: {cost} ({cost_type.name})')

    warehouse_env.show_solution(paths, meetings)


if __name__ == "__main__":
    # Run Co-CBS on a random warehouse
    seed = 2
    k = 8   # number of tasks (i.e., 2*k agents)
    X = 26  # warehouse horizontal size
    Y = 15  # warehouse vertical size
    # Generate a custom warehouse environment
    warehouse_env = Warehouse.rand_warehouse(seed, num_tasks=k, x_dim=X, y_dim=Y)
    # Run Co-CBS and show results using asprilo
    run_and_show(warehouse_env)
