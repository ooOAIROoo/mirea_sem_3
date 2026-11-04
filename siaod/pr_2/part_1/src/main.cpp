#include "student_hash_table.hpp"

#include <exception>
#include <iostream>
#include <limits>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

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
    // Вывод в современных Windows Terminal / консоли Windows выполняется в UTF-8.
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
}

bool readLine(std::string& result) {
#ifdef _WIN32
    const HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    if (input != nullptr && input != INVALID_HANDLE_VALUE && GetConsoleMode(input, &mode)) {
        // Чтение UTF-16 из настоящей консоли не зависит от кодовой страницы CRT.
        // При перенаправлении stdin ниже используется обычный getline с байтами UTF-8.
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
        if (line.size() == 1 && line.front() == L'\x1a') return false; // Ctrl+Z
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
    if (first == std::string::npos) {
        return {};
    }
    const auto last = text.find_last_not_of(" \t\n\r\v\f");
    return text.substr(first, last - first + 1);
}

std::optional<std::string> readField(const char* prompt) {
    while (true) {
        std::cout << prompt << std::flush;
        std::string value;
        if (!readLine(value)) {
            return std::nullopt;
        }
        value = trim(std::move(value));
        if (!value.empty()) {
            return value;
        }
        std::cout << "Поле не может быть пустым. Повторите ввод.\n";
    }
}

void printStudent(const siaod::SearchResult& result) {
    const siaod::Student& student = *result.student;
    std::cout << "Найден студент (ячейка " << result.tableIndex
              << ", индекс записи " << result.recordIndex << "):\n"
              << "  Номер зачётной книжки: " << student.recordBook << '\n'
              << "  Номер группы: " << student.group << '\n'
              << "  ФИО: " << student.fullName << '\n';
}

bool insertStudent(siaod::StudentHashTable& table) {
    const auto key = readField("Номер зачётной книжки: ");
    if (!key) return false;
    const auto group = readField("Номер группы: ");
    if (!group) return false;
    const auto name = readField("ФИО: ");
    if (!name) return false;

    const auto result = table.insert({*key, *group, *name});
    if (!result.inserted) {
        std::cout << "Уже существует студент с номером " << *key
                  << ". Данные не изменены.\n";
        return true;
    }
    if (result.rehash == siaod::RehashKind::grow) {
        std::cout << "Расширение и рехеширование: " << result.oldCapacity
                  << " -> " << result.newCapacity << " ячеек.\n";
    } else if (result.rehash == siaod::RehashKind::compact) {
        std::cout << "Рехеширование без расширения: метки удаления очищены, "
                  << "размер таблицы " << result.newCapacity << ".\n";
    }
    std::cout << "Студент добавлен: ячейка " << result.tableIndex
              << ", индекс записи " << result.recordIndex << ".\n";
    return true;
}

bool findStudent(const siaod::StudentHashTable& table) {
    const auto key = readField("Номер зачётной книжки для поиска: ");
    if (!key) return false;
    if (const auto result = table.find(*key)) {
        printStudent(*result);
    } else {
        std::cout << "Студент с номером " << *key << " не найден.\n";
    }
    return true;
}

bool eraseStudent(siaod::StudentHashTable& table) {
    const auto key = readField("Номер зачётной книжки для удаления: ");
    if (!key) return false;
    if (table.erase(*key)) {
        std::cout << "Запись удалена. Ячейка помечена как удалённая.\n";
    } else {
        std::cout << "Студент с номером " << *key << " не найден.\n";
    }
    return true;
}

void printHelp() {
    std::cout << "Использование: pr_2_app [--empty | --help]\n"
              << "  без параметров  — меню с шестью демонстрационными записями\n"
              << "  --empty         — меню с пустой таблицей\n"
              << "  --help          — эта справка\n"
              << "Данные находятся только в памяти и не сохраняются после выхода.\n";
}

void runMenu(siaod::StudentHashTable& table) {
    while (true) {
        std::cout << "\nСтуденты: " << table.size() << " | ячеек: " << table.capacity()
                  << " | меток удаления: " << table.deletedCount() << '\n'
                  << "1. Добавить студента\n"
                  << "2. Найти студента по номеру зачётной книжки\n"
                  << "3. Удалить студента\n"
                  << "4. Показать массив записей\n"
                  << "5. Показать хеш-таблицу\n"
                  << "0. Выход\nВыбор: " << std::flush;
        std::string choice;
        if (!readLine(choice)) {
            return;
        }
        choice = trim(std::move(choice));
        try {
            if (choice == "0") {
                return;
            } else if (choice == "1") {
                if (!insertStudent(table)) return;
            } else if (choice == "2") {
                if (!findStudent(table)) return;
            } else if (choice == "3") {
                if (!eraseStudent(table)) return;
            } else if (choice == "4") {
                table.printRecords(std::cout);
            } else if (choice == "5") {
                table.printTable(std::cout);
            } else {
                std::cout << "Неизвестный пункт меню. Выберите 0–5.\n";
            }
        } catch (const std::exception& error) {
            std::cerr << "Ошибка: " << error.what() << '\n';
        }
    }
}

} // namespace

int main(int argc, char* argv[]) {
    configureConsole();
    if (argc > 2) {
        printHelp();
        return 2;
    }
    bool startEmpty = false;
    if (argc == 2) {
        const std::string_view arg(argv[1]);
        if (arg == "--help" || arg == "-h") {
            printHelp();
            return 0;
        }
        if (arg != "--empty") {
            std::cerr << "Неизвестный параметр: " << arg << '\n';
            printHelp();
            return 2;
        }
        startEmpty = true;
    }

    try {
        siaod::StudentHashTable table;
        if (startEmpty) {
            std::cout << "Запуск с пустой таблицей.\n";
        } else {
            for (const auto& student : siaod::demoStudents()) {
                table.insert(student);
            }
            std::cout << "Автоматически добавлено 6 демонстрационных записей.\n"
                      << "Ключи 1001, 1013 и 1098 имеют одинаковый h1 при 11 ячейках.\n";
        }
        runMenu(table);
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Ошибка: " << error.what() << '\n';
        return 1;
    }
}
