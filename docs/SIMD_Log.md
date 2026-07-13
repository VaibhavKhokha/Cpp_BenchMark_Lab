# Algorithm Log 04: AVX2 SIMD
**Author:** Vaibhav Khokha   
**Project:** C++ Systems Performance Lab  
**Environment:** MSVC / x86 / Release Mode / 20 Logical Cores

## Introduction
After maxing out the thread pool and getting all 20 logical cores working together, each individual core was still doing standard "SISD" (Single Instruction, Single Data) math. I was trusting the C++ compiler to optimize the innermost math loop, which meant my CPU was pulling and multiplying one integer at a time. It was time to stop trusting the compiler, bypass standard C++, and speak directly to the physical silicon using Intel Hardware Intrinsics.

## Phase 1: The Silicon Awakening (AVX2 SIMD)
To achieve maximum throughput, I utilized AVX2 (Advanced Vector Extensions). Instead of using standard 32-bit integer math, I tapped into the CPU's hidden 256-bit registers.
* **The Architecture:** By using `#include <immintrin.h>`, I gained access to hardware commands that process **8 integers per clock cycle**. 
* **The Broadcasting Trick:** I took a single scalar integer from Matrix A, cloned it 8 times to fill a 256-bit register (`_mm256_set1_epi32`), loaded 8 integers from Matrix B (`_mm256_loadu_si256`), multiplied them all in one shot (`_mm256_mullo_epi32`), and added them to the result matrix (`_mm256_add_epi32`).
* **The SegFault Trap:** I learned a brutal lesson about memory alignment. Because AVX2 blindly grabs 32 bytes of RAM at a time, I had to change my inner loop counter from `j++` to `j += 8`. Furthermore, if the matrix dimension (N) was not a perfect multiple of 8 (like N=250), the final loop step would violently read out-of-bounds memory, causing an instant OS-level Segmentation Fault. I locked my test sizes to perfect multiples (256, 512, 1000, 2000) to keep the hardware safe.

## Phase 2: The Benchmark Shock (N=1000)
To test the raw power of SIMD, I raced a **single core** speaking AVX2 against my **20-core thread pool** speaking standard C++.
* **20-Core Standard C++:** 189,346 µs
* **1-Core AVX2 SIMD:** 115,175 µs
* **The Reality Check:** One single core executing raw hardware instructions absolutely destroyed a 20-core thread pool. It proved exactly how much processing power is left on the table when you rely entirely on the compiler. The data throughput was so violent that the L1 cache was struggling to keep the math engine fed.

## Phase 3: The Grand Finale (20 Cores + AVX2)
For the ultimate test, I combined both architectures. I injected the AVX2 `_mm256` inner loops directly into the lambda payload of my Thread Pool. The OS sliced the memory horizontally, and 20 cores simultaneously chewed through 8 integers per clock tick.
* **The Baseline:** ~889,500 µs
* **The Grand Finale Time:** 27,824 µs (0.027 seconds)
* **The Result:** A massive **32x speedup** for 1 Billion operations.

## Phase 4: Visualizing Hardware Saturation
I wrote a Python master script to aggregate the CSV data from all previous phases into a single Log-Log plot. 
* **The Crossover:** At the smallest matrix sizes (N=256), the Single-Core SIMD line actually beat the Multi-Threaded SIMD line. The math was executed so fast that the single core finished the matrix before the OS could even finish waking up the other 19 cores—the ultimate proof of thread-creation overhead.
* **Amdahl's Law:** As the matrices scaled to N=2000, the thread overhead vanished and the Multi-Threaded SIMD line completely dominated, hitting the absolute theoretical limit of my physical motherboard and memory bandwidth.

## Final Thoughts
This phase marked the transition from writing software to engineering systems. I proved that taking control of memory traffic (cache locality), thread scheduling, and silicon instructions yields results standard code can never reach.
