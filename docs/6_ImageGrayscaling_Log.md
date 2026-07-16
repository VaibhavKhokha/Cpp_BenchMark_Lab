# Algorithm Log 06: Image Gray-Scaling
**Author:** Vaibhav Khokha   
**Project:** C++ Systems Performance Lab  
**Environment:** MSVC / x86 / Release Mode / 20 Logical Cores

## Introduction
After optimizing matrix multiplication, I wanted to apply similar performance optimization techniques to a real-world workload. I chose grayscale image conversion, which involves processing millions of pixels stored as a contiguous array of bytes. Unlike the previous experiments, this task introduced additional considerations such as memory layout, floating-point computation and multi-threaded processing.

## The Optimization Pipeline (4K Image - 8.2 Million Pixels)

### 1. Baseline Implementation
The initial approach iterated through the flat byte array, extracting Red, Green, and Blue values. It utilized the standard luminosity formula `(0.299 * R + 0.587 * G + 0.114 * B)`.
* **The Hardware Flaw:** The standard grayscale formula uses floating-point arithmetic, requiring repeated conversions between integer pixel values and floating-point computations. Since this operation is performed for every pixel in the image, the arithmetic cost becomes noticeable for large images.
* **Execution Time:** 22,691 µs (~22.7 ms)

### 2. Fixed-Point Arithmetic
To reduce the cost of floating-point operations, I replaced the floating-point coefficients with scaled integer coefficients. 
* **The Math:** `uint8_t grey = (r * 77 + g * 150 + b * 29) >> 8;`
* **Execution Time:** 16,246 µs (~16.2 ms)
* **Observation:** Replacing floating-point arithmetic with fixed-point arithmetic reduced the execution time by approximately 28% while producing visually equivalent grayscale output.
* **The Result:** This implementation produced nearly identical grayscale values while performing the computation entirely with integer arithmetic.

### 3. Multi-Threaded Implementation
The final iteration scaled the optimized ALU math across a 20-logical-core Thread Pool. 
* **The Engineering Challenge:** * **Execution Time:** 16,246 µs (~16.2 ms)
* **Observation:** Since each pixel consists of three consecutive bytes (RGB), partitioning the image by arbitrary byte offsets could cause a thread to begin processing in the middle of a pixel. To avoid this, the workload was divided by pixel index, allowing each thread to compute its corresponding byte offset independently.
* **The Result:** The multi-threaded implementation reduced the execution time to **9,462 µs (~9.5 ms)** by distributing the workload across all available logical cores.

## Final Thoughts
This experiment showed that both arithmetic optimization and parallel execution can significantly improve the performance of image processing workloads. Replacing floating-point arithmetic with an equivalent fixed-point implementation reduced the computational cost, while multi-threading further improved throughput by processing independent regions of the image concurrently. Together, these optimizations reduced the execution time from approximately 22.7 ms to 9.5 ms, achieving an overall speedup of about 2.4×.