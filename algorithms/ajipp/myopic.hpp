#include "../../common/network.hpp"
#include "algorithm.hpp"
#include "solution.hpp"
#include "waiter.hpp"

#ifndef MYOPIC_H
#define MYOPIC_H

class MyopicAlgorithm : public Algorithm
{
private:
    const std::string _name = "Myopic algorithm";
    WaiterAlgorithm _waiter;
    double _start_time = 0.0;
    tsl::hopscotch_map<std::size_t, std::vector<Node>> *_cache = NULL;

public:
    std::string name();
    Solution solve(Network &network);
    Solution solve(Network &network, double start_time);
    Solution solve(Network &network,
                   double start_time,
                   tsl::hopscotch_map<std::size_t, std::vector<Node>> &cache);
};

#endif