#include <ilcplex/ilocplex.h>
#include <ilcplex/cplex.h>
ILOSTLBEGIN

#include "mip.hpp"
#include "../../common/stopwatch.hpp"
#include "time_explorer.hpp"
#include <algorithm>
#include <deque>
#include "../../common/gcs/gcs.h"

// TODO: move this callback to a better place
ILOUSERCUTCALLBACK6(GeneralizedCutSetCallbackClassic,
                    int, n,
                    int, Kn,
                    vector<vector<int>> &, D,
                    IloArray<IloArray<IloBoolVarArray>> &, xijk,
                    IloArray<IloBoolVarArray> &, yik,
                    Log &, log)
{
    if (isAfterCutLoop())
        return;

    IloEnv env = getEnv();
    auto nid = getNodeId()._id;

    if (nid != 0)
    {
        return;
    }

    double val;
    for (auto k = 0; k < Kn; k++)
    {
        vector<vector<double>> x(n, vector<double>(n, 0));
        for (auto i = 0; i < n; i++)
        {
            for (auto j = 0; j < n; j++)
            {
                if (i == j)
                    continue;

                val = getValue(xijk[i][j][k]);

                if (val > 1e-4) // tolerance
                {
                    x[i][j] = val;
                }
            }
        }

        for (auto l = 1; l < n; l++)
        {
            vector<int> S;

            // execution the separation routine
            bool found_gcs = separation::separate_gcs(
                D, [&](int xi, int xj)
                { return x[xi][xj % n]; },
                n, l, &S, 0.01);

            if (found_gcs)
            {
                vector<int> NminusS(n, true);
                for (auto i : S)
                    NminusS[i] = false;

                IloNumExpr constraint(env);
                for (auto i : S)
                {
                    for (auto j = 0; j < n; j++)
                    {
                        if (not NminusS[j])
                            continue;

                        constraint += xijk[i][j][k];
                    }
                }
                add(constraint - yik[l][k] >= 0, IloCplex::UseCutForce);
                constraint.end();
            }
        }
    }
}

// TODO: move this callback to a better place
ILOLAZYCONSTRAINTCALLBACK6(GeneralizedLazyCutSetCallbackClassic,
                           int, n,
                           int, Kn,
                           vector<vector<int>> &, D,
                           IloArray<IloArray<IloBoolVarArray>> &, xijk,
                           IloArray<IloBoolVarArray> &, yik,
                           Log &, log)
{
    IloEnv env = getEnv();

    double val;
    for (auto k = 0; k < Kn; k++)
    {
        vector<vector<double>> x(n, vector<double>(n, 0));
        for (auto i = 0; i < n; i++)
        {
            for (auto j = 0; j < n; j++)
            {
                if (i == j)
                    continue;
                val = getValue(xijk[i][j][k]);
                if (val > 1e-4) // tolerance
                {
                    x[i][j] = val;
                }
            }
        }

        for (auto l = 1; l < n; l++)
        {
            vector<int> S;

            // execution the separation routine
            bool found_gcs = separation::separate_gcs(
                D, [&](int i, int j)
                { return x[i][j % n]; },
                n, l, &S, 0.01);

            if (found_gcs)
            {
                vector<int> NminusS(n, true);
                for (auto i : S)
                    NminusS[i] = false;

                IloNumExpr constraint(env);
                for (auto i : S)
                {
                    for (auto j = 0; j < n; j++)
                    {
                        if (not NminusS[j])
                            continue;

                        constraint += xijk[i][j][k];
                    }
                }

                add(constraint - yik[l][k] >= 0, IloCplex::UseCutForce);
                constraint.end();
            }
        }
    }
}

std::string AJIAlgorithm::name()
{
    return _name;
}

