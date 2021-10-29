# Cooperative Multi-Agent Path Finding (Co-MAPF)
Code for the Cooperative Multi-Agent Path Finding (Co-MAPF) framework and the Cooperative Conflict-Based Search (Co-CBS) algorithm.
Co-CBS is based on Conflict-Based Search (CBS) [2].
Please refer to the paper for more details:

Greshler, N., Gordon, O., Salzman, O., & Shimkin, N. (2021). Cooperative Multi-Agent Path Finding: Beyond Path Planning and Collision Avoidance.

[![A Cooperative Warehouse Illustration](warehouse_illustration.png?raw=true "A Cooperative Warehouse Illustration")](https://drive.google.com/file/d/15o0r0-9m2DEK72Ef7LOFgfS-SoU13VYC/view?usp=sharing)

This code is heavily based on the implementation of Jiaoyang Li [1] at [https://github.com/Jiaoyang-Li/CBSH2-RTC](https://github.com/Jiaoyang-Li/CBSH2-RTC).

A Python implementation also exists in the branch [python_implementation](https://github.com/CRL-Technion/Cooperative-MAPF/tree/python_implementation).
 
 
Please cite the following paper if you use the code in your published research:  
Jiaoyang Li, Daniel Harabor, Peter J. Stuckey, and Sven Koenig. 
Pairwise Symmetry Reasoning for Multi-Agent Path Finding Search.
CoRR, abs/2103.07116, 2021.

 
 ## Installation
 To get started, first clone the repository
```
git clone https://github.com/CRL-Technion/Cooperative-MAPF.git
```
The code requires the external library BOOST (https://www.boost.org/). After you installed BOOST and downloaded the source code, go into the directory of the source code and compile it with CMake: 
```
cmake .
make
```

To run the code:
```
./cocbs -m random-32-32-20.map -a random-32-32-20-random-1.scen -o test.csv -k 12 -t 60 -s 2
```

- m: the map file from MAPF benchmark folder ('\benchmarks').
- a: the scenario file for the map file
- o: the output file that contains the search statistics
- k: the number of tasks (there will be 2*k agents)
- t: runtime limit (in seconds)

Standard MAPF benchmarks are taken from the [Nathan Sturtevant's Moving AI Lab](https://movingai.com/benchmarks/mapf.html).
You can download more benchmarks from there and put them in the benchmarks folder.

You can also turn on Prioritizing Conflicts (PC) [3] and Lazy Expansion [LE] by using the options:
- prioritizingConflicts
- lazyExpansion

More options, including solving the task assignment problem can be found by running:

```
./cocbs --help
```

## License
The code is released under USC – Research License. See license.md for further details.

## References

[1] Jiaoyang Li, Daniel Harabor, Peter J. Stuckey, and Sven Koenig. 
Pairwise Symmetry Reasoning for Multi-Agent Path Finding Search.
CoRR, abs/2103.07116, 2021.

[2] Guni Sharon, Roni Stern, Ariel Felner, and Nathan R. Sturtevant.
Conflict-Based Search for Optimal Multi-Agent Pathfinding.
Artificial Intelligence, 219:40–66, 2015.

[3] Eli Boyarski, Ariel  Felner, Roni Stern, Guni Sharon, David Tolpin, Oded Betzalel, and Solomon Eyal Shimony.
ICBS: Improved Conflict-Based Search Algorithm for Multi-Agent Pathfinding. 
In Proceedings of the International Joint Conference on Artificial Intelligence (IJCAI), pages 740–746, 2015.