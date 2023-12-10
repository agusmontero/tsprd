#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <vector>
#include "algorithm.hpp"
#include "../../common/network.hpp"

class Experiment
{
private:
  std::string _name;
  std::vector<Network> _instances;
  std::vector<Algorithm *> _algorithms;

  std::vector<std::vector<Solution>> _solutions;

public:
  Experiment(std::string name, std::vector<Algorithm *> algorithms, std::vector<Network> instances);
  std::string name() const;
  std::vector<Network> instances() const;
  std::vector<Algorithm *> algorithms() const;
  void run();
  void save_results(std::string output_file);
  void save_solutions(std::string output_folder);
};

std::ostream &operator<<(std::ostream &os, const Experiment &experiment);

#endif