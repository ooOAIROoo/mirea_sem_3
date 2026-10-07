#include "siaod/Search.hpp"

#include <algorithm>
#include <stdexcept>

namespace siaod {

std::vector<KeyOffset> buildKeyOffsetIndex(BinaryRecordFile& file) {
    // Читаем записи последовательно один раз. Поиск после этого выполняется в RAM,
    // а byteOffset оставляет ссылку на исходную запись в бинарном файле.
    const auto records = file.readAll();
    std::vector<KeyOffset> index;
    index.reserve(records.size());

    for (std::size_t i = 0; i < records.size(); ++i) {
        index.push_back(KeyOffset{
            getCarNumber(records[i]),
            static_cast<std::uint64_t>(i) * sizeof(CarRecord)
        });
    }

    std::sort(index.begin(), index.end(), [](const KeyOffset& left, const KeyOffset& right) {
        return left.key < right.key;
    });

    const auto duplicate = std::adjacent_find(
        index.begin(), index.end(),
        [](const KeyOffset& left, const KeyOffset& right) { return left.key == right.key; });
    if (duplicate != index.end()) {
        throw std::runtime_error("В бинарном файле обнаружены повторяющиеся ключи");
    }
    return index;
}

std::optional<std::size_t> fibonacciSearchPosition(
    const std::vector<KeyOffset>& sortedIndex,
    const std::string& key) {
    const std::size_t n = sortedIndex.size();
    if (n == 0) {
        return std::nullopt;
    }

    // Подбираем наименьшее число Фибоначчи, не меньшее количества элементов.
    std::size_t fibMm2 = 0; // F(m - 2)
    std::size_t fibMm1 = 1; // F(m - 1)
    std::size_t fibM = fibMm2 + fibMm1; // F(m)
    while (fibM < n) {
        fibMm2 = fibMm1;
        fibMm1 = fibM;
        fibM = fibMm2 + fibMm1;
    }

    // offset — последняя позиция исключенной левой части; -1 означает начало массива.
    std::ptrdiff_t offset = -1;
    while (fibM > 1) {
        const auto candidateSigned = offset + static_cast<std::ptrdiff_t>(fibMm2);
        const std::size_t candidate = std::min(
            static_cast<std::size_t>(candidateSigned), n - 1);
        const std::string& candidateKey = sortedIndex[candidate].key;

        if (candidateKey < key) {
            // Ключ правее кандидата: отбрасываем левую часть и уменьшаем окно.
            fibM = fibMm1;
            fibMm1 = fibMm2;
            fibMm2 = fibM - fibMm1;
            offset = static_cast<std::ptrdiff_t>(candidate);
        } else if (candidateKey > key) {
            // Ключ левее кандидата: продолжаем в меньшем левом окне.
            fibM = fibMm2;
            fibMm1 = fibMm1 - fibMm2;
            fibMm2 = fibM - fibMm1;
        } else {
            return candidate;
        }
    }

    // Последний элемент мог остаться за границей основного цикла.
    const auto finalPosition = offset + 1;
    if (fibMm1 != 0 && finalPosition >= 0 &&
        static_cast<std::size_t>(finalPosition) < n &&
        sortedIndex[static_cast<std::size_t>(finalPosition)].key == key) {
        return static_cast<std::size_t>(finalPosition);
    }
    return std::nullopt;
}

std::optional<std::uint64_t> fibonacciSearchOffset(
    const std::vector<KeyOffset>& sortedIndex,
    const std::string& key) {
    const auto position = fibonacciSearchPosition(sortedIndex, key);
    if (!position) {
        return std::nullopt;
    }
    return sortedIndex[*position].byteOffset;
}

std::optional<CarRecord> fibonacciSearchRecord(
    BinaryRecordFile& file,
    const std::vector<KeyOffset>& sortedIndex,
    const std::string& key) {
    const auto byteOffset = fibonacciSearchOffset(sortedIndex, key);
    if (!byteOffset) {
        return std::nullopt;
    }
    return file.readAtOffset(*byteOffset);
}

} // namespace siaod
