#include "Graph.h"

void Graph::Init(size_t sz) {
    size = sz;
    matrix.resize(size, std::vector<unsigned long long int>(size, 0));
}

Graph::Graph() {
    Init(0);
}

Graph::Graph(size_t sz) {
    Init(sz);
}

Graph::Graph(const std::vector<std::vector<unsigned long long int>>& weightsMatrix) {
    size = weightsMatrix.size();
    matrix = weightsMatrix;
}

Graph::Graph(const nlohmann::json& json) {
    if (!json.is_array()) {
        throw std::invalid_argument("Invalid JSON format: Expected an array.");
    }

    size = json.size();
    matrix.resize(size, std::vector<unsigned long long int>(size, 0));

    for (size_t i = 0; i < size; ++i) {
        if (!json[i].is_array() || json[i].size() != size) {
            throw std::invalid_argument("Invalid JSON format: Each row must be an array of equal length.");
        }

        for (size_t j = 0; j < size; ++j) {
            matrix[i][j] = json[i][j].get<unsigned long long int>();
        }
    }
}

Graph::Graph(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::invalid_argument("Could not open the file: " + filename);
    }

    std::string line;
    size_t maxNode = 0;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        size_t source, target;
        unsigned long long weight;

        if (!(iss >> source >> target >> weight)) {
            throw std::invalid_argument("Invalid file format.");
        }

        maxNode = std::max({ maxNode, source, target });
    }

    size = maxNode + 1;
    Init(size);

    file.clear();
    file.seekg(0);

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        size_t source, target;
        unsigned long long weight;

        if (!(iss >> source >> target >> weight)) {
            throw std::invalid_argument("Invalid file format.");
        }

        Add(source, target, weight);
    }

    //int sum = 0, k = 0;
    //for (size_t i = 0; i < size; i++) {
    //    for (size_t j = 0; j < size; j++) {
    //        if (matrix[i][j] != 0) {
    //            sum += matrix[i][j];
    //            k++;
    //        }
    //    }
    //}

    //for (size_t i = 0; i < size; i++) {
    //    for (size_t j = 0; j < size; j++) {
    //        if (matrix[i][j] == 0) {
    //            matrix[i][j] = sum / k;
    //        }
    //    }
    //}
}

void Graph::Add(size_t a, size_t b, int weight) {
    matrix[a][b] = weight;
    matrix[b][a] = weight;
}

void Graph::Copy(const Graph& graph) {
    matrix = graph.matrix;
}

void Graph::Multiply(const Graph& graph) {
    Graph temp(size);

    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            unsigned long long sum = 0;
            for (size_t k = 0; k < size; ++k) {
                sum += matrix[i][k] * graph.matrix[k][j];
            }
            temp.matrix[i][j] = sum;
        }
    }

    matrix = temp.matrix;
}

void Graph::Write() const {
    std::cout << "Size: " << size << std::endl;
    if (size == 0) return;

    std::vector<size_t> width(size, 0);
    for (size_t j = 0; j < size; j++) {
        for (size_t i = 0; i < size; i++) {
            size_t currentLength = std::to_string(matrix[i][j]).length();
            if (width[j] < currentLength) {
                width[j] = currentLength;
            }
        }
    }

    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            std::cout << std::setw(width[j]) << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int Graph::CompareTo(const Graph& graph) const {
    if (size < graph.size) return -1;
    if (size > graph.size) return 1;

    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            if (matrix[i][j] != graph.matrix[i][j]) return matrix[i][j] < graph.matrix[i][j] ? -1 : 1;
        }
    }

    return 0;
}

unsigned long long int Graph::GetWeightSum() const{
    unsigned long long int sum = 0;
    for (size_t i = 0; i < size; i++) {
        for (size_t j = i + 1; j < size; j++) {
            sum += matrix[i][j];
        }
    }
    return sum;
}

std::vector<unsigned long long int> Graph::Dijkstra(size_t start, size_t end) const {
    std::vector<unsigned long long int> distances(size, std::numeric_limits<unsigned long long int>::max());
    distances[start] = 0;

    using PII = std::pair<unsigned long long int, size_t>;
    std::priority_queue<PII, std::vector<PII>, std::greater<PII>> pq;

    pq.push({ 0, start });

    while (!pq.empty()) {
        PII top = pq.top();
        unsigned long long int currentDistance = top.first;
        size_t currentVertex = top.second;
        pq.pop();

        if (currentDistance > distances[currentVertex]) continue;

        for (size_t neighbor = 0; neighbor < size; ++neighbor) {
            if (matrix[currentVertex][neighbor] != 0) {
                unsigned long long int newDistance = currentDistance + matrix[currentVertex][neighbor];
                if (newDistance < distances[neighbor]) {
                    distances[neighbor] = newDistance;
                    pq.push({ newDistance, neighbor });
                }
            }
        }
    }

    return distances;
}

//Теорема Оре
bool Graph::HasHamiltonianCycle() const {
    if (size < 3) return false;

    std::vector<size_t> degrees(size, 0);

    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            if (matrix[i][j] != 0) {
                degrees[i]++;
            }
        }
    }

    for (size_t u = 0; u < size; ++u) {
        for (size_t v = u + 1; v < size; ++v) {
            if (matrix[u][v] == 0) { 
                if (degrees[u] + degrees[v] < size) {
                    return false;
                }
            }
        }
    }

    //for (size_t i = 0; i < size; ++i) {
    //    std::cout << degrees[i] << " ";
    //}

    return true;
}

void Graph::CompleteHamiltonianCycle() {
    // Вычисляем степени вершин
    std::vector<size_t> degrees(size, 0);
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            if (matrix[i][j] != 0) {
                degrees[i]++;
            }
        }
    }

    size_t addedEdges = 0;

    // Добавление недостающих рёбер
    for (size_t u = 0; u < size; ++u) {
        for (size_t v = u + 1; v < size; ++v) {
            if (matrix[u][v] == 0) { // Если нет ребра
                if (degrees[u] + degrees[v] < size) {
                    // Добавляем ребро между u и v
                    Add(u, v, 1);
                    degrees[u]++;
                    degrees[v]++;
                    addedEdges++;
                }
            }
        }
    }

    // Вывод количества добавленных рёбер
    std::cout << "Количество добавленных рёбер: " << addedEdges << std::endl;
}