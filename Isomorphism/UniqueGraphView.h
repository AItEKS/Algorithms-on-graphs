#pragma once

#include <vector>

#include "Graph.h"
#include "UniqueVertexView.h"

class UniqueGraphView {
public:
    size_t size;
    std::vector<UniqueVertexView> vert;
    std::vector<UniqueVertexView> svert;

    UniqueGraphView(const Graph& graph);
    int CompareTo(const UniqueGraphView& graphView) const;
    void Write() const;
};