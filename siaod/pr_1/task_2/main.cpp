#include "siaod/Benchmark.hpp"
#include "siaod/Console.hpp"
#include "siaod/RecordFile.hpp"

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

const std::filesystem::path kBinaryPath = "data/records.bin";

void ensureDemoFile() {
    if (!std::filesystem::exists(kBinaryPath)) {
        std::cout << "Демонстрационный файл не найден; создаю 100 записей.\n";
        siaod::createGeneratedDataset(100, 2026, "data", "records");
    }
}

void printRecord(const siaod::CarRecord& record) {
    std::cout << "Номер автомобиля: " << siaod::getCarNumber(record) << '\n'
              << "Марка:            " << siaod::getCarMake(record) << '\n'
              << "Владелец:         " << siaod::getOwnerInfo(record) << '\n';
}

void searchByKey(const std::string& key) {
    ensureDemoFile();
    siaod::BinaryRecordFile file(kBinaryPath);
    const auto record = file.linearSearch(key);
    if (record) {
        std::cout << "Запись найдена линейным поиском:\n";
        printRecord(*record);
    } else {
        std::cout << "Запись с номером " << key << " не найдена.\n";
    }
}

void runBenchmark() {
    const auto results = siaod::benchmarkLinearSearch("data");
    siaod::writeBenchmarkCsv("results/linear_search.csv", results);
    siaod::printBenchmarkResults(results);
    std::cout << "CSV сохранен в results/linear_search.csv\n";
}

void printHelp() {
    std::cout << "Использование:\n"
              << "  task_2_app                    интерактивное меню\n"
              << "  task_2_app --search НОМЕР     линейный поиск в бинарном файле\n"
              << "  task_2_app --benchmark        замеры для 100, 1000 и 10000 записей\n"
              << "  task_2_app --help             показать эту справку\n";
}

void runMenu() {
    while (true) {
        std::cout << "\nЗадание 2 — линейный поиск в файле\n"
                  << "1. Найти автомобиль по номеру\n"
                  << "2. Выполнить замеры (100, 1 000, 10 000 записей)\n"
                  << "0. Выход\nВыбор: ";
        std::string choice;
        if (!std::getline(std::cin, choice)) {
            return;
        }
        if (choice == "0") {
            return;
        }
        if (choice == "1") {
            std::cout << "Введите номер, например A123BC77: ";
            std::string key;
            if (std::getline(std::cin, key) && !key.empty()) {
                searchByKey(key);
            }
        } else if (choice == "2") {
            runBenchmark();
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
        if (command == "--search" && argc == 3) {
            searchByKey(argv[2]);
            return 0;
        }
        if (command == "--benchmark" && argc == 2) {
            runBenchmark();
            return 0;
        }
        printHelp();
        return 2;
    } catch (const std::exception& error) {
        std::cerr << "Ошибка: " << error.what() << '\n';
        return 1;
    }
}
