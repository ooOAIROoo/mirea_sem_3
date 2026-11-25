#include "graph.hpp"

#include <exception>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace {

void configureConsole() {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
}

bool readLine(std::string& result) {
#ifdef _WIN32
    const HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    if (input != nullptr && input != INVALID_HANDLE_VALUE && GetConsoleMode(input, &mode)) {
        static std::wstring pending;
        std::size_t end = pending.find(L'\n');
        while (end == std::wstring::npos) {
            wchar_t buffer[256];
            DWORD count = 0;
            if (!ReadConsoleW(input, buffer, 256, &count, nullptr) || count == 0) return false;
            pending.append(buffer, count);
            end = pending.find(L'\n');
        }
        std::wstring line = pending.substr(0, end);
        pending.erase(0, end + 1);
        if (!line.empty() && line.back() == L'\r') line.pop_back();
        if (line.size() == 1 && line.front() == L'\x1a') return false;
        if (line.empty()) { result.clear(); return true; }
        const int length = static_cast<int>(line.size());
        const int bytes = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, line.data(), length,
                                              nullptr, 0, nullptr, nullptr);
        if (bytes == 0) throw std::runtime_error("Ошибка преобразования ввода в UTF-8");
        result.resize(static_cast<std::size_t>(bytes));
        WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, line.data(), length,
                            result.data(), bytes, nullptr, nullptr);
        return true;
    }
#endif
    return static_cast<bool>(std::getline(std::cin, result));
}

std::string trim(std::string s) {
    const auto a = s.find_first_not_of(" \t\r\n\v\f");
    if (a == std::string::npos) return {};
    const auto b = s.find_last_not_of(" \t\r\n\v\f");
    return s.substr(a, b - a + 1);
}

std::optional<std::string> readNonEmpty(const char* prompt) {
    while (true) {
        std::cout << prompt << std::flush;
        std::string s;
        if (!readLine(s)) return std::nullopt;
        s = trim(std::move(s));
        if (!s.empty()) return s;
        std::cout << "Пустой ввод. Повторите.\n";
    }
}

std::optional<int> readInt(const char* prompt) {
    while (true) {
        auto line = readNonEmpty(prompt);
        if (!line) return std::nullopt;
        try {
            std::size_t pos = 0;
            int v = std::stoi(*line, &pos);
            if (pos == line->size()) return v;
        } catch (...) {}
        std::cout << "Введите целое число.\n";
    }
}

void inputGraphFromKeyboard(siaod::Graph& g) {
    auto nOpt = readInt("Количество вершин: ");
    if (!nOpt) return;
    int n = *nOpt;
    if (n <= 0 || n > 100) {
        std::cout << "Некорректное число вершин (допустимо 1..100).\n";
        return;
    }
    g.reset(n);
    std::cout << "Вершины нумеруются 1.." << n << ".\n";
    std::cout << "Вводите рёбра в формате: from to weight (по одному в строке).\n";
    std::cout << "Для завершения ввода введите пустую строку или 0.\n";
    while (true) {
        std::cout << "Ребро: " << std::flush;
        std::string line;
        if (!readLine(line)) return;
        line = trim(line);
        if (line.empty() || line == "0") break;
        std::istringstream iss(line);
        int from, to, w;
        if (!(iss >> from >> to >> w)) {
            std::cout << "Формат: from to weight\n";
            continue;
        }
        if (!g.addEdge(from, to, w)) {
            std::cout << "Ребро не добавлено (проверьте номера вершин, вес >= 0, отсутствие дубликата).\n";
        } else {
            std::cout << "Добавлено: " << from << " -> " << to << " (w=" << w << ")\n";
        }
    }
}

