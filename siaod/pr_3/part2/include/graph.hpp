#pragma once

#include <cstddef>
#include <iosfwd>
#include <limits>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace siaod {

struct Edge {
    int from = 0;
    int to = 0;
    int weight = 0;
};

struct PathResult {
    bool found = false;
    int totalWeight = 0;
    std::vector<int> vertices; // от источника до цели
};

struct DecisionNode {
    int vertex = 0;
    int g = 0; // стоимость пути от источника до этой вершины
    int parentIndex = -1; // индекс родителя в дереве решений (-1 для корня)
    int depth = 0;
    bool pruned = false;  // отсечено по границе
    bool goal = false;    // достигнута цель
    bool deadEnd = false; // тупик (нет непосещённых соседей)
};

struct SearchStats {
    std::size_t generatedNodes = 0;
    std::size_t expandedNodes = 0;
    std::size_t prunedNodes = 0;
    std::size_t goalNodes = 0;
    std::size_t maxDepth = 0;
};

class Graph {
public:
    Graph() = default;
    explicit Graph(int vertexCount);

    void reset(int vertexCount);
    int vertexCount() const noexcept;
    bool addEdge(int from, int to, int weight);
    bool hasEdge(int from, int to) const;
    const std::vector<Edge>& edges() const noexcept;

    // Список исходящих рёбер для вершины.
    std::vector<Edge> outgoing(int from) const;

    void printAdjacency(std::ostream& out) const;
    void printEdges(std::ostream& out) const;

private:
    int vertexCount_ = 0;
    std::vector<Edge> edges_;
};

class DecisionTreeShortestPath {
public:
    // Поиск кратчайшего пути из source в target методом построения дерева решений
    // (ветвей и границ): узлы дерева - состояния (текущая вершина, путь, стоимость).
    // Ветвь отсекается, если g >= длины лучшего уже найденного пути до цели.
    PathResult findShortestPath(const Graph& graph, int source, int target);

    const std::vector<DecisionNode>& tree() const noexcept;
    const SearchStats& stats() const noexcept;

    void printTree(std::ostream& out, const Graph& graph) const;

private:
    std::vector<DecisionNode> tree_;
    SearchStats stats_;

    std::vector<int> buildPath(int nodeIndex) const;
};

// Предложенный граф из варианта 3.
Graph makeVariant3Graph();

} // namespace siaod
