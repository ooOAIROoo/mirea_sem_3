#include "graph.hpp"

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <unordered_set>

namespace siaod {

Graph::Graph(int vertexCount) {
    reset(vertexCount);
}

void Graph::reset(int vertexCount) {
    if (vertexCount < 0) {
        throw std::invalid_argument("Число вершин не может быть отрицательным");
    }
    vertexCount_ = vertexCount;
    edges_.clear();
}

int Graph::vertexCount() const noexcept {
    return vertexCount_;
}

bool Graph::addEdge(int from, int to, int weight) {
    if (from < 1 || from > vertexCount_ || to < 1 || to > vertexCount_) {
        return false;
    }
    if (weight < 0) {
        return false;
    }
    // Не допускаем дубликат того же ребра; разрешаем обновление веса? Нет - запретим дубликат.
    for (const auto& e : edges_) {
        if (e.from == from && e.to == to) return false;
    }
    edges_.push_back({from, to, weight});
    return true;
}

bool Graph::hasEdge(int from, int to) const {
    for (const auto& e : edges_) {
        if (e.from == from && e.to == to) return true;
    }
    return false;
}

const std::vector<Edge>& Graph::edges() const noexcept {
    return edges_;
}

std::vector<Edge> Graph::outgoing(int from) const {
    std::vector<Edge> out;
    for (const auto& e : edges_) {
        if (e.from == from) out.push_back(e);
    }
    // Для детерминированного дерева решений сортируем по номеру вершины-получателя.
    std::sort(out.begin(), out.end(), [](const Edge& a, const Edge& b) {
        if (a.to != b.to) return a.to < b.to;
        return a.weight < b.weight;
    });
    return out;
}

void Graph::printAdjacency(std::ostream& out) const {
    out << "Список смежности (ориентированный граф):\n";
    for (int v = 1; v <= vertexCount_; ++v) {
        out << "  " << v << ": ";
        bool first = true;
        for (const auto& e : edges_) {
            if (e.from == v) {
                if (!first) out << ", ";
                out << v << "->" << e.to << "(w=" << e.weight << ")";
                first = false;
            }
        }
        if (first) out << "(нет исходящих)";
        out << '\n';
    }
}

void Graph::printEdges(std::ostream& out) const {
    out << "Рёбра:\n";
    for (const auto& e : edges_) {
        out << "  " << e.from << " -> " << e.to << "  вес=" << e.weight << '\n';
    }
}

PathResult DecisionTreeShortestPath::findShortestPath(const Graph& graph, int source, int target) {
    tree_.clear();
    stats_ = {};

    PathResult best;
    best.found = false;
    best.totalWeight = std::numeric_limits<int>::max();

    if (source < 1 || source > graph.vertexCount() || target < 1 || target > graph.vertexCount()) {
        return best;
    }

    struct Frame {
        int vertex;
        int g;
        int parentIndex;
        int depth;
        std::vector<int> path;
    };

    // DFS-стек для построения дерева решений. Храним путь явно, так как граф мал.
    std::vector<Frame> stack;
    {
        DecisionNode root;
        root.vertex = source;
        root.g = 0;
        root.parentIndex = -1;
        root.depth = 0;
        tree_.push_back(root);
        stats_.generatedNodes = 1;
        stats_.maxDepth = 0;
    }
    stack.push_back({source, 0, 0, 0, {source}});

    int bestGoalNode = -1;

    while (!stack.empty()) {
        Frame cur = std::move(stack.back());
        stack.pop_back();

        // Проверка границы: если текущая стоимость уже не лучше лучшей - отсекаем.
        // (Для узла, уже положенного в дерево, отметим как pruned при раскрытии.)
        if (best.found && cur.g >= best.totalWeight) {
            tree_[cur.parentIndex].pruned = true;
            ++stats_.prunedNodes;
            continue;
        }

        ++stats_.expandedNodes;

        if (cur.vertex == target) {
            tree_[cur.parentIndex].goal = true;
            ++stats_.goalNodes;
            if (!best.found || cur.g < best.totalWeight) {
                best.found = true;
                best.totalWeight = cur.g;
                best.vertices = cur.path;
                bestGoalNode = cur.parentIndex;
            }
            continue;
        }

        auto outs = graph.outgoing(cur.vertex);
        // Фильтруем вершины, уже находящиеся в текущем пути (избегаем циклов).
        std::vector<Edge> candidates;
        for (const auto& e : outs) {
            bool inPath = false;
            for (int v : cur.path) {
                if (v == e.to) { inPath = true; break; }
            }
            if (!inPath) candidates.push_back(e);
        }

        if (candidates.empty()) {
            tree_[cur.parentIndex].deadEnd = true;
            continue;
        }

        const int curNodeIndex = cur.parentIndex;
        // Добавляем потомков в стек в обратном порядке, чтобы первый кандидат
        // (по номеру вершины-получателя) обрабатывался первым при DFS.
        for (auto it = candidates.rbegin(); it != candidates.rend(); ++it) {
            const auto& e = *it;
            const int ng = cur.g + e.weight;
            DecisionNode child;
            child.vertex = e.to;
            child.g = ng;
            child.parentIndex = curNodeIndex;
            child.depth = cur.depth + 1;
            child.pruned = false;
            child.goal = false;
            child.deadEnd = false;

            if (best.found && ng >= best.totalWeight) {
                child.pruned = true;
                ++stats_.prunedNodes;
                ++stats_.generatedNodes;
                stats_.maxDepth = std::max(stats_.maxDepth, static_cast<std::size_t>(child.depth));
                tree_.push_back(child);
                // Не кладём в стек - отсечён.
                continue;
            }

            const int childIndex = static_cast<int>(tree_.size());
            tree_.push_back(child);
            ++stats_.generatedNodes;
            stats_.maxDepth = std::max(stats_.maxDepth, static_cast<std::size_t>(child.depth));

            std::vector<int> nextPath = cur.path;
            nextPath.push_back(e.to);
            stack.push_back({e.to, ng, childIndex, child.depth, std::move(nextPath)});
        }
    }

    (void)bestGoalNode;
    return best;
}

const std::vector<DecisionNode>& DecisionTreeShortestPath::tree() const noexcept {
    return tree_;
}

const SearchStats& DecisionTreeShortestPath::stats() const noexcept {
    return stats_;
}

std::vector<int> DecisionTreeShortestPath::buildPath(int nodeIndex) const {
    std::vector<int> path;
    for (int i = nodeIndex; i >= 0; i = tree_[i].parentIndex) {
        path.push_back(tree_[i].vertex);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

void DecisionTreeShortestPath::printTree(std::ostream& out, const Graph& /*graph*/) const {
    out << "Дерево решений (узлы в порядке генерации):\n";
    out << "idx | parent | depth | vertex | g | статус\n";
    out << "----+--------+-------+--------+---+----------------\n";
    for (std::size_t i = 0; i < tree_.size(); ++i) {
        const auto& n = tree_[i];
        out << std::setw(3) << i << " | "
            << std::setw(6) << n.parentIndex << " | "
            << std::setw(5) << n.depth << " | "
            << std::setw(6) << n.vertex << " | "
            << std::setw(2) << n.g << " | ";
        if (n.goal) out << "ЦЕЛЬ";
        else if (n.pruned) out << "ОТСЕЧЕНО (g >= best)";
        else if (n.deadEnd) out << "ТУПИК";
        else out << "раскрыт/промежуточный";
        out << '\n';
    }
    out << "\nСтатистика:\n";
    out << "  Сгенерировано узлов: " << stats_.generatedNodes << "\n";
    out << "  Раскрыто узлов:      " << stats_.expandedNodes << "\n";
    out << "  Отсечено узлов:      " << stats_.prunedNodes << "\n";
    out << "  Достижений цели:     " << stats_.goalNodes << "\n";
    out << "  Макс. глубина:       " << stats_.maxDepth << "\n";
}

Graph makeVariant3Graph() {
    Graph g(6);
    // Рёбра по предложенному графу варианта 3.
    g.addEdge(1, 2, 8);
    g.addEdge(1, 3, 4);
    g.addEdge(3, 2, 3);
    g.addEdge(2, 4, 6);
    g.addEdge(2, 5, 3);
    g.addEdge(3, 4, 2);
    g.addEdge(3, 6, 10);
    g.addEdge(4, 5, 3);
    g.addEdge(4, 6, 1);
    g.addEdge(5, 6, 4);
    return g;
}

} // namespace siaod
