#pragma once

#include <cstddef>
#include <iosfwd>
#include <optional>
#include <string>
#include <vector>

namespace siaod {

enum class Color : unsigned char { Red, Black };

struct Node {
    char key = '\0';
    Color color = Color::Red;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
};

struct SearchResult {
    const Node* node = nullptr;
    std::size_t depth = 0; // количество рёбер от корня
};

class RBTree {
public:
    RBTree();
    ~RBTree();

    RBTree(const RBTree&) = delete;
    RBTree& operator=(const RBTree&) = delete;
    RBTree(RBTree&& other) noexcept;
    RBTree& operator=(RBTree&& other) noexcept;

    // Вставка символа. Возвращает true, если элемент добавлен; false, если дубликат.
    bool insert(char key);

    std::optional<SearchResult> find(char key) const;

    // Обходы. Возвращают ключи в порядке посещения.
    std::vector<char> preorder() const;
    std::vector<char> inorder() const; // симметричный обход

    // Сумма кодов листьев. Пустое дерево -> 0.
    long long sumLeaves() const;

    // Список листьев (для вывода/проверки).
    std::vector<char> leaves() const;

    // Среднее арифметическое кодов всех узлов. Пустое дерево -> nullopt.
    std::optional<double> averageAll() const;

    std::size_t size() const noexcept;
    std::size_t height() const noexcept; // высота в рёбрах, пустое дерево = 0, один узел = 0

    bool empty() const noexcept;

    void printTree(std::ostream& out) const;

    // Для тестов/отчёта: проверка инвариантов КЧ-дерева.
    bool validate(std::ostream* err = nullptr) const;

private:
    Node* root_ = nullptr;
    Node* nil_ = nullptr; // общий sentinel-лист
    std::size_t size_ = 0;

    void leftRotate(Node* x);
    void rightRotate(Node* y);
    void insertFixup(Node* z);

    void preorderRec(Node* node, std::vector<char>& out) const;
    void inorderRec(Node* node, std::vector<char>& out) const;
    void sumLeavesRec(Node* node, long long& sum, std::size_t& count) const;
    void leavesRec(Node* node, std::vector<char>& out) const;
    void averageRec(Node* node, long long& sum, std::size_t& count) const;
    std::size_t heightRec(Node* node) const;
    void printRec(Node* node, std::ostream& out, const std::string& prefix,
                  bool isLeft, bool isRoot) const;
    void destroy(Node* node);
    int validateRec(Node* node, std::ostream* err) const;
};

// 10 демонстрационных символов для быстрого заполнения.
std::vector<char> demoKeys();

} // namespace siaod
