# Algorithm Log 01: Vector Addition
**Author:** Vaibhav  
**Project:** C++ Systems Performance Lab  
**Environment:** MSVC / x86 / Release Mode  

## Introduction
This is the first algorithm in my performance lab. Before I can write hyper-optimized matrix multiplications or deep learning kernels, I need to understand the absolute basics of CPU memory and timing using a simple Vector Addition.

## Phase 1: Building a Real Stopwatch
Before I could optimize anything, I needed to learn how to actually measure CPU time. Normal C++ functions like `time()` aren't precise enough for modern computers. 

* **What I built:** A custom `Timer` class using `std::chrono::high_resolution_clock` to measure time in strict microseconds. 
* **Biggest lesson learned:** Never put `std::cout` inside a loop you are trying to benchmark! I realized that printing to the terminal window takes a massive amount of time because it has to talk to the Operating System and the monitor. If you want to benchmark the CPU, you have to just do math.

## Phase 2: The Baseline Algorithm & C++ Templates
I wrote a standard, "dumb" vector addition loop (`result[i] = a[i] + b[i]`) to serve as my baseline. 

* **The Template Trap:** I wanted the function to work for `int`, `float`, and `double`, so I used C++ `template <typename T>`. But I learned that templates aren't actual code; they are just blueprints. If you put the template blueprint in a `.hpp` file but hide the logic in a `.cpp` file, the compiler gets confused and throws Linker Errors.
* **The Fix:** I learned how to use "Explicit Instantiation" at the bottom of my `.cpp` file to force the compiler to build the specific types I needed. 

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