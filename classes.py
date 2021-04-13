from copy import deepcopy
from enum import Enum
from sortedcontainers import SortedList
import json


class Location(object):
    """
    A class to represent a location in a 2D grid.
    Attributes: x, y

    """
    def __init__(self, x=-1, y=-1):
        self.x = x
        self.y = y

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    def __str__(self):
        return str((self.x, self.y))

    def __hash__(self):
        return hash(str(self.x) + str(self.y))

    def __repr__(self):
        return f'({self.x},{self.y})'

    def tuple(self):
        """
        Returns a tuple of the location.

        :return: A tuple (x, y)
        """
        return self.x, self.y


class State(object):
    """
    A class to represent a state in a 2D grid, i.e., location and time.
    Attributes: time, location (Location)

    """
    def __init__(self, time, location):
        self.time = time
        self.location = location

    def __eq__(self, other):
        return self.time == other.time and self.location == other.location

    def __gt__(self, other):
        return self

    def __hash__(self):
        return hash(str(self.time) + str(self.location.x) + str(self.location.y))

    def __add__(self, other):
        return (self.time, self.location.x, self.location.y) + \
               (other.time, other.location.x, other.location.y)

    def __str__(self):
        return str((self.time, self.location.x, self.location.y))

    def __repr__(self):
        return self.__str__()

    def is_equal_except_time(self, state):
        """
        Check if two states have equal locations.

        :param state: the rhs state
        :return: True if the locations are equal
        """
        return self.location == state.location

    def tuple(self):
        """
        Returns a tuple of the state.

        :return: A tuple (time, x, y)
        """
        return self.time, self.location.x, self.location.y


class Meeting(State):
    """
    A class to represent a meeting in a 2D grid.
    Other than the meeting time and location, it also contains the involved agents and the meeting cost.
    Attributes: time, location (Location), agents (list of strings), cost

    """
    def __init__(self, time, location, agents, cost):
        super().__init__(time, location)

        # TODO currently meetings of two agents only
        assert len(agents) == 2
        self.agent_1 = agents[0]
        self.agent_2 = agents[1]

        self.cost = cost

    def __eq__(self, other):
        return self.time == other.time and \
               self.location == other.location and \
               self.agent_1 == other.agent_1 and \
               self.agent_2 == other.agent_2

    def __hash__(self):
        return hash(str(self.time) + str(self.location))

    def __str__(self):
        return self.agent_1 + ' and ' + self.agent_2 + ' at (' + str(self.time) + ', ' + str(self.location) + ')'

    def __repr__(self):
        return self.__str__()

    def state(self):
        """
            Returns a State representation of the meeting.

            :return: A State
            """
        return State(self.time, self.location)


