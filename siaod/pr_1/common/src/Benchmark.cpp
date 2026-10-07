#include "siaod/Benchmark.hpp"

#include "siaod/RecordFile.hpp"
#include "siaod/Search.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <stdexcept>

namespace siaod {
namespace {

constexpr std::size_t kRecordCounts[] = {100, 1000, 10000};
constexpr std::uint64_t kBenchmarkSeed = 20260415ULL;

std::uint64_t seedForCount(std::size_t count) {
    return kBenchmarkSeed + static_cast<std::uint64_t>(count);
}

std::vector<std::string> makeQueries(const std::vector<CarRecord>& records,
                                    std::size_t queryCount,
                                    std::uint64_t seed) {
    if (records.empty()) {
        throw std::runtime_error("Нельзя выполнить замер на пустом наборе записей");
    }
    std::mt19937_64 engine(seed ^ 0x9E3779B97F4A7C15ULL);
    std::uniform_int_distribution<std::size_t> position(0, records.size() - 1);
    std::vector<std::string> queries;
    queries.reserve(queryCount);
    for (std::size_t i = 0; i < queryCount; ++i) {
        queries.push_back(getCarNumber(records[position(engine)]));
    }
    return queries;
}

template <typename SearchFunction>
BenchmarkResult measure(std::size_t recordCount,
                        const std::string& algorithm,
                        const std::vector<std::string>& queries,
                        std::size_t batches,
                        std::size_t queriesPerBatch,
                        SearchFunction search) {
    if (batches == 0 || queriesPerBatch == 0 || queries.size() < batches * queriesPerBatch) {
        throw std::invalid_argument("Для замера требуется хотя бы один запрос в каждой серии");
    }

    // Прогреваем буферы файловой системы и пути кода, не включая разогрев в статистику.
    const std::size_t warmupCount = std::min<std::size_t>(10, queries.size());
    for (std::size_t i = 0; i < warmupCount; ++i) {
        if (!search(queries[i])) {
            throw std::runtime_error("Контрольный ключ не найден перед замером");
        }
    }

    std::vector<double> batchMeans;
    batchMeans.reserve(batches);
    double totalMicroseconds = 0.0;
    std::size_t successfulQueries = 0;

    for (std::size_t batch = 0; batch < batches; ++batch) {
        const std::size_t firstQuery = batch * queriesPerBatch;
        const auto start = std::chrono::steady_clock::now();
        for (std::size_t i = 0; i < queriesPerBatch; ++i) {
            const auto& key = queries[firstQuery + i];
            if (search(key)) {
                ++successfulQueries;
            }
        }
        const auto stop = std::chrono::steady_clock::now();
        const double elapsed = std::chrono::duration<double, std::micro>(stop - start).count();
        const double batchMean = elapsed / static_cast<double>(queriesPerBatch);
        batchMeans.push_back(batchMean);
        totalMicroseconds += elapsed;
    }

    std::vector<double> sortedBatchMeans = batchMeans;
    std::sort(sortedBatchMeans.begin(), sortedBatchMeans.end());
    double median = sortedBatchMeans[sortedBatchMeans.size() / 2];
    if (sortedBatchMeans.size() % 2 == 0) {
        median = (sortedBatchMeans[sortedBatchMeans.size() / 2 - 1] + median) / 2.0;
    }

    const std::size_t totalQueries = batches * queriesPerBatch;
    return BenchmarkResult{
        recordCount,
        algorithm,
        totalQueries,
        totalMicroseconds / static_cast<double>(totalQueries),
        median,
        *std::min_element(batchMeans.begin(), batchMeans.end()),
        *std::max_element(batchMeans.begin(), batchMeans.end()),
        static_cast<double>(successfulQueries) / static_cast<double>(totalQueries)
    };
}

std::filesystem::path prepareDataset(std::size_t count,
                                     const std::filesystem::path& dataDirectory) {
    const auto seed = seedForCount(count);
    const auto stem = "benchmark_" + std::to_string(count);
    return createGeneratedDataset(count, seed, dataDirectory, stem);
}

} // namespace

std::vector<BenchmarkResult> benchmarkLinearSearch(
    const std::filesystem::path& dataDirectory,
    std::size_t batches,
    std::size_t queriesPerBatch) {
    std::vector<BenchmarkResult> results;
    const std::size_t queryCount = batches * queriesPerBatch;

    for (const std::size_t count : kRecordCounts) {
        BinaryRecordFile file(prepareDataset(count, dataDirectory));
        const auto records = file.readAll();
        const auto queries = makeQueries(records, queryCount, seedForCount(count));
        results.push_back(measure(
            count, "linear_file_scan", queries, batches, queriesPerBatch,
            [&file](const std::string& key) { return file.linearSearch(key).has_value(); }));
    }
    return results;
}

std::vector<BenchmarkResult> benchmarkFibonacciSearch(
    const std::filesystem::path& dataDirectory,
    std::size_t batches,
    std::size_t queriesPerBatch) {
    std::vector<BenchmarkResult> results;
    const std::size_t queryCount = batches * queriesPerBatch;

    for (const std::size_t count : kRecordCounts) {
        BinaryRecordFile file(prepareDataset(count, dataDirectory));
        const auto records = file.readAll();
        const auto queries = makeQueries(records, queryCount, seedForCount(count));
        // Построение индекса однократно, вне измеряемого поиска.
        const auto index = buildKeyOffsetIndex(file);
        results.push_back(measure(
            count, "fibonacci_index_file_read", queries, batches, queriesPerBatch,
            [&file, &index](const std::string& key) {
                return fibonacciSearchRecord(file, index, key).has_value();
            }));
    }
    return results;
}

void writeBenchmarkCsv(const std::filesystem::path& csvPath,
                       const std::vector<BenchmarkResult>& results) {
    if (!csvPath.parent_path().empty()) {
        std::filesystem::create_directories(csvPath.parent_path());
    }
    std::ofstream output(csvPath, std::ios::binary | std::ios::trunc);
    if (!output) {
        throw std::runtime_error("Не удалось создать CSV: " + csvPath.string());
    }

    output << "record_count,algorithm,query_count,mean_us,median_batch_us,"
              "min_batch_us,max_batch_us,hit_rate\n";
    output << std::fixed << std::setprecision(6);
    for (const auto& result : results) {
        output << result.recordCount << ',' << result.algorithm << ',' << result.queryCount << ','
               << result.meanMicroseconds << ',' << result.medianBatchMicroseconds << ','
               << result.minBatchMicroseconds << ',' << result.maxBatchMicroseconds << ','
               << result.hitRate << '\n';
    }
    if (!output) {
        throw std::runtime_error("Ошибка записи CSV: " + csvPath.string());
    }
}

void printBenchmarkResults(const std::vector<BenchmarkResult>& results) {
    std::cout << "\nЧисло записей | Запросов | Среднее, мкс | Медиана серии, мкс | Попадания\n";
    std::cout << "-----------------------------------------------------------------------\n";
    std::cout << std::fixed << std::setprecision(3);
    for (const auto& result : results) {
        std::cout << result.recordCount << "\t\t | "
                  << result.queryCount << "\t | "
                  << result.meanMicroseconds << "\t | "
                  << result.medianBatchMicroseconds << "\t\t | "
                  << std::setprecision(1) << result.hitRate * 100.0 << "%\n"
                  << std::setprecision(3);
    }
}

} // namespace siaod
