#include "student_hash_table.hpp"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <utility>

namespace siaod {
namespace {

constexpr std::size_t kInitialCapacity = 11;

bool isAsciiWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
}

void validateField(const std::string& value, const char* description) {
    if (value.empty() || std::all_of(value.begin(), value.end(), isAsciiWhitespace) ||
        value.find_first_of("\r\n") != std::string::npos) {
        throw std::invalid_argument(std::string(description) + " должен быть непустой строкой");
    }
}

// Переполнение uint64_t определено стандартом: вычисление идёт по модулю 2^64.
std::uint64_t polynomial(std::string_view key, std::uint64_t base) {
    std::uint64_t result = 0;
    for (const unsigned char byte : key) {
        result = result * base + byte;
    }
    return result;
}

// То же, что (position + step) % m, но без переполнения size_t.
std::size_t nextProbe(std::size_t position, std::size_t step, std::size_t m) {
    return position >= m - step ? position - (m - step) : position + step;
}

std::size_t loadLimit(std::size_t m) {
    // floor(0.7 * m), вычисление без потенциального переполнения m * 7.
    return (m / 10) * 7 + ((m % 10) * 7) / 10;
}

bool isPrime(std::size_t value) {
    if (value < 2) {
        return false;
    }
    if (value % 2 == 0) {
        return value == 2;
    }
    for (std::size_t divisor = 3; divisor <= value / divisor; divisor += 2) {
        if (value % divisor == 0) {
            return false;
        }
    }
    return true;
}

std::size_t nextPrime(std::size_t value) {
    if (value <= 2) {
        return 2;
    }
    if (value % 2 == 0) {
        ++value; // Максимальное значение size_t нечётное.
    }
    while (!isPrime(value)) {
        if (value > std::numeric_limits<std::size_t>::max() - 2) {
            throw std::length_error("Не удалось увеличить хеш-таблицу");
        }
        value += 2;
    }
    return value;
}

} // namespace

StudentHashTable::StudentHashTable() : table_(kInitialCapacity) {}

std::size_t StudentHashTable::primaryHash(std::string_view key, std::size_t m) {
    if (m < 2) {
        throw std::invalid_argument("Размер хеш-таблицы должен быть не меньше 2");
    }
    return static_cast<std::size_t>(polynomial(key, 31) % m);
}

std::size_t StudentHashTable::secondaryHash(std::string_view key, std::size_t m) {
    if (m < 2) {
        throw std::invalid_argument("Размер хеш-таблицы должен быть не меньше 2");
    }
    return 1 + static_cast<std::size_t>(polynomial(key, 37) % (m - 1));
}

std::optional<std::size_t> StudentHashTable::findSlot(std::string_view key) const {
    const std::size_t m = table_.size();
    std::size_t position = primaryHash(key, m);
    const std::size_t step = secondaryHash(key, m);

    for (std::size_t probe = 0; probe < m; ++probe) {
        const Slot& slot = table_[position];
        if (slot.state == State::empty) {
            return std::nullopt; // За никогда не занятой ячейкой ключа нет.
        }
        if (slot.state == State::occupied && std::string_view(slot.key) == key) {
            return position;
        }
        // Метка удаления не прерывает последовательность проб.
        position = nextProbe(position, step, m);
    }
    return std::nullopt;
}

std::optional<std::size_t> StudentHashTable::insertionSlot(std::string_view key) const {
    const std::size_t m = table_.size();
    std::size_t position = primaryHash(key, m);
    const std::size_t step = secondaryHash(key, m);
    std::optional<std::size_t> firstDeleted;

    for (std::size_t probe = 0; probe < m; ++probe) {
        const Slot& slot = table_[position];
        if (slot.state == State::deleted && !firstDeleted) {
            firstDeleted = position;
        } else if (slot.state == State::empty) {
            return firstDeleted ? firstDeleted : std::optional<std::size_t>{position};
        }
        position = nextProbe(position, step, m);
    }
    return firstDeleted;
}

RehashKind StudentHashTable::prepareForInsert() {
    const std::size_t m = table_.size();
    const std::size_t limit = loadLimit(m);

    // Проверка предстоящей вставки проводится после проверки уникальности ключа.
    if (size_ >= limit) {
        if (m > std::numeric_limits<std::size_t>::max() / 2) {
            throw std::length_error("Не удалось увеличить хеш-таблицу");
        }
        rehash(nextPrime(m * 2));
        return RehashKind::grow;
    }
    if (size_ + deletedCount_ >= limit) {
        rehash(m); // Метки занимают слишком много места: перестроить без роста.
        return RehashKind::compact;
    }
    return RehashKind::none;
}

