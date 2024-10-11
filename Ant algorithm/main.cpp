#include <iostream>

#include "Graph.h"
#include "Ant.h"

int main() {
    setlocale(LC_ALL, "");
    Graph graph(6);

    graph.Add(0, 1, 3);
    graph.Add(1, 0, 3);
    graph.Add(0, 4, 1);
    graph.Add(4, 0, 3);
    graph.Add(1, 5, 3);
    graph.Add(5, 1, 3);
    graph.Add(1, 2, 8);
    graph.Add(2, 1, 3);
    graph.Add(2, 3, 1);
    graph.Add(3, 2, 8);
    graph.Add(2, 5, 1);
    graph.Add(5, 2, 3);
    graph.Add(3, 4, 1);
    graph.Add(4, 3, 3);
    graph.Add(5, 4, 4);
    graph.Add(5, 3, 5);
    graph.Add(5, 0, 3);

    AntColonyOptimization aco(graph);

    AntPath best_path = aco.SolveSalesmansProblem();

    std::cout << "Лучший путь: ";
    for (const auto& vertex : best_path.vertices) {
        std::cout << vertex << "->";
    }
    std::cout << std::endl;
    std::cout << "Длина пути: " << best_path.distance << std::endl;

    return 0;
}
