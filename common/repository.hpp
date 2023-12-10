#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "reader.hpp"
#include <vector>

class Repository
{
private:
  std::string _dir;
  Reader &_reader;
  std::vector<InstanceFile> _instance_files;
  std::vector<std::string> list_files(std::string dir);

public:
  Repository(std::string dir, Reader &reader);
  std::vector<InstanceFile> instance_files();
};

#endif