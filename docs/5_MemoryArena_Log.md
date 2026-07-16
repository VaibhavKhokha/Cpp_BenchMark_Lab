# Algorithm Log 05: Memory Arenas
**Author:** Vaibhav Khokha   
**Project:** C++ Systems Performance Lab  
**Environment:** MSVC / x86 / Release Mode / 20 Logical Cores

## Introduction
After optimizing computation using cache-aware algorithms, multi-threading and AVX2 SIMD instructions, I turned my attention to memory allocation. While the computational kernels had become significantly faster, repeatedly allocating large matrices still introduced overhead. To reduce this cost, I impemented a Linear Memory Arena that performs a single large allocation up front and then services subsequent allocations from that pre-allocated memory block.

## Building the Memory Arena
General-purpose allocators such as `new` and `malloc` are designed to handle a wide variety of allocation patterns. While flexible, repeated allocation and deallocation of large matrices introduces additional overhead during benchmarking. To reduce this cost, I implemented a Linear Memory Arena.
* **The Architecture:** At program startup, the arena performs one large memory allocation (for example, 500 MB) and treats it as a contiguous pool for future allocations.
* **The Execution:** Whenever a matrix is needed, the arena returns a pointer to the next available region within this pool and advances an internal offset. Since allocations occur sequentially, no complex bookkeeping or per-allocation heap management is required.
* **The Reset:** Rather than freeing each matrix individually, the arena simply resets the offset back to the beginning of the buffer. This makes deallocation effectively constant-time while allowing the memory to be reused for the next benchmark iteration.

## Integrating the Arena with the Matrix Implementation
Since the memory arena returns raw pointers, the matrix multiplication routines were modified to operate directly on contiguous arrays instead of `std::vector` objects.
* **Replacing Containers:** Function parameters were changed from `const std::vector<int>&` to raw pointers (`const int*` and `int*`), allowing the computational kernels to access memory supplied by the arena.
* **Initialization:** Matrix data was initialized using `std::fill_n`, which operates directly on contiguous memory blocks returned by the allocator.
* **SIMD Compatibility:** Because AVX2 intrinsics such as `_mm256_loadu_si256` operate on memory addresses rather than C++ containers, the vectorized implementation required only minimal modification after the transition to arena-managed memory.

## Benchmark Results and Analysis
To evaluate the impact of the custom allocator, I compared the arena-backed implementation with the original `std::vector`-based implementation across matrix sizes up to **N = 2048**. At this size, each matrix contains over 4 million elements and the multiplication performs approximately **8.58 billion** arithmetic operations.
* **Vector-Based Allocation:** Execution time increased as matrix size grew, reflecting both computation and the repeated cost of dynamic memory allocation.
* **Arena-Based Allocation:** 181,826 µs (0.18 seconds).
* **Observation:** The arena-backed implementation consistently outperformed the vector-based implementation for larger matrices. Since both versions executed the same matrix multiplication algorithm, the improvement can largely be attributed to reducing repeated allocation and deallocation overhead during benchmarking.

## Final Thoughts
This phase demonstrated that memory allocation strategy can significantly influence the performance of computation-intensive applications. By replacing repeated dynamic allocations with linear memory arena, I reduced allocation overhead and improved memory reuse while leaving the computational kernel unchanged.
