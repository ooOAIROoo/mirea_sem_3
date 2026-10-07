#include "siaod/Console.hpp"
#include "siaod/RecordFile.hpp"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>

namespace {

std::uint64_t randomSeed() {
    std::random_device source;
    return (static_cast<std::uint64_t>(source()) << 32u) ^ source();
}

std::size_t parseCount(const std::string& text) {
    std::size_t used = 0;
    const auto value = std::stoull(text, &used);
    if (used != text.size() || value == 0 || value > 1'000'000) {
        throw std::invalid_argument("Количество должно быть от 1 до 1 000 000");
    }
    return static_cast<std::size_t>(value);
}

void printRecord(const siaod::CarRecord& record, std::size_t position) {
    std::cout << "Запись " << position + 1 << ":\n"
              << "  Номер автомобиля: " << siaod::getCarNumber(record) << '\n'
              << "  Марка:            " << siaod::getCarMake(record) << '\n'
              << "  Владелец:         " << siaod::getOwnerInfo(record) << "\n\n";
}

void generate(std::size_t count, std::uint64_t seed) {
    const auto binary = siaod::createGeneratedDataset(count, seed, "data", "records");
    siaod::BinaryRecordFile file(binary);
    std::cout << "Создан текстовый файл: data/records.txt\n"
              << "Текст преобразован в бинарный файл: " << binary.string() << '\n'
              << "Число записей: " << file.recordCount() << '\n'
              << "Размер одной записи: " << sizeof(siaod::CarRecord) << " байт\n"
              << "Размер бинарного файла: "
              << file.recordCount() * sizeof(siaod::CarRecord) << " байт\n";
}

void showFirst(std::size_t limit) {
    const std::filesystem::path binary = "data/records.bin";
    if (!std::filesystem::exists(binary)) {
        std::cout << "Файл не найден. Сначала выберите создание записей.\n";
        return;
    }

    siaod::BinaryRecordFile file(binary);
    std::cout << "В файле " << file.recordCount() << " записей. Первые записи:\n\n";
    const auto count = std::min(limit, file.recordCount());
    for (std::size_t i = 0; i < count; ++i) {
        const auto record = file.readAt(i);
        if (record) {
            printRecord(*record, i);
        }
    }
}

void printHelp() {
    std::cout << "Использование:\n"
              << "  task_1_app                 интерактивное меню\n"
              << "  task_1_app --generate N [seed]\n"
              << "                             создать N строк текста, затем бинарный файл\n"
              << "  task_1_app --show [limit]  вывести первые записи\n"
              << "  task_1_app --help          показать эту справку\n";
}

void runMenu() {
    while (true) {
        std::cout << "\nЗадание 1 — создание бинарного файла\n"
                  << "1. Сгенерировать записи и создать binary-файл (100 записей)\n"
                  << "2. Показать записи из data/records.bin\n"
                  << "0. Выход\nВыбор: ";
        std::string choice;
        if (!std::getline(std::cin, choice)) {
            return;
        }
        if (choice == "0") {
            return;
        }
        if (choice == "1") {
            try {
                generate(100, randomSeed());
            } catch (const std::exception& error) {
                std::cerr << "Ошибка: " << error.what() << '\n';
            }
        } else if (choice == "2") {
            showFirst(10);
        } else {
            std::cout << "Неизвестный пункт меню.\n";
        }
    }
}

} // namespace

int main(int argc, char* argv[]) {
    siaod::configureUtf8Console();
    try {
        if (argc == 1) {
            runMenu();
            return 0;
        }

        const std::string command = argv[1];
        if (command == "--help" || command == "-h") {
            printHelp();
            return 0;
        }
        if (command == "--generate") {
            if (argc < 3 || argc > 4) {
                printHelp();
                return 2;
            }
            const auto count = parseCount(argv[2]);
            const auto seed = argc == 4 ? std::stoull(argv[3]) : randomSeed();
            generate(count, seed);
            return 0;
        }
        if (command == "--show") {
            const auto limit = argc >= 3 ? parseCount(argv[2]) : 10;
            showFirst(limit);
            return 0;
        }

        printHelp();
        return 2;
    } catch (const std::exception& error) {
        std::cerr << "Ошибка: " << error.what() << '\n';
        return 1;
    }
}
