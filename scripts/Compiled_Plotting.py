import pandas as pd
import matplotlib.pyplot as plt
import glob
import os


    
path = '../../../results/Matrix/*.csv'
all_files = glob.glob(path)
    
if not all_files:
    print("No CSV files found in the results directory.")
    

    
df_list = []
for filename in all_files:
    df = pd.read_csv(filename)
    df_list.append(df)
        
master_df = pd.concat(df_list, axis=0, ignore_index=True)
    
    
master_df.columns = master_df.columns.str.strip()

    
plt.figure(figsize=(12, 8))
    
    
algorithms = master_df['Algorithm'].unique()
    
for algo in algorithms:
        
    algo_data = master_df[master_df['Algorithm'] == algo].sort_values(by='InputSize')
        
        
    plt.plot(algo_data['InputSize'], algo_data['TimeMicroseconds'], 
                marker='o', linewidth=2, label=algo)

    
plt.xscale('log')
plt.yscale('log')
plt.xlabel('Matrix Size (N * N)', fontsize=12)
plt.ylabel('Time (Microseconds)', fontsize=12)
plt.title('The CPU Evolution: Baseline vs. Hardware Saturation', fontsize=16, fontweight='bold')
    
plt.grid(True, which="both", ls="--", alpha=0.5)
plt.legend(loc='upper left', fontsize=10)
plt.tight_layout()

    
save_path = '../../../plots/Master_Final_Graph_CPU.png'
os.makedirs(os.path.dirname(save_path), exist_ok=True)
plt.savefig(save_path, dpi=300)
print(f"Master plot saved to {save_path}")
    
plt.show()

