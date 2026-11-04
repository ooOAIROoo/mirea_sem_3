#include "text_search.hpp"

#include <chrono>
#include <cstdint>
#include <exception>
#include <iostream>
#include <limits>
#include <optional>
#include <random>
#include <stdexcept>
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

// Читает непустую строку, повторяя запрос при пустом вводе.
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

// ---------------------------------------------------------------------------
// Задача 1.
// ---------------------------------------------------------------------------

bool runLongestWordTask() {
    const auto sentence = readField("Введите предложение: ");
    if (!sentence) return false;

    const auto result = siaod::longestWordWithEqualEnds(*sentence);
    if (!result) {
        std::cout << "Подходящих слов нет: нет слова с одинаковыми первой "
                     "и последней буквами.\n";
        return true;
    }
    std::cout << "Самое длинное слово с одинаковыми первой и последней буквами: «"
              << result->word << "».\n"
              << "  Букв в слове: " << result->letterCount
              << ", позиция в предложении (с нуля): " << result->byteIndex << ".\n";
    return true;
}

// ---------------------------------------------------------------------------
// Задача 2.
// ---------------------------------------------------------------------------

void printSearchOutcome(std::string_view text, const char* what,
                        const siaod::SearchOutcome& outcome) {
    if (outcome.index < 0) {
        std::cout << what << ": вхождений нет (сравнений: " << outcome.comparisons << ").\n";
        return;
    }
    const std::size_t byteIndex = static_cast<std::size_t>(outcome.index);
    const std::size_t symbolIndex = siaod::utf8SymbolIndex(text, byteIndex);
    if (symbolIndex == byteIndex) {
        std::cout << what << ": " << symbolIndex
                  << " (сравнений: " << outcome.comparisons << ").\n";
    } else {
        // Для текстов с кириллицей байтовый и символьный номера различаются.
        std::cout << what << ": символ " << symbolIndex << " (байт " << byteIndex
                  << ", сравнений: " << outcome.comparisons << ").\n";
    }
}

bool runKmpTask() {
    const auto text = readField("Введите текст (одна строка): ");
    if (!text) return false;
    const auto pattern = readField("Введите образец: ");
    if (!pattern) return false;

    if (pattern->size() > text->size()) {
        std::cout << "Образец длиннее текста, вхождений нет.\n";
        return true;
    }

    const auto first = siaod::kmpFindFirst(*text, *pattern);
    const auto last = siaod::kmpFindLast(*text, *pattern);
    printSearchOutcome(*text, "Первое вхождение (КМП)", first);
    printSearchOutcome(*text, "Последнее вхождение (КМП)", last);
    return true;
}

// Наглядная демонстрация: префикс-функция и все вхождения.
bool runKmpDemo() {
    const auto text = readField("Введите текст: ");
    if (!text) return false;
    const auto pattern = readField("Введите образец: ");
    if (!pattern) return false;

    const auto pi = siaod::prefixFunction(*pattern);
    std::cout << "Префикс-функция образца (по байтам UTF-8):";
    for (const std::size_t value : pi) {
        std::cout << ' ' << value;
    }
    std::cout << '\n';

    const auto occurrences = siaod::kmpFindAll(*text, *pattern);
    if (occurrences.empty()) {
        std::cout << "Вхождений нет.\n";
    } else {
        std::cout << "Все вхождения (номера символов, с нуля):";
        for (const std::size_t byteIndex : occurrences) {
            std::cout << ' ' << siaod::utf8SymbolIndex(*text, byteIndex);
        }
        std::cout << '\n';
    }

    const auto kmp = siaod::kmpFindFirst(*text, *pattern);
    const auto naive = siaod::naiveFindFirst(*text, *pattern);
    printSearchOutcome(*text, "Первое вхождение (КМП)", kmp);
    printSearchOutcome(*text, "Первое вхождение (наивный алгоритм)", naive);
    return true;
}

// ---------------------------------------------------------------------------
// Демонстрация на готовых примерах (--demo): примеры совпадают с отчётом.
// ---------------------------------------------------------------------------

