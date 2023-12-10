#ifndef SEPARATION_GCS_HPP
#define SEPARATION_GCS_HPP

#include <vector>
#include <functional>

namespace separation
{
	// Separation routine for the GCS.
	// GCS: Given k \in S, S \subset V \ {o, d} (o=origin depot, d=destination depot).
	//		\sum_{ij \in \delta^+(S)} x_ij >= \sum_{ij \in \delta^+(k) x_ij}.
	// Parameters:
	// D: digraph represented as list of successors (D[i] = {j \in \delta^+(i)}).
	// x: function that gives the fractional value in the current solution of arc ij (x(i, j)).
	// end_depot: vertex represented the end_depot (typically n+1).
	// k: vertex k to test if any GCS with such k is violated. (k must not be a depot).
	// S: output parameter that will contain the set S that violates a GCS if some is found.
	// tolerance: a constraint will be considered violated if lhs - rhs < -tolerance.
	// Returns: true if a violated inequality was found (and return such S in the output parameter), false otherwise.
	// Precondition: The digraph D is assumed to have two depots. If the origin_depot == end_depot then there is no
	// 				 warranty of the result.
	bool separate_gcs(const std::vector<std::vector<int>> &D, const std::function<double(int i, int j)> &x,
					  int end_depot, int k, std::vector<int> *S, double tolerance = 0.01);

} // namespace separation.

#endif // SEPARATION_GCS_HPP