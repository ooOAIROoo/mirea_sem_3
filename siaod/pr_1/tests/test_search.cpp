#include "siaod/RecordFile.hpp"
#include "siaod/Search.hpp"

#include <algorithm>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error("Тест не пройден: " + message);
    }
}

void testFibonacciBoundaries() {
    // Проверяем массивы разных длин, в том числе 1, степени Фибоначчи и соседние длины.
    for (std::size_t count = 1; count <= 120; ++count) {
        std::vector<siaod::KeyOffset> index;
        for (std::size_t i = 0; i < count; ++i) {
            std::ostringstream key;
            key << 'K' << std::setw(4) << std::setfill('0') << i;
            index.push_back({key.str(), i * sizeof(siaod::CarRecord)});
        }

        for (std::size_t i = 0; i < count; ++i) {
            std::ostringstream key;
            key << 'K' << std::setw(4) << std::setfill('0') << i;
            const auto found = siaod::fibonacciSearchPosition(index, key.str());
            require(found && *found == i, "поиск Фибоначчи находит каждый существующий индекс");
        }
        require(!siaod::fibonacciSearchPosition(index, "K9999"),
                "поиск Фибоначчи сообщает об отсутствии ключа");
    }
    require(!siaod::fibonacciSearchPosition({}, "A123BC77"),
            "пустой индекс обрабатывается безопасно");
}

void testBinaryFileAndSearch() {
    const auto directory = std::filesystem::temp_directory_path() / "siaod_pr1_cpp_tests";
    std::filesystem::remove_all(directory);
    std::filesystem::create_directories(directory);

    try {
        const auto textPath = directory / "records.txt";
        const auto binaryPath = directory / "records.bin";
        const auto sourceRecords = siaod::makeSampleRecords(100, 424242);

        std::set<std::string> uniqueKeys;
        for (const auto& record : sourceRecords) {
            uniqueKeys.insert(siaod::getCarNumber(record));
        }
        require(uniqueKeys.size() == 100, "генератор создает уникальные ключи");

        siaod::writeTextRecords(textPath, sourceRecords);
        siaod::convertTextToBinary(textPath, binaryPath);

        siaod::BinaryRecordFile file(binaryPath);
        require(file.recordCount() == 100, "после конвертации в файле ровно 100 записей");
        require(std::filesystem::file_size(binaryPath) == 100 * sizeof(siaod::CarRecord),
                "размер файла кратен фиксированному размеру записи");

        const std::vector<std::size_t> samplePositions = {0, 49, 99};
        for (const auto position : samplePositions) {
            const auto direct = file.readAt(position);
            require(direct.has_value(), "прямой доступ возвращает запись внутри файла");
            const auto key = siaod::getCarNumber(sourceRecords[position]);
            require(siaod::getCarNumber(*direct) == key,
                    "конвертация сохраняет ключ исходной записи");
            const auto linear = file.linearSearch(key);
            require(linear && siaod::getCarNumber(*linear) == key,
                    "линейный поиск находит ключ из начала, середины и конца файла");
        }
        require(!file.readAt(100), "прямой доступ за концом файла возвращает пустой результат");
        require(!file.linearSearch("Z999ZZ99"), "линейный поиск обрабатывает отсутствующий ключ");

        const auto index = siaod::buildKeyOffsetIndex(file);
        require(index.size() == 100, "индекс содержит по одной ссылке на каждую запись");
        require(std::is_sorted(index.begin(), index.end(),
                               [](const auto& left, const auto& right) {
                                   return left.key < right.key;
                               }),
                "индекс отсортирован по ключу");
        for (const auto position : samplePositions) {
            const auto key = siaod::getCarNumber(sourceRecords[position]);
            const auto offset = siaod::fibonacciSearchOffset(index, key);
            require(offset && *offset == position * sizeof(siaod::CarRecord),
                    "поиск Фибоначчи возвращает смещение исходной записи");
            const auto found = siaod::fibonacciSearchRecord(file, index, key);
            require(found && siaod::getCarNumber(*found) == key,
                    "прямое чтение по найденному смещению возвращает правильную запись");
        }
        require(!siaod::fibonacciSearchRecord(file, index, "Z999ZZ99"),
                "поиск Фибоначчи обрабатывает отсутствующий ключ");
    } catch (...) {
        std::filesystem::remove_all(directory);
        throw;
    }
    std::filesystem::remove_all(directory);
}

} // namespace

int main() {
    try {
        testFibonacciBoundaries();
        testBinaryFileAndSearch();
        std::cout << "Все проверки пройдены: бинарная запись, уникальные ключи, "
                     "линейный поиск и поиск Фибоначчи.\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
