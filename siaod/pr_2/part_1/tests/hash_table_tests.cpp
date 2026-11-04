#include "student_hash_table.hpp"

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace {

using siaod::RehashKind;
using siaod::Student;
using siaod::StudentHashTable;

void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

template <typename Function>
void requireInvalidArgument(Function function, const std::string& message) {
    try {
        function();
    } catch (const std::invalid_argument&) {
        return;
    }
    throw std::runtime_error(message);
}

Student student(std::string key, std::string group = "IKBO-16-25",
                std::string name = "Иванов Иван Иванович") {
    return {std::move(key), std::move(group), std::move(name)};
}

void requireStudent(const StudentHashTable& table, const Student& expected) {
    const auto found = table.find(expected.recordBook);
    require(found.has_value(), "Не найден существующий ключ " + expected.recordBook);
    require(found->student->recordBook == expected.recordBook &&
            found->student->group == expected.group &&
            found->student->fullName == expected.fullName,
            "Неверные данные для ключа " + expected.recordBook);
}

bool prime(std::size_t n) {
    if (n < 2) return false;
    for (std::size_t d = 2; d <= n / d; ++d) {
        if (n % d == 0) return false;
    }
    return true;
}

void testHashesAndProbeCoverage() {
    require(StudentHashTable::primaryHash("1001", 11) == 6, "Неверный h1(1001)");
    require(StudentHashTable::secondaryHash("1001", 11) == 5, "Неверный h2(1001)");
    require(StudentHashTable::primaryHash("1098", 11) == 6, "Неверный h1(1098)");
    require(StudentHashTable::secondaryHash("1098", 11) == 5, "Неверный h2(1098)");
    require(StudentHashTable::primaryHash("1013", 11) == 6, "Неверный h1(1013)");
    require(StudentHashTable::secondaryHash("1013", 11) == 4, "Неверный h2(1013)");
    require(StudentHashTable::primaryHash(std::string(1000, 'x'), 23) == 15,
            "Неверное переполнение полиномиального h1");
    require(StudentHashTable::secondaryHash(std::string(1000, 'x'), 23) == 17,
            "Неверное переполнение полиномиального h2");
    require(StudentHashTable::primaryHash("Ёж-0001", 23) == 11 &&
            StudentHashTable::secondaryHash("Ёж-0001", 23) == 4,
            "Неверное хеширование байтов UTF-8");
    requireInvalidArgument([] { StudentHashTable::primaryHash("x", 0); },
                           "h1 не отклонил нулевой размер");
    requireInvalidArgument([] { StudentHashTable::secondaryHash("x", 1); },
                           "h2 не отклонил размер один");

    const std::vector<std::string> keys = {"1001", "1013", "1098", "0", "0000", "Ёж-0001",
                                           std::string(1000, 'x')};
    for (const std::size_t m : {11u, 23u, 47u, 97u}) {
        for (const auto& key : keys) {
            std::size_t position = StudentHashTable::primaryHash(key, m);
            const auto step = StudentHashTable::secondaryHash(key, m);
            require(step > 0 && step < m && std::gcd(step, m) == 1,
                    "Шаг проб не взаимно прост с размером");
            std::vector<bool> visited(m, false);
            for (std::size_t i = 0; i < m; ++i) {
                require(!visited[position], "Последовательность проб повторилась слишком рано");
                visited[position] = true;
                position = (position + step) % m;
            }
            require(std::all_of(visited.begin(), visited.end(), [](bool x) { return x; }),
                    "Не все ячейки посещены");
        }
    }
}

void testEmptyAndValidation() {
    StudentHashTable table;
    require(table.capacity() == 11 && table.size() == 0 && table.deletedCount() == 0,
            "Неверное начальное состояние");
    require(!table.find("missing") && !table.erase("missing"), "Поиск/удаление в пустой таблице");
    requireInvalidArgument([&] { table.insert(student("")); }, "Пустой ключ принят");
    requireInvalidArgument([&] { table.insert(student("  \t")); }, "Пробельный ключ принят");
    requireInvalidArgument([&] { table.insert(student("42", "")); }, "Пустая группа принята");
    requireInvalidArgument([&] { table.insert(student("42", "g", " \t")); }, "Пустое ФИО принято");
    requireInvalidArgument([&] { table.insert(student("a\nb")); }, "Многострочный ключ принят");
    require(table.size() == 0, "Неверный размер после ошибок проверки");

    const Student a = student("0001", "Группа 1", "Иванова Мария Ивановна");
    const Student b = student("1", "Группа 2", "Петров Алексей");
    const Student c = student("ABC");
    const Student d = student("abc");
    require(table.insert(a).inserted && table.insert(b).inserted &&
            table.insert(c).inserted && table.insert(d).inserted,
            "Различные строковые ключи должны вставляться");
    for (const auto& item : {a, b, c, d}) requireStudent(table, item);

    std::ostringstream records;
    table.printRecords(records);
    require(records.str().find("Иванова Мария Ивановна") != std::string::npos,
            "Нет записи в выводе массива");
    std::ostringstream slots;
    table.printTable(slots);
    require(slots.str().find("Индекс ячейки | Состояние | Ключ | h1 | h2 | Индекс записи") !=
                std::string::npos,
            "Нет заголовка хеш-таблицы");
}

void testCollisionDeletionAndReuse() {
    StudentHashTable table;
    const Student first = student("1001");
    const Student middle = student("1013");
    const Student last = student("1098", "ИКБО-17-25", "Сидоров Алексей");
    const auto a = table.insert(first);
    const auto b = table.insert(middle);
    const auto c = table.insert(last);
    require(a.tableIndex == 6 && b.tableIndex == 10 && c.tableIndex == 0,
            "Неверное разрешение коллизии двойным хешированием");
    requireStudent(table, last);

    require(table.erase("1001"), "Не удалось удалить первую запись последовательности проб");
    require(table.deletedCount() == 1 && !table.find("1001"), "Неверная метка удаления");
    requireStudent(table, last); // Его поиск проходит через удалённую ячейку 6.
    const auto duplicate = table.insert(student("1098", "Другая группа", "Другое ФИО"));
    require(!duplicate.inserted && duplicate.rehash == RehashKind::none,
            "Дубликат после метки удаления не распознан");
    require(table.size() == 2 && table.deletedCount() == 1, "Дубликат изменил состояние");
    requireStudent(table, last);

    const auto restored = table.insert(first);
    require(restored.inserted && restored.tableIndex == 6 && restored.recordIndex == a.recordIndex,
            "Место после удаления не использовано повторно");
    require(table.deletedCount() == 0 && table.size() == 3, "Неверные счётчики после вставки");
    requireStudent(table, first);
    requireStudent(table, middle);
    requireStudent(table, last);
    require(table.erase("1013") && table.erase("1098") && table.erase("1001"),
            "Не удалось удалить столкнувшиеся ключи");
    require(!table.erase("1001") && table.size() == 0, "Повторное удаление должно быть неуспешным");
}

void testGrowthAndRehash() {
    StudentHashTable table;
    const std::vector<std::string> keys = {"1001", "1098", "1013", "2024001",
                                            "2024002", "2024003", "2024004"};
    std::unordered_map<std::string, std::size_t> originalIndices;
    for (const auto& key : keys) {
        const auto result = table.insert(student(key));
        require(result.inserted && result.rehash == RehashKind::none, "Преждевременное расширение");
        originalIndices[key] = result.recordIndex;
    }
    require(table.capacity() == 11 && table.size() == 7, "Порог заполнения 7/11");
    const auto duplicate = table.insert(student("1001"));
    require(!duplicate.inserted && duplicate.rehash == RehashKind::none && table.capacity() == 11,
            "Дубликат не должен вызывать расширение");

    const auto added = table.insert(student("8888"));
    require(added.inserted && added.rehash == RehashKind::grow &&
            added.oldCapacity == 11 && added.newCapacity == 23 && table.size() == 8,
            "Ожидалось расширение и рехеширование 11 -> 23");
    for (const auto& key : keys) {
        requireStudent(table, student(key));
        require(table.find(key)->recordIndex == originalIndices.at(key),
                "Индекс полезных данных изменился при рехешировании");
    }
    requireStudent(table, student("8888"));
    require(table.erase("1001"), "Удаление после рехеширования");
    requireStudent(table, student("1098"));
}

void testTombstoneCleanup() {
    StudentHashTable table;
    for (int i = 1; i <= 7; ++i) require(table.insert(student(std::to_string(i))).inserted, "Вставка");
    const auto fourthIndex = table.find("4")->recordIndex;
    require(table.erase("1") && table.erase("2") && table.erase("3"), "Удаление трёх ключей");
    require(table.size() == 4 && table.deletedCount() == 3, "Счётчики меток удаления");
    const auto result = table.insert(student("8"));
    require(result.inserted && result.rehash == RehashKind::compact &&
            result.oldCapacity == 11 && result.newCapacity == 11 && table.deletedCount() == 0,
            "Ожидалась очистка меток рехешированием без расширения");
    for (int i = 4; i <= 8; ++i) requireStudent(table, student(std::to_string(i)));
    require(table.find("4")->recordIndex == fourthIndex, "Индекс изменился при очистке меток");
}

void testDemoData() {
    StudentHashTable table;
    const auto demo = siaod::demoStudents();
    require(demo.size() == 6, "В демонстрационном наборе должно быть 6 записей");
    for (const auto& item : demo) require(table.insert(item).inserted, "Повтор ключа в наборе");
    require(table.capacity() == 11, "Демо-набор не должен вызывать расширение");
    for (const auto& item : demo) requireStudent(table, item);
    require(table.find("1001")->tableIndex == 6 && table.find("1013")->tableIndex == 10 &&
            table.find("1098")->tableIndex == 0, "Демо-коллизия отсутствует");
}

void testRandomizedAgainstReference() {
    StudentHashTable table;
    std::unordered_map<std::string, Student> reference;
    std::unordered_map<std::string, std::size_t> recordIndices;
    std::mt19937_64 random(20261029);
    constexpr int keyCount = 300;

    for (int iteration = 0; iteration < 12000; ++iteration) {
        const std::string key = "K" + std::to_string(random() % keyCount);
        const auto operation = random() % 3;
        if (operation == 0) {
            const Student value = student(key, "G" + std::to_string(iteration),
                                          "Student " + std::to_string(iteration));
            const bool existed = reference.find(key) != reference.end();
            const auto result = table.insert(value);
            require(result.inserted != existed, "Расхождение вставки с эталоном");
            if (!existed) {
                reference.emplace(key, value);
                recordIndices.emplace(key, result.recordIndex);
            } else {
                require(result.rehash == RehashKind::none &&
                        result.recordIndex == recordIndices.at(key), "Дубликат изменил таблицу");
            }
        } else if (operation == 1) {
            const bool erased = table.erase(key);
            const bool expected = reference.erase(key) != 0;
            require(erased == expected, "Расхождение удаления с эталоном");
            if (expected) recordIndices.erase(key);
        } else {
            const auto found = table.find(key);
            const auto expected = reference.find(key);
            require(found.has_value() == (expected != reference.end()), "Расхождение поиска с эталоном");
            if (found) requireStudent(table, expected->second);
        }

        require(table.size() == reference.size(), "Расхождение числа записей");
        require(table.size() + table.deletedCount() <= table.capacity(), "Неверное число ячеек");
        require(prime(table.capacity()), "Размер таблицы не простое число");
        if (iteration % 100 == 0) {
            for (int i = 0; i < keyCount; ++i) {
                const std::string candidate = "K" + std::to_string(i);
                const auto expected = reference.find(candidate);
                const auto found = table.find(candidate);
                require(found.has_value() == (expected != reference.end()), "Пропущенный ключ");
                if (found) {
                    requireStudent(table, expected->second);
                    require(found->recordIndex == recordIndices.at(candidate),
                            "Индекс живой записи неожиданно изменился");
                }
            }
        }
    }
    for (const auto& item : reference) {
        require(table.erase(item.first), "Не удалось очистить таблицу");
    }
    require(table.size() == 0 && !table.find("K1"), "Таблица должна быть пустой");
    require(table.insert(student("new")).inserted, "Вставка после полной очистки");
    requireStudent(table, student("new"));
}

} // namespace

int main() {
    try {
        testHashesAndProbeCoverage();
        testEmptyAndValidation();
        testCollisionDeletionAndReuse();
        testGrowthAndRehash();
        testTombstoneCleanup();
        testDemoData();
        testRandomizedAgainstReference();
        std::cout << "Все проверки хеш-таблицы пройдены.\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Ошибка теста: " << error.what() << '\n';
        return 1;
    }
}
