#include <vector>
#include "../../common/tour.hpp"
#include "../../common/log/log.hpp"

#ifndef SOLUTION_H
#define SOLUTION_H

class Solution
{
private:
  std::string _name;
  std::string _source;
  double _objective;
  std::vector<Tour> _tours;
  Network &_network;
  Log _log;

public:
  Solution(std::string name,
           std::string source,
           std::vector<double> departures,
           std::vector<Node> &route,
           double objective,
           Network &Network);
  Solution(std::string name,
           std::string source,
           std::vector<double> departures,
           std::vector<Node> &route,
           double objective,
           Network &Network,
           Log log);
  Solution(std::string name,
           std::string source,
           double objective,
           std::vector<Tour> tours,
           Network &network,
           Log log);
  Solution(std::string name,
           std::string source,
           double objective,
           Network &network,
           Log log);
  Solution(const Solution &);

  std::string name() const;
  std::string source() const;
  Network &network() const;
  double objective() const;
  std::vector<Tour> tours() const;
  bool is_feasible() const;
  bool hasWaitingTime() const;
  Log log() const;
};

std::ostream &operator<<(std::ostream &os, const Solution &solution);
void to_json(nlohmann::json &j, const Solution &);
void to_file(std::ofstream &os, const Solution &solution);

#endif