#include <iostream>

#include "gcs.h"

using namespace std;

int main()
{
	// GCS: Given k \in S, S \subset V \ {o, d} (o=origin depot, d=destination depot).
	//		\sum_{ij \in \delta^+(S)} x_ij >= \sum_{ij \in \delta^+(k) x_ij}.
	//
	// Testing the GCS separation routine.
	// Digraph D has 5 vertices {0,1,2,3,4}. (0, 4 are depots).
	// x_01 = x_14 = 0.5 (a path 014 with flow 0.5).
	// x_12 = x_21 = 0.5 (a subtour 121 with flow 0.5).
	// x_03 = x_34 = 0.5 (a path 034 with flow 0.5).
	// All other x_ij = 0.
	// x is feasible in terms of flow conservation but has a fractional subtour in (1,2,1).
	// The only violated GCS is S = {1, 2}, k = 1. Since
	//		i) \sum_{ij \in \delta^+({1,2})} x_ij == 0.5, but
	//		ii) \sum_{ij \in \delta^+(1)} x_ij == 1.0.

	int n = 5;				  // number of vertices.
	vector<vector<int>> D(n); // digraph represented as list of successors. (D[i] = {j \in \delta^+(i)}).
	D[0] = {1, 2, 3};
	D[1] = {2, 3, 4};
	D[2] = {1, 3, 4};
	D[3] = {1, 2, 4};
	vector<vector<double>> x(n, vector<double>(n, 0.0)); // value of the arc variables in the given fractional solution.
	x[0][1] = x[1][4] = 0.5;
	x[1][2] = x[2][1] = 0.5;
	x[0][3] = x[3][4] = 0.5;

	int o = 0; // origin depot.
	int d = 4; // destination depot.

	// Try GCS with k \in {1,2,3}.
	for (int k : {1, 2, 3})
	{
		vector<int> S; // subset S that will contain the violated inequality.
		// Execution the separation routine.
		bool found_gcs = separation::separate_gcs(
			D, [&](int i, int j)
			{ return x[i][j]; },
			4, k, &S, 0.01);
		if (found_gcs)
		{
			cout << "Found GCS with k = " << k << " and S = { ";
			for (int i : S)
				cout << i << " ";
			cout << "}" << endl;
		}
		else
		{
			cout << "No GCS found with k = " << k << "." << endl;
		}
	}
	return 0;
}