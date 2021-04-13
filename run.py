"""
A script to run a single experiment.
It receives as input the experiment setup:
benchmark name, cost type, timeout, p_root, number of tasks, and scenario number (1-25).

Before running, it checks for the existence of a file "results_<benchmark_name>.json".
If this file exists and contains a line corresponding with the input experiment, it exits without running.
Otherwise, it runs the experiment and the results is appended to "results_<benchmark_name>.json".

"""

import argparse
import json
import os

from benchmarks import Benchmark
from classes import CostType, EnumEncoder, as_enum

parser = argparse.ArgumentParser()
parser.add_argument("benchmark", help="name of benchmark environment")
parser.add_argument("cost_type", help="cost type, 'MKSP' or 'SOC'")
parser.add_argument('timeout', help="CoCBS timeout in minutes")
parser.add_argument("p_root", help="probability for root node selection")
parser.add_argument("num_tasks", help="number of tasks")
parser.add_argument("scenario_num", help="scenario number, 1-25")
parser.add_argument("--show", action='store_true', help="show the found solution")

args = parser.parse_args()
benchmark = Benchmark(args.benchmark)
exp = (CostType.MKSP if args.cost_type == 'MKSP' else CostType.SOC,
       float(args.timeout),
       float(args.p_root),
       int(args.num_tasks))
scenario = ('random', int(args.scenario_num))

# check if we already ran this
if os.path.exists(f'results_{args.benchmark}.json'):
    with open(f'results_{args.benchmark}.json', 'r') as fp:
        lines = fp.readlines()
        for l in lines:
            data = json.loads(l, object_hook=as_enum)
            data['exp'][0] = CostType.MKSP if data['exp'][0][0] == 'MKSP' else CostType.SOC
            if tuple(data['exp']) == exp and tuple(data['scen']) == scenario:
                print('Experiment skipped')
                exit(0)

# run the experiment
print(f'running {exp} {scenario}')
result = benchmark.run_experiment(exp, scenario, args.show)

# write the result to results file
with open(f'results_{args.benchmark}.json', 'a') as fp:
    fp.write(f'{json.dumps(result, cls=EnumEncoder)}\n')
