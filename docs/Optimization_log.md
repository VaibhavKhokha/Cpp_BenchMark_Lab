# Performance Engineering Log
**Project:** Vector Addition Baseline & Optimization
**Environment:** MSVC / x86 / Release Mode

## Phase 3: Memory Pre-allocation & Compiler Artifacts

### Experiment 1: The Debug Build Artifact
* **Hypothesis:** Using `std::vector::reserve()` followed by `emplace_back()` will eliminate memory allocation overhead and outperform the baseline initialization `std::vector<T> result(size, 0)`.
* **Metrics (Debug Mode):**
  * Baseline: 5,769 µs
  * Optimized (`reserve` + `emplace_back`): 42,301 µs
* **Analysis:** The hypothesis failed catastrophically in Debug mode. This occurred because Debug builds disable compiler optimizations and insert safety checks. `emplace_back()` triggers a full function call with iterator/capacity checks on every iteration, whereas the baseline uses raw pointer arithmetic (`operator[]`), which remains relatively fast even without optimizations.
* **Architectural Conclusion:** Performance must never be measured in a Debug environment. 

### Experiment 2: State Mutation vs. Auto-Vectorization (SIMD)
* **Hypothesis:** Re-running the exact same code in a Release build (`/O2`) will allow `emplace_back` to beat the baseline by eliminating the zero-initialization pass.
* **Metrics (Release Mode):**
  * Baseline: 1,436 µs
  * Optimized (`reserve` + `emplace_back`): 1,762 µs
* **Analysis:** The optimized version improved by 24x, but the baseline *still* outperformed it. The baseline's `operator[]` acts on a fixed-size array without mutating the vector's internal state. This allows the compiler to easily apply **SIMD (Single Instruction, Multiple Data)** auto-vectorization, adding multiple elements per clock cycle via AVX registers. Conversely, `emplace_back` increments the vector's internal `size` pointer on every single iteration. This continuous state mutation creates a data dependency that confuses the optimizer and breaks auto-vectorization.
* **Architectural Conclusion:** Avoiding memory initialization is important, but preventing SIMD vectorization carries a heavier performance penalty. To achieve maximum throughput, we must allocate memory *outside* the math kernel and only use raw pointer arithmetic inside the tight loop (Zero-Allocation Execution).