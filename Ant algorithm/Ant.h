#include <iostream>
#include <vector>
#include <random>
#include <limits>
#include <algorithm>
#include <cmath>

// Структура для хранения пути муравья
struct AntPath {
    std::vector<std::size_t> vertices; // Вершины, которые посещает муравей
    double distance = 0; // Общая длина пути
};

// Структура для представления муравья
struct Ant {
    explicit Ant(std::size_t start_vertex = 0) : start_location(start_vertex), current_location(start_vertex) {}

    AntPath path; // Путь муравья
    std::vector<std::size_t> visited; // Посещенные вершины
    std::size_t start_location = 0; // Начальная вершина
    std::size_t current_location = 0; // Текущая вершина
    bool can_continue = true; // Может ли муравей продолжать движение

    void MakeChoice(const Graph& g, const std::vector<std::vector<double>>& p, double a, double b); // Метод для выбора следующей вершины
    double getRandomChoice(); // Метод для получения случайного числа
    std::vector<std::size_t> getNeighborVertexes(const Graph& g); // Метод для получения соседних вершин
};

// Класс для реализации алгоритма оптимизации муравьиной колонии
class AntColonyOptimization {
public:
    explicit AntColonyOptimization(const Graph& graph); // Конструктор с графом

    AntPath SolveSalesmansProblem(); // Метод для решения задачи коммивояжера

private:
    const double kAlpha_ = 1.5; // Параметр альфа (влияние феромонов)
    const double kBeta_ = 1.2; // Параметр бета (влияние расстояния)
    const double kPheromone0_ = 0.6; // Начальное количество феромона
    const double kQ_ = 10.0; // Константа для обновления феромонов
    const double kEvaporation_ = 0.2; // Коэффициент испарения феромонов

    Graph graph_; // Граф, представляющий задачу
    std::vector<std::vector<double>> pheromone_; // Матрица феромонов
    std::vector<Ant> ants_; // Вектор муравьев

    void CreateAnts(); // Метод для создания муравьев
    void UpdateGlobalPheromone(const std::vector<std::vector<double>>& local_pheromone_update); // Метод для обновления глобальных феромонов
};

// Конструктор класса AntColonyOptimization, инициализирует граф и матрицу феромонов
AntColonyOptimization::AntColonyOptimization(const Graph& graph)
    : graph_(graph), pheromone_(graph.GetSize(), std::vector<double>(graph.GetSize(), kPheromone0_)) {}

// Метод для создания муравьев
void AntColonyOptimization::CreateAnts() {
    const auto kAntsCount = 50; // Количество муравьев
    ants_.resize(kAntsCount); // Резервируем место для муравьев

    for (std::size_t i = 0; i < kAntsCount; ++i)
        ants_[i] = Ant(i % graph_.GetSize()); // Инициализируем каждого муравья с начальной вершиной
}

// Метод для обновления глобальных феромонов на основе локального обновления феромонов
void AntColonyOptimization::UpdateGlobalPheromone(const std::vector<std::vector<double>>& lpu) {
    for (std::size_t from = 0; from < pheromone_.size(); ++from) {
        for (std::size_t to = 0; to < pheromone_[from].size(); ++to) {
            pheromone_[from][to] = (1 - kEvaporation_) * pheromone_[from][to] + lpu[from][to]; // Обновляем феромоны с учетом испарения и локального обновления
            if (pheromone_[from][to] < 0.01 && from != to) // Устанавливаем минимальное значение феромона
                pheromone_[from][to] = 0.01;
        }
    }
}

// Метод для получения случайного числа в диапазоне [0, 1)
double Ant::getRandomChoice() {
    static std::random_device rd; // Генератор случайных чисел
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen); // Возвращаем случайное число
}

// Метод для получения соседних вершин текущей вершины муравья в графе
std::vector<std::size_t> Ant::getNeighborVertexes(const Graph& g) {
    std::vector<std::size_t> vertexes;

    for (std::size_t to = 0; to < g.GetSize(); ++to) { // Проходим по всем вершинам графа
        bool edge_is_exist = g.GetMatrix()[current_location][to] != 0; // Проверяем наличие ребра между текущей и соседней вершиной
        bool vertex_is_unvisited = std::find(visited.begin(), visited.end(), to) == visited.end(); // Проверяем, была ли соседняя вершина посещена

        if (edge_is_exist && vertex_is_unvisited) {
            vertexes.push_back(to); // Добавляем соседнюю вершину в список если она доступна и не посещена
        }
    }

    return vertexes; // Возвращаем список соседних вершин
}

// Метод для выбора следующей вершины на основе вероятности и феромонов
void Ant::MakeChoice(const Graph& g, const std::vector<std::vector<double>>& p, double alpha, double beta) {
    if (path.vertices.empty()) {
        path.vertices.push_back(current_location); // Добавляем начальную вершину в путь 
        visited.push_back(current_location);
    }

    std::vector<std::size_t> neighbor_vertexes = getNeighborVertexes(g); // Получаем соседние вершины

    if (neighbor_vertexes.empty()) {
        can_continue = false; // Если нет доступных соседей, останавливаемся 
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

// Метод для решения задачи коммивояжера с использованием алгоритма оптимизации муравьиной колонии
AntPath AntColonyOptimization::SolveSalesmansProblem() {
    constexpr std::size_t kMaxIterationsWithoutImprovement = 10;
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