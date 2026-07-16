# Algorithm Log 08: Disk I/O
**Author:** Vaibhav Khokha   
**Project:** C++ Systems Performance Lab  
**Environment:** MSVC / x86 / Release Mode / 20 Logical Cores

## Introduction
Up until now, I spent this entire lab optimizing the CPU. I used SIMD for the ALU, bit-shifting for the FPU and branchless logic to fix the Instruction Pipeline. I assumed that if the code was mathematically perfect, it would run at light speed. 

This module taught me that the CPU is only as fast as the data you feed it. When dealing with the hard drive, standard C++ operations are agonizingly slow. I had to learn how to bypass the Operating System entirely to unlock the physical speed limit of my SSD.

## The Workload: Parsing a 1-Gigabyte File
To prove the bottleneck, I first had to generate a massive payload. I wrote a C++ generator to create a 1GB text file (`Massive_data.txt`) containing roughly 27.5 million lines of mocked user transaction data. 

### Phase 1: The OS Bottleneck
I started by writing a standard C++ file parser using `std::ifstream` and `std::getline` to simply count the number of lines in the file.
* **Execution Time:** ~7,517,540 µs (7.51 seconds).
* **The Problem:** The CPU was barely doing any work. Most of those 7.5 seconds were spent in **Kernel Context Switches**. Every time my code asked for the next line, the program had to pause, ask the Windows OS for permission, switch to Kernel mode, wait for the SSD to spin up, copy the data to a buffer, switch back to User mode and hand it to me. Doing this millions of times crushed the performance.

### Phase 2: The Memory Map Bypass (`mmap`)
To fix this, I abandoned standard C++ and used the native Win32 API to talk directly to the OS memory manager. 
Using `CreateFileA` and `CreateFileMappingA`, I asked Windows to take the physical location of that 1GB file on my SSD and map it directly into my CPU's virtual RAM space. I then used a raw `char*` pointer to blast through the memory as if it were a giant array, completely eliminating standard file buffers.
* **Execution Time:** ~1,092,020 µs (1.09 seconds).
* **The Result:** A massive **6.8x speedup**. By removing the OS middleman and treating the hard drive like RAM, I maxed out the physical read speed of the drive.

## The Git Disaster (Lessons Learned)
The hardest part of this module wasn't the C++; it was version control. I accidentally allowed Git to track my 1GB `Massive_data.txt` file. When I tried to push, GitHub completely blocked my repository due to its strict 100MB file size limit. 

Because the giant file was already baked into my local Git history, simply adding it to `.gitignore` or using `git commit --amend` wasn't enough. I had to execute a "Nuclear Un-commit" (`git reset origin/master`) to erase my local commit history back to the last safe state without losing my physical files, re-stage with a proper `.gitignore`, and push a clean commit. 

## Final Thoughts
This module shifted my perspective from algorithm design to systems architecture. Writing fast C++ isn't just about math; it's about understanding exactly how your software interacts with the Operating System and the physical motherboard.
