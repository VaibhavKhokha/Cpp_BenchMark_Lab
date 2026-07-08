# Algorithm Log 02: Matrix Multiplication (GEMM)
**Author:** Vaibhav Khokha       
**Project:** C++ Systems Performance Lab  
**Environment:** MSVC / x86 / Release Mode  

## Introduction
Moving from 1D Memory-Bound algorithms to 2D Compute-Bound algorithms. My goal here was to see what happens when the CPU has to do way more math than memory reading, benchmarking an O(N^3) matrix multiplication and exploring CPU caching strategies.

## Phase 1: The Baseline & Flattened Arrays
To build a heavy workload, I started by implementing a standard, "dumb" Matrix Multiplication (also known as GEMM). 

* **What I built:** I specifically avoided making a vector of vectors (`std::vector<std::vector<int>>`) because I learned that fractures memory across the heap. Instead, I used a single flattened 1D array and mathematically pretended it was a 2D grid using the formula `index = (row * N) + col`. I wrapped this in the standard `i-j-k` triple nested loop.
* **Baseline Time (N=500):** ~92,091 µs for 125 million operations.
* **The Discovery:** While thinking about the loops, I realized a massive bottleneck. The innermost loop (`k`) was reading Matrix B vertically. This meant the CPU was jumping over hundreds of memory addresses on every single tick, completely destroying the L1 Cache and causing millions of slow "Cache Misses."

## Phase 2: Spatial Locality & Loop Reordering
To fix the cache thrashing, I applied a technique called **Loop Reordering**.

* **The Fix:** I simply swapped the order of the inner two loops from `i-j-k` to `i-k-j`. This aligned the math perfectly so the CPU reads both Matrix A and Matrix B in straight, horizontal, contiguous lines.
* **The "Too Smart" Compiler:** At $N=500$, my new reordered loop was only 6% faster! I was super confused until I learned that modern C++ compilers (like MSVC in Release Mode) are smart enough to recognize bad matrix math and partially fix it behind the scenes using Auto-Vectorization. I was racing the compiler against itself.
* **Breaking the Compiler:** To actually test the hardware, I scaled the matrix up to $N=1000$ (1 Billion operations). At this massive scale, the compiler's safety net failed, and memory limits took over:
  * **Baseline (1 Billion Ops):** 889,563 µs
  * **Loop Reordered (1 Billion Ops):** 683,176 µs
* **Conclusion:** Just by swapping two lines of code, I got a massive ~23% speedup. This proved the golden rule of memory: **Spatial Locality** (accessing data that lives close together in RAM) is absolutely critical for performance.

## Phase 3: Pushing the L1 Cache (Cache Tiling)
I wanted to hit the true physical limits of the hardware, so I tried a famous optimization called **Cache Tiling** (or Loop Blocking). 

* **What I built:** I wrapped my algorithm in three extra outer control loops to chop the giant $1000 \times 1000$ matrix into tiny $64 \times 64$ sub-grids. The idea was to force the math to happen entirely inside the CPU's tiny 32KB L1 Cache, eliminating RAM lookups completely.
* **The Benchmark (N=1000):**
  * Baseline: 889,563 µs
  * Loop Reordered: 683,176 µs
  * Cache Tiling: 698,071 µs
* **What happened?** I fell into the "Overhead Trap." Surprisingly, my highly complex tiled version was slower than the simple reordered version! 
* **The Reason:** My inner loops needed `std::min()` checks to prevent the tiles from going out of bounds. This forced the CPU to evaluate millions of extra `if/else` branch conditions. Plus, managing 6 nested loop counters confused the C++ compiler so much that it gave up on using SIMD hardware acceleration. 
* **Conclusion:** While Cache Tiling is the theoretical peak of memory optimization, in standard C++, adding too much architectural overhead can actually defeat hardware caching. 

## Phase 4: Automated Pipeline & O(N^3) Visualization
Just like with my vector addition, I wanted to see how these algorithms scaled, so I plugged them into my automated C++ testing pipeline and Python grapher.

* **The Setup:** The C++ suite dynamically scaled $N$ from 250 up to 1500, cleared the memory safely between tests, recorded the execution times, and auto-generated `Matrix_results.csv`.
* **The Log-Log Trick:** Because matrix multiplication scales at $O(N^3)$ (a cubic relationship), putting the results on a normal linear graph completely squished the smaller inputs together, while the massive $N=1500$ times exploded vertically off the chart. I applied a `log-log` scale to both axes in Python to flatten the exponential growth into straight lines, making it perfectly readable.
* **The Visual Proof:** The graph proved definitively that the `Loop Reordered (i-k-j)` algorithm is the absolute champion of single-core C++ performance, saving nearly a full second of execution time at N=1500 compared to the baseline.

## Final Takeaways
Through building compute-bound matrices, I learned:
1. The order of your loops dictates your memory speed. Read data horizontally whenever possible.
2. The C++ Compiler is smart, but you can break its safety nets with big enough data.
3. Complex optimizations (like Tiling) can backfire if the loop overhead and branching logic outweigh the cache benefits.
4. I have officially pushed a single CPU core to its absolute mathematical limit in C++. To go any faster, I have to wake up the rest of the machine.
