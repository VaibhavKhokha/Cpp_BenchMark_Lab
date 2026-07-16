# Algorithm Log 03: Multi-Threading
**Author:** Vaibhav Khokha   
**Project:** C++ Systems Performance Lab  
**Environment:** MSVC / x86 / Release Mode / 20 Logical Cores

## Introduction
After optimizing my single-threaded implementation using the loop-reordered `i-k-j` algorithm, I wanted to see how much more performance I could get by using multiple CPU cores. My CPU has 20 logical cores and the goal for this phase was to build a Thread Pool to divide the massive matrix math across all of them. I honestly went into this expecting a flat 20x speedup. I was very wrong, but what I learned was much more interesting.

## Phase 1: The Setup & The Disappointment (N=1000)
Matrix multiplication is naturally parallel because each row of the output matrix can be computed independently. That meant I could divide the work horizontally across multiple threads.
* **The Architecture:** I used `std::thread::hardware_concurrency()` to detect my 20 cores and set up a `std::vector<std::thread>` to manage them. 
* **The Lambda Trap:** I learned a tough lesson about C++ lambda captures. I had to use the `[&, startRow, endRow]` syntax. This passed the large matrices by reference to avoid copying them into every thread, while `startRow` and `endRow` were captured by value so that each thread received its own copy of the row boundaries.
* **The Benchmark:** Single-Core Best: ~683,176 µs
  * 20-Core Thread Pool: 189,346 µs
* **The Reality Check:** That is a 3.6x speedup. It’s a huge time saver, but it is nowhere near 20x. The benchmark made it clear that simply adding more threads doesn't automatically produce proportional speedups. A few factors were limiting the performance:
  1. **OS Overhead:** Asking Windows/Linux to allocate memory and spin up 20 threads takes thousands of microseconds.
  2. **Asymmetric Cores:** My processor has a mix of Performance cores, Efficiency cores and Hyper-Threading, so not every logical core provides the same level of performance. 
  3. **The Memory Wall:** 20 cores screaming for data at the exact same time created a massive traffic jam on my RAM's memory bus.

## Phase 2: The Comeback (N=2000)
I wanted to see whether the relatively small workload at N = 1000 was making the thread creation overhead more noticeable. To test this, I increased the matrix size to N = 2000, where the computation itself would dominate the total execution time. 
Because matrix math scales at O(N^3), doubling the size from 1000 to 2000 multiplies the work by 8.
* **The N=2000 Benchmark:** 988,962 µs (0.98 seconds).
* **The Observation:** The larger workload achieved much better overall efficiency than I expected from simply scaling the N = 1000 benchmark. One likely reason is that the fixed cost of creating and managing threads becomes much less significant compared to the total computation as the workload grows. Larger matrices also keep the CPU cores busy for longer, allowing more of the execution time to be spent doing useful work instead of setup.

## Phase 3: Visualizing the Bottleneck
To get visual proof, I merged my CSV data and wrote a Python script to plot my best single-core run against my 20-core run across sizes 250 to 2000.
* Because of the O(N^3) explosion, I had to use a `log-log` scale to keep the lines readable.
* **The Verdict:** The graph clearly showed that the single-threaded and multi-threaded implementations performed similarly for smaller matrices, while the performance gap widened as the workload increased. This is consistent with the idea that the fixed overhead of thread creation becomes less significant as the computation grows.  

## Final Thoughts
Throughout this experiment, I observed that scaling performance became increasingly limited by hardware factors such as memory bandwidth and thread management overhead rather than the computation itself. Multi-threading isn't just about turning on more cores; it’s a balancing act between the size of the computation and the physical limits of the hardware.
