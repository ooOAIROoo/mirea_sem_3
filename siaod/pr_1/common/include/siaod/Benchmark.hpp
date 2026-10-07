#pragma once

#include <cstddef>
#include <filesystem>
#include <string>
#include <vector>

namespace siaod {

struct BenchmarkResult {
    std::size_t recordCount = 0;
    std::string algorithm;
    std::size_t queryCount = 0;
    double meanMicroseconds = 0.0;
    double medianBatchMicroseconds = 0.0;
    double minBatchMicroseconds = 0.0;
    double maxBatchMicroseconds = 0.0;
    double hitRate = 0.0;
};

// В обоих сценариях используются одни и те же размеры и воспроизводимые наборы.
std::vector<BenchmarkResult> benchmarkLinearSearch(
    const std::filesystem::path& dataDirectory,
    std::size_t batches = 5,
    std::size_t queriesPerBatch = 100);

std::vector<BenchmarkResult> benchmarkFibonacciSearch(
    const std::filesystem::path& dataDirectory,
    std::size_t batches = 5,
    std::size_t queriesPerBatch = 100);

void writeBenchmarkCsv(const std::filesystem::path& csvPath,
                       const std::vector<BenchmarkResult>& results);
void printBenchmarkResults(const std::vector<BenchmarkResult>& results);

} // namespace siaod
