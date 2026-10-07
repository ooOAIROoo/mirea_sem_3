#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace siaod {

// Длины полей включают завершающий нулевой байт.
// Номер хранится в ASCII (например, A123BC77), марки и сведения о владельце — UTF-8.
inline constexpr std::size_t kCarNumberBytes = 9;
inline constexpr std::size_t kCarMakeBytes = 48;
inline constexpr std::size_t kOwnerInfoBytes = 96;
inline constexpr std::size_t kRecordBytes =
    kCarNumberBytes + kCarMakeBytes + kOwnerInfoBytes;

// Запись имеет фиксированный размер, поэтому ее можно адресовать в файле по индексу.
// Здесь нет числовых полей: побайтовый формат не зависит от порядка байтов процессора.
struct CarRecord {
    char carNumber[kCarNumberBytes];
    char carMake[kCarMakeBytes];
    char ownerInfo[kOwnerInfoBytes];
};

static_assert(sizeof(CarRecord) == kRecordBytes,
              "Размер бинарной записи должен совпадать с суммой размеров полей");

// Создает запись, проверяя, что UTF-8 строка помещается в соответствующее поле.
CarRecord makeCarRecord(const std::string& carNumber,
                        const std::string& carMake,
                        const std::string& ownerInfo);

std::string getCarNumber(const CarRecord& record);
std::string getCarMake(const CarRecord& record);
std::string getOwnerInfo(const CarRecord& record);

// Генерирует записи с уникальными номерами и псевдослучайно выбираемыми марками.
std::vector<CarRecord> makeSampleRecords(std::size_t count, std::uint64_t seed);

} // namespace siaod
