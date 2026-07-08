# Algorithm Log 03: Multi-Threading & The Reality of Hardware Scaling
**Author:** Vaibhav Khokha   
**Project:** C++ Systems Performance Lab  
**Environment:** MSVC / x86 / Release Mode / 20 Logical Cores

## Introduction
After maxing out what a single core could mathematically handle in Phase 6 (using the `Loop Reordered i-k-j` algorithm), it was time to wake up the rest of the machine. My CPU has 20 logical cores and the goal for this phase was to build a Thread Pool to divide the massive matrix math across all of them. I honestly went into this expecting a flat 20x speedup. I was very wrong, but what I learned was much more interesting.

## Phase 1: The Setup & The Disappointment (N=1000)
Because matrix multiplication is "Embarrassingly Parallel" (calculating Row 1 doesn't depend on Row 2), I could easily divide the matrix horizontally.
* **The Architecture:** I used `std::thread::hardware_concurrency()` to detect my 20 cores and set up a `std::vector<std::thread>` to manage them. 
* **The Lambda Trap:** I learned a tough lesson about C++ lambda captures. I had to use the `[&, startRow, endRow]` syntax. This passed the massive memory matrices by reference (so I didn't crash my RAM), but took hard, permanent copies of the loop coordinates so the threads wouldn't accidentally overwrite each other as they spawned.
* **The Benchmark:** * Single-Core Best: ~683,176 µs
  * 20-Core Thread Pool: 189,346 µs
* **The Reality Check:** That is a 3.6x speedup. It’s a huge time saver, but it is nowhere near 20x. I realized I was slamming into physical hardware limits:
  1. **OS Overhead:** Asking Windows/Linux to allocate memory and spin up 20 threads takes thousands of microseconds.
  2. **Asymmetric Cores:** I don't have 20 identical bodybuilders. My CPU mixes Performance Cores, Efficiency Cores, and Hyperthreading. 
  3. **The Memory Wall:** 20 cores screaming for data at the exact same time created a massive traffic jam on my RAM's memory bus.

## Phase 2: The Comeback (N=2000)
To test my theory that the OS thread creation overhead was eating my performance on the smaller matrix, I needed to give the CPU a workload so massive that the setup time wouldn't matter anymore. 
Because matrix math scales at $O(N^3)$, doubling the size from 1000 to 2000 multiplies the work by 8.
* **The N=2000 Benchmark:** 988,962 µs (0.98 seconds).
* **The Discovery:** If my $N=1000$ multi-threaded time scaled linearly, 8 billion operations should have taken over 1.5 seconds. Instead, it finished in under a second. By throwing a mountain of work at the CPU, the thread overhead time vanished into a rounding error. The cores were finally allowed to stay awake and stretch their legs.

## Phase 3: Visualizing the Bottleneck (Log-Log Scaling)
To get visual proof, I merged my CSV data and wrote a Python script to plot my best single-core run against my 20-core run across sizes 250 to 2000.
* Because of the $O(N^3)$ explosion, I had to use a `log-log` scale to keep the lines readable.
* **The Verdict:** The graph was perfect. At the bottom left ($N=250$), the single-core and multi-core lines were almost touching, visually proving that OS overhead makes threads useless for tiny workloads. At the top right ($N=2000$), the gap blew wide open. 

## Final Thoughts
I successfully transitioned my program from being *CPU-bound* to *Memory Bandwidth-bound*. Multi-threading isn't just about turning on more cores; it’s a balancing act between the size of the math and the physical limits of the motherboard. 
