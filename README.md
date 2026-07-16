# Cpp_Benchmark_Lab

A high-performance **C++20** systems programming project focused on understanding how modern hardware affects algorithm performance.

Rather than simply implementing algorithms, this project explores how software interacts with the CPU, memory hierarchy, operating system, and storage subsystem through progressively optimized implementations. Each module includes benchmark results, performance analysis, and architectural observations.

---

## Features

- Matrix multiplication benchmarking
- Cache-aware algorithm optimization
- Multi-threaded implementations
- AVX2 SIMD vectorization
- Custom Linear Memory Arena
- Image processing optimization
- Branchless sorting algorithms
- Memory-mapped file I/O
- Automated benchmarking
- Performance visualization using Python

---

## Optimization Pipeline

The project documents a complete optimization journey across multiple optimization modules.

| Log | Topic |
|-----:|-------|
| 01 | Matrix Multiplication Baseline |
| 02 | Cache Optimization |
| 03 | Multi-Threading |
| 04 | AVX2 SIMD Vectorization |
| 05 | Linear Memory Arena |
| 06 | Image Grayscale Optimization |
| 07 | Branchless Sorting |
| 08 | Memory-Mapped Disk I/O |

Each optimization stage includes:

- Implementation details
- Benchmark results
- Performance analysis
- Hardware observations
- Visualization plots

---

## Topics Covered

- CPU cache locality
- Memory hierarchy
- Thread-level parallelism
- SIMD vectorization (AVX2)
- Branch prediction
- Memory bandwidth
- Custom memory allocators
- Branchless programming
- Memory-mapped files
- Benchmark methodology
- Systems programming
- Performance engineering

---

## Technologies

### Languages

- C++20
- Python

### Libraries

- C++ Standard Library
- AVX2 Intrinsics (`immintrin.h`)
- Win32 API
- Pandas
- Matplotlib

### Build System

- CMake 3.15+
- MSVC (Visual Studio 2022 recommended)

---

## Repository Structure

```text
Cpp_Benchmark_Lab/
│
├── include/               # Header files
│
├── src/
│   ├── algorithms/        # Algorithm implementations
│   ├── benchmark/         # Benchmark framework
│   ├── utils/             # Thread Pool, Memory Arena, utilities
|   └── IO/                # File Parsing & I/O
│
├── docs/                  # Optimization logs
│
├── results/               # Benchmark CSV files
│
├── plots/                 # Generated graphs
│
├── scripts/               # Python visualization scripts
│
├── CMakeLists.txt
├── LICENSE
└── README.md
```

---

## Building

### Requirements

- C++20 compatible compiler
- CMake 3.15 or newer
- Visual Studio 2022 (recommended)

### Clone the Repository

```bash
git clone https://github.com/VaibhavKhokha/Cpp_Benchmark_Lab.git

cd Cpp_Benchmark_Lab
```

### Configure

```bash
cmake -S . -B build
```

### Build

```bash
cmake --build build --config Release
```

> **Note:** All benchmarks should be executed in **Release** mode. Debug builds disable many compiler optimizations and do not produce representative performance measurements.

---

## Running Benchmarks

Execute the generated executable.

Benchmark results are exported as CSV files in the `results/` directory.

The Python scripts inside `scripts/` can then be used to generate performance visualizations in the `plots/` directory.

---

## Benchmark Methodology

Benchmarks are performed using:

- Multiple benchmark iterations
- High-resolution timers
- Large workloads
- Best execution time recorded
- Release-mode compilation

Performance is analyzed using generated CSV files and visualization scripts.

---

## Learning Objectives

This project explores practical aspects of systems programming and performance engineering, including:

- Cache-aware programming
- Performance optimization
- SIMD programming with AVX2
- Multi-threading
- Custom memory management
- Branch prediction
- Operating system interaction
- High-performance file I/O
- Benchmark design and analysis

---

## License

This project is licensed under the **MIT License**.

See the [LICENSE](LICENSE) file for more information.

---

## Author

**Vaibhav Khokha**

Systems Programming • Performance Engineering • Modern C++