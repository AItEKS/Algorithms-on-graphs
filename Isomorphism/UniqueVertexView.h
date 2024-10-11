#pragma once

#include <vector>

#include "UniqueRibView.h"

class UniqueVertexView {
public:
    size_t size;
    std::vector<UniqueRibView> rib;
    std::vector<UniqueRibView> srib;

    UniqueVertexView(size_t sz);
    int CompareTo(const UniqueVertexView& vertexView) const;
    void Write() const;
    bool operator<(const UniqueVertexView& other) const;
};