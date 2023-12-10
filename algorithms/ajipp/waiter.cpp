#include <algorithm>
#include "waiter.hpp"
#include "../../common/stopwatch.hpp"
extern "C"
{
#include <concorde.h>
}

#define SCALE_FACTOR 1000

std::string WaiterAlgorithm::name()
{
    return _name;
}

Solution WaiterAlgorithm::solve(Network &network)
{
    Log log;
    StopWatch omega;
    omega.start();

    // concorde structures
    int rval = 0;
    int seed = 42;
    double szeit, optval, *in_val, *timebound;
    int success, foundtour, hit_timebound = 0;
    int *in_tour = (int *)NULL;
    int *out_tour = (int *)NULL;
    CCrandstate rstate;
    char *name = (char *)NULL;
    static int silent = 1;
    CCutil_sprand(seed, &rstate);
    auto ncount = network.size();
    int ecount = (ncount * (ncount - 1)) / 2;
    int *elist = CC_SAFE_MALLOC(ecount * 2, int);
    int *elen = CC_SAFE_MALLOC(ecount, int);
    int edge = 0;
    int edge_weight = 0;
    for (int i = 0; i < ncount; i++)
    {
        for (int j = i + 1; j < ncount; j++)
        {
            elist[edge] = i;
            elist[edge + 1] = j;
            elen[edge_weight] = network.travel_time(network.node(i), network.node(j));
            edge_weight++;
            edge += 2;
        }
    }

    in_tour = CC_SAFE_MALLOC(ncount, int);
    for (int i = 0; i < ncount; i++)
    {
        in_tour[i] = i;
    }

    out_tour = CC_SAFE_MALLOC(ncount, int);
    name = CCtsp_problabel(" ");

    CCdatagroup dat;
    rval = CCutil_graph2dat_matrix(ncount, ecount, elist, elen, 1, &dat);

    rval = CCtsp_solve_dat(ncount, &dat, in_tour, out_tour, NULL, &optval,
                           &success, &foundtour, name, timebound,
                           &hit_timebound, silent, &rstate);

    std::vector<Node> tours;
    for (int i = 0; i < ncount; i++)
    {
        tours.push_back(network.node(out_tour[i]));
    }
    tours.push_back(network.node(out_tour[0]));

    double max_release = network.node(0).release;
    for (int i = 0; i < ncount; i++)
    {
        max_release = std::max(max_release, network.node(i).release);
    }

    std::vector<double> departures(1, max_release);
    double t = max_release + optval;

    double elapsed_time = omega.check();

    // TODO: the solve_time must be at the end of the log from where experiment reads.
    // This needs to be changed.
    log.write<>("solve_time", elapsed_time);

    szeit = CCutil_zeit();
    CC_IFFREE(elist, int);
    CC_IFFREE(elen, int);
    CC_IFFREE(in_tour, int);
    CC_IFFREE(out_tour, int);
    CC_IFFREE(name, char);
    CC_IFFREE(timebound, double);
    CC_IFFREE(in_val, double);
    CCutil_freedatagroup(&dat);

    return Solution(network.name, _name, departures, tours, t, network, log);
}