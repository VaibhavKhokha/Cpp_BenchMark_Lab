import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

script_dir = Path(__file__).parent

csv_path = script_dir.parent / "results" / "Image" / "ImageGrayscaling_results.csv"
graph_path = script_dir.parent / "plots" / "Image_BenchMarking.png"

df = pd.read_csv(csv_path)
df.columns = df.columns.str.strip()

plt.bar(
    df["Algorithm"],
    df["TimeMicroseconds"],
    color = ['#e74c3c', '#f39c12', '#2ecc71']
)


plt.xlabel("Algorithm")
plt.ylabel("TimeMicroseconds")

plt.title("Image Grayscale Benchmarking")

plt.savefig(graph_path)

plt.show()