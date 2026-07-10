import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

script_dir = Path(__file__).parent

matrix_csv = script_dir.parent / "results" / "Matrix" / "Matrix_results.csv"
threading_csv =  script_dir.parent / "results" / "Matrix" / "Threading_results.csv"
simd_csv = script_dir.parent / "results" / "Matrix" / "SIMD_results.csv"

df_matrix = pd.read_csv(matrix_csv)
df_thread = pd.read_csv(threading_csv)
df_simd = pd.read_csv(simd_csv)

for df in [df_matrix, df_thread, df_simd]:
    df.columns = df.columns.str.strip()

best_matrix = df_matrix[df_matrix["Algorithm"] == "MultiplyMatrices_LoopReordered"]
best_thread = df_thread[df_thread["Algorithm"] == "MultiplyMatrices_MultiThreading"]
best_simd_single = df_simd[df_simd["Algorithm"] == "MatrixMultiply_SIMD_1Core"]
best_simd_multi = df_simd[df_simd["Algorithm"] == "MatrixMultiply_SIMD_MultiThreaded"]

df_combined = pd.concat([best_matrix, best_thread, best_simd_single, best_simd_multi])


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
plt.title(" The Final Compiled (Matrix Math) Hardware Scaling ")
plt.grid(True, which = "both", ls = "--", alpha = 0.5)

graph_path = script_dir.parent / "plots" / "MatrixMath_Compiled_Benchmarking_BestAlgo.png"
plt.savefig(graph_path)
plt.show()