void runSearch(const siaod::Graph& g) {
    if (g.vertexCount() == 0) {
        std::cout << "Граф пуст. Сначала создайте граф.\n";
        return;
    }
    auto sOpt = readInt("Вершина-источник: ");
    if (!sOpt) return;
    auto tOpt = readInt("Вершина-цель: ");
    if (!tOpt) return;
    int s = *sOpt, t = *tOpt;
    if (s < 1 || s > g.vertexCount() || t < 1 || t > g.vertexCount()) {
        std::cout << "Номера вершин должны быть в диапазоне 1.." << g.vertexCount() << ".\n";
        return;
    }

    siaod::DecisionTreeShortestPath solver;
    auto result = solver.findShortestPath(g, s, t);

    std::cout << "\n=== Результат ===\n";
    if (!result.found) {
        std::cout << "Путь из " << s << " в " << t << " не найден.\n";
    } else {
        std::cout << "Кратчайший путь: ";
        for (std::size_t i = 0; i < result.vertices.size(); ++i) {
            if (i) std::cout << " -> ";
            std::cout << result.vertices[i];
        }
        std::cout << "\nСуммарный вес: " << result.totalWeight << "\n";
    }

    std::cout << "\n=== Статистика дерева решений ===\n";
    const auto& st = solver.stats();
    std::cout << "Сгенерировано узлов: " << st.generatedNodes << "\n";
    std::cout << "Раскрыто узлов:      " << st.expandedNodes << "\n";
    std::cout << "Отсечено узлов:      " << st.prunedNodes << "\n";
    std::cout << "Достижений цели:     " << st.goalNodes << "\n";
    std::cout << "Макс. глубина:       " << st.maxDepth << "\n";

    std::cout << "\nПоказать дерево решений? (y/n): " << std::flush;
    std::string ans;
    if (readLine(ans)) {
        ans = trim(ans);
        if (ans == "y" || ans == "Y" || ans == "д" || ans == "Д") {
            solver.printTree(std::cout, g);
        }
    }
}

void printHelp() {
    std::cout
        << "Использование: pr3_part2 [--demo | --help]\n"
        << "  без параметров  — меню, граф не загружен\n"
        << "  --demo          — загрузить предложенный граф варианта 3\n"
        << "  --help          — справка\n";
}

void runMenu(siaod::Graph& g, bool loaded) {
    while (true) {
        std::cout << "\n--- Графы: кратчайший путь (дерево решений) ---\n";
        if (loaded) {
            std::cout << "Граф: " << g.vertexCount() << " вершин, " << g.edges().size() << " рёбер\n";
        } else {
            std::cout << "Граф не загружен.\n";
        }
        std::cout
            << "1. Загрузить предложенный граф (вариант 3)\n"
            << "2. Ввести граф с клавиатуры\n"
            << "3. Показать граф\n"
            << "4. Найти кратчайший путь (дерево решений)\n"
            << "0. Выход\n"
            << "Выбор: " << std::flush;
        std::string choice;
        if (!readLine(choice)) return;
        choice = trim(choice);
        try {
            if (choice == "0") return;
            else if (choice == "1") {
                g = siaod::makeVariant3Graph();
                loaded = true;
                std::cout << "Загружен предложенный граф варианта 3.\n";
                g.printAdjacency(std::cout);
            } else if (choice == "2") {
                inputGraphFromKeyboard(g);
                loaded = g.vertexCount() > 0;
            } else if (choice == "3") {
                if (!loaded) { std::cout << "Граф не загружен.\n"; continue; }
                g.printAdjacency(std::cout);
                g.printEdges(std::cout);
            } else if (choice == "4") {
                if (!loaded) { std::cout << "Граф не загружен.\n"; continue; }
                runSearch(g);
            } else {
                std::cout << "Неизвестный пункт меню.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << "\n";
        }
    }
}

} // namespace

int main(int argc, char* argv[]) {
    configureConsole();
    bool demo = false;
    if (argc > 2) { printHelp(); return 2; }
    if (argc == 2) {
        std::string_view arg(argv[1]);
        if (arg == "--help" || arg == "-h") { printHelp(); return 0; }
        if (arg == "--demo") demo = true;
        else { std::cerr << "Неизвестный параметр: " << arg << "\n"; printHelp(); return 2; }
    }
    try {
        siaod::Graph g;
        bool loaded = false;
        if (demo) {
            g = siaod::makeVariant3Graph();
            loaded = true;
            std::cout << "Загружен предложенный граф варианта 3.\n";
            g.printAdjacency(std::cout);
        }
        runMenu(g, loaded);
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Фатальная ошибка: " << e.what() << "\n";
        return 1;
    }
}