Solution AJIAlgorithm::solve(Network &network)
{
    Log log;
    StopWatch omega;
    omega.start();

    std::vector<Node> tours;
    std::vector<double> departures;
    double t, elapsed_time;
    double t_relaxation;
    int n_nodes, ncuts_cplex = 0;

    TimeExplorerAlgorithm timeExplorerAlgorithm;
    std::vector<Solution> mipstart_solutions;

    Solution solution = timeExplorerAlgorithm.solve(network);
    std::cout << timeExplorerAlgorithm.name() << " = " << solution.objective() << std::endl;

    double heuristics_elapsed_time = omega.check();

    mipstart_solutions.push_back(solution);

    auto upperBound = 999999999.;
    if (not mipstart_solutions.empty())
    {
        upperBound = std::min_element(
                         mipstart_solutions.begin(),
                         mipstart_solutions.end(),
                         [](auto &s1, auto &s2)
                         { return s1.objective() < s2.objective(); })
                         ->objective();
    }

    double rmax = network.node(0).release;
    for (auto node : network.nodes())
        rmax = max(rmax, node.release);

    std::cout << "Upper-Bound = " << upperBound << std::endl;

    IloInt i, j, k;
    IloEnv env;
    bool solutionFound = false;
    try
    {
        IloModel model(env);
        IloCplex cplex(env);

        auto n = network.size();
        int Kn = n - 1;

        IloArray<IloArray<IloBoolVarArray>> xijk(env);
        IloArray<IloBoolVarArray> yik(env);
        IloNumVarArray tk(env, Kn + 1, 0, upperBound);

        for (auto k = 0; k < Kn; k++)
        {
            stringstream ss;
            ss << "t_" << k;
            tk[k].setName(ss.str().c_str());
        }

        for (i = 0; i < n; i++)
        {
            IloArray<IloBoolVarArray> xij(env);
            for (j = 0; j < n; j++)
            {
                xij.add(IloBoolVarArray(env, Kn));
                for (auto k = 0; k < Kn; k++)
                {
                    stringstream ss;
                    ss << "x_{" << i << "," << j << "," << k << "}";
                    xij[j][k].setName(ss.str().c_str());
                }
            }
            xijk.add(xij);

            yik.add(IloBoolVarArray(env, Kn));
            for (auto k = 0; k < Kn; k++)
            {
                stringstream ss;
                ss << "y_{" << i << "," << k << "}";
                yik[i][k].setName(ss.str().c_str());
            }
        }
        std::cout << "Variables created." << std::endl;

        // objective function
        model.add(IloMinimize(env, tk[Kn]));

        // constraints
        for (i = 1; i < n; i++)
        {
            model.add(IloSum(yik[i]) == 1);
        }

        for (i = 0; i < n; i++)
        {
            for (k = 0; k < Kn; k++)
            {
                IloIntExpr s1(env), s2(env);
                for (j = 0; j < n; j++)
                {
                    if (i == j)
                        continue;
                    s1 += xijk[i][j][k];
                    s2 += xijk[j][i][k];
                }
                model.add(s1 == yik[i][k]);
                model.add(s2 == yik[i][k]);
            }
        }

        double tij;
        for (k = 0; k < Kn; k++)
        {
            IloNumExpr s(env);
            for (i = 0; i < n; i++)
            {
                for (j = 0; j < n; j++)
                {
                    if (i == j)
                        continue;
                    tij = network.travel_time(network.node(i), network.node(j));
                    s += tij * xijk[i][j][k];
                }
            }
            model.add(tk[k + 1] == tk[k] + s);
            // model.add(tk[k + 1] >= tk[k] + s);
        }

        for (k = 0; k < Kn; k++)
        {
            for (i = 1; i < n; i++)
            {
                // model.add(tk[k] >= network.node(i).release * yik[i][k]);
                IloNumExpr s(env);
                for (auto h = 0; h <= k; h++)
                {
                    s += yik[i][h];
                }
                model.add(tk[k] >= network.node(i).release * s);
            }
        }

        for (k = 0; k < Kn - 1; k++)
        {
            model.add(tk[k] <= rmax);
        }

        for (k = 0; k < Kn - 1; k++)
        {
            model.add(yik[0][k] - yik[0][k + 1] <= 0);
        }

        for (i = 1; i < n; i++)
        {
            for (k = 0; k < Kn; k++)
            {
                IloNumExpr s(env);
                for (int h = 0; h <= k; h++)
                {
                    s += yik[i][h];
                }
                model.add(s - yik[0][k] <= 0);
            }
        }

        std::cout << "Extracting model.." << std::endl;
        cplex.extract(model);

        // MIP start
        // TODO: extract this to a better place
        std::cout << "Building MIP start.." << std::endl;

        for (auto solution : mipstart_solutions)
        {
            IloNumVarArray startVar(env);
            IloNumArray startVal(env);

            // build tours with ids
            std::vector<std::vector<int>> tours;
            for (auto k = 0; k < solution.tours().size(); k++)
            {
                std::vector<int> ids;
                for (auto &node : solution.tours()[k].nodes())
                {
                    ids.push_back(node.id);
                }
                tours.push_back(ids);
            }

            auto offset = Kn - tours.size();

            // set yik
            for (int k = 0; k < Kn; k++)
            {
                for (auto i = 0; i < n; i++)
                {
                    startVar.add(yik[i][(offset + k) % Kn]);
                    startVal.add(k < tours.size() and
                                 find(tours[k].begin(), tours[k].end(), i) != tours[k].end());
                }
            }

            // set xijk
            // we just say which variables are enabled
            for (auto k = 0; k < tours.size(); k++) // intra tours
            {
                for (auto index = 0; index < tours[k].size() - 1; index++)
                {
                    auto i = tours[k][index];
                    auto j = tours[k][index + 1];
                    startVar.add(xijk[i][j][(offset + k) % Kn]);
                    startVal.add(true);
                }
            }

            for (auto k = 0; k < tours.size() - 1; k++) // inter tours
            {
                auto i = tours[k].back();
                auto j = tours[k + 1].front();
                startVar.add(xijk[i][j][(offset + k) % Kn]);
                startVal.add(true);
            }

            auto i = tours.back().back(); // last arc to a duplicated depot
            auto j = 0;
            startVar.add(xijk[i][j][Kn - 1]);
            startVal.add(true);

            for (auto k = 0; k < solution.tours().size(); k++)
            {
                auto tour = solution.tours()[k];

                startVar.add(tk[(offset + k) % Kn]);
                startVal.add(tour.departure());
            }

            auto tour = solution.tours().back();
            for (auto k = solution.tours().size(); k < Kn; k++)
            {
                startVar.add(tk[(offset + k) % Kn]);
                startVal.add(tour.completion_time());
            }

            cplex.addMIPStart(startVar, startVal, IloCplex::MIPStartSolveMIP);
            startVal.end();
            startVar.end();
        }

        IloNumVarArray ordvar(env);
        IloNumArray ordpri(env);
        for (auto k = 0; k < Kn; k++)
        {
            ordvar.add(yik[0][k]);
            ordpri.add(1000);
        }
        for (auto i = 1; i < n; i++)
        {
            for (auto k = 0; k < Kn; k++)
            {
                ordvar.add(yik[i][k]);
                ordpri.add(100);
            }
        }
        for (auto i = 0; i < n; i++)
        {
            for (auto j = 0; j < n; j++)
            {
                if (i == j)
                    continue;
                for (auto k = 0; k < Kn; k++)
                {
                    ordvar.add(xijk[i][j][k]);
                    ordpri.add(10);
                }
            }
        }
        cplex.setPriorities(ordvar, ordpri);
        for (auto i = 0; i < ordvar.getSize(); i++)
        {
            cplex.setDirection(ordvar[i], IloCplex::BranchUp);
        }

        // D[i] := {j \in \delta^+(i)}
        vector<vector<int>> D(n + 1);
        for (auto i = 0; i < n; i++)
        {
            for (auto j = 1; j < n + 1; j++)
            {
                if (i == j)
                    continue;
                D[i].push_back(j);
            }
        }

        // classic "old" callbacks
        // hey! remember that dynamic search will be turned off if you use callbacks the old way
        cplex.use(GeneralizedCutSetCallbackClassic(env, n, Kn, D, xijk, yik, log));
        cplex.use(GeneralizedLazyCutSetCallbackClassic(env, n, Kn, D, xijk, yik, log));

        cplex.setParam(IloCplex::Param::MIP::Strategy::Search, IloCplex::Traditional);
        if (this->_root_only)
        {
            cplex.setParam(IloCplex::NodeLim, 0);
        }
        cplex.setParam(IloCplex::Param::MIP::Limits::CutPasses, 100);

        int numCPU = sysconf(_SC_NPROCESSORS_ONLN);
        cplex.setParam(IloCplex::Threads, numCPU);
        // cplex.setParam(IloCplex::Threads, 1);

        cplex.setParam(IloCplex::Param::TimeLimit, 3600.0);
        cplex.setParam(IloCplex::Param::RandomSeed, 21);

        solutionFound = cplex.solve();

        std::vector<IloCplex::CutType> cut_names{
            IloCplex::CutType::CutBenders,
            IloCplex::CutType::CutBQP,
            IloCplex::CutType::CutClique,
            IloCplex::CutType::CutCover,
            IloCplex::CutType::CutDisj,
            IloCplex::CutType::CutFlowCover,
            IloCplex::CutType::CutFlowPath,
            IloCplex::CutType::CutFrac,
            IloCplex::CutType::CutGubCover,
            IloCplex::CutType::CutImplBd,
            IloCplex::CutType::CutLiftProj,
            IloCplex::CutType::CutLocalCover,
            IloCplex::CutType::CutLocalImplBd,
            IloCplex::CutType::CutMCF,
            IloCplex::CutType::CutMir,
            IloCplex::CutType::CutObjDisj,
            IloCplex::CutType::CutRLT,
            IloCplex::CutType::CutSolnPool,
            IloCplex::CutType::CutTable,
            IloCplex::CutType::CutTighten,
            IloCplex::CutType::CutZeroHalf};
        for (auto cut_name : cut_names)
        {
            ncuts_cplex += cplex.getNcuts(cut_name);
        }

        std::cout << "ncuts_cplex = " << ncuts_cplex << std::endl;

        std::cout << " --------------------------------------------------" << std::endl;
        std::cout << "Solution status: " << cplex.getStatus() << std::endl;
        std::cout << std::endl
                  << "Solution found:" << std::endl;
        std::cout << " Best Obj. value = "
                  << cplex.getBestObjValue() << std::endl
                  << std::endl;

        elapsed_time = omega.check();
        t_relaxation = cplex.getBestObjValue();

        if (solutionFound)
        {
            std::cout << " Objective value = "
                      << cplex.getObjValue() << std::endl
                      << std::endl;

            t = cplex.getObjValue();
            n_nodes = cplex.getNnodes();

            double val, t;
            for (k = 0; k < Kn; k++)
            {
                val = cplex.getValue(yik[0][k]);
                if (abs(val) <= 1e-4)
                    continue;

                std::map<int, int> edges;
                for (i = 0; i < n; i++)
                {
                    for (j = 0; j < n; j++)
                    {
                        if (i == j)
                            continue;
                        val = cplex.getValue(xijk[i][j][k]);
                        if (abs(val - 1) <= 1e-4)
                        {
                            edges[i] = j;
                        }
                    }
                }

                t = cplex.getValue(tk[k]);
                departures.push_back(t);

                i = 0;
                do
                {
                    j = edges[i];
                    tours.push_back(network.node(i));
                    i = j;
                } while (j != 0);
            }
            tours.push_back(network.node(0));
        }
        else
        {
            std::cout << " No solution found " << std::endl;
        }
        model.end();
        cplex.end();
    }
    catch (IloException &e)
    {
        cerr << "Concert exception caught: " << e << std::endl;
    }
    env.end();

    log.write<>("heuristics_solve_time", heuristics_elapsed_time);
    log.write<>("mip_solve_time", elapsed_time - heuristics_elapsed_time);
    log.write<>("best_obj_val", t_relaxation);
    log.write<>("n_nodes", n_nodes);
    log.write<>("n_cuts_cplex", ncuts_cplex);

    // TODO: the solve_time must be at the end of the log from where experiment reads.
    // This needs to be changed.
    log.write<>("solve_time", elapsed_time);

    if (not solutionFound) // return a dummy solution...
    {
        std::cout << "Warning :: No solution found. A dummy solution will be returned." << std::endl;
        std::vector<double> dummy_departures(1, 0);
        std::vector<Node> dummy_tours(1, network.node(0));
        dummy_tours.push_back(network.node(1));
        dummy_tours.push_back(network.node(0));

        return Solution(network.name, _name, dummy_departures, dummy_tours, -1, network, log);
    }

    return Solution(network.name, _name, departures, tours, t, network, log);
}
