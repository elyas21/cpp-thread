
# import pandas as pd
# import matplotlib.pyplot as plt
# import numpy as np

# # Function to plot execution times in a grid layout
# def plot_execution_times(csv_file):
#     # Read the CSV data into a DataFrame
#     df = pd.read_csv(csv_file, header=None, names=['Iterations', 'Threads', 'Sequential Time', 'Pthreads Time', 'OpenMP Time'])

#     # Create a figure for plotting in grid
#     fig, axes = plt.subplots(2, 2, figsize=(15, 12))
#     axes = axes.flatten()  # Flatten axes array for easy iteration

#     # Iterate over each thread count (up to 16 threads) and plot
#     thread_counts = [2, 4, 8, 16]
#     for i, thread_count in enumerate(thread_counts):
#         ax = axes[i]
#         df_thread = df[df['Threads'] == thread_count]

#         # Plot Sequential, Pthreads, and OpenMP times
#         ax.plot(df_thread['Iterations'], df_thread['Sequential Time'], label='Sequential Time', marker='o')
#         ax.plot(df_thread['Iterations'], df_thread['Pthreads Time'], label='Pthreads Time', marker='x')
#         ax.plot(df_thread['Iterations'], df_thread['OpenMP Time'], label='OpenMP Time', marker='^')

#         # Set plot details
#         ax.set_xscale('log')  # Logarithmic scale for Iterations
#         ax.set_xlabel('Iterations (log scale)')
#         ax.set_ylabel('Time (seconds)')
#         ax.set_title(f'Execution Time Comparison for {thread_count} Threads')
#         ax.legend()
#         ax.grid(True)

#     # Adjust layout and show the plot
#     plt.tight_layout()
#     plt.show()

# # Main function to visualize both execution times and speedup
# def visualize():
#     # Plot execution times
#     plot_execution_times('ppc.csv')

  

# # Run the visualization
# visualize()
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Function to plot execution times based on mean values for each CSV file
def plot_execution_times(csv_files ,save_path=None):
    # Initialize an empty DataFrame to store all the data from CSV files
    combined_df = pd.DataFrame(columns=['Iterations', 'Threads', 'Sequential Time', 'Pthreads Time', 'OpenMP Time'])
    
    # Iterate over each CSV file and load the data
    for file in csv_files:
        df = pd.read_csv(file, header=None, names=['Iterations', 'Threads', 'Sequential Time', 'Pthreads Time', 'OpenMP Time'])
        combined_df = pd.concat([combined_df, df], ignore_index=True)
    
    # Calculate the mean of each column, grouped by 'Iterations' and 'Threads'
    mean_df = combined_df.groupby(['Iterations', 'Threads']).mean().reset_index()

    # Create a figure for plotting in grid
    fig, axes = plt.subplots(2, 2, figsize=(15, 12))
    axes = axes.flatten()  # Flatten axes array for easy iteration

    # Iterate over each thread count (up to 16 threads) and plot
    thread_counts = [2, 4, 8, 16]
    for i, thread_count in enumerate(thread_counts):
        ax = axes[i]
        df_thread = mean_df[mean_df['Threads'] == thread_count]

        # Plot Sequential, Pthreads, and OpenMP mean times
        ax.plot(df_thread['Iterations'], df_thread['Sequential Time'], label='Sequential Time', marker='o')
        ax.plot(df_thread['Iterations'], df_thread['Pthreads Time'], label='Pthreads Time', marker='x')
        ax.plot(df_thread['Iterations'], df_thread['OpenMP Time'], label='OpenMP Time', marker='^')

        # Set plot details
        ax.set_xscale('log')  # Logarithmic scale for Iterations
        ax.set_xlabel('Iterations (log scale)')
        ax.set_ylabel('Time (seconds)')
        ax.set_title(f'Execution Time Comparison for {thread_count} Threads')
        ax.legend()
        ax.grid(True)

    # Adjust layout and show the plot
    if save_path:
        plt.savefig(save_path)
        print(f"Plot saved to {save_path}")
    plt.tight_layout()
    plt.show()

# Main function to visualize both execution times and speedup
def visualize():
    # List of CSV files
    csv_files = ['ppc1.csv', 'ppc2.csv', 'ppc3.csv', 'ppc4.csv', 'ppc5.csv']
    csv_files = ['mppc1.csv', 'mppc2.csv', 'mppc3.csv', 'mppc4.csv', 'mppc5.csv']
    # Plot execution times based on mean values
    save_path = '10-10000000000.png'    
    plot_execution_times(csv_files, save_path)

    save_path = '10-10000.png'
    plot_execution_times(csv_files, save_path)

# Run the visualization
visualize()
