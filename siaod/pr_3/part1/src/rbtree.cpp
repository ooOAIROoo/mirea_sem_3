#include "rbtree.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace siaod {

RBTree::RBTree() {
    nil_ = new Node{};
    nil_->color = Color::Black;
    nil_->left = nil_;
    nil_->right = nil_;
    nil_->parent = nil_;
    root_ = nil_;
}

RBTree::~RBTree() {
    if (nil_ != nullptr) {
        destroy(root_);
        // nil_ указывает сам на себя, это нормально для удаления.
        nil_->left = nullptr;
        nil_->right = nullptr;
        nil_->parent = nullptr;
        delete nil_;
        nil_ = nullptr;
        root_ = nullptr;
    }
}

RBTree::RBTree(RBTree&& other) noexcept
    : root_(other.root_), nil_(other.nil_), size_(other.size_) {
    other.root_ = nullptr;
    other.nil_ = nullptr;
    other.size_ = 0;
}

RBTree& RBTree::operator=(RBTree&& other) noexcept {
    if (this == &other) return *this;
    if (nil_ != nullptr) {
        destroy(root_);
        nil_->left = nullptr;
        nil_->right = nullptr;
        nil_->parent = nullptr;
        delete nil_;
    }
    root_ = other.root_;
    nil_ = other.nil_;
    size_ = other.size_;
    other.root_ = nullptr;
    other.nil_ = nullptr;
    other.size_ = 0;
    return *this;
}

