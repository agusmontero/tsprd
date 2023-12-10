#include "log.hpp"

Log::Log()
{
    _log = nlohmann::json::array();
}

nlohmann::json Log::read() const
{
    return _log;
}

void to_json(nlohmann::json &j, const Log &log)
{
    j = log.read();
}