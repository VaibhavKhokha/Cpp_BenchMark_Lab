# Algorithm Log 04: AVX2 SIMD
**Author:** Vaibhav Khokha   
**Project:** C++ Systems Performance Lab  
**Environment:** MSVC / x86 / Release Mode / 20 Logical Cores

## Introduction
After maxing out the thread pool and getting all 20 logical cores working together, each individual core was still doing standard "SISD" (Single Instruction, Single Data) math. After exploring cache optimizations and multi-threading, I wanted to take direct control over how each CPU core executed the inner computation. Rather than relying solely on the compiler's auto-vectorization, I implemented the innermost loop using AVX2 SIMD intrinsics to explicitly operate on multiple integers with a single instruction.

## Phase 1: The Silicon Awakening (AVX2 SIMD)
To achieve maximum throughput, I utilized AVX2 (Advanced Vector Extensions). Instead of using standard 32-bit integer math, I tapped into the CPU's hidden 256-bit registers.
* **The Architecture:**  By including `immintrin.h`, I gained access to AVX2 intrinsic functions that map closely to SIMD instructions supported by the processor. Using 256-bit vector registers allowed each instruction to operate on eight 32-bit integers simultaneously, increasing the amount of work performed per instruction.
* **The Broadcasting Trick:** I took a single scalar integer from Matrix A, cloned it 8 times to fill a 256-bit register (`_mm256_set1_epi32`), loaded 8 integers from Matrix B (`_mm256_loadu_si256`), multiplied them all in one shot (`_mm256_mullo_epi32`), and added them to the result matrix (`_mm256_add_epi32`).
* **The Boundary Trap:** I quickly discovered that vectorized code requires much more careful loop control that scalar code. Since each AVX2 load processes eight 32-bit integers at a time, I changed the inner loop from `j++` to `j += 8`.  However, if remaining elements at the end of a row were fewer than eight, the final load could read past the end of the array, resulting in undefined behavior or a crash. To simplify the implementation, I benchmarked matrix sizes that were multiples of eight (256, 512, 1000 and 2000).

## Phase 2: Performance Comparison (N=1000)
To test the raw power of SIMD, I raced a **single core** speaking AVX2 against my **20-core thread pool** speaking standard C++.
* **20-Core Standard C++:** 189,346 µs
* **1-Core AVX2 SIMD:** 115,175 µs
* **The Observation:** I expected 20 CPU threads to outperform a single core without much difficulty. Instead, the single-core AVX2 implementation finished substantially faster. This experiment showed me that improving the efficiency of each instruction through SIMD can have a greater impact than simply increasing the number of executing threads.

## Phase 3: The Grand Finale (20 Cores + AVX2)
The final step was to combine the two optimization techniques. I integrated the AVX2 vectorized inner loop into each worker thread so that the workload was distributed across CPU cores while each core simultaneously processed multiple integers using SIMD instructions.
* **The Baseline:** ~889,500 µs
* **The Grand Finale Time:** 27,824 µs (0.027 seconds)
* **The Result:** Compared to the original baseline implementation, the combined approach achieved an overall speedup  of approximately **32x**. This experiment demonstrated that thread-level parallelism (using multiple CPU cores) and data-level parallelism (using SIMD vector instructions) complement one another, allowing the processor to exploit multiple forms of parallelism simultaneously.

## Phase 4: Visualizing Hardware Saturation
I wrote a Python script to combine the benchmark data from all previous phases into a single log-log plot. 
* **The Crossover:** For smaller matrices (N = 256), the single-core SIMD implementation outperformed the multi-threaded SIMD version. At this scale, the overhead of creating and scheduling threads represented a significant portion of the total execution time, reducing benefit of parallel execution. 
* **Scaling Behaviour:** As the matrix size increased toward N = 2000, the multi-threaded SIMD implementation consistently became the fastest approach. With a larger computational workload, the fixed thread-management overhead became relatively insignificant, allowing both SIMD vectorization and multiple CPU cores to be utilized more effectively.

## Final Thoughts
This phase marked the transition from writing software to engineering systems. I proved that taking control of memory traffic (cache locality), thread scheduling, and silicon instructions yields results standard code can never reach.
