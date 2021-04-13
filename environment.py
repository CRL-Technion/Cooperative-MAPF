from matplotlib import colors, pyplot
from constraints import Constraints
from classes import Location, State
from planners import TimeAStar, GoalTimeAStar, AllDijkstra
from enum import Enum
import numpy as np
from config import *
from asprilo.visualizer.visualizer.control import VizControl


class GridEnvironment(object):
    """
    A class to represent a 2D grid environment.

    """
    def __init__(self, dimension, tasks, agents, obstacles):
        self.dimension = dimension
        self.obstacles = obstacles

        self.agent_dict = {}
        self.task_dict = {}

        self.make_agent_dict(agents)
        self.make_task_dict(tasks, agents)

        # initialize planners
        self.AStar = TimeAStar(self.dimension, self.obstacles)  # A* with time dimension
        self.TAStar = GoalTimeAStar(self.dimension, self.obstacles)  # A* with time goal
        self.ADijkstra = AllDijkstra(self.dimension, self.obstacles)  # Dijkstra to all locations

    def make_agent_dict(self, agents):
        for agent in agents:
            start_state = State(agent['time'], Location(agent['start'][0], agent['start'][1]))
            goal_state = State(agent['time'], Location(agent['goal'][0], agent['goal'][1]))

            self.agent_dict.update({
                f'{agent["type"]}{agent["id"]}':
                    {
                        'start': start_state,
                        'goal': goal_state,
                        'type': agent['type'],
                        'name': agent['type'] + str(agent["id"])
                    }
            })

    def make_task_dict(self, tasks, agents):
        for ti, task in enumerate(tasks):
            start_state = State(0, Location(task['start'][0], task['start'][1]))  # first agent is going to task start
            goal_state = State(0, Location(task['goal'][0], task['goal'][1]))  # second agent is going to task goal

            self.task_dict.update({
                f'task{task["id"]}':
                    {
                        'start': start_state,
                        'goal': goal_state,
                        'id': task["id"],
                        'agentA': self.agent_dict[agents[ti*2]['type'] + str(agents[ti*2]['id'])],  # TODO handle assignments better
                        'agentB': self.agent_dict[agents[ti*2+1]['type'] + str(agents[ti*2+1]['id'])]
                    }
            })

    def get_assigned_agent(self, task_id):
        agentA = self.task_dict[f'task{task_id}']['agentA']
        agentB = self.task_dict[f'task{task_id}']['agentB']
        return agentA, agentB

    def get_all_locations(self):
        locations = []
        for x in range(0, self.dimension[0]):
            for y in range(0, self.dimension[1]):
                locations.append((x, y))
        return locations

    def get_free_locations(self):
        all_meetings = []
        all_locations = self.get_all_locations()
        for loc in all_locations:
            if loc not in self.obstacles:
                all_meetings.append(loc)
        return all_meetings

    def plan(self, start, goal=None, constraints=Constraints(), goal_time=False):
        if goal is None:
            # plan to all possible locations from start
            return self.ADijkstra.search(start)
        if goal_time:
            # plan from start time and location to goal time and location
            return self.TAStar.search(start,
                                      goal,
                                      constraints)
        else:
            # plan from start location to goal location
            return self.AStar.search(start,
                                     goal,
                                     constraints)

    def show_solution(self, paths, meetings):
        colormap = colors.ListedColormap(["white", "black"])
        pyplot.figure(figsize=(5, 5))
        map = np.zeros((self.dimension[1], self.dimension[0]))
        for o in self.obstacles:
            map[o[1]][o[0]] = 1
        pyplot.imshow(map, cmap=colormap)

        # show paths
        for task in self.task_dict.values():
            agentA = task['agentA']
            agentB = task['agentB']
            pathA = paths[agentA['name']]
            pathB = paths[agentB['name']]
            pyplot.scatter([p['x'] for p in pathA], [p['y'] for p in pathA], s=10, c='red', marker='.')
            pyplot.scatter([p['x'] for p in pathB], [p['y'] for p in pathB], s=10, c='blue', marker='.')

        # show task start and goal
        pyplot.scatter([task['start'].location.x for task in self.task_dict.values()],
                       [task['start'].location.y for task in self.task_dict.values()],
                       s=50, c='green', marker='o', label=f'Task start')
        pyplot.scatter([task['goal'].location.x for task in self.task_dict.values()],
                       [task['goal'].location.y for task in self.task_dict.values()],
                       s=50, c='orange', marker='o', label=f'Task goal')

        # show agents start
        pyplot.scatter([task['agentA']['start'].location.x for task in self.task_dict.values()],
                       [task['agentA']['start'].location.y for task in self.task_dict.values()],
                       s=50, c='red', marker='*', label=f'Agent A start')
        pyplot.scatter([task['agentB']['start'].location.x for task in self.task_dict.values()],
                       [task['agentB']['start'].location.y for task in self.task_dict.values()],
                       s=50, c='blue', marker='*', label=f'Agent B start')

        # show meetings
        pyplot.scatter([m.location.x for m in meetings.values()],
                       [m.location.y for m in meetings.values()],
                       s=50, c='purple', marker='*', linewidths=1, label=f'Meeting Point')

        pyplot.legend()
        pyplot.show()


