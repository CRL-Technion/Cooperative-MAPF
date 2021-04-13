import time
from itertools import product

from CoCBS import CoCBS
from config import *
from environment import GridEnvironment, Warehouse
from matplotlib import pyplot, colors

char_dict = {
    '.': 0,  # passable terrain
    'G': 0,  # passable terrain
    '@': 1,  # out of bounds
    'O': 1,  # out of bounds
    'T': 2,  # trees (unpassable)
    'S': 3,  # swamp (passable from regular terrain)
    'W': 4  # water (traversable, but not passable from terrain)
}


class Benchmark:
    """
    A class to parse, run and show a MAPF benchmark.

    """
    def __init__(self, name):
        self.name = name
        self.map = []
        self.scen_data = []
        if name.startswith('custom_warehouse'):  # TODO this should be more generic
            sz = name.replace('custom_warehouse', '').split('_')[1:]
            self.width = int(sz[0])
            self.height = int(sz[1])
        else:
            self.width = -1
            self.height = -1
            self.parse_map()

    def get_env(self, type='random', num=1, num_tasks=4):
        if self.name.startswith('custom_warehouse'):  # TODO this should be more generic
            return Warehouse.rand_warehouse(num, self.width, self.height, num_tasks)
        else:
            # create agents and tasks
            # get from scen_data start and goal points:
            data = self.parse_scen(type, num)[:num_tasks*2]
            # we use every two scen_data entries as task start and goal, and agent starts
            agents = []
            tasks = []
            for i in range(0, len(data), 2):
                tasks.append({
                    'start': [data[i]['start_x'], data[i]['start_y']],
                    'goal': [data[i]['goal_x'], data[i]['goal_y']],
                    'id': int(i/2)
                })
                agents.append({
                    'type': 'red',
                    'time': 0,
                    'start': [data[i+1]['start_x'], data[i+1]['start_y']],
                    'goal': [data[i+1]['start_x'], data[i+1]['start_y']],  # TODO the goal doesn't matter
                    'id': int(i/2)
                })
                agents.append({
                    'type': 'green',
                    'time': 0,
                    'start': [data[i+1]['goal_x'], data[i+1]['goal_y']],
                    'goal': [data[i+1]['start_x'], data[i+1]['start_y']],
                    'id': int(i/2)
                })
            # create obstacles
            obstacles = []
            for i in range(self.width):
                for j in range(self.height):
                    if self.map[j][i] > 0:
                        obstacles.append((i, j))

            return GridEnvironment([self.width, self.height], tasks, agents, obstacles)

    def parse_map(self):
        with open(f'{benchmarks_folder}/{self.name}/{self.name}.map') as f:
            text = f.read().splitlines()

        type_line = text[0].split(' ')[1]
        self.height = int(text[1].split(' ')[1])
        self.width = int(text[2].split(' ')[1])
        self.map = []
        assert text[3] == 'map'
        for line in text[4:]:
            self.map.append(self.parse_line(line))

    def parse_line(self, line):
        return [char_dict[l] for l in line]

    def parse_scen(self, type='even', num=1):
        with open(f'{benchmarks_folder}/{self.name}/{self.name}-{type}-{num}.scen') as f:
            text = f.read().splitlines()

        scen_data = []

        for line in text[1:]:
            data = line.split()
            scen_data.append({
                'bucket': int(data[0]),
                'map': data[1],
                'map_width': int(data[2]),
                'map_height': int(data[3]),
                'start_x': int(data[4]),
                'start_y': int(data[5]),
                'goal_x': int(data[6]),
                'goal_y': int(data[7]),
                'optimal_length': float(data[8])
            })
        return scen_data

    def show_map(self):
        colormap = colors.ListedColormap(["white", "black", "green", "green", "blue"])
        pyplot.figure(figsize=(5, 5))
        pyplot.imshow(self.map, cmap=colormap)
        x = [d['start_x'] for d in self.scen_data]
        y = [d['start_y'] for d in self.scen_data]
        pyplot.scatter(x, y, s=5, c='red', marker='.')
        x = [d['goal_x'] for d in self.scen_data]
        y = [d['goal_y'] for d in self.scen_data]
        pyplot.scatter(x, y, s=5, c='green', marker="*")
        pyplot.title(self.name,
                     size=24)
        pyplot.show()

    def show_solution(self, solution):
        colormap = colors.ListedColormap(["white", "black", "green", "green", "blue"])
        pyplot.figure(figsize=(5, 5))
        pyplot.imshow(self.map, cmap=colormap)
        for agent in solution:
            path = solution[agent]
            x = []
            y = []
            for p in path:
                x.append(p['x'])
                y.append(p['y'])
            pyplot.scatter(x, y, s=5, c='red', marker='.')
        pyplot.title(self.name,
                     size=24)
        pyplot.show()

    def run_experiment(self, exp, scen, show=False):
        cost_type, timeout, p_root, num_tasks = exp
        scen_type, scen_num = scen
        env = self.get_env(scen_type, scen_num, num_tasks=num_tasks)
        co_cbs = CoCBS(env, cost_type, timeout=timeout, p_root=p_root)
        t = time.time()
        paths, P, statistics = co_cbs.search()
        # env.show_solution(paths, P.meetings_dict)
        elapsed = time.time() - t

        if not paths:
            sol = {}
        else:
            if show:
                env.show_solution(paths, P.meetings_dict)

            sol = {
                'root': P.root,
                'cost': P.cost,
                'num_constraints': sum([x.num_constraints() for x in P.constraint_dict.values()]),
                'meetings_set': P.meetings_set
            }

        return {
            'exp': exp,
            'scen': scen,
            'solution': sol,
            'time': elapsed,
            'statistics': statistics
        }

    def run_experiments(self, variables):
        experiments = product(variables["cost_type"], variables["timeout"], variables["p_root"], variables["num_tasks"])
        results = []

        for exp in experiments:
            scenarios = product(['even', 'random'], range(1, 26))
            cost_type, timeout, p_root, num_tasks = exp
            # loop over all scenarios in the map
            for scen in scenarios:
                r = self.run_experiment(exp, scen)
                results.append(r)

        return results
