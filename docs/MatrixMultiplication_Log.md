# Algorithm Log 02: Matrix Multiplication (GEMM)
**Author:** Vaibhav Khokha       
**Project:** C++ Systems Performance Lab  
**Environment:** MSVC / x86 / Release Mode  

## Benchmark Environment

| Component | Specification |
|-----------|---------------|
| CPU | 13th Gen Intel® Core™ i7-13650HX @ 2.60 GHz |
| RAM | 32 GB |
| Compiler | MSVC |
| Target Architecture | x86 |
| Operating System | Windows 11 |
| Build Configuration | Release |  

## Introduction
After exploring a memory-bound workload with vector addition, I wanted to move on to something much more computationally intensive: matrix multiplication (GEMM). My goal was to understand how memory access patterns, cache behavior and loop ordering affect the performance of an algorithm whose computational complexity grows as O(N^3).  

## Phase 1: The Baseline & Flattened Arrays
To build a heavy workload, I started by implementing the standard Matrix Multiplication algorithm (GEMM). 

* **What I built:** I specifically avoided making a vector of vectors (`std::vector<std::vector<int>>`) because I learned that it fragments memory across the heap. Instead, I used a single flattened 1D array and mathematically pretended it was a 2D grid using the formula `index = (row * N) + col`. I wrapped this in the standard `i-j-k` triple nested loop.
* **Baseline Time (N=500):** ~92,091 µs for 125 million operations.
* **The Discovery:** While studying the access pattern, I noticed a potential bottleneck. The innermost loop (`k`) was reading Matrix B column by column. Since the data is stored in row-major order, this results in strided memory accesses, which are much less cache-friendly and are more likely to cause cache misses. 

## Phase 2: Spatial Locality & Loop Reordering
To fix the cache thrashing, I applied a technique called **Loop Reordering**.

* **The Fix:** I simply swapped the order of the inner two loops from `i-j-k` to `i-k-j`. This aligned the math perfectly so the CPU reads both Matrix A and Matrix B in straight, horizontal, contiguous lines.
* **The Surprise:** At $N=500$, the reordered loop was only about 6% faster. I expected a much bigger improvement. My best guess is that the compiler was already applying several optimizations in Release mode, reducing the performance gap between the two implementations. 
* **Scaling the Problem:** To actually test the hardware, I scaled the matrix up to $N=1000$ (1 Billion operations). At this larger problem size, the performance difference became much more noticeable as memory behavior started to dominate execution time.
  * **Baseline (1 Billion Ops):** 889,563 µs
  * **Loop Reordered (1 Billion Ops):** 683,176 µs
* **Conclusion:** Just by swapping two lines of code, I got a massive ~23% speedup. This experiment showed me how important spatial locality is. Simply changing the loop order improved cache utilization enough to produce a significant speedup without changing the algorithm itself.

## Phase 3: Pushing the L1 Cache (Cache Tiling)
I wanted to see whether I could push the CPU a little further using cache-aware optimizations, so I tried a famous optimization called **Cache Tiling** (or Loop Blocking). 

* **What I built:** I wrapped my algorithm in three extra outer control loops to chop the giant $1000 \times 1000$ matrix into tiny $64 \times 64$ sub-grids. The idea was to divide the matrices into smaller blocks so that the working data would fit better inside the CPU cache, reducing accesses to slower levels of the memory hierarchy.
* **The Benchmark (N=1000):**
  * Baseline: 889,563 µs
  * Loop Reordered: 683,176 µs
  * Cache Tiling: 698,071 µs
* **What happened?** I fell into the "Overhead Trap." Surprisingly, my highly complex tiled version was slower than the simple reordered version! 
* **The Reason:** My inner loops needed `std::min()` checks to prevent the tiles from going out of bounds. This forced the CPU to evaluate millions of extra `if/else` branch conditions. The additional loop structure and boundary checks also made it harder for the compiler to optimize the code as aggressively as the simpler implementation. 
* **Conclusion:** Although cache tiling is a widely used optimization technique, this experiment showed me that adding complexity doesn't automatically improve performance. The extra overhead outweighed the benefits on my implementation. 

## Phase 4: Automated Pipeline & O(N^3) Visualization
Just like with my vector addition, I wanted to see how these algorithms scaled, so I plugged them into my automated C++ testing pipeline and Python grapher.

* **The Setup:** The C++ suite dynamically scaled $N$ from 250 up to 1500, reset the matrices between tests, recorded the execution times, and auto-generated `Matrix_results.csv`.
* **The Log-Log Trick:** Because matrix multiplication scales at $O(N^3)$ (a cubic relationship), putting the results on a normal linear graph completely squished the smaller inputs together, while the massive $N=1500$ times exploded vertically off the chart. I applied a `log-log` scale to both axes in Python to flatten the exponential growth into straight lines, making it perfectly readable.
* **The Visual Proof:** The graph clearly showed that the loop-reordered implementation consistently outperformed my other implementations, especially as the matrix size increased.

## Final Takeaways
Through building compute-bound matrices, I learned:
1. The order of your loops dictates your memory speed. Access memory sequentially whenever possible to improve cache utilization.
2. Modern C++ compilers already perform many optimizations, so it's important to benchmark instead of assuming an optimization will always help.
3. Complex optimizations (like Tiling) can backfire if the loop overhead and branching logic outweigh the cache benefits.
4. I reached the limits of what I could achieve using straightforward single-threaded C++ optimizations. The next step is to explore multithreading, SIMD intrinsics and eventually GPU acceleration. 
