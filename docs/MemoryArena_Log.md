# Algorithm Log 05: Custom Memory Arenas & Escaping the Kernel
**Author:** Vaibhav Khokha   
**Project:** C++ Systems Performance Lab  
**Environment:** MSVC / x86 / Release Mode / 20 Logical Cores

## Introduction
After maxing out the CPU's computational limits with AVX2 Hardware Intrinsics and a 20-core thread pool, I noticed a lingering bottleneck. Even though the math was blisteringly fast, the program still had to constantly halt execution to ask the Windows Operating System for RAM every time it created a matrix using `std::vector`. This meant my code was constantly context-switching between User Mode and Kernel Mode. To achieve absolute hardware saturation, I had to stop asking the OS for permission and manage the physical RAM myself.

## Building the Memory Arena
Standard memory allocation (`new` or `malloc`) is slow because the OS must search for free space, handle security checks and update page tables. To bypass this, I built a Linear Memory Arena.
* **The Architecture:** When the program boots, it asks the OS for a massive, contiguous block of RAM exactly once (e.g., 500 Megabytes). 
* **The Execution:** From that point forward, when my matrices need memory, they don't talk to Windows. They talk to my Arena. The Arena simply hands out a raw pointer to the start of the block and slides an offset counter forward.
* **The Reset:** Instead of individually deallocating matrices (which is slow), the Arena resets instantly by just sliding the offset counter back to zero, allowing the next loop iteration to blindly overwrite the old memory.

## Wiring the Silicon to the Arena
The beauty of C++ vectors is that they are just wrappers around C-style arrays. This meant I didn't have to rewrite my complex AVX2 SIMD logic.
* I replaced the heavy `const std::vector<int>&` parameters with lightweight `const int*` raw pointers.
* I used `std::fill_n` to initialize the memory blocks directly on the physical silicon.
* The SIMD instructions (`_mm256_loadu_si256`) seamlessly accepted the raw pointers, completely unaware that the memory was now being managed entirely in User Mode.

## The Benchmark Shock & Visualizing the Kernel Trap
To test the speed, I ran the Arena-backed SIMD algorithm against the Vector-backed SIMD algorithm up to N=2048 (matrices containing over 4.1 million elements, resulting in roughly 8.58 Billion math operations).
* **Vector-Backed (OS Allocation):** The time scaled aggressively upward as the OS struggled to find and zero out massive blocks of memory.
* **Arena-Backed (User Allocation):** 181,826 µs (0.18 seconds).
* **The Verdict:** I plotted both algorithms on a standard linear graph. The math performed by the CPU was absolutely identical in both algorithms. The widening visual gap between the two lines represented pure Operating System overhead. By keeping the program entirely in User Mode on a pre-allocated RAM block, I stripped away the final layer of software latency.

## Final Thoughts
I have successfully isolated the CPU from the OS. My program now controls the cache logic, the thread scheduler, the AVX2 registers, and the RAM distribution.
