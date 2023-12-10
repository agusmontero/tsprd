#include <algorithm>
#include <deque>
#include "time_explorer.hpp"
#include "../../common/stopwatch.hpp"
#include "../../common/log/log.hpp"

#include <tsl/hopscotch_map.h>

std::string TimeExplorerAlgorithm::name()
{
    return _name;
}

Solution TimeExplorerAlgorithm::solve(Network &network)
{
    cache.clear(); // very important if TE is used alone on experiments!

    StopWatch omega;
    omega.start();

    std::vector<double> start_times;
    double min_release = network.node(1).release;
    double max_release = network.node(1).release;
    for (int i = 1; i < network.size(); i++)
    {
        min_release = std::min(min_release, network.node(i).release);
        max_release = std::max(max_release, network.node(i).release);
    }
    for (int r = min_release; r <= max_release; r++)
    {
        start_times.push_back(r);
    }

    std::vector<Solution> solutions;
    for (auto t : start_times)
    {
        Solution solution = _myopic_algorithm.solve(network, t, cache);
        solutions.push_back(solution);
    }

    int best_index = 0;
    double best_objective = solutions[best_index].objective();
    for (int i = 0; i < (int)solutions.size(); i++)
    {
        if (solutions[i].objective() < best_objective)
        {
            best_index = i;
            best_objective = solutions[i].objective();
        }
    }

    double elapsed_time = omega.check();

    Log log;
    log.write<>("has_waiting_time", solutions[best_index].hasWaitingTime());
    log.write<>("solve_time", elapsed_time);

    return Solution(
        solutions[best_index].name(),
        _name,
        solutions[best_index].objective(),
        solutions[best_index].tours(),
        network,
        log);
}