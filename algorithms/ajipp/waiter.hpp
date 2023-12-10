#include "../../common/network.hpp"
#include "algorithm.hpp"
#include "solution.hpp"

#ifndef WAITER_H
#define WAITER_H

class WaiterAlgorithm : public Algorithm
{
private:
    const std::string _name = "Waiter algorithm";

public:
    std::string name();
    Solution solve(Network &network);
};

#endif