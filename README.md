# ⚡️ Cpp_BenchMark_Lab

A high-performance C++ benchmarking and systems optimization laboratory. 

This project is not just a collection of algorithms; it is a deep dive into hardware-level optimization. Starting with standard C++ implementations, this lab systematically strips away compiler reliance and OS overhead to push physical silicon to its theoretical limits.

## 🎯 Project Goals
- Implement classic mathematical algorithms and data structures.
- Benchmark execution time across massive datasets (1 Billion+ operations).
- Study and optimize L1/L2 cache spatial locality.
- Eliminate thread-creation overhead using custom Thread Pools.
- Bypass standard C++ utilizing AVX2 SIMD Hardware Intrinsics.
- Eradicate OS Kernel context-switching via Custom Memory Arenas.

## 🚀 The Optimization Journey (Matrix Multiplication)
This lab traces the architectural evolution of an `O(N^3)` matrix multiplication algorithm:
1. **The Baseline:** Standard 3-loop implementation trusting the compiler.
2. **Memory Bandwidth & Cache:** Loop reordering (`i-k-j`) to optimize for CPU cache lines, preventing cache misses.
3. **Hardware Concurrency:** Horizontal matrix slicing distributed across a custom 20-logical-core Thread Pool.
4. **Silicon Intrinsics:** Injecting Intel AVX2 instructions (`_mm256`) to process 8 integers per clock cycle, bypassing standard SISD math.
5. **The Memory Arena:** Replacing `std::vector` with a custom pre-allocated memory block to bypass Windows OS memory allocation latency (User Mode vs. Kernel Mode). 

*Result: Achieved a massive multi-magnitude speedup, dropping 8.5+ Billion operations from seconds down to fractions of a second.*

## 🛠️ Build & Run
This project uses standard C++ features and compiler intrinsics.
1. Clone the repository to your local machine.
2. Open the project in your preferred IDE (e.g., Visual Studio or via CMake).
3. Build the project strictly in **Release Mode**. *(Note: This is critical for accurate hardware benchmarking; Debug mode will completely skew the CPU results).*
4. Execute the binary to run the benchmarks. The program will output `.csv` data logs into the `results/` folder.
5. Run the Python scripts located in `scripts/` to generate the log-log hardware scaling visualizations in the `plots/` folder.

## 📂 Project Structure
```text
Cpp_BenchMark_Lab/
├── src/algorithms/     # Core algorithmic logic (SIMD, standard, etc.)
├── src/benchmark/      # Benchmarking triggers and Timer classes
├── src/utils/          # Custom Memory Arenas and Thread Pools
├── include/            # C++ Header files
├── docs/               # Architecture logs and performance teardowns
└── scripts/            # Python Pandas/Matplotlib visualization scripts
```