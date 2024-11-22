#include "LIAN.h"

bool LIAN::isValid(int x, int y, const std::vector<std::vector<int>>& map) {
	return (x >= 0 && x < map.size() && y >= 0 && y < map[0].size() && map[x][y] == 0);
}

std::vector<std::pair<int, int>> LIAN::getPath(Node* node) {
	std::vector<std::pair<int, int>> path;
	while (node) {
		path.push_back({ node->x, node->y });
		node = node->parent;
	}
	std::reverse(path.begin(), path.end());
	return path;
}

std::set<std::pair<int, int>> LIAN::midPointCircle(int cx, int cy, int delta) {
	std::set<std::pair<int, int>> points;
	int x = 0;
	int y = delta;
	int d = 1 - delta;

	while (x <= y) {
		points.insert({ cx + x, cy + y });
		points.insert({ cx - x, cy + y });
		points.insert({ cx + x, cy - y });
		points.insert({ cx - x, cy - y });
		points.insert({ cx + y, cy + x });
		points.insert({ cx - y, cy + x });
		points.insert({ cx + y, cy - x });
		points.insert({ cx - y, cy - x });

		if (d < 0) {
			d += 2 * x + 3;
		}

		else {
			d += 2 * (x - y) + 5;
			--y;
		}
		++x;
	}
	return points;
}

double LIAN::EuclidianDistance(int x1, int y1, int x2, int y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

std::set<std::pair<int, int>> LIAN::generateSuccessors(Node* a, int delta, double alpha, const std::set<Node*>& CLOSED,
	const std::pair<int, int>& goal, const std::vector<std::vector<int>>& map) {
	std::set<std::pair<int, int>> SUCC;

	std::set<std::pair<int, int>> circle = midPointCircle(a->x, a->y, delta);

	for (const auto& a_prime : circle) {
		if (!isValid(a_prime.first, a_prime.second, map)) continue;
		if (CLOSED.count(new Node(a_prime.first, a_prime.second))) continue;
		SUCC.insert(a_prime);
	}

	if (EuclidianDistance(a->x, a->y, goal.first, goal.second) < delta) {
		SUCC.insert(goal);
	}

	return SUCC;
}

std::vector<std::pair<int, int>> LIAN::findPath() {
	Node* startNode = new Node(start.first, start.second);
	startNode->g = 0;

	std::set<Node*> OPEN;
	std::set<Node*> CLOSED;
	OPEN.insert(startNode);

	while (!OPEN.empty()) {
		Node* a = *OPEN.begin();

		for (auto node : OPEN) {
			if (node->g < a->g) {
				a = node;
			}
		}

		OPEN.erase(a);

		if (a->x == goal.first && a->y == goal.second) {
			std::vector<std::pair<int, int>> path = getPath(a);
			delete startNode;
			findedPath = path;
			return path;
		}

		CLOSED.insert(a);

		std::set<std::pair<int, int>> successors = generateSuccessors(a, delta, std::numeric_limits<double>::infinity(), CLOSED, goal, map);

		for (const auto& succ : successors) {
			Node* neighbor = new Node(succ.first, succ.second);

			double tentative_g = a->g + EuclidianDistance(a->x, a->y, succ.first, succ.second);

			if (tentative_g < neighbor->g) {
				neighbor->g = tentative_g;
				neighbor->parent = a;

				if (!OPEN.count(neighbor)) {
					OPEN.insert(neighbor);
				}
			}
			else {
				delete neighbor;
			}
		}
	}

	delete startNode;
	return {};
}

void LIAN::printSolution() {
	for (auto const& i : findedPath) {
		map[i.first][i.second] = 2;
	}

	for (int i = 0; i < map.size(); i++) {
		std::cout << std::endl;
		for (int j = 0; j < map[0].size(); j++) {
			std::cout << map[i][j] << " ";
		}
	}
}