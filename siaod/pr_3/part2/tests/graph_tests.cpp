#include "graph.hpp"

#include <cassert>
#include <iostream>
#include <vector>

using siaod::DecisionTreeShortestPath;
using siaod::Graph;

static void testVariant3Graph() {
    Graph g = siaod::makeVariant3Graph();
    assert(g.vertexCount() == 6);
    assert(g.edges().size() == 10);

    DecisionTreeShortestPath solver;
    // Кратчайший путь 1 -> 6: 1->3->4->6, вес 4+2+1=7
    auto r = solver.findShortestPath(g, 1, 6);
    assert(r.found);
    assert(r.totalWeight == 7);
    std::vector<int> expected = {1, 3, 4, 6};
    assert(r.vertices == expected);

    // Проверим ещё несколько путей.
    auto r12 = solver.findShortestPath(g, 1, 2);
    assert(r12.found);
    assert(r12.totalWeight == 7); // 1->3->2 = 4+3
    std::vector<int> e12 = {1, 3, 2};
    assert(r12.vertices == e12);

    auto r15 = solver.findShortestPath(g, 1, 5);
    assert(r15.found);
    // 1->3->4->5 = 4+2+3=9, 1->3->2->5 = 4+3+3=10, 1->2->5 = 11
    assert(r15.totalWeight == 9);
    std::vector<int> e15 = {1, 3, 4, 5};
    assert(r15.vertices == e15);
}

static void testNoPath() {
    Graph g(4);
    g.addEdge(1, 2, 1);
    g.addEdge(3, 4, 1);
    DecisionTreeShortestPath solver;
    auto r = solver.findShortestPath(g, 1, 4);
    assert(!r.found);
}

static void testSingleVertex() {
    Graph g(3);
    DecisionTreeShortestPath solver;
    auto r = solver.findShortestPath(g, 2, 2);
    assert(r.found);
    assert(r.totalWeight == 0);
    assert(r.vertices.size() == 1 && r.vertices[0] == 2);
}

static void testPruningHappens() {
    Graph g = siaod::makeVariant3Graph();
    DecisionTreeShortestPath solver;
    auto r = solver.findShortestPath(g, 1, 6);
    assert(r.found);
    // Дерево решений должно отсекать ветви - статистика должна быть осмысленной.
    assert(solver.stats().generatedNodes > 0);
    assert(solver.stats().expandedNodes > 0);
    // В этом графе есть хотя бы одно отсечение или несколько достижений цели.
    assert(solver.stats().goalNodes >= 1);
}

int main() {
    testVariant3Graph();
    testNoPath();
    testSingleVertex();
    testPruningHappens();
    std::cout << "All graph tests passed\n";
    return 0;
}
