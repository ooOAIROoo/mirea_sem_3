#pragma once

#include "siaod/RecordFile.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace siaod {

// Элемент дополнительной структуры: поисковый ключ и байтовое смещение записи в файле.
struct KeyOffset {
    std::string key;
    std::uint64_t byteOffset = 0;
};

// Построить упорядоченный по ключу индекс всех записей файла.
std::vector<KeyOffset> buildKeyOffsetIndex(BinaryRecordFile& file);

// Найти позицию ключа в предварительно отсортированном массиве методом Фибоначчи.
std::optional<std::size_t> fibonacciSearchPosition(
    const std::vector<KeyOffset>& sortedIndex,
    const std::string& key);

// Найти ключ и вернуть именно ссылку на запись в файле — ее байтовое смещение.
std::optional<std::uint64_t> fibonacciSearchOffset(
    const std::vector<KeyOffset>& sortedIndex,
    const std::string& key);

// Найти ключ в индексе и прочитать соответствующую запись прямым доступом к файлу.
std::optional<CarRecord> fibonacciSearchRecord(
    BinaryRecordFile& file,
    const std::vector<KeyOffset>& sortedIndex,
    const std::string& key);

} // namespace siaod
