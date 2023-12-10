#include <vector>
#include "json/json.hpp"
#include "network.hpp"

#ifndef TOUR_H
#define TOUR_H

class Tour
{
private:
    std::vector<Node> _nodes;
    double _qacum;
    double _profit;
    double _completion_time;
    double _distance;

public:
    Tour(double departure, std::vector<Node> &nodes, double distance);
    Tour(double departure, std::vector<Node> &nodes, double distance, double qacum, double profit);
    int size();
    double distance() const;
    double departure() const;
    double completion_time() const;
    std::vector<Node> nodes() const;
    double qacum() const;
    double profit() const;

    double _departure; // TODO: fix this
};

std::ostream &operator<<(std::ostream &os, const Tour &tour);
void to_json(nlohmann::json &j, const Tour &);

#endif