from classes import Location


class Conflict(object):
    """
    A class to represent a conflict between two agents.

    """
    VERTEX = 1
    EDGE = 2

    def __init__(self):
        self.time = -1
        self.type = -1

        self.agent_1 = ''
        self.agent_2 = ''

        self.location_1 = Location()
        self.location_2 = Location()

    def __str__(self):
        if self.type == self.VERTEX:
            return '(' + str(self.time) + ', ' + self.agent_1 + ', ' + self.agent_2 + \
                   ', ' + str(self.location_1) + ')'
        return '(' + str(self.time) + ', ' + self.agent_1 + ', ' + self.agent_2 + \
               ', ' + str(self.location_1) + ', ' + str(self.location_2) + ')'

    def __repr__(self):
        return self.__str__()


class VertexConstraint(object):
    """
    A class to represent a vertex constraint imposed on an agent.

    """
    def __init__(self, time, location):
        self.time = time
        self.location = location

    def __eq__(self, other):
        return self.time == other.time and self.location == other.location

    def __hash__(self):
        return hash(str(self.time) + str(self.location))

    def __str__(self):
        return '(' + str(self.time) + ', ' + str(self.location) + ')'

    def __repr__(self):
        return self.__str__()


class EdgeConstraint(object):
    """
    A class to represent an edge constraint imposed on an agent.

    """
    def __init__(self, time, location_1, location_2):
        self.time = time
        self.location_1 = location_1
        self.location_2 = location_2

    def __eq__(self, other):
        return self.time == other.time and self.location_1 == other.location_1 \
               and self.location_2 == other.location_2

    def __hash__(self):
        return hash(str(self.time) + str(self.location_1) + str(self.location_2))

    def __str__(self):
        return '(' + str(self.time) + ', ' + str(self.location_1) + ', ' + str(self.location_2) + ')'

    def __repr__(self):
        return self.__str__()


class Constraints(object):
    """
    A container of constraints.

    """
    def __init__(self):
        self.vertex_constraints = set()
        self.edge_constraints = set()

    def add_constraint(self, other):
        self.vertex_constraints |= other.vertex_constraints
        self.edge_constraints |= other.edge_constraints

    def num_constraints(self):
        return len(self.vertex_constraints) + \
            len(self.edge_constraints)

    def __str__(self):
        return "VC: " + str([str(vc) for vc in self.vertex_constraints]) + \
               "EC: " + str([str(ec) for ec in self.edge_constraints])

    def __repr__(self):
        return self.__str__()
