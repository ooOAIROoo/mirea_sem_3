#include "text_search.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace {

using siaod::SearchOutcome;

void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void requireInvalidArgument(std::string_view text, std::string_view pattern) {
    bool thrown = false;
    try {
        static_cast<void>(siaod::kmpFindFirst(text, pattern));
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    require(thrown, "Ожидалось std::invalid_argument для пустого образца");
}

void testPrefixFunction() {
    require(siaod::prefixFunction("").empty(), "Префикс-функция пустой строки не пуста");
    require(siaod::prefixFunction("a") == std::vector<std::size_t>{0},
            "Неверная префикс-функция строки a");
    require(siaod::prefixFunction("aaaa") == std::vector<std::size_t>{0, 1, 2, 3},
            "Неверная префикс-функция строки aaaa");
    require(siaod::prefixFunction("abacaba") ==
                std::vector<std::size_t>{0, 0, 1, 0, 1, 2, 3},
            "Неверная префикс-функция строки abacaba");
    require(siaod::prefixFunction("aaab") == std::vector<std::size_t>{0, 1, 2, 0},
            "Неверная префикс-функция строки aaab");
    // Кириллица занимает по два байта на букву, префикс-функция байтовая.
    require(siaod::prefixFunction("шалаш") ==
                std::vector<std::size_t>{0, 0, 0, 0, 0, 0, 0, 0, 1, 2},
            "Неверная префикс-функция строки шалаш");
}

void testKmpKnownCases() {
    const SearchOutcome first = siaod::kmpFindFirst("ababababa", "aba");
    require(first.index == 0, "Неверный индекс первого вхождения в ababababa");
    const SearchOutcome last = siaod::kmpFindLast("ababababa", "aba");
    require(last.index == 6, "Неверный индекс последнего вхождения в ababababa");
    require(siaod::kmpFindAll("ababababa", "aba") ==
                std::vector<std::size_t>{0, 2, 4, 6},
            "Неверный список перекрывающихся вхождений");

    const SearchOutcome overlap = siaod::kmpFindLast("aaaaaa", "aa");
    require(overlap.index == 4, "Неверное последнее перекрывающееся вхождение");

    // Пример из задания для самоподготовки (вопрос 9).
    const SearchOutcome period = siaod::kmpFindLast("aaabaaabaaab", "aaabaaa");
    require(period.index == 4, "Неверное вхождение в периодическом тексте");

    require(siaod::kmpFindFirst("abcdef", "abcdef").index == 0,
            "Образец, равный тексту, должен находиться на позиции 0");
    require(siaod::kmpFindFirst("ab", "abc").index == -1,
            "Образец длиннее текста не может быть найден");
    require(siaod::kmpFindLast("шалаш стоит и шалаш", "шалаш").index == 25,
            "Неверное последнее вхождение для кириллицы (байтовый индекс)");
    require(siaod::utf8SymbolCount("шалаш стоит и шалаш") == 19,
            "Неверное число символов в строке с кириллицей");
    require(siaod::utf8SymbolIndex("шалаш стоит и шалаш", 25) == 14,
            "Неверный пересчёт байта в номер символа");
    require(siaod::utf8SymbolIndex("aboba", 3) == 3,
            "Для ASCII байтовый и символьный номера должны совпадать");
}

void testLongestWord() {
    const auto check = [](const std::string& sentence, const std::string& expectedWord,
                          std::size_t expectedLetters) {
        const auto result = siaod::longestWordWithEqualEnds(sentence);
        require(result.has_value(),
                "Слово не найдено в предложении: " + sentence);
        require(result->word == expectedWord,
                "Неверное слово «" + result->word + "» вместо «" + expectedWord + "»");
        require(result->letterCount == expectedLetters,
                "Неверная длина слова в предложении: " + sentence);
    };

    check("Шалаш и дом", "Шалаш", 5);
    check("Ароза азора упала на лапу", "Ароза", 5); // Регистр концов различается.
    check("Шалаш, дом и сад.", "Шалаш", 5);          // Знаки препинания — разделители.
    check("level kayak sun", "level", 5);            // Равная длина — остаётся первое.
    check("дед око", "дед", 3);                      // То же для кириллицы.
    check("привет Ёж и ёлка", "и", 1);               // Однобуквенное слово подходит.
    check("а б в г", "а", 1);                        // То же, первый из равных.
    check("Ёж Ёмё", "Ёмё", 3);                       // Ё и ё сравниваются без учёта регистра.

    // «е» и «ё» — разные буквы: слово «ещё» не подходит.
    require(!siaod::longestWordWithEqualEnds("ещё еж").has_value(),
            "Буквы е и ё не должны считаться одинаковыми");

    require(!siaod::longestWordWithEqualEnds("").has_value(),
            "Для пустой строки результат должен отсутствовать");
    require(!siaod::longestWordWithEqualEnds("мама раму").has_value(),
            "Не должно найтись слово с разными концами");
    require(!siaod::longestWordWithEqualEnds("123 456 ...").has_value(),
            "Цифры и знаки — не буквы");
}

void testComparisonBounds() {
    const std::string text = "aaaaaaaaabaaaaaaaaaa";
    const SearchOutcome kmp = siaod::kmpFindLast(text, "aaab");
    require(kmp.index == 6, "Неверный индекс в тесте границы сравнений");
    // Стандартная оценка для КМП: не более 2n сравнений символов текста.
    require(kmp.comparisons <= 2 * text.size(),
            "Слишком много сравнений КМП: " + std::to_string(kmp.comparisons));
}

void testRandomAgainstNaive() {
    // Проверка КМП на случайных данных против наивного поиска.
    std::mt19937_64 generator(20260206);
    const std::string_view alphabets[] = {"ab", "abc", "аибв"}; // Включая кириллицу.
    for (std::size_t trial = 0; trial < 4000; ++trial) {
        const std::string_view alphabet = alphabets[trial % 3];
        std::uniform_int_distribution<std::size_t> pickChar(0, alphabet.size() - 1);

        auto randomString = [&](std::size_t length) {
            std::string value;
            for (std::size_t i = 0; i < length; ++i) {
                value.push_back(alphabet[pickChar(generator)]);
            }
            return value;
        };

        const std::string text = randomString(1 + generator() % 40);
        const std::string pattern = randomString(1 + generator() % 6);

        const auto expectedFirst = siaod::naiveFindFirst(text, pattern);
        const auto expectedLast = siaod::naiveFindLast(text, pattern);
        const auto kmpFirst = siaod::kmpFindFirst(text, pattern);
        const auto kmpLast = siaod::kmpFindLast(text, pattern);

        require(kmpFirst.index == expectedFirst.index,
                "КМП и наивный поиск разошлись (первое вхождение)");
        require(kmpLast.index == expectedLast.index,
                "КМП и наивный поиск разошлись (последнее вхождение)");
        require(siaod::kmpFindAll(text, pattern) == siaod::naiveFindAll(text, pattern),
                "Списки вхождений КМП и наивного поиска различаются");
        require(kmpFirst.comparisons <= 2 * text.size() + pattern.size(),
                "Слишком много сравнений КМП на случайных данных");
        require(kmpLast.comparisons <= 2 * text.size() + pattern.size(),
                "Слишком много сравнений КМП при полном просмотре");
    }
}

} // namespace

int main() {
    try {
        requireInvalidArgument("текст", "");
        testPrefixFunction();
        testKmpKnownCases();
        testLongestWord();
        testComparisonBounds();
        testRandomAgainstNaive();
        std::cout << "Все проверки text_search пройдены.\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Тест провален: " << error.what() << '\n';
        return 1;
    }
}
