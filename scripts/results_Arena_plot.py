import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

script_dir = Path(__file__).parent

simd_csv = script_dir.parent / "results" / "Matrix" / "SIMD_results.csv"
Memory_csv = script_dir.parent / "results" / "Matrix" / "Memory_Arena_results.csv"

df_simd = pd.read_csv(simd_csv)
df_memory = pd.read_csv(Memory_csv)

for df in [df_memory, df_simd]:
    df.columns = df.columns.str.strip()

best_simd = df_simd[df_simd["Algorithm"] == "MatrixMultiply_SIMD_MultiThreaded"]
best_memory = df_memory[df_memory["Algorithm"] == "MatrixMultiply_MemoryArena_SIMD"]

df_combined = pd.concat([best_simd, best_memory])

plt.figure(figsize = (10, 6))

for algo, grp in df_combined.groupby("Algorithm"):
    sorted_grp = grp.sort_values("InputSize")
    plt.plot(
        sorted_grp["InputSize"],
        sorted_grp["TimeMicroseconds"],
        label = algo,
        marker = 'o',
        linewidth = 2
    )


plt.xscale('log')
plt.yscale('log')

plt.legend()
plt.xlabel("Size (N*N)")
plt.ylabel("Time in Microseconds")
plt.title("MemoryArena_SIMD VS SIMD_MultiThreaded")
plt.grid(True, which = "both", ls = "--", alpha = 0.5)

graph_path = script_dir.parent / "plots" / "MatrixMath_MemoryArena_Benchmarking.png"
plt.savefig(graph_path)
plt.show()

