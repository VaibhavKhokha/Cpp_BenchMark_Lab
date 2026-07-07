# Algorithm Log 02: Matrix Multiplication (GEMM)
**Author:** Vaibhav  
**Project:** C++ Systems Performance Lab  
**Environment:** MSVC / x86 / Release Mode  

## Introduction
Moving from 1D Memory-Bound algorithms to 2D Compute-Bound algorithms.
The goal is to benchmark an O(N^3) baseline matrix multiplication and explore CPU caching strategies.

## Phase 1: The O(N^3) Baseline & Flattened Arrays
To build a compute-bound workload, I implemented a standard Matrix Multiplication (GEMM). 
* **The Architecture:** I specifically avoided `std::vector<std::vector<int>>` because it fractures memory across the heap. Instead, I used a single flattened 1D array and mathematically treated it as a 2D grid using the formula `index = (row * N) + col`.
* **The Baseline (i-j-k):** I wrote the standard triple-nested loop. At $N=500$ (125 million operations), it took **~86,120 µs**. 
* **The Bottleneck:** While testing, I realized the innermost loop (`k`) reads Matrix B vertically. This means it jumps hundreds of memory addresses on every single tick, completely destroying the CPU's L1 Cache and causing millions of "Cache Misses."

## Phase 2: Spatial Locality & Loop Reordering (i-k-j)
To fix the cache thrashing, I applied a technique called **Loop Reordering**.
* **The Fix:** By simply swapping the order of the inner two loops from `i-j-k` to `i-k-j`, I aligned the math so that the CPU reads both Matrix A and Matrix B in perfectly straight, horizontal contiguous lines.
* **The "Too Smart" Compiler:** At $N=500$, the reordered loop was only 6% faster than the baseline. I learned this is because modern C++ compilers (like MSVC in Release Mode) are smart enough to recognize inefficient matrix math and partially optimize it behind the scenes using Auto-Vectorization. I was racing the compiler against itself!
* **Breaking the Compiler ($N=1000$):** To truly test the hardware, I scaled the matrix up to $1000 \times 1000$ (1 Billion operations). At this scale, the compiler's safety net failed, and memory limits took over:
  * **Baseline (1 Billion Ops):** 858,683 µs
  * **Loop Reordered (1 Billion Ops):** 655,855 µs
  * **Conclusion:** Reordering the loops yielded a massive ~23% speedup, proving the critical importance of Spatial Locality (accessing data close together in memory).

## Phase 3: Pushing the L1 Cache (Cache Tiling)
To hit the true physical limits of the hardware, I implemented Cache Tiling (Loop Blocking). 
* **The Architecture:** I wrapped the `i-k-j` algorithm in three outer control loops to chop the $1000 \times 1000$ matrix into tiny $64 \times 64$ sub-grids. The goal was to force the math to happen entirely inside the CPU's 32KB L1 Cache, eliminating Cache Misses completely.
* **The Benchmark:**
  * Baseline: 880,911 µs
  * Loop Reordered: 642,604 µs
  * Cache Tiling: 715,440 µs
* **The "Overhead Trap":** Surprisingly, the tiled version was slower than the simple reordered version! I discovered three reasons why:
  1. **Branching Overhead:** The inner loops relied on `std::min()` to prevent out-of-bounds errors, forcing the CPU to evaluate millions of extra `if/else` conditions.
  2. **Compiler Confusion:** The 6-deep nested loop structure with dynamic bounds confused the MSVC compiler, causing it to abandon Auto-Vectorization (SIMD) safety nets.
  3. **Loop Management:** The sheer overhead of managing 6 integer counters outweighed the memory speed gained from the L1 cache at $N=1000$.
* **Conclusion:** While Cache Tiling is the theoretical peak of memory optimization, in raw C++, architectural overhead can defeat hardware caching. To go faster without dropping into raw Assembly language, I have to stop optimizing a single core and start using the whole CPU.
