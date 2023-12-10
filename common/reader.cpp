#include "reader.hpp"

InstanceFile::InstanceFile(std::string path,
                           std::string source,
                           std::vector<std::pair<double, double>> &coordinates,
                           std::vector<double> &releases)
    : _path(path), _source(source), _coordinates(coordinates), _releases(releases)
{
    _size = _coordinates.size();
    _tmax = 99999999.; // so it's not an actual constraint
    _dmax = 99999999.;
    _pmin = 0;
    _cap = 99999999.;
    _profits = std::vector<double>(_size, 0); // TODO: remove this, it was added bc non-profitable instances doesn't have profits
}

InstanceFile::InstanceFile(std::string path,
                           std::string source,
                           std::vector<std::pair<double, double>> &coordinates,
                           std::vector<double> &releases,
                           double tmax)
    : InstanceFile(path, source, coordinates, releases)
{
    _tmax = tmax;
}

InstanceFile::InstanceFile(std::string path,
                           std::string source,
                           std::vector<std::pair<double, double>> &coordinates,
                           std::vector<double> &releases,
                           double tmax,
                           std::vector<double> &demands,
                           double cap)
    : InstanceFile(path, source, coordinates, releases, tmax)
{
    _demands = demands;
    _cap = cap;
}

InstanceFile::InstanceFile(std::string path,
                           std::string source,
                           std::vector<std::pair<double, double>> &coordinates,
                           std::vector<double> &releases,
                           double tmax,
                           std::vector<double> &demands,
                           double cap,
                           std::vector<double> &profits,
                           double dmax,
                           double pmin)
    : InstanceFile(path, source, coordinates, releases, tmax, demands, cap)
{
    _profits = profits;
    _dmax = dmax;
    _pmin = pmin;
}

std::string InstanceFile::name() { return _path; }

std::string InstanceFile::source() { return _source; }

int InstanceFile::size() { return _size; }

std::vector<std::pair<double, double>> const InstanceFile::coordinates() { return _coordinates; }

std::vector<double> const InstanceFile::releases() { return _releases; }

std::vector<double> const InstanceFile::demands() { return _demands; }

std::vector<double> const InstanceFile::profits() { return _profits; }

double InstanceFile::tmax() { return _tmax; }

double InstanceFile::dmax() { return _dmax; }

double InstanceFile::pmin() { return _pmin; }

double InstanceFile::cap() { return _cap; }

InstanceFile TSPLIBReader::read(std::string path)
{
    int size;
    std::vector<std::pair<double, double>> coordinates;
    std::vector<double> releases;

    std::ifstream file(path);
    if (file.is_open())
    {
        getline(file, line);
        ss.str(line);
        ss >> line;
        ss >> size;
        getline(file, line);
        getline(file, line);
        getline(file, line);
        getline(file, line);
        for (int i = 0; i < size; i++)
        {
            getline(file, line);
            ss.str(line);
            std::pair<double, double> coordinate;
            ss >> coordinate.first;
            ss >> coordinate.second;
            coordinates.push_back(coordinate);
            ss >> line >> line >> line >> line;
            double release;
            ss >> release;
            releases.push_back(release);
        }
    }
    else
    {
        std::cerr << "Couldn't read from: '" << path << "'" << std::endl;
        exit(1);
    }
    file.close();

    return InstanceFile(path, "TSPLIB", coordinates, releases);
}

InstanceFile SolomonReader::read(std::string path)
{
    int size, distance;
    std::vector<std::pair<double, double>> coordinates;
    std::vector<double> releases;
    std::vector<double> demands;
    double tmax, cap;

    std::ifstream file(path);
    if (file.is_open())
    {
        getline(file, line);
        ss.str(line);
        ss >> line >> size;
        ss.clear();
        getline(file, line);
        ss.str(line);
        ss >> line >> cap;
        getline(file, line);
        ss.clear();
        getline(file, line); // <TIME_HORIZON> tmax
        ss.str(line);
        ss >> line >> tmax;

        getline(file, line);
        ss.clear();
        for (int i = 0; i < size; i++)
        {
            getline(file, line);
            ss.str(line);
            std::pair<double, double> coordinate;
            ss >> coordinate.first;
            ss >> coordinate.second;
            coordinates.push_back(coordinate);
            double release, q;
            ss >> q >> line >> line >> line >> release;
            demands.push_back(q);
            releases.push_back(release);
        }
    }
    else
    {
        std::cerr << "Couldn't read from: '" << path << "'" << std::endl;
        exit(1);
    }
    file.close();
    ss.clear();

    return InstanceFile(path, "SOLOMON", coordinates, releases, tmax, demands, cap);
}

InstanceFile ProfitableSolomonReader::read(std::string path)
{
    int size, distance;
    std::vector<std::pair<double, double>> coordinates;
    std::vector<double> releases;
    std::vector<double> demands;
    std::vector<double> profits;
    double tmax, cap;
    double dmax = 0;
    double pmin = 0;

    std::ifstream file(path);
    if (file.is_open())
    {
        getline(file, line);
        ss.str(line);
        ss >> line >> size;

        ss.clear();
        getline(file, line);
        ss.str(line);
        ss >> line >> cap;
        getline(file, line);

        ss.clear();
        getline(file, line); // <TIME_HORIZON> tmax
        ss.str(line);
        ss >> line >> tmax;

        ss.clear();
        getline(file, line); // <TRIP_MAX_DISTANCE> dmax
        ss.str(line);
        ss >> line >> dmax;

        ss.clear();
        getline(file, line); // <MIN_PROFIT> pmin
        ss.str(line);
        ss >> line >> pmin;

        getline(file, line);
        for (int i = 0; i < size; i++)
        {
            ss.clear();
            getline(file, line);
            ss.str(line);
            std::pair<double, double> coordinate;
            ss >> coordinate.first;
            ss >> coordinate.second;
            coordinates.push_back(coordinate);
            double release, q, p;
            ss >> q >> line >> line >> line >> release >> p;
            demands.push_back(q);
            releases.push_back(release);
            profits.push_back(p);
        }
    }
    else
    {
        std::cerr << "Couldn't read from: '" << path << "'" << std::endl;
        exit(1);
    }
    file.close();
    ss.clear();

    return InstanceFile(path, "PROFITABLE_SOLOMON", coordinates, releases, tmax, demands, cap, profits, dmax, pmin);
}