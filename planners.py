from functools import lru_cache
from math import fabs
from constraints import *
from classes import Location, State
from heapq import heappush, heappop
import numpy as np


class SearchNode:
    __slots__ = ('data', 'gscore', 'fscore',
                 'closed', 'came_from', 'out_openset')

    def __init__(self, data, gscore=np.inf, fscore=np.inf):
        self.data = data
        self.gscore = gscore
        self.fscore = fscore
        self.closed = False
        self.out_openset = True
        self.came_from = None

    def __lt__(self, b):
        return self.fscore < b.fscore

    def __repr__(self):
        return self.data.__repr__()


class SearchNodeDict(dict):

    def __missing__(self, k):
        v = SearchNode(k)
        self.__setitem__(k, v)
        return v


class TimeAStar:
    """
    Plan A* on a grid world with time

    """

    def __init__(self, dim, obstacles):
        self.dim = dim
        self.obstacles = obstacles  # static obstacles
        self.constraints = Constraints()  # "dynamic" obstacles
        self.start = []
        self.goal = []

    @lru_cache(maxsize=None)
    def search(self, start, goal, constraints=Constraints()):
        self.start = start
        self.goal = goal
        self.constraints = constraints

        path = self.astar(start, goal)
        return list(path) if path is not None else None

    def astar(self, start, goal, reversePath=False):
        if self.is_goal_reached(start, goal):
            return [start]
        searchNodes = SearchNodeDict()
        startNode = searchNodes[start] = SearchNode(
            start, gscore=.0, fscore=self.heuristic_cost_estimate(start, goal))
        openSet = []
        heappush(openSet, startNode)
        while openSet:
            current = heappop(openSet)
            if self.is_goal_reached(current.data, goal):
                return self.reconstruct_path(current, reversePath)
            current.out_openset = True
            current.closed = True
            for neighbor in map(lambda n: searchNodes[n], self.neighbors(current.data, self.constraints, self.goal)):
                if neighbor.closed:
                    continue
                tentative_gscore = current.gscore + \
                    self.distance_between(current.data, neighbor.data)
                if tentative_gscore >= neighbor.gscore:
                    continue
                neighbor.came_from = current
                neighbor.gscore = tentative_gscore
                neighbor.fscore = tentative_gscore + \
                    self.heuristic_cost_estimate(neighbor.data, goal)
                if neighbor.out_openset:
                    neighbor.out_openset = False
                    heappush(openSet, neighbor)
                else:
                    # re-add the node in order to re-sort the heap
                    openSet.remove(neighbor)
                    heappush(openSet, neighbor)
        return None

    def is_goal_reached(self, current, goal):
        return current.is_equal_except_time(goal)

    def heuristic_cost_estimate(self, current, goal):
        return fabs(current.location.x - goal.location.x) + fabs(current.location.y - goal.location.y)

    def distance_between(self, n1, n2):
        return 1

    @lru_cache(maxsize=None)
    def neighbors(self, state, constraints, goal):
        neighbors = []
        t = state.time + 1

        # Wait action
        n = State(t, Location(state.location.x, state.location.y))
        if self.state_valid(n, constraints, goal):
            neighbors.append(n)

        # Up action
        n = State(t, Location(state.location.x, state.location.y + 1))
        if self.state_valid(n, constraints, goal) and \
                self.transition_valid(state, n, constraints):
            neighbors.append(n)

        # Down action
        n = State(t, Location(state.location.x, state.location.y - 1))
        if self.state_valid(n, constraints, goal) and \
                self.transition_valid(state, n, constraints):
            neighbors.append(n)

        # Left action
        n = State(t, Location(state.location.x - 1, state.location.y))
        if self.state_valid(n, constraints, goal) and \
                self.transition_valid(state, n, constraints):
            neighbors.append(n)

        # Right action
        n = State(t, Location(state.location.x + 1, state.location.y))
        if self.state_valid(n, constraints, goal) and \
                self.transition_valid(state, n, constraints):
            neighbors.append(n)
        return neighbors

    def reconstruct_path(self, last, reversePath=False):
        def _gen():
            current = last
            while current:
                yield current.data
                current = current.came_from

        if reversePath:
            return _gen()
        else:
            return reversed(list(_gen()))

    @lru_cache(maxsize=None)
    def state_valid(self, state, constraints, goal):
        valid = True
        valid &= 0 <= state.location.x < self.dim[0]
        valid &= 0 <= state.location.y < self.dim[1]
        valid &= VertexConstraint(state.time, state.location) not in constraints.vertex_constraints
        valid &= state.location.tuple() not in self.obstacles or \
            self.is_goal_reached(state, goal)  # the current goal is always valid

        return valid

    @lru_cache(maxsize=None)
    def transition_valid(self, state_1, state_2, constraints):
        return EdgeConstraint(state_1.time, state_1.location, state_2.location) not in constraints.edge_constraints


