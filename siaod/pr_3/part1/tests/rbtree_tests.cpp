#include "rbtree.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <vector>

using siaod::RBTree;

static void testEmpty() {
    RBTree t;
    assert(t.empty());
    assert(t.size() == 0);
    assert(t.sumLeaves() == 0);
    assert(!t.averageAll().has_value());
    assert(t.preorder().empty());
    assert(t.inorder().empty());
    assert(t.validate());
}

static void testInsertAndDuplicates() {
    RBTree t;
    assert(t.insert('M'));
    assert(t.insert('F'));
    assert(!t.insert('M')); // дубликат
    assert(t.size() == 2);
    assert(t.validate());
}

static void testTraversalsAndDemo() {
    RBTree t;
    for (char c : siaod::demoKeys()) {
        assert(t.insert(c));
    }
    assert(t.size() == 10);
    assert(t.validate());

    // Симметричный обход BST должен давать отсортированную последовательность.
    auto in = t.inorder();
    std::vector<char> sorted = siaod::demoKeys();
    std::sort(sorted.begin(), sorted.end());
    assert(in == sorted);

    // Прямой обход: корень первый, и все элементы присутствуют.
    auto pre = t.preorder();
    assert(pre.size() == 10);
    std::set<char> preSet(pre.begin(), pre.end());
    std::set<char> expected(sorted.begin(), sorted.end());
    assert(preSet == expected);
}

static void testSumLeavesAndAverage() {
    RBTree t;
    // Простое дерево: вставим A, B, C
    //        B(B)
    //       /   backslash
    //     A(R)  C(R)
    t.insert('B');
    t.insert('A');
    t.insert('C');
    assert(t.validate());
    auto leaves = t.leaves();
    std::sort(leaves.begin(), leaves.end());
    std::vector<char> expLeaves = {'A', 'C'};
    assert(leaves == expLeaves);
    long long sum = static_cast<unsigned char>('A') + static_cast<unsigned char>('C');
    assert(t.sumLeaves() == sum);

    long long total = static_cast<unsigned char>('A') + static_cast<unsigned char>('B') +
                      static_cast<unsigned char>('C');
    double avg = static_cast<double>(total) / 3.0;
    assert(t.averageAll().has_value());
    assert(std::abs(*t.averageAll() - avg) < 1e-9);
}

static void testDemoValues() {
    RBTree t;
    const std::vector<char> keys = {'M', 'F', 'S', 'B', 'H', 'R', 'W', 'A', 'D', 'K'};
    for (char c : keys) t.insert(c);
    assert(t.validate());
    assert(t.size() == 10);

    auto in = t.inorder();
    std::vector<char> sorted = keys;
    std::sort(sorted.begin(), sorted.end());
    assert(in == sorted);

    // Проверим сумму листьев и среднее вручную.
    auto leaves = t.leaves();
    long long sumLeaves = 0;
    for (char c : leaves) sumLeaves += static_cast<unsigned char>(c);
    assert(t.sumLeaves() == sumLeaves);

    long long sumAll = 0;
    for (char c : keys) sumAll += static_cast<unsigned char>(c);
    double avg = static_cast<double>(sumAll) / 10.0;
    assert(std::abs(*t.averageAll() - avg) < 1e-9);
}

static void testRandomInvariants() {
    std::mt19937 rng(12345);
    for (int trial = 0; trial < 200; ++trial) {
        RBTree t;
        std::set<char> seen;
        std::uniform_int_distribution<int> dist(33, 126); // печатные ASCII
        const int ops = 30 + (trial % 50);
        for (int i = 0; i < ops; ++i) {
            char c = static_cast<char>(dist(rng));
            bool inserted = t.insert(c);
            bool expected = seen.insert(c).second;
            assert(inserted == expected);
            assert(t.validate());
        }
        assert(t.size() == seen.size());
        auto in = t.inorder();
        assert(in.size() == seen.size());
        for (std::size_t i = 1; i < in.size(); ++i) {
            assert(in[i - 1] < in[i]);
        }
        long long sumAll = 0;
        for (char c : seen) sumAll += static_cast<unsigned char>(c);
        if (!seen.empty()) {
            double avg = static_cast<double>(sumAll) / static_cast<double>(seen.size());
            assert(std::abs(*t.averageAll() - avg) < 1e-9);
        }
    }
}

int main() {
    testEmpty();
    testInsertAndDuplicates();
    testTraversalsAndDemo();
    testSumLeavesAndAverage();
    testDemoValues();
    testRandomInvariants();
    std::cout << "All rbtree tests passed\n";
    return 0;
}
