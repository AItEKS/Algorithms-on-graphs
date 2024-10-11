#include <iostream>

#include "Graph.h"
#include "IsomorphusCheck.h"

int main() {
    setlocale(LC_ALL, "");

    Graph g1(6);
    Graph g2(6);

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

    int result = isomorphusCheck.Heuristic(g1, g2);

    if (result == IsomorphusCheck::YES)
        std::cout << "Графы изоморфны" << std::endl;
    else
        std::cout << "Графы не изоморфны" << std::endl;

    return 0;
}
