#include "../../common/network.hpp"
#include "solution.hpp"
#include "myopic.hpp"

#include <tsl/hopscotch_map.h>

#ifndef MIP_H
#define MIP_H

const bool RootOnly = true;

class AJIAlgorithm : public Algorithm
{
private:
    std::string _name = "AJI++";
    bool _root_only; // the same pattern can be extended to use, eg, "_with_warn_start" or "_custom_branching"

public:
    AJIAlgorithm(bool root_only = false) : _root_only(root_only)
    {
        if (root_only)
        {
            _name = "AJI++-Root";
        }
    }
    std::string name();
    Solution solve(Network &network);
};

#endif