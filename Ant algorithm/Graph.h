#pragma once

#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <queue>
#include <algorithm>

class Graph {
private:
    size_t size;
    std::vector<std::vector<unsigned long long int>> matrix;

public:
    void Init(size_t sz);

    Graph();
    Graph(size_t sz);
    Graph(const std::vector<std::vector<unsigned long long int>>& weightsMatrix);

    void Add(size_t a, size_t b, int weight);
    void Copy(const Graph& graph);
    void Multiply(const Graph& graph);
    int CompareTo(const Graph& graph) const;
    unsigned long long int GetWeightSum() const;

    size_t GetSize() const { return size; }
    const std::vector<std::vector<unsigned long long int>>& GetMatrix() const { return matrix; }

    void Write() const;

    std::vector<unsigned long long int> Dijkstra(size_t start, size_t end) const;
};