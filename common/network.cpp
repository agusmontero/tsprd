#include "network.hpp"

Network::Network(InstanceFile &instance_file)
{
    name = instance_file.name();
    source = instance_file.source();
    int n = instance_file.size();

    tmax = instance_file.tmax();
    dmax = instance_file.dmax();
    pmin = instance_file.pmin();
    cap = instance_file.cap();

    std::vector<std::pair<double, double>> coordinates = instance_file.coordinates();
    std::vector<double> releases = instance_file.releases();
    std::vector<double> demands = instance_file.demands();
    std::vector<double> profits = instance_file.profits();
    for (int i = 0; i < n; i++)
    {
        _nodes.push_back(Node(i, coordinates[i].first, coordinates[i].second, releases[i], demands[i], profits[i]));
    }

    // build rounded distance matrix
    for (auto nodei : _nodes)
    {
        std::vector<uint> row(n, 0);
        for (auto nodej : _nodes)
        {
            auto distance = sqrt(pow(nodei.x - nodej.x, 2) + pow(nodei.y - nodej.y, 2));
            row[nodej.id] = round(floor(distance * 10.) / 10.); // one decimal rounding
        }
        _distances.push_back(row);
    }

    // fix matrix with Floyd-Warshall (to ensure triangular inequality)
    for (auto nodei : _nodes)
    {
        for (auto nodej : _nodes)
        {
            for (auto nodek : _nodes)
            {
                auto i = nodei.id;
                auto j = nodej.id;
                auto k = nodek.id;
                _distances[i][j] = std::min(_distances[i][j], _distances[i][k] + _distances[k][j]);
            }
        }
    }
}

Network::Network(std::string name, std::string source, std::vector<Node> &nodes, std::vector<std::vector<uint>> &distances)
    : name(name), source(source), _nodes(nodes), _distances(distances) {}

int Network::size() const { return _nodes.size(); }

const std::vector<Node> &Network::nodes() const
{
    return _nodes;
}

const std::vector<std::vector<uint>> &Network::distances() const
{
    return _distances;
}

const Node &Network::node(int i) const
{
    return _nodes[i];
}

double Network::travel_time(Node nodei, Node nodej)
{
    return _distances[nodei.id][nodej.id];
}

bool Node::operator==(const Node &node) const
{
    return id == node.id;
}

bool Node::operator!=(const Node &node) const
{
    return !(*this == node);
}

bool Node::operator<(const Node &node) const
{
    return id < node.id;
}

bool Node::release_sorting(const Node &node1, const Node &node2)
{
    if (node1.release < node2.release)
        return true;
    else if (abs(node1.release - node2.release) <= 0.0001)
        return node1.id < node2.id;
    else
        return false;
}

std::ostream &operator<<(std::ostream &os, const Node &node)
{
    os << "Node(" << node.id << ", r: " << node.release << ", p: " << node.p << ")";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Network &network)
{
    os << "Network(" << network.name << ")<" << std::endl;
    for (auto node : network.nodes())
        os << node << " ";
    os << ">";
    return os;
}

void to_json(nlohmann::json &j, const Node &node)
{
    j = nlohmann::json{
        {"id", node.id},
        {"x", node.x},
        {"y", node.y},
        {"release", node.release},
        {"q", node.q},
        {"p", node.p}};
}

void to_json(nlohmann::json &j, const Network &network)
{
    j = nlohmann::json{
        {"source", network.source},
        {"n", network.size() - 1},
        {"tmax", network.tmax},
        {"dmax", network.dmax},
        {"pmin", network.pmin},
        {"cap", network.cap},
        {"nodes", network.nodes()}};
}