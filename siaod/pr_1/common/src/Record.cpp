#include "siaod/Record.hpp"

#include <cstring>
#include <iomanip>
#include <random>
#include <set>
#include <sstream>
#include <stdexcept>

namespace siaod {
namespace {

template <std::size_t N>
void putString(char (&destination)[N], const std::string& value, const char* fieldName) {
    if (value.size() >= N) {
        throw std::invalid_argument(std::string("Значение поля «") + fieldName +
                                    "» слишком длинное для бинарной записи");
    }
    // Запись предварительно обнулена; копируем только полезные байты UTF-8/ASCII.
    std::memcpy(destination, value.data(), value.size());
}

template <std::size_t N>
std::string getString(const char (&source)[N]) {
    std::size_t length = 0;
    while (length < N && source[length] != '\0') {
        ++length;
    }
    return std::string(source, length);
}

std::string makeRandomPlate(std::mt19937_64& engine) {
    // Буквы совпадают с латинскими начертаниями разрешенных букв российского госномера.
    static const std::string allowedLetters = "ABEKMHOPCTYX";
    std::uniform_int_distribution<std::size_t> letter(0, allowedLetters.size() - 1);
    std::uniform_int_distribution<int> digit(0, 9);
    std::uniform_int_distribution<int> region(1, 99);

    std::string result;
    result.reserve(8);
    result.push_back(allowedLetters[letter(engine)]);
    for (int i = 0; i < 3; ++i) {
        result.push_back(static_cast<char>('0' + digit(engine)));
    }
    result.push_back(allowedLetters[letter(engine)]);
    result.push_back(allowedLetters[letter(engine)]);
    const int regionCode = region(engine);
    result.push_back(static_cast<char>('0' + regionCode / 10));
    result.push_back(static_cast<char>('0' + regionCode % 10));
    return result;
}

} // namespace

CarRecord makeCarRecord(const std::string& carNumber,
                        const std::string& carMake,
                        const std::string& ownerInfo) {
    CarRecord record{};
    putString(record.carNumber, carNumber, "номер автомобиля");
    putString(record.carMake, carMake, "марка");
    putString(record.ownerInfo, ownerInfo, "сведения о владельце");
    return record;
}

std::string getCarNumber(const CarRecord& record) {
    return getString(record.carNumber);
}

std::string getCarMake(const CarRecord& record) {
    return getString(record.carMake);
}

std::string getOwnerInfo(const CarRecord& record) {
    return getString(record.ownerInfo);
}

std::vector<CarRecord> makeSampleRecords(std::size_t count, std::uint64_t seed) {
    // Формат A123BC77 дает много уникальных ключей; set дополнительно гарантирует
    // отсутствие повторов, как требует условие лабораторной работы.
    constexpr std::size_t kMaximumNumberOfKeys = 12u * 1000u * 12u * 12u * 99u;
    if (count > kMaximumNumberOfKeys) {
        throw std::invalid_argument("Запрошено больше уникальных номеров, чем поддерживает формат");
    }

    static const std::vector<std::string> makes = {
        "Лада Гранта", "Киа Рио", "Тойота Камри", "Хендай Солярис",
        "Фольксваген Поло", "Рено Логан", "Шкода Октавия", "Ниссан Кашкай",
        "Мазда 6", "Форд Фокус"
    };
    static const std::vector<std::string> owners = {
        "Иванов Иван Иванович", "Петров Петр Петрович", "Сидорова Анна Сергеевна",
        "Кузнецов Алексей Олегович", "Смирнова Мария Андреевна",
        "Попов Дмитрий Викторович", "Волкова Елена Павловна",
        "Соколов Михаил Ильич"
    };

    std::mt19937_64 engine(seed);
    std::uniform_int_distribution<std::size_t> makeIndex(0, makes.size() - 1);
    std::set<std::string> uniqueKeys;
    std::vector<CarRecord> records;
    records.reserve(count);

    for (std::size_t i = 0; i < count; ++i) {
        std::string number;
        do {
            number = makeRandomPlate(engine);
        } while (!uniqueKeys.insert(number).second);

        std::ostringstream owner;
        owner << owners[i % owners.size()] << ", тел. +7-900-"
              << std::setw(3) << std::setfill('0') << ((i * 37u) % 1000u) << '-'
              << std::setw(2) << ((i * 13u) % 100u) << '-'
              << std::setw(2) << ((i * 29u) % 100u);

        records.push_back(makeCarRecord(number, makes[makeIndex(engine)], owner.str()));
    }
    return records;
}

} // namespace siaod
