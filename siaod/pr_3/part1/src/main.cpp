#include "rbtree.hpp"

#include <exception>
#include <iostream>
#include <limits>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
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
            if (!ReadConsoleW(input, buffer, 256, &count, nullptr) || count == 0) {
                return false;
            }
            pending.append(buffer, count);
            end = pending.find(L'\n');
        }
        std::wstring line = pending.substr(0, end);
        pending.erase(0, end + 1);
        if (!line.empty() && line.back() == L'\r') line.pop_back();
        if (line.size() == 1 && line.front() == L'\x1a') return false;
        if (line.empty()) {
            result.clear();
            return true;
        }
        if (line.size() > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
            throw std::length_error("Строка ввода слишком длинная");
        }
        const int length = static_cast<int>(line.size());
        const int bytes = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, line.data(), length,
                                              nullptr, 0, nullptr, nullptr);
        if (bytes == 0) {
            throw std::runtime_error("Не удалось преобразовать ввод консоли в UTF-8");
        }
        result.resize(static_cast<std::size_t>(bytes));
        if (WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, line.data(), length,
                                result.data(), bytes, nullptr, nullptr) == 0) {
            throw std::runtime_error("Не удалось преобразовать ввод консоли в UTF-8");
        }
        return true;
    }
#endif
    return static_cast<bool>(std::getline(std::cin, result));
}

std::string trim(std::string text) {
    const auto first = text.find_first_not_of(" \t\n\r\v\f");
    if (first == std::string::npos) return {};
    const auto last = text.find_last_not_of(" \t\n\r\v\f");
    return text.substr(first, last - first + 1);
}

std::optional<std::string> readNonEmpty(const char* prompt) {
    while (true) {
        std::cout << prompt << std::flush;
        std::string value;
        if (!readLine(value)) return std::nullopt;
        value = trim(std::move(value));
        if (!value.empty()) return value;
        std::cout << "Пустой ввод. Повторите.\n";
    }
}

std::optional<char> readSymbol(const char* prompt) {
    while (true) {
        auto line = readNonEmpty(prompt);
        if (!line) return std::nullopt;
        if (line->size() == 1) {
            return (*line)[0];
        }
        std::cout << "Введите ровно один символ (латинская буква, цифра или знак).\n";
    }
}

void printSequence(const char* title, const std::vector<char>& seq) {
    std::cout << title;
    if (seq.empty()) {
        std::cout << "(пусто)\n";
        return;
    }
    for (std::size_t i = 0; i < seq.size(); ++i) {
        if (i) std::cout << ' ';
        std::cout << seq[i];
    }
    std::cout << '\n';
}

void showSumLeaves(const siaod::RBTree& tree) {
    const auto leafList = tree.leaves();
    if (leafList.empty()) {
        std::cout << "Дерево пустое, листьев нет. Сумма = 0.\n";
        return;
    }
    std::cout << "Листья: ";
    long long sum = 0;
    for (std::size_t i = 0; i < leafList.size(); ++i) {
        if (i) std::cout << ", ";
        const auto code = static_cast<unsigned char>(leafList[i]);
        sum += code;
        std::cout << "'" << leafList[i] << "'(" << static_cast<int>(code) << ")";
    }
    std::cout << "\nСумма кодов листьев: " << sum << "\n";
}

void showAverage(const siaod::RBTree& tree) {
    const auto avg = tree.averageAll();
    if (!avg) {
        std::cout << "Дерево пустое, среднее арифметическое не определено.\n";
        return;
    }
    std::cout << "Среднее арифметическое кодов всех узлов: " << *avg << "\n";
    std::cout << "Количество узлов: " << tree.size() << "\n";
}

void insertMany(siaod::RBTree& tree) {
    auto line = readNonEmpty("Введите символы через пробел или слитно: ");
    if (!line) return;
    std::size_t inserted = 0;
    std::size_t duplicates = 0;
    for (char c : *line) {
        if (c == ' ' || c == '\t') continue;
        if (tree.insert(c)) ++inserted;
        else ++duplicates;
    }
    std::cout << "Добавлено: " << inserted << ", дубликатов пропущено: " << duplicates << "\n";
}

