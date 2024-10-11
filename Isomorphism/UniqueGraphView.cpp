#include "UniqueGraphView.h"

UniqueGraphView::UniqueGraphView(const Graph& graph) : size(graph.GetSize()), vert(size, UniqueVertexView(size)), svert(size, UniqueVertexView(size)) {
    Graph temp(graph.GetSize());

    temp.Copy(graph);

    for (size_t k = 0; k < graph.GetSize(); k++) {
        for (size_t i = 0; i < graph.GetSize(); i++) {
            for (size_t j = 0; j < graph.GetSize(); j++) {
                vert[i].rib[j].values.resize(graph.GetSize());
                vert[i].rib[j].values[k].push_back(temp.GetMatrix()[i][j]);
            }
        }
        temp.Multiply(graph);
    }

    for (size_t i = 0; i < size; ++i) {
        svert[i] = vert[i];
        for (size_t j = 0; j < size; ++j) {
            svert[i].srib[j] = vert[i].rib[j];
        }
    }

    for (size_t i = 0; i < size; ++i) {
        std::sort(svert[i].srib.begin(), svert[i].srib.end());
    }

    std::sort(svert.begin(), svert.end());
}

int UniqueGraphView::CompareTo(const UniqueGraphView& graphView) const {
    for (size_t i = 0; i < size; ++i) {
        int result = svert[i].CompareTo(graphView.svert[i]);
        if (result != 0)
            return result;
    }
    return 0;
}

void UniqueGraphView::Write() const {
    for (size_t i = 0; i < size; i++) {
        std::cout << i << ": ";
        for (size_t j = 0; j < size; j++) {
            vert[i].rib[j].WriteHash();
            std::cout << "   ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}