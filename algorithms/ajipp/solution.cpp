#include "solution.hpp"
#include <algorithm>
#include <map>

Solution::Solution(std::string name,
                   std::string source,
                   std::vector<double> departures,
                   std::vector<Node> &nodes,
                   double objective,
                   Network &network)
    : _name(name), _source(source), _objective(objective), _network(network)
{
    double tour_distance = 0.0;
    Node depot = nodes[0];
    std::vector<Node> tour_nodes(1, depot);

    int tour_index = 0;
    for (auto node_it = nodes.begin() + 1; node_it != nodes.end(); ++node_it)
    {
        if (*node_it == depot)
        {
            tour_distance += network.travel_time(*node_it, tour_nodes.back());
            _tours.push_back(Tour(departures[tour_index], tour_nodes, tour_distance));
            tour_distance = 0.0;
            tour_nodes.clear();
            tour_index++;
        }
        else
        {
            tour_distance += network.travel_time(*node_it, tour_nodes.back());
        }
        tour_nodes.push_back(*node_it);
    }
    _log = Log();

    // fix waiting time
    if (hasWaitingTime())
    {
        for (auto i = 0; i < _tours.size() - 1; i++)
        {
            if (_tours[i].completion_time() + 1e-4 < _tours[i + 1].departure())
            {
                double wt = _tours[i + 1].departure() - _tours[i].completion_time();
                for (auto j = 0; j <= i; j++)
                {
                    _tours[j]._departure += wt;
                }
            }
        }
    }
}

Solution::Solution(std::string name,
                   std::string source,
                   std::vector<double> departures,
                   std::vector<Node> &nodes,
                   double objective,
                   Network &network,
                   Log log)
    : Solution(name, source, departures, nodes, objective, network)
{
    _log = log;
}

Solution::Solution(std::string name,
                   std::string source,
                   double objective,
                   std::vector<Tour> tours,
                   Network &network,
                   Log log)
    : _name(name), _source(source), _objective(objective), _tours(tours), _log(log), _network(network) {}

Solution::Solution(const Solution &solution)
    : _network(solution._network)
{
    _name = solution._name;
    _source = solution._source;
    _objective = solution._objective;
    _tours = solution._tours;
    _log = solution._log;
}

std::string Solution::name() const
{
    return _name;
}

std::string Solution::source() const
{
    return _source;
}

Network &Solution::network() const
{
    return _network;
}

double Solution::objective() const
{
    return _objective;
}

std::vector<Tour> Solution::tours() const
{
    return _tours;
}

bool Solution::is_feasible() const
{
    bool is_feasible = true;
    std::map<Node, int> node_count;

    const Node depot = _network.nodes()[0];
    for (auto node : _network.nodes())
    {
        node_count[node] = 0;
    }
    for (auto tour : _tours)
    {
        for (auto node : tour.nodes())
        {
            if (node.release > tour.departure() + 1e-4)
            {
                std::cerr << "Warning: " << node << " wasn't ready before departure (" << tour.departure() << ")" << std::endl;
                is_feasible = false;
            }
            auto it = node_count.find(node);
            if (it == node_count.end())
            {
                std::cerr << "Warning: " << node << " is invalid." << std::endl;
                is_feasible = false;
            }
            else if (it->second >= 1 and it->first != depot)
            {
                std::cerr << "Warning: " << node << " already visited." << std::endl;
                is_feasible = false;
            }
            else
            {
                it->second++;
            }
        }
    }
    if (node_count[depot] != (int)_tours.size())
    {
        std::cerr << "Warning: " << depot << " visited more or less than " << _tours.size() << " times (number of tours)." << std::endl;
        is_feasible = false;
    }

    for (auto [node, count] : node_count)
    {
        if (count == 0)
        {
            std::cerr << "Warning: " << node << " wasn't visited." << std::endl;
            is_feasible = false;
        }
    }

    double objective = 0.0;
    for (int i = 0; i < (int)_tours.size(); i++)
    {
        double tour_distance = 0.0;
        std::vector<Node> tour_nodes = _tours[i].nodes();
        for (int j = 1; j < (int)tour_nodes.size(); j++)
        {
            tour_distance += _network.travel_time(tour_nodes[j], tour_nodes[j - 1]);
        }
        tour_distance += _network.travel_time(tour_nodes[tour_nodes.size() - 1], tour_nodes[0]);
        if (abs(tour_distance - _tours[i].distance()) > 1e-4)
        {
            std::cerr << "Warning: " << _tours[i] << " distance mismatch with actual tour distance: " << tour_distance << std::endl;
            is_feasible = false;
        }
        if (i < (int)_tours.size() - 1 and _tours[i].completion_time() > 1e-4 + _tours[i + 1].departure())
        {
            std::cerr << "Warning: " << _tours[i + 1] << " departs before " << _tours[i] << " is finished" << std::endl;
            is_feasible = false;
        }
        objective = _tours[i].completion_time();
    }
    if (abs(objective - _objective) > 1e-2)
    {
        std::cerr << "Warning: " << _objective << " mismatch with actual objective: " << objective << std::endl;
        is_feasible = false;
    }

    if (!is_feasible)
        std::cerr << "WARNING: Feasibility issues found." << std::endl;

    return is_feasible;
}

bool Solution::hasWaitingTime() const
{
    for (auto i = 0; i < _tours.size() - 1; i++)
    {
        if (_tours[i].completion_time() + 1e-4 < _tours[i + 1].departure())
        {
            return true;
        }
    }
    return false;
}

Log Solution::log() const
{
    return _log;
}

std::ostream &operator<<(std::ostream &os, const Solution &solution)
{
    os << "Solution(" << solution.objective();
    os << ", wt: " << solution.hasWaitingTime() << ")";
    os << "{" << std::endl;
    for (auto tour : solution.tours())
    {
        os << tour << std::endl;
    }
    os << "}" << std::endl;
    os << solution.log().read() << std::endl;
    solution.is_feasible();
    return os;
}

void to_json(nlohmann::json &j, const Solution &solution)
{
    j = nlohmann::json{
        {"name", solution.name()},
        {"source", solution.source()},
        {"network", solution.network()},
        {"objective", solution.objective()},
        {"tours", solution.tours()},
        {"log", solution.log()}};
}

void to_file(std::ofstream &os, const Solution &solution)
{
    /*
    Nodes
    0 1 2 3
    0 5 7 8
    0 6
    Departures
    10 20 88
    Objective
    150
    */
    os << "Nodes" << std::endl;
    for (auto tour : solution.tours())
    {
        for (auto node : tour.nodes())
        {
            os << node.id << " ";
        }
        os << std::endl;
    }
    os << "Departures" << std::endl;
    for (auto tour : solution.tours())
    {
        os << tour.departure() << " ";
    }
    os << std::endl
       << "Objective" << std::endl
       << solution.objective() << std::endl;
}