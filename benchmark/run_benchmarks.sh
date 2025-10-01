#!/bin/bash

# TrpJSON Benchmark Suite with Valgrind Profiling
# This script runs comprehensive performance analysis using valgrind and other tools

set -e

echo "================================================="
echo "TrpJSON Parser - Performance Analysis Suite"
echo "================================================="

# Build the benchmark executable
echo "Building benchmark executable..."
cd "$(dirname "$0")/.."
make benchmark

# Create results directory
mkdir -p benchmark/results
cd benchmark

echo -e "\n1. Running basic benchmark..."
./benchmark > results/benchmark_basic.txt 2>&1

echo -e "\n2. Running memory profiling with Valgrind Massif..."
valgrind --tool=massif --stacks=yes --massif-out-file=results/massif.out ./benchmark > results/benchmark_memory.txt 2>&1

echo -e "\n3. Running cache profiling with Valgrind Cachegrind..."
valgrind --tool=cachegrind --cachegrind-out-file=results/cachegrind.out ./benchmark > results/benchmark_cache.txt 2>&1

echo -e "\n4. Running call graph profiling with Valgrind Callgrind..."
valgrind --tool=callgrind --callgrind-out-file=results/callgrind.out ./benchmark > results/benchmark_callgrind.txt 2>&1

echo -e "\n5. Analyzing memory usage with Massif..."
ms_print results/massif.out > results/memory_analysis.txt

echo -e "\n6. Analyzing cache performance..."
cg_annotate results/cachegrind.out > results/cache_analysis.txt

echo -e "\n7. Running performance profiling with perf (if available)..."
if command -v perf &> /dev/null; then
    perf record -g --output=results/perf.data ./benchmark > results/benchmark_perf.txt 2>&1
    perf report --input=results/perf.data --stdio > results/perf_analysis.txt 2>/dev/null
    echo "Perf analysis complete."
else
    echo "Perf not available, skipping performance profiling."
fi

echo -e "\n8. Generating memory leak report..."
valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./benchmark > results/memcheck.txt 2>&1

echo -e "\n9. Generating comprehensive report..."
cat > results/performance_summary.md << 'EOF'
# TrpJSON Parser Performance Analysis

## Overview
This report contains comprehensive performance analysis of the TrpJSON parser using various profiling tools.

## Test Environment
- Compiler: g++ with -std=c++98 -O2 flags
- System: Linux x86_64
- Tools: Valgrind (Massif, Cachegrind, Callgrind, Memcheck), Perf

## Files Generated
1. `benchmark_basic.txt` - Basic benchmark results
2. `benchmark_memory.txt` - Memory profiling output  
3. `benchmark_cache.txt` - Cache profiling output
4. `benchmark_callgrind.txt` - Call graph profiling output
5. `memory_analysis.txt` - Detailed memory usage analysis
6. `cache_analysis.txt` - Cache performance analysis
7. `memcheck.txt` - Memory leak detection results
8. `perf_analysis.txt` - CPU performance analysis (if available)

## Key Metrics Measured
- **Parse Time**: Average time to parse JSON files (microsecond precision)
- **Memory Usage**: Peak memory consumption and allocation patterns
- **Cache Performance**: L1/L2/L3 cache hit rates and miss penalties
- **Call Graph**: Function call frequency and execution time distribution
- **Memory Safety**: Leak detection and bounds checking

## Usage
Run individual analyses:
```bash
# Memory profiling
valgrind --tool=massif --stacks=yes ./benchmark

# Cache analysis  
valgrind --tool=cachegrind ./benchmark

# Call graph profiling
valgrind --tool=callgrind ./benchmark

# Memory leak detection
valgrind --tool=memcheck --leak-check=full ./benchmark

# CPU profiling (requires perf)
perf record -g ./benchmark && perf report
```

## Interpreting Results
- Lower parse times indicate better performance
- Lower memory usage indicates efficiency
- Higher cache hit rates indicate better memory access patterns
- Zero memory leaks indicate proper resource management
EOF

echo -e "\nPerformance analysis complete!"
echo "Results saved in benchmark/results/"
echo ""
echo "Key files:"
echo "- benchmark/results/performance_summary.md"
echo "- benchmark/results/benchmark_basic.txt"  
echo "- benchmark/results/memory_analysis.txt"
echo "- benchmark/results/cache_analysis.txt"
echo ""
echo "To view memory usage graphically:"
echo "  massif-visualizer benchmark/results/massif.out"
echo ""
echo "To view call graph:"
echo "  kcachegrind benchmark/results/callgrind.out"