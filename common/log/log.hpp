#include "../json/json.hpp"

#ifndef LOG_H
#define LOG_H

class Log
{
private:
  nlohmann::json _log;

public:
  Log();

  template <typename T>
  void write(std::string key, T value);
  nlohmann::json read() const;
};

void to_json(nlohmann::json &j, const Log &);

template <typename T>
void Log::write(std::string key, T value)
{
  _log.push_back(nlohmann::json({{key, value}}));
}

#endif