void runDemo() {
    struct SearchExample {
        const char* text;
        const char* pattern;
    };
    const SearchExample searchExamples[] = {
        {"aabaaab", "aaab"},   // Успешный поиск из отчёта.
        {"ababab", "abb"},     // Безуспешный поиск из отчёта.
        {"aaabaaa aaabaaa", "aaabaaa"}, // Пример из вопроса 9 для самоподготовки.
        {"шалаш на опушке, а лучше шалаш", "шалаш"}, // Кириллица, перекрытий нет.
        {"aaaaaa", "aa"},      // Перекрывающиеся вхождения.
    };

    std::cout << "Задача 2. КМП: индексы первого и последнего вхождений, число сравнений.\n";
    for (const SearchExample& example : searchExamples) {
        const auto first = siaod::kmpFindFirst(example.text, example.pattern);
        const auto last = siaod::kmpFindLast(example.text, example.pattern);
        const auto naive = siaod::naiveFindFirst(example.text, example.pattern);
        std::cout << "Текст \"" << example.text << "\", образец \"" << example.pattern
                  << "\":\n";
        printSearchOutcome(example.text, "  первое вхождение (КМП)", first);
        printSearchOutcome(example.text, "  последнее вхождение (КМП)", last);
        printSearchOutcome(example.text, "  первое вхождение (наивный)", naive);
    }

    std::cout << "\nЗадача 1. Самое длинное слово с одинаковыми первой и последней буквами.\n";
    const char* wordExamples[] = {
        "Ароза азора упала на лапу",   // Совпадение концов без учёта регистра.
        "Шалаш, дом и сад",            // Знаки препинания — разделители.
        "что-то новое",                // Подходящих слов нет.
    };
    for (const char* example : wordExamples) {
        const auto result = siaod::longestWordWithEqualEnds(example);
        if (result) {
            std::cout << "  \"" << example << "\" -> «" << result->word
                      << "» (букв: " << result->letterCount << ").\n";
        } else {
            std::cout << "  \"" << example << "\" -> подходящих слов нет.\n";
        }
    }
}

// ---------------------------------------------------------------------------
// Бенчмарк (--bench): практическая сложность наивного поиска и КМП.
// ---------------------------------------------------------------------------

class Timer {
public:
    void start() {
        begin_ = std::chrono::steady_clock::now();
    }
    double microseconds() const {
        const auto end = std::chrono::steady_clock::now();
        return static_cast<double>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin_).count()) / 1000.0;
    }

private:
    std::chrono::steady_clock::time_point begin_;
};

std::string makeRandomText(std::size_t length, std::uint64_t seed) {
    const std::string_view alphabet = "abcd";
    std::mt19937_64 generator(seed);
    std::uniform_int_distribution<std::size_t> pick(0, alphabet.size() - 1);
    std::string text;
    text.reserve(length);
    for (std::size_t i = 0; i < length; ++i) {
        text.push_back(alphabet[pick(generator)]);
    }
    return text;
}

void benchmarkSuccessful(std::size_t textLength, std::size_t patternLength,
                         std::string_view text, std::string_view pattern) {
    const auto kmp = siaod::kmpFindLast(text, pattern);
    const auto naive = siaod::naiveFindLast(text, pattern);

    Timer timer;
    constexpr int repeats = 50;
    timer.start();
    for (int i = 0; i < repeats; ++i) {
        static_cast<void>(siaod::kmpFindLast(text, pattern));
    }
    const double kmpUs = timer.microseconds() / repeats;

    timer.start();
    for (int i = 0; i < repeats; ++i) {
        static_cast<void>(siaod::naiveFindLast(text, pattern));
    }
    const double naiveUs = timer.microseconds() / repeats;

    std::cout << "  " << textLength << "\t" << patternLength
              << "\t" << naive.comparisons << "\t" << naiveUs
              << "\t" << kmp.comparisons << "\t" << kmpUs
              << "\t" << kmp.index << '\n';
}

