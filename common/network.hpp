#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <math.h>
#include <boost/functional/hash_fwd.hpp>
#include "reader.hpp"
#include "./json/json.hpp"

#ifndef NETWORK_H
#define NETWORK_H

class Node
{
public:
  int id;
  double x;
  double y;
  double release;
  double q;
  double p;
  Node(int id, double x, double y, double release, double q, double p) : id(id), x(x), y(y), release(release), q(q), p(p) {}

  bool operator==(const Node &node) const;
  bool operator!=(const Node &node) const;
  bool operator<(const Node &node) const;
  static bool release_sorting(const Node &node1, const Node &node2);
  friend std::size_t hash_value(Node const &node)
  {
    size_t seed = 0;
    boost::hash_combine(seed, node.id);
    return seed;
  }
};

std::ostream &operator<<(std::ostream &os, const Node &node);
void to_json(nlohmann::json &j, const Node &);

class Network
{
private:
  std::vector<Node> _nodes;
  std::vector<std::vector<uint>> _distances;

public:
  std::string name;
  std::string source;
  Network(InstanceFile &instance_file);
  Network(std::string name, std::string source, std::vector<Node> &nodes, std::vector<std::vector<uint>> &distances);
  int size() const;
  const std::vector<Node> &nodes() const;
  const std::vector<std::vector<uint>> &distances() const;
  const Node &node(int i) const;
  double travel_time(Node nodei, Node nodej);
  double tmax;
  double dmax;
  double pmin;
  double cap;
};

std::ostream &operator<<(std::ostream &os, const Network &network);
void to_json(nlohmann::json &j, const Network &network);

#endif