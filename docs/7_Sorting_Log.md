# Algorithm Log 07: Sorting
**Author:** Vaibhav Khokha  
**Project:** C++ Systems Performance Lab  
**Environment:** MSVC / x86 / Release Mode / 20 Logical Cores

## Introduction
After exploring optimizations for matrix multiplication and image processing, I wanted to investigate another important aspect of CPU performance: branch prediction. Many comparison-based sorting algorithms rely heavily on conditional branches, and the efficiency of these branches depends on how accurately the processor can predict their outcome. This experiment compares a traditional comparison-based sorting algorithm with a branch-minimized radix sort while also exploring the practical limits of manual cache optimizations.

## The Workload: Sorting 100 Million Random Integers

### 1. Baseline Implementation (`std::sort`)
The first benchmark used the C++ Standard Library implementation of `std::sort`, which is typically implemented as Introsort—a hybrid of Quicksort, Heapsort, and Insertion Sort. Since comparison-based sorting repeatedly evaluates conditions such as `a < b`, its performance can be influenced by branch prediction.

* **Random Input:** 6,480,810 µs (~6.48 s)
* **Pre-Sorted Input:** 582,486 µs (~0.58 s)

* **Observation:** Sorting pre-sorted data completed significantly faster than sorting randomly ordered data. One contributing factor is that predictable branch outcomes allow the processor's branch predictor to operate more effectively, reducing the number of costly pipeline stalls.

---

### 2. Branch-Minimized Sorting (Base-256 Radix Sort)

To reduce the dependence on conditional comparisons, I implemented a Base-256 Least Significant Digit (LSD) Radix Sort. Rather than comparing elements, the algorithm processes one byte at a time using bitwise operations and counting arrays.

* **Implementation:**
    * Individual bytes were extracted using bitwise shifts (`>>`) and masks (`& 0xFF`).
    * A 256-element histogram counted the frequency of each byte value.
    * Prefix sums converted these counts into output positions for stable redistribution.

* **Execution Time:** 854,858 µs (~0.85 s)

* **Observation:** Since radix sort avoids most comparison-based branching, it performed substantially better than `std::sort` on randomly distributed integers, achieving an overall speedup of approximately **7.5×** for this workload.

---

### 3. Exploring Software Write Buffering

To investigate whether memory writes could be optimized further, I implemented a software-managed buffering strategy. The idea was to accumulate writes into fixed-size buffers before copying them back to the output array, with the goal of improving cache locality.

* **Execution Time:** 990,339 µs (~0.99 s)

* **Observation:** Despite the additional optimization effort, this implementation performed slightly worse than the original radix sort.

Several factors likely contributed to this result:

1. **Cache Capacity:** The additional buffering structures increased memory usage and may have exceeded the capacity of the L1 data cache, introducing additional cache misses.

2. **Additional Control Logic:** Buffer management required extra conditional checks inside the inner loop, partially offsetting the benefit of reducing memory writes.

3. **Hardware Optimizations:** Modern processors already contain sophisticated write buffers and cache management mechanisms, making it difficult for a software implementation to outperform the hardware's built-in memory subsystem.

---

## Final Thoughts

This experiment demonstrated that algorithm design and processor architecture are closely connected. Comparison-based sorting algorithms can be affected by branch prediction, while radix sort benefits from replacing comparisons with predictable memory accesses and bitwise operations. It also showed that not every optimization produces a performance improvement—modern processors already perform many low-level memory optimizations automatically. Profiling and benchmarking remain essential for determining whether an optimization is genuinely beneficial.