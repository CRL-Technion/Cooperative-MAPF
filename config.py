import os

files_folder = f'{os.getcwd()}/files/'  # path to input and output files
if not os.path.exists(files_folder):
    os.makedirs(files_folder)
benchmarks_folder = f'{os.getcwd()}/benchmarks/'  # path to benchmark files
verbose = 2  # level of output to print during the run - 0, 1, 2
