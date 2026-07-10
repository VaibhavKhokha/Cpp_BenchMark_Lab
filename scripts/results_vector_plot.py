import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

script_dir = Path(__file__).parent

csv_path = script_dir.parent / "results" / "Vector" / "vector_results.csv"
graph_path = script_dir.parent / "plots" / "Vector_Benchmarking.png"

df = pd.read_csv(csv_path)

df.columns = df.columns.str.strip()

for algo, grp in df.groupby("Algorithm"):

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

plt.xlabel("Size")
plt.ylabel("Time in Microseconds")

plt.title("BenchMarking")

plt.savefig(graph_path)

plt.show()