#include "UniqueVertexView.h"

UniqueVertexView::UniqueVertexView(size_t sz) : size(sz), rib(sz, UniqueRibView(sz)), srib(sz, UniqueRibView(sz)) {}

int UniqueVertexView::CompareTo(const UniqueVertexView& vertexView) const {
    for (size_t i = 0; i < size; i++) {
        int cmpResult = srib[i].CompareTo(vertexView.srib[i]);
        if (cmpResult != 0) return cmpResult;
    }
    return 0;
}

void UniqueVertexView::Write() const {
    std::cout << "{";
    for (size_t i = 0; i < size; i++) {
        srib[i].Write();
        if (i < size - 1) std::cout << ", ";
    }
    std::cout << "}";
}

bool UniqueVertexView::operator<(const UniqueVertexView& other) const {
    return CompareTo(other) < 0;
}