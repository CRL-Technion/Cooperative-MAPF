# Cooperative Multi-Agent Path Finding (Co-MAPF)
Project description, reference to paper...

[![A Cooperative Warehouse Illustration](warehouse-illustration_final.png?raw=true "A Cooperative Warehouse Illustration")](https://drive.google.com/file/d/12byXaWdHV-CQZonSEQ0lzeAN2CkwTuJk/view?usp=sharing)

## Getting Started
### Installation
To run and visualize the Co-MAPF environment and Co-CBS algorithm, 
we provide an [Anaconda](https://docs.anaconda.com/anaconda/install/) environment with all prerequisites already installed.

To get started, first clone the repository
```
git clone https://github.com/CRL-Technion/Cooperative-MAPF.git
```
Run the following commands to setup the environment using Anaconda
```
cd CooperativeWarehouse
conda env create -f Co-MAPF.yml
conda activate Co-MAPF
```
### Running an example

To run Co-CBS algorithm on an example warehouse environment, and visualize using the [asprilo visualizer](https://asprilo.github.io/visualizer/), run
```
python Co-CBS.py
```

You can change the level of output printed to console by Co-CBS during the run,
by changing the verbose variable in ```config.py```, to 0,1,2.


## Running Experiments
You can backtrack benchmark experiments presented in the paper by running the ```run.py``` script using the following arguments

### Standard benchmarks
This repository contains standard MAPF benchmarks taken from the [Nathan Sturtevant's Moving AI Lab](https://movingai.com/benchmarks/mapf.html):
* empty-48-48
* den312d
* maze-32-32-4
* warehouse-10-20-10-2-1

All benchmark files are stored in the benchmarks folder.
You can download more benchmarks from the website, put all the files in a folder with the benchmark name, and run it.

### Custom warehouse benchmark
In addition to standard MAPF benchmarks, you can generate and run on custom warehouse environments.
To do so, no external files need to be created, just use the pattern ```custom_warehouse_<x>_<y>``` as the benchmark name,
with the desired x and y dimensions.

### Single experiment
```
python run.py [--show] <benchmark_name> <cost_type> <timeout_min> <p_root> <num_tasks> <scenario_num>
```
where
* [--show] - use this flag to show the found solution.
* <benchmark_name> - the name of the benchmark to run. This repository provides four benchmark sets taken 
  from [Nathan Sturtevant's Moving AI Lab](https://movingai.com/benchmarks/mapf.html).
  In addition, you can also run a custom warehouse environment by using a benchmark name of the 
  form ```custom_warehouse_<x>_<y>```, for example ```custom_warehouse_20_10```.
* <cost_type> - either ```SOC``` or ```MKSP```.
* <timeout_min> - Co-CBS timeout in minutes.
* <p_root> - p_root value, a real number between 0.0 and 1.0.
* <num_tasks> - number of tasks, an integer greater than 0.
* <scenario_num> - benchmark scenario number. For the standard benchmark shoud be an integer between 1-25.
    For the custom warehouse environment can be any integer to set the seed of the random environment.

Examples:
```
python run.py den312d SOC 10 0.0 8 7
python run.py --show custom_warehouse_20_10 MKSP 10 0.0 6 182
```
### Batch of experiments
The ```run_batch.py``` script provided is used to create a shell script to run a batch of experiments.

At the top of the scripts one can define the benchmarks and variables to run, and running ```python run_batch.py```
will create ```run_experiments.bat``` and ```run_experiments.sh``` to run on Windows and Linux, respectively.
The run script created will contain a series of ```python run.py ...``` commands with the defined benchmarks and variables.

You can also create mini-batches by passing the ```-e <i>``` argument with an integer to ```run_batch.py```.
Please see the script documentation for more details.


## Acknowledgments
* The Co-CBS implementation is partially based on CBS implemented in [atb033/multi_agent_path_planning](https://github.com/atb033/multi_agent_path_planning.git).
* The custom warehouse visualizer is done using [asprilo](https://asprilo.github.io/visualizer/), with minor modifications.
* Standard MAPF benchmarks are taken from the [Nathan Sturtevant's Moving AI Lab](https://movingai.com/benchmarks/mapf.html).
