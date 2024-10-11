#include <iostream>
#include <vector>
#include <random>
#include <limits>
#include <algorithm>
#include <cmath>

struct AntPath {
    std::vector<std::size_t> vertices;
    double distance = 0;
};

struct Ant {
    explicit Ant(std::size_t start_vertex = 0) : start_location(start_vertex), current_location(start_vertex) {}

    AntPath path;
    std::vector<std::size_t> visited;
    std::size_t start_location = 0;
    std::size_t current_location = 0;
    bool can_continue = true;

    void MakeChoice(const Graph& g, const std::vector<std::vector<double>>& p, double a, double b);
    double getRandomChoice();
    std::vector<std::size_t> getNeighborVertexes(const Graph& g);
};

class AntColonyOptimization {
public:
    explicit AntColonyOptimization(const Graph& graph);

    AntPath SolveSalesmansProblem();

private:
    const double kAlpha_ = 1.0;
    const double kBeta_ = 2.0;
    const double kPheromone0_ = 1;
    const double kQ_ = 5.0;
    const double kEvaporation_ = 0.2;

    Graph graph_;
    std::vector<std::vector<double>> pheromone_;
    std::vector<Ant> ants_;

    void CreateAnts();
    void UpdateGlobalPheromone(const std::vector<std::vector<double>>& local_pheromone_update);
};

AntColonyOptimization::AntColonyOptimization(const Graph& graph)
    : graph_(graph), pheromone_(graph.GetSize(), std::vector<double>(graph.GetSize(), kPheromone0_)) {}

void AntColonyOptimization::CreateAnts() {
    const auto kAntsCount = graph_.GetSize();
    ants_.resize(kAntsCount);

    for (std::size_t i = 0; i < kAntsCount; ++i)
        ants_[i] = Ant(i % graph_.GetSize());
}

void AntColonyOptimization::UpdateGlobalPheromone(const std::vector<std::vector<double>>& lpu) {
    for (std::size_t from = 0; from < pheromone_.size(); ++from) {
        for (std::size_t to = 0; to < pheromone_[from].size(); ++to) {
            pheromone_[from][to] = (1 - kEvaporation_) * pheromone_[from][to] + lpu[from][to];
            if (pheromone_[from][to] < 0.01 && from != to)
                pheromone_[from][to] = 0.01;
        }
    }
}

double Ant::getRandomChoice() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen);
}

std::vector<std::size_t> Ant::getNeighborVertexes(const Graph& g) {
    std::vector<std::size_t> vertexes;

    for (std::size_t to = 0; to < g.GetSize(); ++to) {
        bool edge_is_exist = g.GetMatrix()[current_location][to] != 0;
        bool vertex_is_unvisited = std::find(visited.begin(), visited.end(), to) == visited.end();

        if (edge_is_exist && vertex_is_unvisited) {
            vertexes.push_back(to);
        }
    }

    return vertexes;
}

void Ant::MakeChoice(const Graph& g, const std::vector<std::vector<double>>& p, double alpha, double beta) {
    if (path.vertices.empty()) {
        path.vertices.push_back(current_location);
        visited.push_back(current_location);
    }

    std::vector<std::size_t> neighbor_vertexes = getNeighborVertexes(g);

    if (neighbor_vertexes.empty()) {
        can_continue = false;
        if (g.GetMatrix()[current_location][start_location] != 0) {
            path.vertices.push_back(start_location);
            path.distance += g.GetMatrix()[current_location][start_location];
        }
        return;
    }

    std::vector<double> wish(neighbor_vertexes.size());

    double summary_wish = 0.0f;

    for (std::size_t i = 0; i < neighbor_vertexes.size(); ++i) {
        auto v = neighbor_vertexes[i];

        double t = p[current_location][v];
        double w = g.GetMatrix()[current_location][v];

        if (w > 0) {
            double n = 1 / w;
            wish[i] = std::pow(t, alpha) * std::pow(n, beta);
            summary_wish += wish[i];
        }
        else {
            wish[i] = 0;
        }
    }

    if (summary_wish == 0) return;

    std::vector<double> choosing_probability(neighbor_vertexes.size());

    for (std::size_t neighbor = 0; neighbor < neighbor_vertexes.size(); ++neighbor) {
        choosing_probability[neighbor] = wish[neighbor] / summary_wish;

        if (neighbor > 0)
            choosing_probability[neighbor] += choosing_probability[neighbor - 1];
    }

    double choose = getRandomChoice();

    for (std::size_t n = 0; n < neighbor_vertexes.size(); ++n) {
        if (choose <= choosing_probability[n]) {
            current_location = neighbor_vertexes[n];
            path.vertices.push_back(current_location);
            path.distance += g.GetMatrix()[path.vertices[path.vertices.size() - 2]][current_location];
            visited.push_back(current_location);
            break;
        }
    }
}

AntPath AntColonyOptimization::SolveSalesmansProblem() {
    constexpr std::size_t kMaxIterationsWithoutImprovement = 2500;
    const std::size_t kVertexesCount = graph_.GetSize();
    std::size_t counter = 0;
    std::size_t iteration = 0;

    AntPath path;
    path.distance = std::numeric_limits<double>::infinity();

    while (counter++ != kMaxIterationsWithoutImprovement) {
        iteration++;
        std::vector<std::vector<double>> summary_pheromone(kVertexesCount, std::vector<double>(kVertexesCount, 0.0));
        CreateAnts();
        for (auto& ant : ants_) {
            while (ant.can_continue)
                ant.MakeChoice(graph_, pheromone_, kAlpha_, kBeta_);

            auto ant_path = ant.path;
            if (ant_path.vertices.size() == kVertexesCount + 1) {
                if (path.distance > ant.path.distance) {
                    path = std::move(ant.path);
                    counter = 0;
                }

                for (std::size_t v = 0; v != ant_path.vertices.size() - 1; ++v)
                    summary_pheromone[ant_path.vertices[v]][ant_path.vertices[v + 1]] += kQ_ / ant_path.distance;
            }
        }
        UpdateGlobalPheromone(summary_pheromone);
    }

    return path;
}