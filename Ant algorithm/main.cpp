#include <iostream>
#include <fstream>

#include "Graph.h"
#include "Ant.h"
#include "IsomorphusCheck.h"

#include <nlohmann/json.hpp>

int main() {
    setlocale(LC_ALL, "");
    /*
    Graph g1(6);
    Graph g2(6);
    Graph g3(5);

    IsomorphusCheck isomorphusCheck;

    g1.Add(0, 1, 1);
    g1.Add(0, 3, 1);
    g1.Add(0, 5, 1);
    g1.Add(1, 2, 1);
    g1.Add(1, 4, 1);
    g1.Add(2, 3, 1);
    g1.Add(2, 5, 1);
    g1.Add(3, 4, 1);
    g1.Add(4, 5, 1);

    g2.Add(0, 1, 1);
    g2.Add(0, 4, 1);
    g2.Add(0, 3, 1);
    g2.Add(1, 2, 1);
    g2.Add(1, 5, 1);
    g2.Add(2, 4, 1);
    g2.Add(2, 3, 1);
    g2.Add(3, 5, 1);
    g2.Add(4, 5, 1);

    g3.Add(0, 1, 10);
    g3.Add(1, 2, 5);
    g3.Add(2, 3, 2);
    g3.Add(3, 4, 1);
    g3.Add(0, 4, 100);


    int result = isomorphusCheck.Heuristic(g1, g2);

    if (result == IsomorphusCheck::YES)
        std::cout << "Графы изоморфны" << std::endl;
    else
        std::cout << "Графы не изоморфны" << std::endl;

    auto distances = g3.Dijkstra(0, 3);
    std::cout << "Путь : " << distances[3] << std::endl;
    */


    /*
    Graph graph(4);

    graph.Add(0, 1, 3);
    graph.Add(1, 0, 3);
    graph.Add(0, 2, 1);
    graph.Add(2, 0, 3);
    graph.Add(1, 3, 3);
    graph.Add(3, 1, 3);
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

    graph.FindHamiltonianCycle();
    graph.WriteHamiltonianCycle();
    AntColonyOptimization aco(graph);

    AntPath best_path = aco.SolveSalesmansProblem();

    std::cout << "Лучший путь: ";
    for (const auto& vertex : best_path.vertices) {
        std::cout << vertex << "->";
    }
    std::cout << std::endl;
    std::cout << "Длина пути: " << best_path.distance << std::endl;
    */


    /*
    std::ifstream inputFile("graph100.json");
    nlohmann::json jsonData;
    inputFile >> jsonData;

    Graph graph(jsonData);

    if (graph.HasHamiltonianCycle()) {
        AntColonyOptimization aco(graph);

        AntPath best_path = aco.SolveSalesmansProblem();

        std::cout << "Лучший путь: ";
        for (const auto& vertex : best_path.vertices) {
            std::cout << vertex << "->";
        }
        std::cout << std::endl;
        std::cout << "Длина пути: " << best_path.distance << std::endl;
    }
    */

    
    std::string filename = "1000.txt";
    Graph graph(filename);
    
    graph.CompleteHamiltonianCycle();

    if (graph.HasHamiltonianCycle()) {
        AntColonyOptimization aco(graph);

        AntPath best_path = aco.SolveSalesmansProblem();

        std::cout << "Лучший путь: ";
        for (const auto& vertex : best_path.vertices) {
            std::cout << vertex << "->";
        }
        std::cout << std::endl;
        std::cout << "Длина пути: " << best_path.distance << std::endl;
    }
    

    return 0;
}