class MeetingsTable:
    """
    A class to represent a meetings table for a single task.
    All meetings are calculated upon creation.

    """
    def __init__(self, env, task, cost_type):
        """
        Initializes and calculates the meetings table.

        :param env: the environment
        :param task: a task dictionary
        :param cost_type: the cost type (MKSP or SOC)
        """

        self.env = env
        self.task = task
        self.agents = [task['agentA'], task['agentB']]
        self.cost_type = cost_type

        self.meetings = []  # a list of snapshots of the table
        self.calc_meetings()

    def calc_meetings(self):
        """
        Calculates the initial snapshot of the meetings table.
        All meetings are stored in the 'meetings' attribute.

        """
        agentA, agentB = self.agents
        task_start = self.task['start']
        task_goal = self.task['goal']

        # plan a path for the agent A to task start location WITHOUT constraints
        agentA_to_task_start = self.env.plan(agentA['start'], task_start)
        if not agentA_to_task_start:
            assert False
        task_start_time = agentA_to_task_start[-1].time

        # plan with triple dijkstra for all states
        # plan from task start to all points
        path_agentA_to_meeting = self.env.plan(task_start)
        distance_agentA_to_all_via_task_start = {k: task_start_time + len(path_agentA_to_meeting[k])
                                         for k in path_agentA_to_meeting}

        # plan for agent B to all points
        path_agentB_to_meeting = self.env.plan(agentB['start'])
        distance_agentB_start_to_all = {k: len(path_agentB_to_meeting[k]) for k in path_agentB_to_meeting}

        # plan for goal to all points
        path_meeting_to_goal = self.env.plan(task_goal)
        distance_goal_to_all = {k: len(path_meeting_to_goal[k]) for k in path_meeting_to_goal}

        # save the meetings (location, time, cost, length to goal) as a priority queue
        self.meetings.append({
            'sorted': SortedList(key=lambda x: x[1]),
            'costs': {},
            'meeting_time': {},
            'len_to_goal': {}
        })

        # TODO the issue of free location should be handled more carefully, also in the planners
        meeting_locations = self.env.get_free_locations()

        for loc in meeting_locations:
            if loc not in path_agentA_to_meeting.keys() or \
                loc not in path_agentB_to_meeting.keys() or \
                    loc not in path_meeting_to_goal.keys():
                continue
            meeting_time = max(distance_agentA_to_all_via_task_start[loc], distance_agentB_start_to_all[loc])
            len_to_goal = distance_goal_to_all[loc]
            cost = (meeting_time + len_to_goal) if self.cost_type == CostType.MKSP else (2 * meeting_time + len_to_goal)

            self.meetings[0]['sorted'].add((loc, cost))
            self.meetings[0]['costs'][loc] = cost
            self.meetings[0]['meeting_time'][loc] = meeting_time
            self.meetings[0]['len_to_goal'][loc] = len_to_goal

    def get_next_meeting(self, snapshot_idx=0):
        """
        Get the next best meetings of the desired snapshot.
        Whenever the best meeting in table is returned, a new snapshot is created by advancing the time
        of the best meeting by 1.

        :param snapshot_idx:
        :return:
        """
        last_snapshot = len(self.meetings) - 1
        if snapshot_idx > last_snapshot:
            # this snapshot does not exist, create all snapshots until this
            while last_snapshot < snapshot_idx:
                # take the current_meeting from last snapshot
                current_loc, meeting_cost = self.meetings[last_snapshot]['sorted'][0]  # accessing a sorted list
                current_time = self.meetings[last_snapshot]['meeting_time'][current_loc]
                assert meeting_cost == self.meetings[last_snapshot]['costs'][current_loc]  # make sure list is consistent

                cost_in_table = self.meetings[last_snapshot]['costs'][current_loc]
                time_in_table = self.meetings[last_snapshot]['meeting_time'][current_loc]
                # key_in_sorted = (current_loc, cost_in_table)
                # idx_in_sorted = self.meetings[last_snapshot]['sorted'].index(key_in_sorted)

                # calculate the next meeting time (and its cost)
                next_time = current_time + 1
                next_cost = cost_in_table + ((next_time - time_in_table)
                                             if self.cost_type == CostType.MKSP
                                             else 2 * (next_time - time_in_table))

                # create a new snapshot
                new_snapshot = deepcopy(self.meetings[last_snapshot])

                # update the new snapshot (cost and time) to the next meeting in current location
                new_snapshot['costs'][current_loc] = next_cost
                new_snapshot['meeting_time'][current_loc] = next_time
                del new_snapshot['sorted'][0]
                new_snapshot['sorted'].add((current_loc, next_cost))
                self.meetings.append(new_snapshot)
                last_snapshot += 1

            assert len(self.meetings) == snapshot_idx + 1

        min_meeting, meeting_cost = self.meetings[snapshot_idx]['sorted'][0]  # accessing a sorted list
        assert meeting_cost == self.meetings[snapshot_idx]['costs'][min_meeting]  # make sure list is consistent
        meeting_time = self.meetings[snapshot_idx]['meeting_time'][min_meeting]  # access in O(1)

        return Meeting(meeting_time,
                       Location(min_meeting[0], min_meeting[1]),
                       [self.agents[0]['name'], self.agents[1]['name']],
                       meeting_cost)


class CostType(Enum):
    """
    A cost type enumeration.

    """
    MKSP = 0
    SOC = 1


class EnumEncoder(json.JSONEncoder):
    """
    A class to write the CostType enum to a json file.

    """
    def default(self, obj):
        if isinstance(obj, CostType):
            return {"__costtype__": obj.name}
        return json.JSONEncoder.default(self, obj)


def as_enum(d):
    """
    A method to convert a string to enum when reading from a json file.

    :param d: string.
    :return: A CostType enum.
    """
    if "__costtype__" in d:
        return [d["__costtype__"]]
    else:
        return d