void StudentHashTable::rehash(std::size_t newCapacity) {
    std::vector<Slot> newTable(newCapacity);
    for (const Slot& oldSlot : table_) {
        if (oldSlot.state != State::occupied) {
            continue;
        }
        std::size_t position = primaryHash(oldSlot.key, newCapacity);
        const std::size_t step = secondaryHash(oldSlot.key, newCapacity);
        bool placed = false;
        for (std::size_t probe = 0; probe < newCapacity; ++probe) {
            if (newTable[position].state == State::empty) {
                newTable[position] = oldSlot; // Индекс записи в массиве сохраняется.
                placed = true;
                break;
            }
            position = nextProbe(position, step, newCapacity);
        }
        if (!placed) {
            throw std::logic_error("Не удалось разместить запись при рехешировании");
        }
    }
    table_.swap(newTable);
    deletedCount_ = 0;
}

InsertResult StudentHashTable::insert(const Student& student) {
    validateField(student.recordBook, "Номер зачётной книжки");
    validateField(student.group, "Номер группы");
    validateField(student.fullName, "ФИО");

    const std::size_t oldCapacity = table_.size();
    if (const auto existing = findSlot(student.recordBook)) {
        return {false, RehashKind::none, oldCapacity, oldCapacity,
                *existing, table_[*existing].recordIndex};
    }

    const RehashKind kind = prepareForInsert();
    const auto target = insertionSlot(student.recordBook);
    if (!target) {
        throw std::logic_error("В хеш-таблице не осталось свободных ячеек");
    }
    const bool reuseDeletedSlot = table_[*target].state == State::deleted;
    const bool reuseRecord = !freeRecordIndices_.empty();
    const std::size_t recordIndex = reuseRecord ? freeRecordIndices_.back() : records_.size();

    // Сначала создаём ключ и запись. До их успешного создания ячейка не занята.
    Slot newSlot{State::occupied, student.recordBook, recordIndex};
    if (reuseRecord) {
        records_[recordIndex].emplace(student);
        freeRecordIndices_.pop_back();
    } else {
        records_.emplace_back(student);
    }
    table_[*target] = std::move(newSlot);
    ++size_;
    if (reuseDeletedSlot) {
        --deletedCount_;
    }
    return {true, kind, oldCapacity, table_.size(), *target, recordIndex};
}

std::optional<SearchResult> StudentHashTable::find(std::string_view recordBook) const {
    const auto index = findSlot(recordBook);
    if (!index) {
        return std::nullopt;
    }
    const Slot& slot = table_[*index];
    return SearchResult{&*records_[slot.recordIndex], *index, slot.recordIndex};
}

bool StudentHashTable::erase(std::string_view recordBook) {
    const auto index = findSlot(recordBook);
    if (!index) {
        return false;
    }
    Slot& slot = table_[*index];
    // Добавляем индекс в список свободных до изменения записи и ячейки.
    freeRecordIndices_.push_back(slot.recordIndex);
    records_[slot.recordIndex].reset();
    slot.key.clear();
    slot.recordIndex = 0;
    slot.state = State::deleted;
    --size_;
    ++deletedCount_;
    return true;
}

void StudentHashTable::printRecords(std::ostream& out) const {
    if (size_ == 0) {
        out << "Записей нет.\n";
        return;
    }
    for (std::size_t i = 0; i < records_.size(); ++i) {
        if (records_[i]) {
            const Student& student = *records_[i];
            out << "Индекс записи: " << i << '\n'
                << "  Номер зачётной книжки: " << student.recordBook << '\n'
                << "  Номер группы: " << student.group << '\n'
                << "  ФИО: " << student.fullName << "\n\n";
        }
    }
}

void StudentHashTable::printTable(std::ostream& out) const {
    out << "Индекс ячейки | Состояние | Ключ | h1 | h2 | Индекс записи\n";
    for (std::size_t i = 0; i < table_.size(); ++i) {
        const Slot& slot = table_[i];
        out << i << " | ";
        if (slot.state == State::empty) {
            out << "пусто | - | - | - | -\n";
        } else if (slot.state == State::deleted) {
            out << "удалено | - | - | - | -\n";
        } else {
            out << "занято | " << slot.key << " | "
                << primaryHash(slot.key, table_.size()) << " | "
                << secondaryHash(slot.key, table_.size()) << " | "
                << slot.recordIndex << '\n';
        }
    }
}

std::size_t StudentHashTable::size() const noexcept {
    return size_;
}

std::size_t StudentHashTable::capacity() const noexcept {
    return table_.size();
}

std::size_t StudentHashTable::deletedCount() const noexcept {
    return deletedCount_;
}

std::vector<Student> demoStudents() {
    return {
        {"1001", "ИКБО-16-25", "Иванов Иван Иванович"},
        {"1013", "ИКБО-16-25", "Петрова Мария Сергеевна"},
        {"1098", "ИКБО-17-25", "Сидоров Алексей Дмитриевич"},
        {"2024001", "ИКБО-17-25", "Кузнецова Анна Андреевна"},
        {"2024002", "ИКБО-18-25", "Смирнов Дмитрий Олегович"},
        {"2024003", "ИКБО-18-25", "Волкова Елена Павловна"}
    };
}

} // namespace siaod