class RobotType(Enum):
    fetch = 0
    drive = 1


class Warehouse(GridEnvironment):
    def __init__(self, warehouse_env):
        self.env_dict = warehouse_env

        dim = warehouse_env['dim']

        # set shelves as obstacles
        obstacles = [(s[0], s[1]) for s in warehouse_env['shelves']]
        stations = [(s[0], s[1]) for s in warehouse_env['stations']]

        # set orders
        orders = list(warehouse_env['orders'].values())
        K = len(warehouse_env['robots'])  # number of total agents
        assert K % 2 == 0

        # set agents
        agents = []
        for si, (agent_type, s) in enumerate(warehouse_env['robots']):
            agents.append({
                'type': agent_type.name,
                'time': 0,
                'start': s,
                'goal': s,
                'id': int(si / 2)
            })

        # set tasks
        tasks = []
        for ti, t in enumerate(orders):
            tasks.append({
                'start': t,
                'goal': warehouse_env['stations'][ti],
                'id': ti
            })
        super().__init__(dim, tasks, agents, obstacles + stations)

    def asprilo_env(self):
        # asprilo start from 1
        # y axis starts from top
        x_dim = self.env_dict['dim'][0]
        y_dim = self.env_dict['dim'][1]

        lines = []

        # create nodes
        n_id = 1
        for x in range(1, x_dim + 1):
            for y in range(1, y_dim + 1):
                lines.append(f'init(object(node, {n_id}), value(at, ({x}, {y}))).\n')
                n_id += 1

        # create shelves
        for si, s in enumerate(self.env_dict['shelves']):
            shelf_id = si + 1
            x, y = self.transform_loc(s, self.env_dict['dim'])
            lines.append(f'init(object(shelf, {shelf_id}), value(at, ({x}, {y}))).\n')

        # create picking stations
        for pi, p in enumerate(self.env_dict['stations']):
            x, y = self.transform_loc(p, self.env_dict['dim'])
            lines.append(f'init(object(pickingStation, {pi + 1}), value(at, ({x}, {y}))).\n')

        # create robots
        for ri, (rtype, start) in enumerate(self.env_dict['robots']):
            x, y = self.transform_loc(start, self.env_dict['dim'])
            robot_id = int(ri/2) + 1
            robot_type = str(rtype.name)
            lines.append(f'init(object({robot_type}, {robot_id}), value(at, ({x}, {y}))).\n')
            lines.append(f'init(object({robot_type}, {robot_id}), value(max_energy, 0)).\n')
            lines.append(f'init(object({robot_type}, {robot_id}), value(energy, 0)).\n')

        # create products and orders (assuming one product per order)
        # for simplicity, set the product id as the shelf id
        for oi in self.env_dict['orders'].keys():
            shelf_id = self.env_dict['shelves'].index(self.env_dict['orders'].get(oi)) + 1
            quantity = 1
            picking_station_id = 1  # currently assuming one picking station
            lines.append(f'init(object(product, {shelf_id}), value(on, ({shelf_id}, {quantity}))).\n')
            lines.append(f'init(object(order, {oi}), value(pickingStation, {picking_station_id})).\n')
            lines.append(f'init(object(order, {oi}), value(line, ({shelf_id}, {quantity}))).\n')

        return lines

    def asprilo_viz(self, input_file):
        control = VizControl()
        args = [os.path.join(os.getcwd(), 'asprilo', 'visualizer', 'scripts', 'viz'),
                '-t', input_file]
        control.run(args)

    def transform_loc(self, loc, dim):
        x = loc[0] + 1
        y = dim[1] - loc[1]
        return x, y

    def show_env(self):
        lines = self.asprilo_env()
        instance_file = f'{files_folder}instance.txt'
        # write env to file
        with open(instance_file, 'w') as f:
            f.writelines(lines)
        self.asprilo_viz(instance_file)

    def show_solution(self, paths, meetings):
        """
        shows the plans in asprilo (by writing them first to a file)

        :param plans: dictionary of plans with agent name as key
        :return:
        """
        lines = self.asprilo_env()
        # write the plan file for asprilo
        plan_file = f'{files_folder}plan.txt'
        self.write_plan(plan_file,
                        lines,
                        paths)

        # run asprilo visualizer
        self.asprilo_viz(plan_file)

    def write_plan(self, plan_file, instance_lines, plans):
        """
        parses the plans, adds them to instance lines and create a plan file for asprilo

        :param plan_file: path to plan file
        :param instance_lines: lines from instance file
        :param plans: dictionary of plans with agent name as key

        """
        for pi, plan in enumerate(plans):
            # convert to dict
            if isinstance(plans, dict):
                plan_dict = plans[plan]
            else:
                plan_dict = [{'t': p[0], 'x': p[1], 'y': p[2]} for p in plan]
            # parse the plan
            rtype = self.name2type(plan).name
            rid = int(plan.replace(rtype, ''))
            parsed_plan = self.parse_plan(plan_dict, rid, rtype)
            instance_lines += parsed_plan

        with open(plan_file, 'w') as pf:
            pf.writelines(instance_lines)

    def parse_plan(self, plan, agent_id, agent_type):
        """
        parses a plan of (t, x, y) to a list of 'occurs' statements for asprilo

        :param plan: list of steps for agent
        :param agent_id: the agent id (starting in 0)
        :param agent_type: the agent type
        :return: list of lines to be written in a plan file for asprilo
        """
        lines = []
        prev_step = plan[0]
        for step in plan[1:]:
            t = step['t'] - 1
            x_dir = step['x'] - prev_step['x']
            y_dir = -(step['y'] - prev_step['y'])
            prev_step = step
            lines += [f'occurs(object({agent_type}, {agent_id + 1}), action(move, ({x_dir}, {y_dir})), {t}).\n']
        return lines

    def name2type(self, robot_name):
        """
        extract an agent name to a type
        :param robot_name:
        :return: a RobotType value
        """
        for a in RobotType:
            if robot_name.count(a.name) > 0:
                return a
        assert False

    @staticmethod
    def rand_warehouse(seed=0, x_dim=20, y_dim=12, num_tasks=4):
        """
        creates a random warehouse environment

        :param seed: the seed for the random warehouse
        :param x_dim: x dimension
        :param y_dim: y dimension
        :param num_tasks: number of tasks (i.e., number of orders, assuming one product per order)
        :return: a warehouse environment
        """

        # make sure setting is feasible
        if x_dim < 2*num_tasks:
            print('Infeasible warehouse settings')
            exit(1)

        np.random.seed(seed)

        ADJACENT_COLS = 5  # how many adjacent cols with no free passage between them

        k = num_tasks * 2
        num_stations = num_tasks

        dim = [x_dim, y_dim]
        starts = [[x, 0] for x in np.random.choice(range(x_dim), k, replace=False)]

        # create robots
        robots = []
        for si, s in enumerate(starts):
            robots.append((RobotType.fetch if si % 2 == 0 else RobotType.drive, s))

        # set the warehouse layout
        rows_without_shelves = [0, 1] + list(range(4, y_dim - 2, 3)) + [y_dim - 1, y_dim - 2]
        cols_without_shelves = [0] + list(range(ADJACENT_COLS + 1, x_dim - 1, ADJACENT_COLS + 1)) + [x_dim - 1]

        # create shelves
        shelves = []
        for i in range(x_dim):
            for j in range(y_dim):
                if j not in rows_without_shelves and i not in cols_without_shelves:
                    shelves.append([i, j])

        # create products and orders, one product per shelf
        products_on_shelves = [shelves[i] for i in np.random.choice(len(shelves), num_tasks, replace=False)]
        # TODO assuming one product per order
        orders = {i + 1: s for i, s in enumerate(products_on_shelves)}

        # create pickup stations
        stations = [[x, y_dim - 1] for x in np.random.choice(range(x_dim), num_stations, replace=False)]

        # return an environment dictionary
        warehouse_env = {
            'dim': dim,
            'robots': robots,
            'shelves': shelves,
            'orders': orders,
            'stations': stations
        }

        return Warehouse(warehouse_env)
