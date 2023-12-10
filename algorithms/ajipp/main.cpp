#include <iostream>
#include <vector>
#include "../../common/reader.hpp"
#include "../../common/repository.hpp"
#include "../../common/network.hpp"
#include "experiment.hpp"
#include "myopic.hpp"
#include "time_explorer.hpp"
#include "mip.hpp"

int main(int argc, char *argv[])
{
    std::cout << "Hey!" << std::endl;

    SolomonReader reader;
    Repository repository("../../data/tsprd-time/test/", reader);

    std::vector<Network> instances;
    for (auto instance_file : repository.instance_files())
        instances.push_back(Network(instance_file));

    AJIAlgorithm milp;
    AJIAlgorithm root(RootOnly);

    Experiment experiment(
        "AJI++",
        {&milp,
         &root},
        instances);

    experiment.run();
    experiment.save_results("../../data/tsprd-time/test/results.json");

    return 0;
}