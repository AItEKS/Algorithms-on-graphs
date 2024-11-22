#pragma once

#include <iostream>
#include <limits>
#include <vector>
#include <tuple>
#include <set>
#include <cmath>

struct Node {
	int x, y;
	double g;
	Node* parent;

	Node(int x, int y) : x(x), y(y), g(std::numeric_limits<double>::infinity()), parent(nullptr) {}

	bool operator < (const Node& other) const {
		return std::tie(g, x, y) < std::tie(other.g, other.x, other.y);
	}
};

class LIAN {
private:
	std::vector<std::vector<int>> map;
	std::pair<int, int> start, goal;
	int delta;
	std::vector<std::pair<int, int>> findedPath;

	bool isValid(int x, int y, const std::vector<std::vector<int>>& map);
	std::vector<std::pair<int, int>> getPath(Node* node);
	std::set<std::pair<int, int>> midPointCircle(int cx, int cy, int delta);
	double EuclidianDistance(int x1, int y1, int x2, int y2);
	std::set<std::pair<int, int>> generateSuccessors(Node* a, int delta, double alpha, const std::set<Node*>& CLOSED,
		const std::pair<int, int>& goal, const std::vector<std::vector<int>>& map);
	
public:
	LIAN(std::vector<std::vector<int>> map, std::pair<int, int> start, std::pair<int, int> goal, int delta) : map(map), start(start), goal(goal), delta(delta) {}
	std::vector<std::pair<int, int>> findPath();
	void printSolution();
};