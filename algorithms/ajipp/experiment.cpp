#include "experiment.hpp"
#include "../../common/json/json.hpp"
#include <boost/filesystem.hpp>

Experiment::Experiment(std::string name, std::vector<Algorithm *> algorithms, std::vector<Network> instances)
    : _name(name), _instances(instances), _algorithms(algorithms)
{
    std::cout << *this << std::endl;
}

std::string Experiment::name() const
{
    return _name;
}

std::vector<Network> Experiment::instances() const
{
    return _instances;
}

std::vector<Algorithm *> Experiment::algorithms() const
{
    return _algorithms;
}

void Experiment::run()
{
    std::cout << "Experiment started" << std::endl;
    for (auto algorithm : _algorithms)
    {
        std::vector<Solution> algorithm_solutions;
        for (auto &instance : _instances)
        {
            std::cout << "Algorithm = " << algorithm->name() << std::endl;
            std::cout << "Instance = " << instance.name << std::endl;
            Solution solution = algorithm->solve(instance);
            algorithm_solutions.push_back(solution);

            solution.is_feasible();
            std::cout << solution.name() << " Finished";
            std::cout << " after " << solution.log().read().back()["solve_time"] << " secs.";
            std::cout << std::endl
                      << std::endl;
        }
        std::cout << "Algorithm " << algorithm->name() << " finished.";
        std::cout << std::endl
                  << std::endl;
        _solutions.push_back(algorithm_solutions);
    }
    std::cout << "Experiment finished." << std::endl;
}

void Experiment::save_results(std::string output_file)
{
    nlohmann::json solutions = nlohmann::json::array();
    std::ofstream o(output_file);
    for (auto algorithm_solutions : _solutions)
    {
        for (auto solution : algorithm_solutions)
        {
            nlohmann::json solution_json = solution;
            solutions.push_back(solution_json);
        }
    }
    o << solutions << std::endl;
    o.close();
}

void Experiment::save_solutions(std::string output_folder)
{
    for (int i = 0; i < _algorithms.size(); i++)
    {
        std::string subfolder = output_folder + _algorithms[i]->name() + "/";
        if (boost::filesystem::create_directories(subfolder))
        {
            std::cout << "Solutions folder '" << subfolder << "' already exists." << std::endl;
        }
        else
        {
            std::cout << "Solutions folder '" << subfolder << "' created." << std::endl;
        };
        for (auto solution : _solutions[i])
        {
            std::string path = solution.name();
            path.replace(0, 8, ""); // len("../data/") = 8

            auto end_path_index = path.find_last_of('/');

            std::string instance_path = subfolder + path.substr(0, end_path_index + 1);
            std::string instance_name = path.substr(end_path_index + 1, path.length() - end_path_index);
            instance_name.replace(instance_name.find_last_of('.'), 4, ".sol");

            boost::filesystem::create_directories(instance_path);

            std::cout << instance_path << std::endl;
            std::cout << instance_name << std::endl;

            std::ofstream o(instance_path + instance_name);
            to_file(o, solution);
            o.close();
        }
    }
}

std::ostream &operator<<(std::ostream &os, const Experiment &experiment)
{
    os << "Experiment(" << experiment.name() << "){" << std::endl;
    os << "Algorithms(";
    std::vector<Algorithm *> algorithms = experiment.algorithms();
    for (int i = 0; i < (int)algorithms.size(); i++)
    {
        if (i != 0)
            os << ", ";
        os << algorithms[i]->name();
    }
    os << ")" << std::endl;
    os << "Instances(" << experiment.instances().size() << ")" << std::endl;
    for (auto i : experiment.instances())
    {
        os << i.name << std::endl;
    }
    os << "}";
    return os;
}