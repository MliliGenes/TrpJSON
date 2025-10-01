# TrpJSON Performance Benchmarking System

This directory contains a comprehensive benchmarking suite for the TrpJSON parser that provides **legitimate performance metrics** using professional profiling tools.

## Quick Start

```bash
# Build and run basic benchmarks
make benchmark
./benchmark/benchmark

# Run comprehensive analysis with valgrind
make run-benchmarks
```

## What Makes These Benchmarks Legitimate

### 1. High-Precision Timing
- Uses `gettimeofday()` for **microsecond precision** timing
- Runs **1000 iterations** per test for statistical accuracy
- Measures both parsing and serialization separately
- Calculates averages to eliminate outliers

### 2. Real Memory Profiling
- **Valgrind Massif**: Tracks actual memory allocations and peak usage
- **Valgrind Memcheck**: Detects memory leaks and errors
- **Heap analysis**: Shows allocation patterns and efficiency

### 3. Cache Performance Analysis
- **Valgrind Cachegrind**: Measures L1/L2/L3 cache hit rates
- **Cache miss analysis**: Identifies memory access bottlenecks
- **Performance counters**: Real hardware-level metrics

### 4. CPU Profiling
- **Valgrind Callgrind**: Function-level performance profiling
- **Perf integration**: Hardware performance counters (if available)
- **Call graph analysis**: Hotspot identification

## Benchmark Test Suite

### Test Files
1. **Simple Config** (625B): Basic JSON configuration file
2. **API Response** (1.5KB): Typical REST API response with nested objects
3. **Large Dataset** (3.3KB): Complex data structure with arrays and deep nesting
4. **Complex Nested** (3.5KB): Deeply nested objects testing parser limits

### Metrics Measured
- **Parse Time**: Time to build AST from JSON string (ms)
- **Serialize Time**: Time to convert AST back to string (ms)  
- **Throughput**: MB/s processing rate
- **Memory Usage**: Peak heap allocation (KB)
- **Cache Performance**: Hit rates and miss penalties

## Running Benchmarks

### Basic Benchmarks
```bash
make benchmark          # Build benchmark executable
./benchmark/benchmark   # Run basic timing tests
```

### Memory Analysis
```bash
# Memory usage profiling
valgrind --tool=massif --stacks=yes ./benchmark/benchmark
ms_print massif.out.*

# Memory leak detection
valgrind --tool=memcheck --leak-check=full ./benchmark/benchmark
```

### Cache Analysis
```bash
# Cache performance profiling
valgrind --tool=cachegrind ./benchmark/benchmark
cg_annotate cachegrind.out.*
```

### CPU Profiling
```bash
# Function-level profiling
valgrind --tool=callgrind ./benchmark/benchmark
kcachegrind callgrind.out.*  # GUI analysis

# Hardware counters (if perf available)
perf record -g ./benchmark/benchmark
perf report
```

### Comprehensive Analysis
```bash
./benchmark/run_benchmarks.sh   # Runs all analyses and generates report
```

## Results Format

### Console Output
```
=== Benchmarking: Simple Config ===
File size: 625 bytes
Running 1000 iterations...
Average parse time: 0.081 ms
Average serialize time: 0.025 ms  
Throughput: 7.34 MB/s
```

### Memory Report (Massif)
- Peak memory usage
- Allocation timeline
- Heap growth patterns
- Stack trace of allocations

### Cache Report (Cachegrind)
- L1 instruction cache: hit rate, miss rate
- L1 data cache: hit rate, miss rate
- L2 unified cache: hit rate, miss rate
- Cache line utilization

## Interpreting Results

### Performance Metrics
- **Sub-millisecond parsing**: Indicates efficient implementation
- **High throughput (>10 MB/s)**: Good for real-time applications
- **Low memory usage**: Memory-efficient parser
- **Zero memory leaks**: Proper resource management

### Memory Patterns
- **Linear growth**: Indicates O(n) memory complexity
- **Low fragmentation**: Efficient memory allocation
- **Small peak usage**: Memory-conscious design

### Cache Performance
- **High hit rates (>95%)**: Good memory locality
- **Low miss penalties**: Efficient data structures
- **Sequential access patterns**: Cache-friendly code

## Building for Optimization

The benchmarks use optimized builds:
```bash
# Compiler flags used
-std=c++98 -O2 -Wall -Wextra -Werror
```

For maximum performance profiling:
```bash
# Release build with debug symbols
CXXFLAGS="-O3 -DNDEBUG -g" make benchmark
```

## Integration with Website

The benchmark system automatically generates data for the project website:

```bash
./update_website_benchmarks.sh  # Updates html/index.html with real metrics
```

This replaces placeholder values with actual performance measurements.

## Continuous Integration

Add to CI/CD pipeline:
```yaml
- name: Run Performance Benchmarks
  run: |
    make benchmark
    ./benchmark/run_benchmarks.sh
    # Upload results as artifacts
```

## System Requirements

- Linux/Unix system with:
  - `valgrind` (memory/cache/cpu profiling)
  - `perf` (optional, hardware counters)
  - `bc` (calculations)
  - C++ compiler with C++98 support

## Files Generated

After running benchmarks:
- `benchmark/results/benchmark_basic.txt` - Basic performance metrics
- `benchmark/results/memory_analysis.txt` - Detailed memory usage
- `benchmark/results/cache_analysis.txt` - Cache performance data
- `benchmark/results/performance_summary.md` - Comprehensive report
- `massif.out.*` - Raw memory profiling data
- `cachegrind.out.*` - Raw cache profiling data
- `callgrind.out.*` - Raw CPU profiling data

These provide **scientific, reproducible performance measurements** rather than approximations.