class GoalTimeAStar(TimeAStar):
    """
    Plan A* on a grid world with a time goal,
    i.e., we need to get to a goal location on a specific given time step.

    """
    def is_goal_reached(self, current, goal):
        return current == goal

    def neighbors(self, state, constraints, goal):
        if state.time >= self.goal.time:
            return []
        return super().neighbors(state, constraints, goal)


class AllDijkstra:
    """
    Plan paths from a single location to all other locations.

    """
    def __init__(self, dim, obstacles):
        self.dim = dim
        self.obstacles = obstacles
        self.start = []
        self.goal = []

        self.visited = {}
        self.came_from = {}

    @lru_cache(maxsize=None)
    def search(self, start):
        self.start = start

        path = self.dijkstra(start)
        return path

    def dijkstra(self, start, reversePath=False):
        searchNodes = SearchNodeDict()
        self.came_from = {}
        startNode = searchNodes[start] = SearchNode(
            start, gscore=.0, fscore=.0)
        openSet = []
        heappush(openSet, startNode)
        while openSet:
            current = heappop(openSet)
            if current.data.location != startNode.data.location:
                self.visited[current.data.location.tuple()] = True
            current.out_openset = True
            current.closed = True
            for neighbor in map(lambda n: searchNodes[n], self.neighbors(current.data)):
                if neighbor.closed:
                    continue
                tentative_gscore = current.gscore + \
                                   self.distance_between(current.data, neighbor.data)
                if tentative_gscore >= neighbor.gscore:
                    continue
                neighbor.came_from = current
                self.came_from[neighbor.data.location.tuple()] = current.data.location.tuple()
                neighbor.gscore = tentative_gscore
                neighbor.fscore = tentative_gscore
                if neighbor.out_openset:
                    neighbor.out_openset = False
                    heappush(openSet, neighbor)
                else:
                    # re-add the node in order to re-sort the heap
                    openSet.remove(neighbor)
                    heappush(openSet, neighbor)
        return self.reconstruct_path()

    def distance_between(self, n1, n2):
        return 1

    def reconstruct_path(self, reversePath=False):
        def _gen(came_from, goal):
            current = goal
            while current in came_from.keys():
                yield current
                current = came_from[current]

        paths = {}
        for key in self.visited:
            if reversePath:
                paths[key] = list(_gen(self.came_from, key))
            else:
                paths[key] = list(reversed(list(_gen(self.came_from, key))))
        return paths

    def neighbors(self, state):
        neighbors = []
        t = state.time

        # Wait action
        n = State(t, Location(state.location.x, state.location.y))
        if self.state_valid(n):
            neighbors.append(n)

        # Up action
        n = State(t, Location(state.location.x, state.location.y + 1))
        if self.state_valid(n):
            neighbors.append(n)

        # Down action
        n = State(t, Location(state.location.x, state.location.y - 1))
        if self.state_valid(n):
            neighbors.append(n)

        # Left action
        n = State(t, Location(state.location.x - 1, state.location.y))
        if self.state_valid(n):
            neighbors.append(n)

        # Right action
        n = State(t, Location(state.location.x + 1, state.location.y))
        if self.state_valid(n):
            neighbors.append(n)
        return neighbors

    def state_valid(self, state):
        valid = True
        valid &= 0 <= state.location.x < self.dim[0]
        valid &= 0 <= state.location.y < self.dim[1]
        valid &= state.location.tuple() not in self.obstacles
        return valid

    def transition_valid(self, state_1, state_2):
        return EdgeConstraint(state_1.time, state_1.location, state_2.location) not in self.constraints.edge_constraints
