"""
A python script to create run scripts for running multiple experiments, e.g., on a remote server.
It defines the benchmarks and parameters to run and creates run scripts for several operating systems.
It gets one optional argument (-e) to determine for which experiments batch to create the run script.
If not passed, or e=0, all experiments are included in the run scripts.
For each benchmark there are 25 scripts, and by setting a value for num_batches, we can also divided the 25
experiments to several batches. num_batches should be 1, 5, or 25.

"""
import argparse
from itertools import product

from classes import CostType

benchmarks = [  # list of benchmarks to run
    'empty-48-48',
    'den312d',
    'maze-32-32-4',
    'warehouse-10-20-10-2-1',
    'custom_warehouse_20_10',
    'custom_warehouse_26_15'
]

variables = {  # list of variables to run with
    'cost_type': [CostType.SOC],
    'timeout': [10],
    'p_root': [0.0, 0.2, 0.5, 0.8],
    'num_tasks': range(1, 9)
}

parser = argparse.ArgumentParser()
parser.add_argument("-e", default=0, help="serial number of experiments setup, 0 means all")
args = parser.parse_args()

scenarios_num = range(1, 26)
num_batches = 1
scenarios_per_batch = int(len(scenarios_num) / num_batches)

num_combs = len(benchmarks) * len(variables['p_root']) * num_batches
all_combs = list(product(benchmarks, variables['p_root'], range(1, num_batches + 1)))
e = int(args.e)
if num_combs >= e > 0:
    idx = e - 1
    benchmarks = [all_combs[idx][0]]
    variables['p_root'] = [all_combs[idx][1]]
    batch = all_combs[idx][2]
    scenarios_num = range((batch - 1) * num_batches + 1, batch * scenarios_per_batch + 1)

num_exp = 0

with open('run_experiments.bat', 'w') as fp_windows, open('run_experiments.sh', 'w') as fp_linux:
    fp_linux.write('#!/bin/bash\n\n')
    for b in benchmarks:
        experiments = product(variables["cost_type"], variables["timeout"], variables["p_root"],
                              variables["num_tasks"])
        for exp in experiments:
            scenarios = product(['random'], scenarios_num)
            cost_type, timeout, p_root, num_tasks = exp
            # loop over all scenarios in the map
            for scen in scenarios:
                scen_type, scen_num = scen

                fp_windows.write(
                    f'python run.py {b} {cost_type.name} {timeout} {p_root} {num_tasks} {scen_num}\n')
                fp_linux.write(
                    f'python run.py {b} {cost_type.name} {timeout} {p_root} {num_tasks} {scen_num}\n')
                num_exp += 1

print(f'Created run_experiments.bat, run_experiments.sh scripts with {num_exp} experiments')
