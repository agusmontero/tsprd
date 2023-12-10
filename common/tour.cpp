#include "tour.hpp"
#include <algorithm>
#include <map>

Tour::Tour(double departure, std::vector<Node> &nodes, double distance)
    : _departure(departure), _nodes(nodes), _distance(distance)
{
    _qacum = 0;
    _profit = 0;
}

Tour::Tour(double departure, std::vector<Node> &nodes, double distance, double qacum, double profit)
    : Tour(departure, nodes, distance)
{
    _qacum = qacum;
    _profit = profit;
}

int Tour::size()
{
    return _nodes.size();
}

double Tour::distance() const
{
    return _distance;
}

double Tour::departure() const
{
    return _departure;
}

double Tour::completion_time() const
{
    return departure() + distance();
}

std::vector<Node> Tour::nodes() const
{
    return _nodes;
}

double Tour::qacum() const
{
    return _qacum;
}

double Tour::profit() const
{
    return _profit;
}

std::ostream &operator<<(std::ostream &os, const Tour &tour)
{
    os << "Tour(d:" << tour.distance() << ", dep:" << tour.departure() << ", ct:" << tour.completion_time();
    os << ", q: " << tour.qacum() << ", p: " << tour.profit() << ")<" << std::endl;
    for (auto node : tour.nodes())
    {
        os << node << std::endl;
    }
    os << ">";
    return os;
}

void to_json(nlohmann::json &j, const Tour &tour)
{
    j = nlohmann::json{
        {"distance", tour.distance()},
        {"departure", tour.departure()},
        {"completion_time", tour.completion_time()},
        {"qacum", tour.qacum()},
        {"profit", tour.profit()},
        {"tour", tour.nodes()}};
}