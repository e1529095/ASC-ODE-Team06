import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Configuration
FILENAME = "chain_simulation.tsv"

# Define the anchor point which is not in the output file
ANCHOR = (0.0, 0.0) 

def load_data(filename): # Loads simulation data from TSV file
    try:
        # Read tab-separated values
        data = pd.read_csv(filename, sep='\t')
        return data
    except FileNotFoundError:
        print(f"Error: File '{filename}' not found. Run the C++ simulation first.")
        exit()

def plot_time_history(data): # Plots the position of each mass over time
    
    time = data['t']
    
    # Identify mass columns
    x_cols = [c for c in data.columns if c.startswith('x')]
    y_cols = [c for c in data.columns if c.startswith('y')]

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8), sharex=True)

    # Plot X coordinates
    for col in x_cols:
        ax1.plot(time, data[col], label=f'Mass {col[1:]}')
    ax1.set_ylabel('X Position [m]')
    ax1.set_title('Time History of Positions')
    ax1.grid(True)
    ax1.legend()

    # Plot Y coordinates
    for col in y_cols:
        ax2.plot(time, data[col], label=f'Mass {col[1:]}')
    ax2.set_ylabel('Y Position [m]')
    ax2.set_xlabel('Time [s]')
    ax2.grid(True)
    
    plt.tight_layout()
    plt.savefig('plot_time_history.png')
    print("Saved 'plot_time_history.png'")
    plt.show()

def plot_trajectories(data): # Plots the trajectory of each mass in 2D space
    
    x_cols = [c for c in data.columns if c.startswith('x')]
    y_cols = [c for c in data.columns if c.startswith('y')]

    plt.figure(figsize=(8, 8))
    
    # Plot Anchor
    plt.plot(ANCHOR[0], ANCHOR[1], 'ko', markersize=10, label='Anchor')

    # Plot Trajectories
    for xc, yc in zip(x_cols, y_cols):
        mass_id = xc[1:]
        plt.plot(data[xc], data[yc], label=f'Trajectory M{mass_id}')
        # Mark the start and end
        plt.plot(data[xc].iloc[0], data[yc].iloc[0], 'g.', markersize=5) # Start
        plt.plot(data[xc].iloc[-1], data[yc].iloc[-1], 'r.', markersize=5) # End

    plt.title('2D Trajectories of Masses')
    plt.xlabel('X Position [m]')
    plt.ylabel('Y Position [m]')
    plt.axis('equal')
    plt.grid(True)
    plt.legend()
    
    plt.savefig('plot_trajectories.png')
    print("Saved 'plot_trajectories.png'")
    plt.show()

def animate_system(data): # Creates an animation of the chain

    print("Generating animation...")
    
    fig, ax = plt.subplots(figsize=(8, 8))
    ax.set_aspect('equal')
    ax.grid(True)
    ax.set_title(f'Simulation Animation (Chain)')
    ax.set_xlabel('X [m]')
    ax.set_ylabel('Y [m]')

    # Determine plot limits based on data range
    all_x = data.filter(like='x').values.flatten()
    all_y = data.filter(like='y').values.flatten()
    pad = 0.5
    ax.set_xlim(min(all_x.min(), ANCHOR[0]) - pad, max(all_x.max(), ANCHOR[0]) + pad)
    ax.set_ylim(min(all_y.min(), ANCHOR[1]) - pad, max(all_y.max(), ANCHOR[1]) + pad)

    # Line object representing the springs/rods
    line, = ax.plot([], [], 'o-', lw=2, markersize=8)
    time_text = ax.text(0.05, 0.95, '', transform=ax.transAxes)

    x_cols = [c for c in data.columns if c.startswith('x')]
    y_cols = [c for c in data.columns if c.startswith('y')]
    
    # Skip frames to speed up animation if data is dense
    skip = 5 
    frames = range(0, len(data), skip)

    def init():
        line.set_data([], [])
        time_text.set_text('')
        return line, time_text

    def update(frame_idx):
        # Build the chain geometry for this frame:
        # Anchor -> Mass 0 -> Mass 1 -> ...
        xs = [ANCHOR[0]] + [data[xc].iloc[frame_idx] for xc in x_cols]
        ys = [ANCHOR[1]] + [data[yc].iloc[frame_idx] for yc in y_cols]
        
        line.set_data(xs, ys)
        time_text.set_text(f"Time: {data['t'].iloc[frame_idx]:.2f} s")
        return line, time_text

    ani = animation.FuncAnimation(fig, update, frames=frames,
                                  init_func=init, blit=True, interval=30)
    
    print("Saving to chain.gif...")
    ani.save('chain.gif', writer='pillow', fps=30)
    print("Animation saved!")

    plt.show()

if __name__ == "__main__":
    # Load Data
    df = load_data(FILENAME)
    
    # Static Plots
    plot_time_history(df)
    plot_trajectories(df)
    
    # Animation
    animate_system(df)