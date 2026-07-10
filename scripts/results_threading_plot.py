import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

script_dir = Path(__file__).parent


threading_csv = script_dir.parent / "results" / "Matrix" / "Threading_results.csv"
matrix_csv = script_dir.parent / "results" / "Matrix" / "Matrix_results.csv"
graph_path = script_dir.parent / "plots" / "Threading_Benchmarking.png"


df_thread = pd.read_csv(threading_csv)
df_matrix = pd.read_csv(matrix_csv)


df_thread.columns = df_thread.columns.str.strip()
df_matrix.columns = df_matrix.columns.str.strip()


df_single_core = df_matrix[df_matrix["Algorithm"] == "MultiplyMatrices_LoopReordered"]


df_combined = pd.concat([df_single_core, df_thread])


for algo, grp in df_combined.groupby("Algorithm"):
    sorted_grp = grp.sort_values("InputSize")
    plt.plot(
        sorted_grp["InputSize"],
        sorted_grp["TimeMicroseconds"],
        label = algo,
        marker = 'o',
        linewidth = 2,
    )

plt.xscale('log')
plt.yscale('log')

plt.legend()
plt.xlabel("Size (N^2)")
plt.ylabel("Time in Microseconds")
plt.title("Single-Core vs 20-Core Matrix Math")

plt.savefig(graph_path)
plt.show()