void RBTree::destroy(Node* node) {
    if (node == nil_) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

void RBTree::leftRotate(Node* x) {
    Node* y = x->right;
    if (y == nil_) {
        throw std::logic_error("leftRotate: правый потомок nil");
    }
    x->right = y->left;
    if (y->left != nil_) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nil_) {
        root_ = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void RBTree::rightRotate(Node* y) {
    Node* x = y->left;
    if (x == nil_) {
        throw std::logic_error("rightRotate: левый потомок nil");
    }
    y->left = x->right;
    if (x->right != nil_) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == nil_) {
        root_ = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

bool RBTree::insert(char key) {
    // Обычная вставка BST.
    Node* parent = nil_;
    Node* cur = root_;
    while (cur != nil_) {
        parent = cur;
        if (key == cur->key) {
            return false; // дубликат
        }
        if (key < cur->key) {
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }

    Node* z = new Node{};
    z->key = key;
    z->color = Color::Red;
    z->left = nil_;
    z->right = nil_;
    z->parent = parent;

    if (parent == nil_) {
        root_ = z;
    } else if (key < parent->key) {
        parent->left = z;
    } else {
        parent->right = z;
    }

    ++size_;
    insertFixup(z);
    return true;
}

void RBTree::insertFixup(Node* z) {
    while (z->parent->color == Color::Red) {
        if (z->parent == z->parent->parent->left) {
            Node* uncle = z->parent->parent->right;
            if (uncle->color == Color::Red) {
                // Случай 1: дядя красный - перекрашивание.
                z->parent->color = Color::Black;
                uncle->color = Color::Black;
                z->parent->parent->color = Color::Red;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    // Случай 2: z - правый ребёнок -> левый поворот.
                    z = z->parent;
                    leftRotate(z);
                }
                // Случай 3: z - левый ребёнок -> правый поворот.
                z->parent->color = Color::Black;
                z->parent->parent->color = Color::Red;
                rightRotate(z->parent->parent);
            }
        } else {
            // Симметричный случай: отец - правый ребёнок.
            Node* uncle = z->parent->parent->left;
            if (uncle->color == Color::Red) {
                z->parent->color = Color::Black;
                uncle->color = Color::Black;
                z->parent->parent->color = Color::Red;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = Color::Black;
                z->parent->parent->color = Color::Red;
                leftRotate(z->parent->parent);
            }
        }
    }
    root_->color = Color::Black;
}

std::optional<SearchResult> RBTree::find(char key) const {
    Node* cur = root_;
    std::size_t depth = 0;
    while (cur != nil_) {
        if (key == cur->key) {
            return SearchResult{cur, depth};
        }
        cur = (key < cur->key) ? cur->left : cur->right;
        ++depth;
    }
    return std::nullopt;
}

std::vector<char> RBTree::preorder() const {
    std::vector<char> out;
    out.reserve(size_);
    preorderRec(root_, out);
    return out;
}

void RBTree::preorderRec(Node* node, std::vector<char>& out) const {
    if (node == nil_) return;
    out.push_back(node->key);
    preorderRec(node->left, out);
    preorderRec(node->right, out);
}

std::vector<char> RBTree::inorder() const {
    std::vector<char> out;
    out.reserve(size_);
    inorderRec(root_, out);
    return out;
}

void RBTree::inorderRec(Node* node, std::vector<char>& out) const {
    if (node == nil_) return;
    inorderRec(node->left, out);
    out.push_back(node->key);
    inorderRec(node->right, out);
}

long long RBTree::sumLeaves() const {
    long long sum = 0;
    std::size_t count = 0;
    sumLeavesRec(root_, sum, count);
    return sum;
}

void RBTree::sumLeavesRec(Node* node, long long& sum, std::size_t& count) const {
    if (node == nil_) return;
    const bool isLeaf = (node->left == nil_ && node->right == nil_);
    if (isLeaf) {
        sum += static_cast<unsigned char>(node->key);
        ++count;
        return;
    }
    sumLeavesRec(node->left, sum, count);
    sumLeavesRec(node->right, sum, count);
}

std::vector<char> RBTree::leaves() const {
    std::vector<char> out;
    leavesRec(root_, out);
    return out;
}

void RBTree::leavesRec(Node* node, std::vector<char>& out) const {
    if (node == nil_) return;
    const bool isLeaf = (node->left == nil_ && node->right == nil_);
    if (isLeaf) {
        out.push_back(node->key);
        return;
    }
    leavesRec(node->left, out);
    leavesRec(node->right, out);
}

std::optional<double> RBTree::averageAll() const {
    if (size_ == 0) return std::nullopt;
    long long sum = 0;
    std::size_t count = 0;
    averageRec(root_, sum, count);
    if (count == 0) return std::nullopt;
    return static_cast<double>(sum) / static_cast<double>(count);
}

void RBTree::averageRec(Node* node, long long& sum, std::size_t& count) const {
    if (node == nil_) return;
    sum += static_cast<unsigned char>(node->key);
    ++count;
    averageRec(node->left, sum, count);
    averageRec(node->right, sum, count);
}

std::size_t RBTree::size() const noexcept {
    return size_;
}

bool RBTree::empty() const noexcept {
    return size_ == 0;
}

std::size_t RBTree::height() const noexcept {
    if (root_ == nil_) return 0;
    // Высота в рёбрах: лист имеет высоту 0.
    const std::size_t h = heightRec(root_);
    return h == 0 ? 0 : h - 1;
}

std::size_t RBTree::heightRec(Node* node) const {
    if (node == nil_) return 0;
    const std::size_t lh = heightRec(node->left);
    const std::size_t rh = heightRec(node->right);
    return 1 + std::max(lh, rh);
}

void RBTree::printTree(std::ostream& out) const {
    if (root_ == nil_) {
        out << "(дерево пустое)\n";
        return;
    }
    printRec(root_, out, "", false, true);
}

void RBTree::printRec(Node* node, std::ostream& out, const std::string& prefix,
                      bool isLeft, bool isRoot) const {
    if (node == nil_) return;
    // Сначала правое поддерево, чтобы дерево "лежало" на боку.
    if (node->right != nil_) {
        printRec(node->right, out, prefix + (isRoot ? "" : (isLeft ? "|   " : "    ")),
                 false, false);
    }
    out << prefix;
    if (!isRoot) {
        out << (isLeft ? "L-- " : "R-- ");
    }
    out << node->key << " (" << (node->color == Color::Red ? "R" : "B") << ")\n";
    if (node->left != nil_) {
        printRec(node->left, out, prefix + (isRoot ? "" : (isLeft ? "|   " : "    ")),
                 true, false);
    }
}

bool RBTree::validate(std::ostream* err) const {
    if (nil_ == nullptr) {
        if (err) *err << "nil не инициализирован\n";
        return false;
    }
    if (nil_->color != Color::Black) {
        if (err) *err << "nil должен быть чёрным\n";
        return false;
    }
    if (root_ == nil_) return true;
    if (root_->color != Color::Black) {
        if (err) *err << "корень должен быть чёрным\n";
        return false;
    }
    return validateRec(root_, err) >= 0;
}

int RBTree::validateRec(Node* node, std::ostream* err) const {
    if (node == nil_) return 1; // чёрная высота nil-листа
    if (node->color == Color::Red) {
        if (node->left->color == Color::Red || node->right->color == Color::Red) {
            if (err) *err << "красный узел " << node->key << " имеет красного потомка\n";
            return -1;
        }
    }
    if (node->left != nil_) {
        if (node->left->parent != node) {
            if (err) *err << "нарушена связь parent у левого потомка " << node->key << "\n";
            return -1;
        }
        if (node->left->key >= node->key) {
            if (err) *err << "нарушено BST-свойство слева от " << node->key << "\n";
            return -1;
        }
    }
    if (node->right != nil_) {
        if (node->right->parent != node) {
            if (err) *err << "нарушена связь parent у правого потомка " << node->key << "\n";
            return -1;
        }
        if (node->right->key <= node->key) {
            if (err) *err << "нарушено BST-свойство справа от " << node->key << "\n";
            return -1;
        }
    }
    const int lh = validateRec(node->left, err);
    if (lh < 0) return -1;
    const int rh = validateRec(node->right, err);
    if (rh < 0) return -1;
    if (lh != rh) {
        if (err) *err << "разная чёрная высота у узла " << node->key << "\n";
        return -1;
    }
    return lh + (node->color == Color::Black ? 1 : 0);
}

std::vector<char> demoKeys() {
    // 10 символов, дающих несколько перекрашиваний и поворотов.
    return {'M', 'F', 'S', 'B', 'H', 'R', 'W', 'A', 'D', 'K'};
}

} // namespace siaod