void printHelp() {
    std::cout
        << "Использование: pr3_part1 [--demo | --empty | --help]\n"
        << "  без параметров  — запустить меню с пустым деревом\n"
        << "  --demo          — загрузить 10 демонстрационных символов\n"
        << "  --empty         — пустое дерево\n"
        << "  --help          — справка\n";
}

void runMenu(siaod::RBTree& tree) {
    while (true) {
        std::cout << "\n--- Красно-чёрное дерево (символы) ---\n"
                  << "Узлов: " << tree.size() << "\n"
                  << "1. Вставить элемент\n"
                  << "2. Вставить несколько элементов\n"
                  << "3. Прямой обход (preorder)\n"
                  << "4. Симметричный обход (inorder)\n"
                  << "5. Найти сумму значений листьев\n"
                  << "6. Найти среднее арифметическое всех узлов\n"
                  << "7. Показать дерево\n"
                  << "8. Проверить свойства КЧ-дерева\n"
                  << "0. Выход\n"
                  << "Выбор: " << std::flush;
        std::string choice;
        if (!readLine(choice)) return;
        choice = trim(std::move(choice));
        try {
            if (choice == "0") {
                return;
            } else if (choice == "1") {
                auto sym = readSymbol("Символ: ");
                if (!sym) return;
                if (tree.insert(*sym)) {
                    std::cout << "Символ '" << *sym << "' вставлен.\n";
                } else {
                    std::cout << "Символ '" << *sym << "' уже есть в дереве (дубликат).\n";
                }
            } else if (choice == "2") {
                insertMany(tree);
            } else if (choice == "3") {
                printSequence("Прямой обход: ", tree.preorder());
            } else if (choice == "4") {
                printSequence("Симметричный обход: ", tree.inorder());
            } else if (choice == "5") {
                showSumLeaves(tree);
                // Дополнительно покажем, какие узлы являются листьями.
                // Для простоты - соберём листья через обход с доступом к структуре нельзя
                // через публичный API; выводим только сумму. Детали в отчёте.
            } else if (choice == "6") {
                showAverage(tree);
            } else if (choice == "7") {
                tree.printTree(std::cout);
            } else if (choice == "8") {
                std::string err;
                // validate принимает ostream*; используем stringstream-подобный подход через cerr? проще:
                if (tree.validate(&std::cerr)) {
                    std::cout << "Свойства КЧ-дерева выполнены.\n";
                } else {
                    std::cout << "Нарушение свойств КЧ-дерева (см. сообщения выше).\n";
                }
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
    if (argc > 2) {
        printHelp();
        return 2;
    }
    if (argc == 2) {
        std::string_view arg(argv[1]);
        if (arg == "--help" || arg == "-h") {
            printHelp();
            return 0;
        } else if (arg == "--demo") {
            demo = true;
        } else if (arg == "--empty") {
            demo = false;
        } else {
            std::cerr << "Неизвестный параметр: " << arg << "\n";
            printHelp();
            return 2;
        }
    }

    try {
        siaod::RBTree tree;
        if (demo) {
            for (char c : siaod::demoKeys()) {
                tree.insert(c);
            }
            std::cout << "Загружено 10 демонстрационных символов: ";
            for (std::size_t i = 0; i < siaod::demoKeys().size(); ++i) {
                if (i) std::cout << ' ';
                std::cout << siaod::demoKeys()[i];
            }
            std::cout << "\n";
        } else {
            std::cout << "Красно-чёрное дерево (вариант 3, тип ключа: символ).\n";
            std::cout << "Используйте меню для вставки и операций.\n";
        }
        runMenu(tree);
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Фатальная ошибка: " << e.what() << "\n";
        return 1;
    }
}
