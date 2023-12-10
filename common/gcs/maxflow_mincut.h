#ifndef MAXFLOW_MINCUT_HPP
#define MAXFLOW_MINCUT_HPP

#include <functional>
#include <vector>

namespace separation
{
	// Represents an S-T cut in a network. S are the vertices in the side of the source,
	// T the vertices in the side of the sink.
	struct STCut
	{
		std::vector<int> S, T;
	};

	// Solves a maxflow problem on the network D with capacities c with source s and sink t.
	// Parameters:
	// D: digraph represented as list of successors (D[i] = {j \in \delta^+(i)}).
	// x: function that gives the capacities c(i,j) of each arc ij.
	// s: source vertex.
	// t: sink vertex.
	// Returns (max_flow, min_cut) of the network.
	std::pair<double, STCut> maxflow_mincut(const std::vector<std::vector<int>> &D, const std::function<double(int i, int j)> &c, int s, int t);
} // namespace separation.

#endif // MAXFLOW_MINCUT_HPP