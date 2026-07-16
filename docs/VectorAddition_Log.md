# Algorithm Log 01: Vector Addition - Benchmarking CPU Performance and Memory Behavior
**Author:** Vaibhav Khokha       
**Project:** C++ Systems Performance Lab   

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
This is the first algorithm in my C++ Systems Performance Lab. Before optimizing matrix multiplication kernels, I first need to understand how modern CPUs execute the simplest possible data-parallel workload: vector addition.

## Phase 1: Building a Real Stopwatch
Before I could optimize anything, I first needed a reliable way to measure execution time. Standard C++ timing functions such as `time()` do not provide sufficient precision for micro-benchmarks.

* **What I built:** A custom `Timer` class using `std::chrono::high_resolution_clock` to measure execution time with microsecond resolution. 
* **Biggest lesson learned:** Never place `std::cout` inside a benchmark loop. Console output introduces significant I/O overhead that can easily dominate the execution time of the algorithm being measured. If the goal is to benchmark computation, the measured region should perform only the computation itself.

## Phase 2: The Baseline Algorithm & C++ Templates
I implemented a baseline scalar vector addition algorithm (`result[i] = a[i] + b[i]`) to serve as the performance reference for all subsequent optimizations. 

* **The Template Trap:** I wanted the function to support `int`, `float` and `double`, so I implemented it using C++ templates (`template <typename T>`). I learned that templates are compile-time blueprints rather than compiled functions. When the template definition is hidden inside a `.cpp` file instead of being visible in the corresponding header, the compiler cannot generate the required template instantiations, resulting in linker errors.
* **The Fix:** I solved the problem using **explicit template instantiation**, which instructs the compiler to generate concrete implementations for the required types (`int`, `float` and `double`) even though the template definition resides in a `.cpp` file. 

## Phase 3: Memory Pre-allocation & Compiler Quirks

### Experiment 1: The Debug Trap
I tried to make the baseline faster. My baseline was asking the OS for memory and filling it with zeros (`std::vector<T> result(size, 0)`), which felt slow. I wrote an optimized version using `.reserve()` and `.emplace_back()` to avoid those zeros.
* **Baseline Time:** 5,769 µs
* **Optimized Time:** 42,301 µs (7x slower!)
* **What happened?** I panicked at first, but then I learned I was running the code in **Debug Mode**. In Debug mode, the C++ compiler turns off all speed optimizations and runs heavy safety checks on every single `emplace_back()` call. You can never trust benchmark numbers in Debug mode!

### Experiment 2: Release Mode & The SIMD Secret
I switched Visual Studio to **Release Mode** and ran the exact same code again. The times plummeted, making the code 24x faster!
* **Baseline Time:** 1,436 µs
* **Optimized Time:** 1,762 µs
* **What happened?** Even in Release mode, my "optimized" version lost! It turns out, using `result[i]` on an array with a fixed size is so simple that the CPU can use special hardware called **SIMD (Single Instruction, Multiple Data)** to add multiple integers together in a single clock cycle. 
* By using `emplace_back()`, I was constantly updating the vector's internal `size` pointer on every single loop iteration. This constant state mutation confused the compiler and accidentally disabled SIMD! 
* **Conclusion:** To get the absolute best speed, we need to completely separate memory allocation from the math loop. Memory should be allocated outside the timer, and we should only do raw pointer math inside the timer.

### Experiment 3: Zero-Allocation Execution
To finally beat the baseline, I realized I had to remove the Operating System from the timer completely. I wrote a third function (`addVectors_prealloc`) that takes a pre-allocated output vector by reference. Instead of asking for memory inside the function, I allocated it beforehand. Inside the timer, the code *only* does the raw pointer math.
* **Baseline Time (1M elements):** 1,229 µs
* **Pre-Allocated Time (1M elements):** 692 µs
* **Conclusion:** It worked! By removing memory allocation (`new`, `reserve()`, or zero-initialization) from the execution path, the CPU instantly blasted through the math using SIMD hardware acceleration. I nearly cut the time in half. This proved the golden rule of high-performance loops: **Zero-Allocation Execution**.

## Phase 4: Algorithmic Scaling and the Memory Wall
In enterprise systems, you never benchmark just a single size. An algorithm might be fast for 1,000 elements but crash at 10,000,000. I automated my `main.cpp` to run all three algorithms across a scaling range (10^3 to 10^7 elements) and export the data to a CSV.

* **The Data Pipeline:** I built a custom C++ `BenchMarkReporter` to store the metrics in RAM (`std::vector`) during execution and flush them to a `.csv` file only at the very end. This prevented slow disk I/O from corrupting my CPU timers.
* **Error Handling:** I learned how to build safe libraries. I added a `throw std::invalid_argument` inside my math function if the input sizes didn't match, and caught it with a `try/catch` block in `main.cpp` so the program safely logs the error instead of silently crashing.
* **The "Memory Wall" Discovery:** When reviewing my scaling CSV for the Pre-Allocated algorithm, I noticed a bottleneck:
  * `1,000,000` elements = 609 µs
  * `10,000,000` elements = 6,381 µs (10.4x slower)
* Even though the input size multiplied exactly by 10, the execution time multiplied by **10.4**. Why? Because of the **CPU Cache**. At 1 million elements (~4MB of data), everything fits snugly inside the CPU's ultra-fast L3 cache. But at 10 million elements (~40MB), the data spills over. The CPU is forced to wait for my motherboard's slower main RAM to deliver the integers. I officially hit the hardware Memory Wall.

## Phase 5: Visualizing Scaling and Data-Science Artifacts
Numbers in a CSV are great, but drawing them out tells the true story of how code interacts with hardware. Because C++ isn't built for graphics, I built a Python data pipeline using `pandas` and `matplotlib` to ingest my `results.csv` and plot the scaling trends.

* **What I Built:** The script groups rows by the algorithm name, sorts them by input size to ensure smooth left-to-right plotting, and maps everything out. 
* **The Log-Log Transformation:** Because my test sizes jump exponentially from 1,000 to 10,000,000 elements, a normal graph would totally squish the smaller test sizes against the left axis. I applied a logarithmic scale to both the X and Y axes. This turned exponential curves into straight diagonal lines, making it incredibly easy to compare algorithms at scale.
* **The Visual Proof:** Looking at the final plot, the green `Reserving` line stays firmly at the top (the slowest), the blue `Baseline` line sits in the middle, and the orange `Prealloc` line runs perfectly parallel underneath them as the undisputed speed champion. 
* **The Log(0) Disappearing Act:** I noticed a strange visual bug on my graph-at 1,000 elements, the orange line suddenly dove straight down into the floor and disappeared! After looking closely at my data, I figured it out: my Preallocated function was so blazingly fast at 1,000 elements that my C++ stopwatch recorded exactly `0` microseconds. Because the graph uses a logarithmic scale, and the logarithm of zero ($log(0)$) is mathematically undefined, the graphing library panicked and drew the line straight down off the grid. 

## Final Takeaways
Through this first algorithm, I didn't just learn how to make code run faster; I learned how to systematically analyze performance:
1. Debug builds lie-always benchmark in Release mode.
2. State mutations inside a loop can ruin the compiler's ability to apply hardware acceleration (SIMD).
3. Memory allocations belong outside the critical path (Zero-Allocation Execution).
4. Hardware has limits, and eventually, you will smash face-first into the Memory Wall.