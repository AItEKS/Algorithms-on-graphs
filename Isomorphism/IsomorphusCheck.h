#pragma once

#include "Graph.h"
#include "UniqueGraphView.h"

class IsomorphusCheck {
public:
    static constexpr int NO = 0;
    static constexpr int YES = 1;
    static constexpr int AMBIGUOUS = -1;

    int Heuristic(const Graph& graphA, const Graph& graphB) {
        UniqueGraphView graphViewA(graphA);
        UniqueGraphView graphViewB(graphB);

        if (graphViewA.size != graphViewB.size) return NO;
        if (graphViewA.CompareTo(graphViewB) != 0) return NO;
        else return YES;
    }
};