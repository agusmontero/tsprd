## Traveling Salesman Problem with release dates (TSP-rd)

Please, find here the **code** developed during the following research projects:

- Montero, A., Méndez-Díaz, I., & Miranda-Bront, J. J (2023). [Solving the Traveling Salesman Problem with release dates via branch-and-cut](https://www.sciencedirect.com/science/article/pii/S2192437623000183). EURO Journal on Transportation and Logistics, 100121.

- Montero, A., Méndez-Díaz, I., & Miranda-Bront, J. J (2020). [An Integer Programming approach for the Traveling Salesman Problem with release dates and completion time minimization](https://iccl2021.nl/wp-content/uploads/2020/09/BookOfAbstractsICCL2020-v20200919.pdf#page=93). International Conference on Computational Logistics 2020 (ICCL 2020), Twente, Netherlands (virtual).

---

### Problem instances

The problem instances reported in the aforementioned projects are included in the **data** folder. There is a folder for each of the following TSP-rd variants, both with completion-time and distance minimization:

 - **TSP-rd**: Traveling Salesman Problem with release dates
 - **CTSP-rd**: Capacitated Traveling Salesman Problem with release dates
 - **PC-TSP-rd**: Prize-Collecting Traveling Salesman Problem with release dates

### Code

The following **dependencies** are needed:

- [Boost C++ libraries](https://www.boost.org/doc/libs/1_66_0/libs/graph/doc/index.html)
- [CPLEX 12.9](https://www.ibm.com/products/ilog-cplex-optimization-studio)
- [Hopscotch-map](https://github.com/Tessil/hopscotch-map)
- [Concorde TSP Solver](https://www.math.uwaterloo.ca/tsp/concorde/index.html) (already included)
- [JSON for Modern C++](https://github.com/nlohmann) (already included)

Finally, you need first to execute *make* in the *common/concorde* folder, and only then in the *algoritmhs* folder. Please, review and adjust the corresponding makefiles as needed.

That's all, **have fun**!

PS. Use at your own risk.  We make no guarantees about the correctness or usefulness of this code.
