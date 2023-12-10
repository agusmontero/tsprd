#include "gcs.h"

#include <tuple>
#include <math.h>

#include "maxflow_mincut.h"

using namespace std;

namespace separation
{
	bool separate_gcs(
		const vector<vector<int>> &D,
		const function<double(int i, int j)> &x,
		int end_depot,
		int k,
		vector<int> *S,
		double tolerance)
	{
		// Get y_k flow passing through vertex k.
		double y_k = 0.0;
		for (int j : D[k])
			y_k += x(k, j);

		// If y_k == 0 then there will be no violated inequality.
		if (fabs(y_k) < tolerance)
			return false;

		// Solve max_flow in network D with capacities x, with source=k, sink=end_depot.
		double max_flow;
		STCut min_cut;
		tie(max_flow, min_cut) = maxflow_mincut(D, x, k, end_depot);

		// If no inequality is violated more than the tolerance, then return false.
		if (max_flow >= y_k - tolerance)
			return false;

		// Here we know there is a violated inequality.
		*S = min_cut.S;
		return true;
	}

} // namespace separation.