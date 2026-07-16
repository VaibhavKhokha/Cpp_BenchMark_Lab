# Algorithm Log 06: Image Gray-Scaling
**Author:** Vaibhav Khokha   
**Project:** C++ Systems Performance Lab  
**Environment:** MSVC / x86 / Release Mode / 20 Logical Cores

## Introduction
After maximizing CPU throughput on pure mathematical matrices, the lab transitioned to raw memory manipulation via Image Processing (Grayscale Conversion). Processing a real `.png` file requires decoding compressed data into a massive 1-Dimensional array of bytes, presenting new challenges in data structuring, hardware bottlenecks, and concurrent memory alignment. 

## The Optimization Pipeline (4K Image - 8.2 Million Pixels)

### 1. The Naive Baseline (The FPU Bottleneck)
The initial approach iterated through the flat byte array, extracting Red, Green, and Blue values. It utilized the standard luminosity formula `(0.299 * R + 0.587 * G + 0.114 * B)`.
* **The Hardware Flaw:** This forced the CPU to constantly switch between integer data (`uint8_t`) and floating-point math (`float`). Floating-point operations require the CPU's Floating-Point Unit (FPU), which takes significantly more clock cycles than basic integer math.
* **Execution Time:** 22,691 µs (~22.7 ms)

### 2. Fixed-Point Arithmetic (ALU Isolation)
To bypass the slow FPU and expensive type-casting, the algorithm was rewritten using fixed-point arithmetic. The decimal multipliers were scaled by 256, allowing the division to be handled by a lightning-fast bitwise right-shift `>> 8`. 
* **The Math:** `uint8_t grey = (r * 77 + g * 150 + b * 29) >> 8;`
* **The Result:** The math was isolated entirely within the CPU's Arithmetic Logic Unit (ALU). Execution time dropped to **16,246 µs (~16.2 ms)**, yielding a ~28% reduction in execution time without changing the algorithm's Big-O complexity.

### 3. Hardware Concurrency (Escaping the Pixel Trap)
The final iteration scaled the optimized ALU math across a 20-logical-core Thread Pool. 
* **The Engineering Challenge:** Distributing work by raw bytes causes memory misalignment (threads starting on a Green byte instead of a Red byte, corrupting the image). The architecture was refactored to chunk by *pixel index*, with each thread independently calculating its absolute byte offset in memory.
* **The Result:** Execution time plummeted to **9,462 µs (~9.5 ms)**. 

## Final Thoughts
By stripping away floating-point overhead and correctly aligning a multi-threaded workload across a flat memory buffer, the algorithm achieved a **2.4x total speedup**. The CPU is now capable of processing 8.2 million pixels in under 10 milliseconds.
