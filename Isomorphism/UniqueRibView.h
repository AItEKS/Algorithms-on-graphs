#pragma once

#include <vector>
#include <iostream>

class UniqueRibView {
public:
    size_t size;
    std::vector<std::vector<unsigned long long int>> values;

    UniqueRibView(size_t sz);
    int CompareTo(const UniqueRibView& rp) const;
    void WriteHash() const;
    void Write() const;
    bool operator<(const UniqueRibView& other) const;
};