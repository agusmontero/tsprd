#ifndef READER_H
#define READER_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

class InstanceFile
{
private:
  std::string _path;
  std::string _source;
  int _size;
  std::vector<std::pair<double, double>> _coordinates;
  std::vector<double> _releases;
  double _tmax;
  double _cap;
  double _dmax;
  double _pmin;
  std::vector<double> _demands;
  std::vector<double> _profits;

public:
  InstanceFile(std::string path,
               std::string source,
               std::vector<std::pair<double, double>> &coordinates,
               std::vector<double> &releases);
  InstanceFile(std::string path,
               std::string source,
               std::vector<std::pair<double, double>> &coordinates,
               std::vector<double> &releases,
               double tmax);
  InstanceFile(std::string path,
               std::string source,
               std::vector<std::pair<double, double>> &coordinates,
               std::vector<double> &releases,
               double tmax,
               std::vector<double> &demands,
               double cap);
  InstanceFile(std::string path,
               std::string source,
               std::vector<std::pair<double, double>> &coordinates,
               std::vector<double> &releases,
               double tmax,
               std::vector<double> &demands,
               double cap,
               std::vector<double> &profits,
               double dmax,
               double pmin);
  std::string name();
  std::string source();
  int size();
  std::vector<std::pair<double, double>> const coordinates();
  std::vector<double> const releases();
  std::vector<double> const demands();
  std::vector<double> const profits();
  double tmax();
  double dmax();
  double pmin();
  double cap();
};

class Reader
{
protected:
  std::string line;
  std::stringstream ss;

public:
  virtual InstanceFile read(std::string) = 0;
};

class TSPLIBReader : public Reader
{
public:
  InstanceFile read(std::string);
};

class SolomonReader : public Reader
{
public:
  InstanceFile read(std::string);
};

class ProfitableSolomonReader : public Reader
{
public:
  InstanceFile read(std::string);
};

#endif