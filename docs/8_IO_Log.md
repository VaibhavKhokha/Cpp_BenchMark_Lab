# Algorithm Log 08: Disk I/O
**Author:** Vaibhav Khokha  
**Project:** C++ Systems Performance Lab  
**Environment:** MSVC / x86 / Release Mode / 20 Logical Cores

## Introduction
Throughout the previous modules, I focused on improving CPU performance using cache optimizations, SIMD instructions, multi-threading and custom memory management. This time, I shifted my attention to storage performance. Even highly optimized code cannot execute efficiently if it spends most of its time waiting for data to arrive from disk. This experiment explores the impact of file I/O and compares traditional file reading with memory-mapped file access.

## The Workload: Parsing a 1 GB File

To create a realistic workload, I first wrote a C++ program that generated a 1 GB text file (`Massive_data.txt`) containing approximately 27.5 million lines of simulated transaction data.

---

## 1. Standard File I/O

The baseline implementation used `std::ifstream` together with `std::getline` to sequentially read the file and count the number of lines.

* **Execution Time:** 7,517,540 µs (~7.51 s)

* **Observation:** Although the computation itself was minimal, reading the file line by line introduced significant overhead. Each read operation required interaction with the operating system and the standard library buffering mechanisms, making file access the dominant cost rather than the computation.

---

## 2. Memory-Mapped File Access

To reduce this overhead, I implemented a memory-mapped file reader using the Win32 API. The file was opened with `CreateFileA`, mapped into the process's virtual address space using `CreateFileMappingA` and `MapViewOfFile`, and then accessed directly through a `char*` pointer.

* **Execution Time:** 1,092,020 µs (~1.09 s)

* **Observation:** Accessing the mapped memory eliminated the need for repeated line-by-line read operations through `std::ifstream`. Since the operating system could manage paging internally while the program traversed the mapped memory sequentially, the overall execution time was reduced by approximately **6.8×** for this workload.

---

## Lessons Learned: Managing Large Files with Git

One unexpected challenge during this experiment involved version control rather than C++. I accidentally committed the generated 1 GB dataset to my Git repository. Since GitHub enforces a 100 MB file size limit, the push was rejected.

Simply adding the file to `.gitignore` was not sufficient because the file had already been committed. To recover, I reset the local repository to the previous commit, removed the large file from version control while keeping it on disk, updated `.gitignore`, and created a clean commit before pushing again.

This was a useful reminder that generated benchmark datasets should never be tracked in version control.

---

## Final Thoughts

This experiment demonstrated that storage access can become a major performance bottleneck even when the computational workload is small. Memory-mapped files significantly reduced the overhead associated with traditional file reading for this sequential parsing task. It also reinforced an important systems programming lesson: optimizing overall performance requires understanding not only CPU execution but also how applications interact with the operating system and the storage subsystem.