void benchmarkUnsuccessful(std::size_t textLength, std::size_t patternLength,
                           std::string_view text) {
    // Буква 'e' отсутствует в алфавите текста, поэтому поиск гарантированно
    // безуспешный: текст просматривается целиком.
    std::string pattern = makeRandomText(patternLength - 1, 42);
    pattern.push_back('e');

    const auto kmp = siaod::kmpFindFirst(text, pattern);
    const auto naive = siaod::naiveFindFirst(text, pattern);
    if (kmp.index != -1 || naive.index != -1) {
        throw std::runtime_error("Бенчмарк: поиск должен быть безуспешным");
    }

    Timer timer;
    constexpr int repeats = 50;
    timer.start();
    for (int i = 0; i < repeats; ++i) {
        static_cast<void>(siaod::kmpFindFirst(text, pattern));
    }
    const double kmpUs = timer.microseconds() / repeats;

    timer.start();
    for (int i = 0; i < repeats; ++i) {
        static_cast<void>(siaod::naiveFindFirst(text, pattern));
    }
    const double naiveUs = timer.microseconds() / repeats;

    std::cout << "  " << textLength << "\t" << patternLength
              << "\t" << naive.comparisons << "\t" << naiveUs
              << "\t" << kmp.comparisons << "\t" << kmpUs << '\n';
}

void runBenchmark() {
    const std::size_t lengths[] = {1000, 10000, 100000};
    const std::size_t patternLengths[] = {3, 16, 64};

    std::cout << "Успешный поиск последнего вхождения (как в задании 2); случайный\n"
                 "текст над алфавитом {a, b, c, d}, образец взят из конца текста.\n"
              << "  n\tm\tнаив: сравнений\tнаив: мкс\tКМП: сравнений\tКМП: мкс\tиндекс\n";
    for (const std::size_t n : lengths) {
        const std::string text = makeRandomText(n, 2026);
        for (const std::size_t m : patternLengths) {
            // Образец занимает последние m символов текста, поэтому вхождение
            // точно существует, а поиск последнего вхождения просматривает текст.
            const std::string pattern = text.substr(n - m, m);
            benchmarkSuccessful(n, m, text, pattern);
        }
    }

    std::cout << "\nБезуспешный поиск (образец содержит букву 'e', которой нет в тексте).\n"
              << "  n\tm\tнаив: сравнений\tнаив: мкс\tКМП: сравнений\tКМП: мкс\n";
    for (const std::size_t n : lengths) {
        const std::string text = makeRandomText(n, 2026);
        for (const std::size_t m : patternLengths) {
            benchmarkUnsuccessful(n, m, text);
        }
    }

    std::cout << "\nХудший случай для наивного поиска: текст из n букв 'a' и буквы 'b',\n"
                 "образец — (m-1) букв 'a' и буква 'b'; вхождение только в конце.\n"
              << "  n\tm\tнаив: сравнений\tнаив: мкс\tКМП: сравнений\tКМП: мкс\n";
    for (const std::size_t n : lengths) {
        for (const std::size_t m : patternLengths) {
            if (m < 2 || m > n) {
                continue;
            }
            std::string text(n, 'a');
            text[n - 1] = 'b';
            std::string pattern(m - 1, 'a');
            pattern.push_back('b');
            benchmarkSuccessful(n, m, text, pattern);
        }
    }
}

void printHelp() {
    std::cout << "Использование: text_search_app [--demo | --bench | --help]\n"
              << "  без параметров  — меню с задачами варианта 3\n"
              << "  --demo          — прогон готовых примеров из отчёта\n"
              << "  --bench         — таблицы практической сложности поиска\n"
              << "  --help          — эта справка\n";
}

void runMenu() {
    while (true) {
        std::cout << "\nПрактическая работа 6.2, вариант 3\n"
                  << "1. Самое длинное слово, первая и последняя буквы которого одинаковы\n"
                  << "2. КМП: индекс последнего вхождения образца в текст\n"
                  << "3. Демонстрация КМП: префикс-функция и все вхождения\n"
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
                if (!runLongestWordTask()) return;
            } else if (choice == "2") {
                if (!runKmpTask()) return;
            } else if (choice == "3") {
                if (!runKmpDemo()) return;
            } else {
                std::cout << "Неизвестный пункт меню. Выберите 0–3.\n";
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
    if (argc == 2) {
        const std::string_view arg(argv[1]);
        if (arg == "--help" || arg == "-h") {
            printHelp();
            return 0;
        }
        try {
            if (arg == "--demo") {
                runDemo();
                return 0;
            }
            if (arg == "--bench") {
                runBenchmark();
                return 0;
            }
        } catch (const std::exception& error) {
            std::cerr << "Ошибка: " << error.what() << '\n';
            return 1;
        }
        std::cerr << "Неизвестный параметр: " << arg << '\n';
        printHelp();
        return 2;
    }

    runMenu();
    return 0;
}
