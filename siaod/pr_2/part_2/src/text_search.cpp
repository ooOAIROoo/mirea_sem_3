#include "text_search.hpp"

#include <stdexcept>
#include <utility>

namespace siaod {

namespace {

// Декодирует одну кодовую точку UTF-8, начиная с байта pos.
// Некорректная или обрезанная последовательность возвращается одним байтом,
// поэтому функция всегда продвигается вперёд и не может зациклиться.
std::pair<std::uint32_t, std::size_t> decodeUtf8(std::string_view s, std::size_t pos) {
    const auto byteAt = [&s](std::size_t i) {
        return static_cast<std::uint32_t>(static_cast<unsigned char>(s[i]));
    };

    const std::uint32_t first = byteAt(pos);
    if (first < 0x80) {
        return {first, 1}; // Однобайтовый символ ASCII.
    }

    std::size_t length = 0;
    std::uint32_t value = 0;
    if ((first & 0xE0) == 0xC0) {
        length = 2;
        value = first & 0x1F;
    } else if ((first & 0xF0) == 0xE0) {
        length = 3;
        value = first & 0x0F;
    } else if ((first & 0xF8) == 0xF0) {
        length = 4;
        value = first & 0x07;
    } else {
        return {first, 1}; // Одиночный байт вне допустимых форматов.
    }
    if (pos + length > s.size()) {
        return {first, 1};
    }
    for (std::size_t k = 1; k < length; ++k) {
        const std::uint32_t next = byteAt(pos + k);
        if ((next & 0xC0) != 0x80) {
            return {first, 1}; // Ожидался байт продолжения 10xxxxxx.
        }
        value = (value << 6) | (next & 0x3F);
    }
    return {value, length};
}

// Буквы, существенные для задачи: латиница и кириллица (включая Ё и ё).
bool isLetterCodePoint(std::uint32_t codePoint) {
    return (codePoint >= 'A' && codePoint <= 'Z') ||
           (codePoint >= 'a' && codePoint <= 'z') ||
           (codePoint >= 0x0410 && codePoint <= 0x044F) || // А..Я, а..я
           codePoint == 0x0401 || codePoint == 0x0451;     // Ё и ё
}

// Простейшее приведение буквы к нижнему регистру для сравнения концов слова.
std::uint32_t toLowerCodePoint(std::uint32_t codePoint) {
    if (codePoint >= 'A' && codePoint <= 'Z') {
        return codePoint + 0x20;                 // A..Z -> a..z
    }
    if (codePoint >= 0x0410 && codePoint <= 0x042F) {
        return codePoint + 0x20;                 // А..Я -> а..я
    }
    if (codePoint == 0x0401) {
        return 0x0451;                           // Ё -> ё
    }
    return codePoint;
}

void requireNonEmptyPattern(std::string_view pattern) {
    if (pattern.empty()) {
        throw std::invalid_argument("Образец не может быть пустым");
    }
}

} // namespace

std::size_t utf8SymbolCount(std::string_view s) {
    std::size_t position = 0;
    std::size_t symbols = 0;
    while (position < s.size()) {
        const auto [codePoint, width] = decodeUtf8(s, position);
        static_cast<void>(codePoint);
        ++symbols;
        position += width;
    }
    return symbols;
}

std::size_t utf8SymbolIndex(std::string_view s, std::size_t byteIndex) {
    std::size_t position = 0;
    std::size_t symbols = 0;
    while (position < s.size() && position < byteIndex) {
        const auto [codePoint, width] = decodeUtf8(s, position);
        static_cast<void>(codePoint);
        ++symbols;
        position += width;
    }
    return symbols;
}

std::optional<LongestWordResult> longestWordWithEqualEnds(std::string_view sentence) {
    std::optional<LongestWordResult> best;
    std::size_t position = 0;

    while (position < sentence.size()) {
        const auto [codePoint, width] = decodeUtf8(sentence, position);
        if (!isLetterCodePoint(codePoint)) {
            position += width; // Разделитель: пробел, знак препинания, цифра...
            continue;
        }

        // Начало слова: запоминаем первую букву и читаем слово целиком.
        const std::size_t wordStart = position;
        const std::uint32_t firstLetter = toLowerCodePoint(codePoint);
        std::uint32_t lastLetter = firstLetter;
        std::size_t letters = 0;
        while (position < sentence.size()) {
            const auto [letter, letterWidth] = decodeUtf8(sentence, position);
            if (!isLetterCodePoint(letter)) {
                break;
            }
            lastLetter = toLowerCodePoint(letter);
            ++letters;
            position += letterWidth;
        }

        // Первая и последняя буквы совпали без учёта регистра.
        if (firstLetter == lastLetter && (!best || letters > best->letterCount)) {
            best = LongestWordResult{
                std::string(sentence.substr(wordStart, position - wordStart)),
                letters,
                wordStart};
        }
    }
    return best;
}

std::vector<std::size_t> prefixFunction(std::string_view s) {
    std::vector<std::size_t> pi(s.size(), 0);
    for (std::size_t i = 1; i < s.size(); ++i) {
        std::size_t candidate = pi[i - 1];
        // Подбираем длину предыдущей границы, продолжение которой равно s[i].
        while (candidate > 0 && s[i] != s[candidate]) {
            candidate = pi[candidate - 1];
        }
        if (s[i] == s[candidate]) {
            ++candidate;
        }
        pi[i] = candidate;
    }
    return pi;
}

SearchOutcome kmpFindFirst(std::string_view text, std::string_view pattern) {
    requireNonEmptyPattern(pattern);

    const std::vector<std::size_t> pi = prefixFunction(pattern);
    SearchOutcome result; // index = -1 по умолчанию.
    std::size_t matched = 0; // Сколько символов образца совпало к текущему моменту.

    for (std::size_t i = 0; i < text.size(); ++i) {
        // Совпавшая часть не должна «рваться»: подбираем меньшую границу,
        // пока text[i] не продолжит одну из найденных границ.
        while (matched > 0 && text[i] != pattern[matched]) {
            ++result.comparisons;
            matched = pi[matched - 1];
        }
        ++result.comparisons; // Очередное сравнение text[i] и pattern[matched].
        if (text[i] == pattern[matched]) {
            ++matched;
            if (matched == pattern.size()) {
                result.index = static_cast<std::ptrdiff_t>(i + 1 - pattern.size());
                return result; // Первое вождение найдено — просмотр окончен.
            }
        }
    }
    return result;
}

SearchOutcome kmpFindLast(std::string_view text, std::string_view pattern) {
    requireNonEmptyPattern(pattern);

    const std::vector<std::size_t> pi = prefixFunction(pattern);
    SearchOutcome result; // index = -1, если вхождений нет.
    std::size_t matched = 0;

    for (std::size_t i = 0; i < text.size(); ++i) {
        while (matched > 0 && text[i] != pattern[matched]) {
            ++result.comparisons;
            matched = pi[matched - 1];
        }
        ++result.comparisons;
        if (text[i] == pattern[matched]) {
            ++matched;
            if (matched == pattern.size()) {
                // Запоминаем вхождение и продолжаем поиск:
                // следующее может перекрываться с текущим.
                result.index = static_cast<std::ptrdiff_t>(i + 1 - pattern.size());
                matched = pi[matched - 1];
            }
        }
    }
    return result;
}

std::vector<std::size_t> kmpFindAll(std::string_view text, std::string_view pattern) {
    requireNonEmptyPattern(pattern);

    const std::vector<std::size_t> pi = prefixFunction(pattern);
    std::vector<std::size_t> occurrences;
    std::size_t matched = 0;

    for (std::size_t i = 0; i < text.size(); ++i) {
        while (matched > 0 && text[i] != pattern[matched]) {
            matched = pi[matched - 1];
        }
        if (text[i] == pattern[matched]) {
            ++matched;
            if (matched == pattern.size()) {
                occurrences.push_back(i + 1 - pattern.size());
                matched = pi[matched - 1];
            }
        }
    }
    return occurrences;
}

SearchOutcome naiveFindFirst(std::string_view text, std::string_view pattern) {
    requireNonEmptyPattern(pattern);

    SearchOutcome result; // index = -1 по умолчанию.
    if (pattern.size() > text.size()) {
        return result;
    }
    for (std::size_t start = 0; start + pattern.size() <= text.size(); ++start) {
        bool matched = true;
        for (std::size_t k = 0; k < pattern.size(); ++k) {
            ++result.comparisons;
            if (text[start + k] != pattern[k]) {
                matched = false;
                break; // Сдвиг образца на одну позицию вправо.
            }
        }
        if (matched) {
            result.index = static_cast<std::ptrdiff_t>(start);
            return result;
        }
    }
    return result;
}

SearchOutcome naiveFindLast(std::string_view text, std::string_view pattern) {
    requireNonEmptyPattern(pattern);

    SearchOutcome result; // index = -1, если вхождений нет.
    if (pattern.size() > text.size()) {
        return result;
    }
    for (std::size_t start = 0; start + pattern.size() <= text.size(); ++start) {
        bool matched = true;
        for (std::size_t k = 0; k < pattern.size(); ++k) {
            ++result.comparisons;
            if (text[start + k] != pattern[k]) {
                matched = false;
                break;
            }
        }
        if (matched) {
            result.index = static_cast<std::ptrdiff_t>(start);
        }
    }
    return result;
}

std::vector<std::size_t> naiveFindAll(std::string_view text, std::string_view pattern) {
    requireNonEmptyPattern(pattern);

    std::vector<std::size_t> occurrences;
    if (pattern.size() > text.size()) {
        return occurrences;
    }
    for (std::size_t start = 0; start + pattern.size() <= text.size(); ++start) {
        bool matched = true;
        for (std::size_t k = 0; k < pattern.size(); ++k) {
            if (text[start + k] != pattern[k]) {
                matched = false;
                break;
            }
        }
        if (matched) {
            occurrences.push_back(start);
        }
    }
    return occurrences;
}

} // namespace siaod
