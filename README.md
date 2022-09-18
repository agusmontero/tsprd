# Traveling Salesman Problem with release dates (TSPrd)

This repository accompanies the following research projects:

- A. Montero, I. Méndez-Díaz, J. J. Miranda-Bront (2021). [Solving the Traveling Salesman Problem with release dates via branch-and-cut](http://www.optimization-online.org/DB_FILE/2021/04/8350.pdf). Technical Report.

- A. Montero, I. Méndez-Díaz, J. J. Miranda-Bront (2020). [An Integer Programming approach for the Traveling Salesman Problem with release dates and completion time minimization](https://iccl2021.nl/wp-content/uploads/2020/09/BookOfAbstractsICCL2020-v20200919.pdf#page=93). International Conference on Computational Logistics 2020 (ICCL 2020), Twente, Netherlands (virtual).

The `data/` directory contains the instances used for experimentation during the aforementioned projects. Each folder contains problem instances of the following TSPrd variants:

 - `tsprd-time`: TSP with release dates and completion-time minimization
 - `tsprd-distance`: TSP with release dates and distance minimization

 - `ctsprd-time`: Capacitated TSP with release dates and completion-time minimization
 - `ctsprd-distance`: Capacitated TSP with release dates and distance minimization

 - `pc-tsprd-time`: Prize-Collecting TSP with release dates and completion-time minimization
 - `pc-ctsprd-distance`: Prize-Collecting Capacitated TSP with release dates and distance minimization

The source code of the algorithms will be added soon 🔜