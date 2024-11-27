import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file
data = pd.read_csv("problem1output.csv")

# Get unique n values and processor counts (p)
n_values = data['n'].unique()
p_values = data['p'].unique()

# Create subplots: one for Speedup and one for Efficiency
fig, axs = plt.subplots(2, 2, figsize=(15, 10))

fig.suptitle('Speedup and Efficiency for different values of n and p')

# Plot Speedup and Efficiency for each n
for idx, n in enumerate(n_values):
    ax = axs[0, idx // 3]
    ax.plot(p_values, data[data['n'] == n]['Speedup'], marker='o', label=f'n={n}')
    ax.set_title(f'Speedup for n={n}')
    ax.set_xlabel('p (processors)')
    ax.set_ylabel('Speedup')
    ax.grid(True)
    ax.legend()

    ax = axs[1, idx // 3]
    ax.plot(p_values, data[data['n'] == n]['Efficiency'], marker='o', label=f'n={n}')
    ax.set_title(f'Efficiency for n={n}')
    ax.set_xlabel('p (processors)')
    ax.set_ylabel('Efficiency')
    ax.grid(True)
    ax.legend()

plt.tight_layout(rect=[0, 0, 1, 0.96])
plt.show()
