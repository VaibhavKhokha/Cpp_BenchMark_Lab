# Algorithm Log 07: Sorting
**Author:** Vaibhav Khokha   
**Project:** C++ Systems Performance Lab  
**Environment:** MSVC / x86 / Release Mode / 20 Logical Cores

## Introduction
Modern CPUs utilize a Branch Predictor to guess the outcome of conditional statements (`if/else`) to keep their instruction pipelines full. If the prediction is wrong, the CPU suffers a massive latency penalty known as a Pipeline Flush. This module demonstrates the physical cost of branch mispredictions during sorting, implements a branchless algorithm to bypass the predictor entirely, and explores the limits of software-defined memory optimizations.

## The Workload: Sorting 100 Million Random Integers

### Phase 1: The Branch Prediction Penalty (`std::sort`)
The C++ Standard Library uses IntroSort (a hybrid of Quicksort, Heapsort, and Insertion Sort). While algorithmically optimal ($O(N \log N)$), it relies heavily on evaluating elements via conditional branches (`if a < b`). 
* **Random Data Execution:** 6,480,810 µs (~6.48 seconds). The predictor guessed incorrectly roughly 50% of the time, resulting in millions of pipeline flushes.
* **Pre-Sorted Data Execution:** 582,486 µs (~0.58 seconds). With perfectly predictable data, the pipeline never flushed, yielding an 11x speedup on the exact same hardware and code.

### Phase 2: Bypassing the Predictor (Base-256 Radix Sort)
To defeat the hardware penalty of random data, the architecture was rewritten to eliminate conditional comparisons entirely. A Base-256 Radix Sort was implemented. It isolates individual bytes using bitwise shifts (`>>`) and masks (`& 0xFF`), tallying occurrences into a 256-element histogram. A prefix-sum array then converts these tallies into exact memory offsets.
* **Execution Time:** 854,858 µs (~0.85 seconds).
* **The Result:** By replacing `if` logic with pure, forward-marching memory reads/writes, the algorithm achieved a **7.5x speedup** over `std::sort` on completely random data.

### Phase 3: The Hardware Reality (Failed Optimization)
An attempt was made to further optimize the Radix Sort by implementing "Software Write-Combining." The goal was to stop Cache Thrashing by writing data into 64-integer L1 Cache buffers, and only flushing to main RAM when a buffer was completely full (aligning perfectly with 64-byte Cache Lines). 
* **Execution Time:** 990,339 µs (~0.99 seconds).
* **The Post-Mortem:** The optimization was logically sound but physically slower for three reasons:
  1. **L1 Cache Spill:** The 65KB buffer matrix exceeded the physical L1 Data Cache capacity of the cores, spilling into L2 and causing internal cache thrashing.
  2. **Re-introducing Branches:** The logic required an `if (buffer_full)` check in the innermost loop, re-introducing the exact Branch Predictor penalty we had just built the algorithm to escape.
  3. **Redundant Engineering:** Modern x86 processors feature dedicated silicon called Write-Combining Buffers (WCBs). The CPU was already buffering and aligning the memory writes at the hardware level for free. 

## Final Thoughts
This module proved that bypassing the CPU's Branch Predictor can yield massive performance gains. However, it also served as a strict reminder: in lower-level Systems Engineering, theoretical software optimizations will fail if they conflict with the physical architecture of the silicon.
