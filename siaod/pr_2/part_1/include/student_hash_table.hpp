#pragma once

#include <cstddef>
#include <iosfwd>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace siaod {

struct Student {
    std::string recordBook; // Ключ: номер зачётной книжки, в том числе с ведущими нулями.
    std::string group;
    std::string fullName;
};

enum class RehashKind {
    none,
    compact, // Удалены служебные метки без изменения размера таблицы.
    grow     // Таблица увеличена, все ключи размещены заново.
};

struct InsertResult {
    bool inserted;
    RehashKind rehash;
    std::size_t oldCapacity;
    std::size_t newCapacity;
    std::size_t tableIndex;
    std::size_t recordIndex;
};

struct SearchResult {
    // Указатель действителен только до следующей изменяющей операции над таблицей.
    const Student* student;
    std::size_t tableIndex;
    std::size_t recordIndex;
};

class StudentHashTable {
public:
    StudentHashTable();

    InsertResult insert(const Student& student);
    std::optional<SearchResult> find(std::string_view recordBook) const;
    bool erase(std::string_view recordBook);

    void printRecords(std::ostream& out) const;
    void printTable(std::ostream& out) const;

    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;
    std::size_t deletedCount() const noexcept;

    // m >= 2. В самой таблице m всегда простое, поэтому 1 <= h2 < m
    // взаимно просто с m, и последовательность проб обходит все ячейки.
    static std::size_t primaryHash(std::string_view key, std::size_t m);
    static std::size_t secondaryHash(std::string_view key, std::size_t m);

private:
    enum class State { empty, occupied, deleted };

    struct Slot {
        State state = State::empty;
        std::string key;
        std::size_t recordIndex = 0;
    };

    // Полезные данные и хеш-таблица — два отдельных массива.
    std::vector<std::optional<Student>> records_;
    std::vector<std::size_t> freeRecordIndices_;
    std::vector<Slot> table_;
    std::size_t size_ = 0;
    std::size_t deletedCount_ = 0;

    std::optional<std::size_t> findSlot(std::string_view key) const;
    std::optional<std::size_t> insertionSlot(std::string_view key) const;
    RehashKind prepareForInsert();
    void rehash(std::size_t newCapacity);
};

// Шесть фиксированных записей для автоматического заполнения при запуске.
std::vector<Student> demoStudents();

} // namespace siaod
