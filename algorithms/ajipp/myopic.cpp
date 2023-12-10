#include <algorithm>
#include "myopic.hpp"
#include <deque>
#include <boost/functional/hash.hpp>
#include "../../common/stopwatch.hpp"
#include "../../common/log/log.hpp"

std::string MyopicAlgorithm::name()
{
    return _name;
}

Solution MyopicAlgorithm::solve(Network &network,
                                double start_time,
                                tsl::hopscotch_map<std::size_t, std::vector<Node>> &cache)
{
    _start_time = start_time;
    _cache = &cache;
    return solve(network);
}

Solution MyopicAlgorithm::solve(Network &network, double start_time)
{
    _start_time = start_time;
    return solve(network);
}

Solution MyopicAlgorithm::solve(Network &network)
{
    StopWatch omega;
    omega.start();

    std::vector<Node> nodes = network.nodes();

    Node depot = nodes[0];
    std::deque<Node> request(nodes.begin() + 1, nodes.end());
    std::sort(request.begin(), request.end(), Node::release_sorting);

    std::vector<Node> tours(1, depot);
    std::vector<double> departures;

    double t = _start_time;
    while (!request.empty())
    {
        Node node = request.front();
        std::vector<Node> tour(1, depot);
        t = std::max(t, node.release);
        departures.push_back(t);

        // build next tour
        while (!request.empty() and node.release <= t)
        {
            tour.push_back(node);
            request.pop_front();
            node = request.front();
        }

        std::size_t hash = boost::hash_range(tour.begin(), tour.end());
        if (_cache == NULL or _cache->find(hash) == _cache->end())
        {
            // visit current tour
            if (tour.size() <= 4)
            {
                std::vector<Node> options(tour);

                double best_objective = -1.0;
                do
                {
                    double objective = 0.0;
                    for (int i = 0; i < (int)options.size() - 1; i++)
                    {
                        objective += network.travel_time(options[i], options[i + 1]);
                    }
                    objective += network.travel_time(options[options.size() - 1], options[0]);

                    if (best_objective < 0.0 or objective < best_objective)
                    {
                        best_objective = objective;
                        tour = options;
                    }

                } while (std::next_permutation(options.begin() + 1, options.end()));
            }
            else
            {
                // TODO: remove this unnecessary copy
                std::vector<std::vector<uint>> tour_distances(network.distances());
                Network tour_network("tour_network", _name, tour, tour_distances);
                Solution tour_solution = _waiter.solve(tour_network);
                tour = tour_solution.tours()[0].nodes();
            }

            if (_cache != NULL)
            {
                (*_cache)[hash] = tour;
            }
        }
        else
        {
            // cache != NULL and tour in cache
            tour = _cache->at(hash);
        }

        for (auto tour_node : tour)
        {
            if (tour_node == depot)
                continue;
            t += network.travel_time(tours.back(), tour_node);
            tours.push_back(tour_node);
        }
        t += network.travel_time(tours.back(), depot);
        tours.push_back(depot);
    }

    double elapsed_time = omega.check();

    Log log;
    log.write<>("solve_time", elapsed_time);

    return Solution(network.name, _name, departures, tours, t, network, log);
}