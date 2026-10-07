#pragma once

#include "siaod/Record.hpp"

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

namespace siaod {

// Создает каталоги при необходимости, затем записывает строки в UTF-8 текстовый файл.
void writeTextRecords(const std::filesystem::path& textPath,
                      const std::vector<CarRecord>& records);

// Читает текстовые строки вида «номер|марка|сведения» и создает бинарный файл.
// Для простоты и надежности разделитель «|» не используется внутри значений полей.
void convertTextToBinary(const std::filesystem::path& textPath,
                         const std::filesystem::path& binaryPath);

void writeBinaryRecords(const std::filesystem::path& binaryPath,
                        const std::vector<CarRecord>& records);

// Выполняет рекомендуемый заданием этап: генерация текста, затем конвертация в binary.
std::filesystem::path createGeneratedDataset(std::size_t count,
                                             std::uint64_t seed,
                                             const std::filesystem::path& directory,
                                             const std::string& fileStem);

class BinaryRecordFile {
public:
    explicit BinaryRecordFile(const std::filesystem::path& path);

    std::size_t recordCount() const noexcept { return recordCount_; }
    const std::filesystem::path& path() const noexcept { return path_; }

    // Прямой доступ к записи с нумерацией от нуля.
    std::optional<CarRecord> readAt(std::size_t index);

    // Прямой доступ по байтовому смещению; смещение должно быть кратно sizeof(CarRecord).
    std::optional<CarRecord> readAtOffset(std::uint64_t byteOffset);

    // Линейно читает файл от начала и сравнивает номер каждой записи с ключом.
    std::optional<CarRecord> linearSearch(const std::string& key);

    // Считывает весь файл последовательно; используется при построении индекса в памяти.
    std::vector<CarRecord> readAll();

private:
    std::filesystem::path path_;
    std::ifstream stream_;
    std::size_t recordCount_ = 0;
};

} // namespace siaod
