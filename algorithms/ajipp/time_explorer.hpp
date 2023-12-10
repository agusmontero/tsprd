#include "../../common/network.hpp"
#include "algorithm.hpp"
#include "solution.hpp"
#include "myopic.hpp"

#ifndef TIME_EXPLORER_H
#define TIME_EXPLORER_H

class TimeExplorerAlgorithm : public Algorithm
{
private:
    const std::string _name = "Time Explorer algorithm";
    MyopicAlgorithm _myopic_algorithm;

public:
    std::string name();
    Solution solve(Network &network);

    tsl::hopscotch_map<std::size_t, std::vector<Node>> cache; // make this private...
};

#endif