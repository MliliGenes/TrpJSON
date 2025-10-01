#include "../include/parser/TrpJsonParser.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#include <algorithm>

// High-precision timer for C++98
class Timer {
private:
    struct timeval start_time;
    struct timeval end_time;

public:
    void start() {
        gettimeofday(&start_time, NULL);
    }
    
    void stop() {
        gettimeofday(&end_time, NULL);
    }
    
    // Returns elapsed time in microseconds
    double elapsed() const {
        return (end_time.tv_sec - start_time.tv_sec) * 1000000.0 + 
               (end_time.tv_usec - start_time.tv_usec);
    }
    
    // Returns elapsed time in milliseconds
    double elapsedMs() const {
        return elapsed() / 1000.0;
    }
};

// Convert number to string (C++98 compatible)
std::string toString(double value, int precision = 2) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}

std::string toString(size_t value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

class JsonBenchmark {
private:
    struct BenchmarkResult {
        std::string testName;
        size_t fileSize;
        double parseTime;
        double serializeTime;
        size_t memoryUsed;
        bool success;
    };

    std::vector<BenchmarkResult> results;

public:
    void runBenchmark(const std::string& filename, const std::string& testName, int iterations) {
        std::cout << "\n=== Benchmarking: " << testName << " ===" << std::endl;
        
        // Get file size
        std::ifstream file(filename.c_str(), std::ios::ate | std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open file " << filename << std::endl;
            return;
        }
        
        size_t fileSize = file.tellg();
        file.close();
        
        std::cout << "File size: " << fileSize << " bytes" << std::endl;
        std::cout << "Running " << iterations << " iterations..." << std::endl;
        
        double totalParseTime = 0.0;
        double totalSerializeTime = 0.0;
        int successfulRuns = 0;
        
        for (int i = 0; i < iterations; ++i) {
            Timer parseTimer;
            
            // Parse timing
            parseTimer.start();
            TrpJsonParser parser(filename);
            bool parseSuccess = parser.parse();
            parseTimer.stop();
            
            if (!parseSuccess) {
                std::cerr << "Parse failed on iteration " << i << std::endl;
                continue;
            }
            
            // Serialize timing
            Timer serializeTimer;
            serializeTimer.start();
            std::string output = parser.astToString();
            serializeTimer.stop();
            
            totalParseTime += parseTimer.elapsed();
            totalSerializeTime += serializeTimer.elapsed();
            successfulRuns++;
            
            // Progress indicator
            if ((i + 1) % 10 == 0) {
                std::cout << "Completed " << (i + 1) << "/" << iterations << " iterations\r" << std::flush;
            }
        }
        
        if (successfulRuns == 0) {
            std::cerr << "All benchmark iterations failed!" << std::endl;
            return;
        }
        
        double avgParseTime = totalParseTime / successfulRuns;
        double avgSerializeTime = totalSerializeTime / successfulRuns;
        
        BenchmarkResult result;
        result.testName = testName;
        result.fileSize = fileSize;
        result.parseTime = avgParseTime / 1000.0; // Convert to milliseconds
        result.serializeTime = avgSerializeTime / 1000.0;
        result.memoryUsed = 0; // Will be filled by memory profiling
        result.success = true;
        
        results.push_back(result);
        
        std::cout << std::endl;
        std::cout << "Average parse time: " << std::fixed << std::setprecision(3) 
                  << result.parseTime << " ms" << std::endl;
        std::cout << "Average serialize time: " << std::fixed << std::setprecision(3) 
                  << result.serializeTime << " ms" << std::endl;
        std::cout << "Throughput: " << std::fixed << std::setprecision(2) 
                  << (fileSize / 1024.0 / 1024.0) / (result.parseTime / 1000.0) << " MB/s" << std::endl;
    }
    
    void generateReport() {
        std::cout << "\n" << std::string(80, '=') << std::endl;
        std::cout << "BENCHMARK RESULTS SUMMARY" << std::endl;
        std::cout << std::string(80, '=') << std::endl;
        
        std::cout << std::left 
                  << std::setw(25) << "Test Name"
                  << std::setw(12) << "Size"
                  << std::setw(15) << "Parse Time"
                  << std::setw(15) << "Serialize"
                  << std::setw(13) << "Throughput"
                  << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        
        for (size_t i = 0; i < results.size(); ++i) {
            const BenchmarkResult& result = results[i];
            std::cout << std::left 
                      << std::setw(25) << result.testName
                      << std::setw(12) << formatSize(result.fileSize)
                      << std::setw(15) << (toString(result.parseTime) + " ms")
                      << std::setw(15) << (toString(result.serializeTime) + " ms")
                      << std::setw(13) << (toString((result.fileSize / 1024.0 / 1024.0) / (result.parseTime / 1000.0)) + " MB/s")
                      << std::endl;
        }
        
        std::cout << std::string(80, '=') << std::endl;
    }
    
    void generateWebsiteData() {
        std::ofstream htmlFile("benchmark_results.html");
        if (!htmlFile.is_open()) {
            std::cerr << "Cannot create HTML output file" << std::endl;
            return;
        }
        
        htmlFile << "<!-- Benchmark Results Generated by TrpJSON Benchmarker -->" << std::endl;
        htmlFile << "<div class=\"benchmark-grid\">" << std::endl;
        
        for (size_t i = 0; i < results.size(); ++i) {
            const BenchmarkResult& result = results[i];
            htmlFile << "    <div class=\"benchmark-card\">" << std::endl;
            htmlFile << "        <div class=\"benchmark-value\">" << std::fixed << std::setprecision(2) 
                     << result.parseTime << "ms</div>" << std::endl;
            htmlFile << "        <div class=\"benchmark-label\">" << result.testName 
                     << " (" << formatSize(result.fileSize) << ")</div>" << std::endl;
            htmlFile << "        <div class=\"progress-bar\">" << std::endl;
            htmlFile << "            <div class=\"progress-fill\" style=\"width: " 
                     << std::min(100.0, (result.parseTime / 50.0) * 100) << "%\"></div>" << std::endl;
            htmlFile << "        </div>" << std::endl;
            htmlFile << "    </div>" << std::endl;
        }
        
        htmlFile << "</div>" << std::endl;
        htmlFile.close();
        
        std::cout << "Website benchmark data written to benchmark_results.html" << std::endl;
    }

private:
    std::string formatSize(size_t bytes) {
        if (bytes < 1024) return toString(bytes) + "B";
        if (bytes < 1024 * 1024) return toString(bytes / 1024) + "KB";
        return toString(bytes / (1024 * 1024)) + "MB";
    }
};

int main() {
    std::cout << "TrpJSON Parser Benchmark Suite" << std::endl;
    std::cout << "Compilation flags: -std=c++98 -O2" << std::endl;
    
    JsonBenchmark benchmark;
    
    // Test different JSON file sizes and types
    std::vector<std::pair<std::string, std::string> > testFiles;
    testFiles.push_back(std::make_pair("benchmark/data/simple_config.json", "Simple Config"));
    testFiles.push_back(std::make_pair("benchmark/data/api_response.json", "API Response"));
    testFiles.push_back(std::make_pair("benchmark/data/large_dataset.json", "Large Dataset"));
    testFiles.push_back(std::make_pair("benchmark/data/nested_complex.json", "Complex Nested"));
    
    for (size_t i = 0; i < testFiles.size(); ++i) {
        const std::string& filename = testFiles[i].first;
        const std::string& testName = testFiles[i].second;
        
        std::ifstream testFile(filename.c_str());
        if (testFile.good()) {
            benchmark.runBenchmark(filename, testName, 1000); // 1000 iterations for accuracy
        } else {
            std::cout << "Skipping " << testName << " - file not found: " << filename << std::endl;
        }
        testFile.close();
    }
    
    benchmark.generateReport();
    benchmark.generateWebsiteData();
    
    std::cout << "\nTo run memory profiling:" << std::endl;
    std::cout << "valgrind --tool=massif --stacks=yes ./benchmark" << std::endl;
    std::cout << "\nTo run performance profiling:" << std::endl;
    std::cout << "perf record -g ./benchmark && perf report" << std::endl;
    
    return 0;
}