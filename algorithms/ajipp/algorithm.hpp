#include <tsl/hopscotch_map.h>
#include "solution.hpp"
#include "../../common/network.hpp"

#ifndef ALGORITHM_H
#define ALGORITHM_H

class Algorithm
{
public:
  virtual std::string name() = 0;
  virtual Solution solve(Network &network) = 0;
};

#endif