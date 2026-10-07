#include "siaod/RecordFile.hpp"

#include <limits>
#include <stdexcept>
#include <string>

namespace siaod {
namespace {

void createParentDirectory(const std::filesystem::path& path) {
    const auto parent = path.parent_path();
    if (!parent.empty()) {
        std::filesystem::create_directories(parent);
    }
}

} // namespace

void writeTextRecords(const std::filesystem::path& textPath,
                      const std::vector<CarRecord>& records) {
    createParentDirectory(textPath);
    std::ofstream output(textPath, std::ios::binary | std::ios::trunc);
    if (!output) {
        throw std::runtime_error("Не удалось создать текстовый файл: " + textPath.string());
    }

    for (const auto& record : records) {
        // Условие гарантирует, что символ-разделитель отсутствует внутри полей.
        output << getCarNumber(record) << '|'
               << getCarMake(record) << '|'
               << getOwnerInfo(record) << '\n';
    }
    if (!output) {
        throw std::runtime_error("Ошибка при записи текстового файла: " + textPath.string());
    }
}

void writeBinaryRecords(const std::filesystem::path& binaryPath,
                        const std::vector<CarRecord>& records) {
    createParentDirectory(binaryPath);
    std::ofstream output(binaryPath, std::ios::binary | std::ios::trunc);
    if (!output) {
        throw std::runtime_error("Не удалось создать бинарный файл: " + binaryPath.string());
    }

    for (const auto& record : records) {
        output.write(reinterpret_cast<const char*>(&record),
                     static_cast<std::streamsize>(sizeof(CarRecord)));
    }
    if (!output) {
        throw std::runtime_error("Ошибка при записи бинарного файла: " + binaryPath.string());
    }
}

void convertTextToBinary(const std::filesystem::path& textPath,
                         const std::filesystem::path& binaryPath) {
    std::ifstream input(textPath, std::ios::binary);
    if (!input) {
        throw std::runtime_error("Не удалось открыть текстовый файл: " + textPath.string());
    }

    std::vector<CarRecord> records;
    std::string line;
    std::size_t lineNumber = 0;
    while (std::getline(input, line)) {
        ++lineNumber;
        // На Windows текстовая строка может завершаться парой CR/LF;
        // std::getline удаляет LF, поэтому убираем оставшийся CR.
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) {
            continue;
        }

        const auto first = line.find('|');
        const auto second = first == std::string::npos
                                ? std::string::npos
                                : line.find('|', first + 1);
        if (first == std::string::npos || second == std::string::npos ||
            line.find('|', second + 1) != std::string::npos) {
            throw std::runtime_error("Неверный формат записи в строке " +
                                     std::to_string(lineNumber));
        }

        const std::string number = line.substr(0, first);
        const std::string make = line.substr(first + 1, second - first - 1);
        const std::string owner = line.substr(second + 1);
        try {
            records.push_back(makeCarRecord(number, make, owner));
        } catch (const std::exception& error) {
            throw std::runtime_error("Ошибка в строке " + std::to_string(lineNumber) +
                                     ": " + error.what());
        }
    }
    if (input.bad()) {
        throw std::runtime_error("Ошибка чтения текстового файла: " + textPath.string());
    }

    writeBinaryRecords(binaryPath, records);
}

std::filesystem::path createGeneratedDataset(std::size_t count,
                                             std::uint64_t seed,
                                             const std::filesystem::path& directory,
                                             const std::string& fileStem) {
    std::filesystem::create_directories(directory);
    const auto textPath = directory / (fileStem + ".txt");
    const auto binaryPath = directory / (fileStem + ".bin");

    // Сначала сохраняем читаемый текстовый источник, затем отдельно конвертируем его.
    const auto records = makeSampleRecords(count, seed);
    writeTextRecords(textPath, records);
    convertTextToBinary(textPath, binaryPath);
    return binaryPath;
}

BinaryRecordFile::BinaryRecordFile(const std::filesystem::path& path)
    : path_(path), stream_(path, std::ios::binary) {
    if (!stream_) {
        throw std::runtime_error("Не удалось открыть бинарный файл: " + path.string());
    }

    std::error_code error;
    const auto byteSize = std::filesystem::file_size(path, error);
    if (error) {
        throw std::runtime_error("Не удалось определить размер файла: " + path.string());
    }
    if (byteSize % sizeof(CarRecord) != 0) {
        throw std::runtime_error("Размер бинарного файла не кратен размеру записи (" +
                                 std::to_string(sizeof(CarRecord)) + " байт)");
    }
    if (byteSize / sizeof(CarRecord) > std::numeric_limits<std::size_t>::max()) {
        throw std::runtime_error("Файл слишком велик для адресации в этой программе");
    }
    recordCount_ = static_cast<std::size_t>(byteSize / sizeof(CarRecord));
}

std::optional<CarRecord> BinaryRecordFile::readAt(std::size_t index) {
    if (index >= recordCount_) {
        return std::nullopt;
    }
    return readAtOffset(static_cast<std::uint64_t>(index) * sizeof(CarRecord));
}

std::optional<CarRecord> BinaryRecordFile::readAtOffset(std::uint64_t byteOffset) {
    const auto recordSize = static_cast<std::uint64_t>(sizeof(CarRecord));
    if (byteOffset % recordSize != 0 || byteOffset / recordSize >= recordCount_) {
        return std::nullopt;
    }

    stream_.clear();
    stream_.seekg(static_cast<std::streamoff>(byteOffset), std::ios::beg);
    if (!stream_) {
        throw std::runtime_error("Не удалось перейти к смещению " +
                                 std::to_string(byteOffset) + " в бинарном файле");
    }

    CarRecord record{};
    stream_.read(reinterpret_cast<char*>(&record),
                 static_cast<std::streamsize>(sizeof(CarRecord)));
    if (stream_.gcount() != static_cast<std::streamsize>(sizeof(CarRecord))) {
        throw std::runtime_error("Не удалось прочитать полную запись по смещению " +
                                 std::to_string(byteOffset));
    }
    return record;
}

std::optional<CarRecord> BinaryRecordFile::linearSearch(const std::string& key) {
    stream_.clear();
    stream_.seekg(0, std::ios::beg);
    if (!stream_) {
        throw std::runtime_error("Не удалось установить начало бинарного файла");
    }

    for (std::size_t i = 0; i < recordCount_; ++i) {
        CarRecord record{};
        stream_.read(reinterpret_cast<char*>(&record),
                     static_cast<std::streamsize>(sizeof(CarRecord)));
        if (stream_.gcount() != static_cast<std::streamsize>(sizeof(CarRecord))) {
            throw std::runtime_error("Ошибка чтения записи " + std::to_string(i));
        }
        if (getCarNumber(record) == key) {
            return record;
        }
    }
    return std::nullopt;
}

std::vector<CarRecord> BinaryRecordFile::readAll() {
    std::vector<CarRecord> records;
    records.reserve(recordCount_);
    stream_.clear();
    stream_.seekg(0, std::ios::beg);
    if (!stream_) {
        throw std::runtime_error("Не удалось установить начало бинарного файла");
    }

    for (std::size_t i = 0; i < recordCount_; ++i) {
        CarRecord record{};
        stream_.read(reinterpret_cast<char*>(&record),
                     static_cast<std::streamsize>(sizeof(CarRecord)));
        if (stream_.gcount() != static_cast<std::streamsize>(sizeof(CarRecord))) {
            throw std::runtime_error("Ошибка чтения записи " + std::to_string(i));
        }
        records.push_back(record);
    }
    return records;
}

} // namespace siaod
