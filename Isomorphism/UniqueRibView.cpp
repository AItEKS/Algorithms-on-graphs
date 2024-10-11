#include "UniqueRibView.h"

UniqueRibView::UniqueRibView(size_t sz) : size(sz) {
    values.resize(size, std::vector<unsigned long long int>(size, 0));
}

int UniqueRibView::CompareTo(const UniqueRibView& rp) const {
    for (size_t i = 0; i < size; i++) {
        if (values[i] != rp.values[i]) return (values[i] < rp.values[i]) ? -1 : 1;
    }
    return 0;
}

void UniqueRibView::WriteHash() const {
    unsigned long long h = 0;

    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < values[i].size(); j++) {
            h = h * 499 + values[i][j];
        }
    }

    h %= 10000;

    if (h < 1000) std::cout << " ";
    if (h < 100) std::cout << " ";
    if (h < 10) std::cout << " ";

    std::cout << h;
}

void UniqueRibView::Write() const {
    std::cout << "{";
    for (size_t i = 0; i < size; i++) {
        std::cout << "{";
        for (size_t j = 0; j < values[i].size(); j++) {
            std::cout << values[i][j];
            if (j < values[i].size() - 1) std::cout << ", ";
        }
        std::cout << "}";
        if (i < size - 1) std::cout << ", ";
    }
    std::cout << "}";
}

bool UniqueRibView::operator<(const UniqueRibView& other) const {
    return CompareTo(other) < 0;
}