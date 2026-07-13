import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

script_dir = Path(__file__).parent

csv_path = script_dir.parent / "results" / "Sorting" / "Sorting_results.csv"
graph_path = script_dir.parent / "plots" / "Sorintg_Benchmarking.png"

df = pd.read_csv(csv_path)
df.columns = df.columns.str.strip()

bars = plt.bar(
    df["Algorithm"],
    df["TimeMicroseconds"],
    color = ['#e74c3c', '#2ecc71']
)

plt.xlabel("Algorithm")
plt.ylabel("Time in Microseconds")
plt.title("Sorting 100 Million Integers")

for bar in bars:
    yval = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2.0, yval + (yval * 0.02),
             f"{int(yval):,} us", ha='center', va='bottom', fontweight = 'bold')

plt.ylim(0, df["TimeMicroseconds"].max() * 1.15)

plt.savefig(graph_path)
plt.show()