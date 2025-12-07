import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Configuration
FILENAME = "crane_simulation.tsv"

# Define the anchor points
ANCHOR_BOT = (0.0, 0.0)
ANCHOR_TOP = (0.0, 1.0)

def load_data(filename): # Loads simulation data from TSV file
    try:
        data = pd.read_csv(filename, sep='\t')
        return data
    except FileNotFoundError:
        print(f"Error: File '{filename}' not found. Run the C++ simulation first.")
        exit()

def plot_time_history(data): # Plots the vertical position (Y) of the tip masses over time

    time = data['t']
    
    # Filter columns
    y_cols = [c for c in data.columns if c.startswith('y')]
    
    # Last bottom and last top mass
    tip_indices = [y_cols[-2], y_cols[-1]]

    plt.figure(figsize=(10, 6))
    
    for col in tip_indices:
        plt.plot(time, data[col], label=f'Mass {col[1:]} (Tip)')
        
    plt.ylabel('Y Position [m]')
    plt.xlabel('Time [s]')
    plt.title('Vertical Vibration of Crane Tip')
    plt.grid(True)
    plt.legend()
    
    plt.tight_layout()
    plt.savefig('crane_time_history.png')
    print("Saved 'crane_time_history.png'")
    plt.show()

def plot_trajectories(data): # Plots the final shape of the crane compared to initial
    
    x_cols = [c for c in data.columns if c.startswith('x')]
    y_cols = [c for c in data.columns if c.startswith('y')]

    plt.figure(figsize=(10, 6))
    
    # Plot Anchors
    plt.plot(ANCHOR_BOT[0], ANCHOR_BOT[1], 'ks', markersize=10, label='Anchors')
    plt.plot(ANCHOR_TOP[0], ANCHOR_TOP[1], 'ks', markersize=10)

    # Reconstruct geometry at last frame
    last_idx = -1
    
    # Split into bottom and top chords for plotting
    # Indices: 0, 2, 4... are bottom. 1, 3, 5... are top
    
    # Bottom Chord
    bot_x = [ANCHOR_BOT[0]] + [data[x_cols[i]].iloc[last_idx] for i in range(0, len(x_cols), 2)]
    bot_y = [ANCHOR_BOT[1]] + [data[y_cols[i]].iloc[last_idx] for i in range(0, len(y_cols), 2)]
    
    # Top Chord
    top_x = [ANCHOR_TOP[0]] + [data[x_cols[i]].iloc[last_idx] for i in range(1, len(x_cols), 2)]
    top_y = [ANCHOR_TOP[1]] + [data[y_cols[i]].iloc[last_idx] for i in range(1, len(y_cols), 2)]

    plt.plot(bot_x, bot_y, 'o-', label='Bottom Chord (Final)')
    plt.plot(top_x, top_y, 'o-', label='Top Chord (Final)')

    # Plot trajectories of the tip
    tip_x_col = x_cols[-1]
    tip_y_col = y_cols[-1]
    plt.plot(data[tip_x_col], data[tip_y_col], 'r--', linewidth=0.5, label='Tip Trajectory')

    plt.title('Crane Structure Deflection')
    plt.xlabel('X Position [m]')
    plt.ylabel('Y Position [m]')
    plt.axis('equal')
    plt.grid(True)
    plt.legend()
    
    plt.savefig('crane_trajectories.png')
    print("Saved 'crane_trajectories.png'")
    plt.show()

def animate_system(data): # Creates an animation of the vibrating crane
    
    print("Generating animation...")
    
    fig, ax = plt.subplots(figsize=(10, 6))
    ax.set_aspect('equal')
    ax.grid(True)
    ax.set_title(f'Simulation Animation (Crane Structure)')
    ax.set_xlabel('X [m]')
    ax.set_ylabel('Y [m]')

    # Determine plot limits
    all_x = data.filter(like='x').values.flatten()
    all_y = data.filter(like='y').values.flatten()
    pad = 1.0
    ax.set_xlim(min(all_x.min(), 0) - pad, max(all_x.max(), 0) + pad)
    ax.set_ylim(min(all_y.min(), 0) - pad, max(all_y.max(), 1) + pad)

    # We use two lines: one for bottom chord, one for top chord
    line_bot, = ax.plot([], [], 'o-', lw=2, markersize=6, color='blue', label='Bottom Chord')
    line_top, = ax.plot([], [], 'o-', lw=2, markersize=6, color='green', label='Top Chord')

    # Anchors
    ax.plot([ANCHOR_BOT[0], ANCHOR_TOP[0]], [ANCHOR_BOT[1], ANCHOR_TOP[1]], 'ks', markersize=10)

    time_text = ax.text(0.05, 0.95, '', transform=ax.transAxes)

    x_cols = [c for c in data.columns if c.startswith('x')]
    y_cols = [c for c in data.columns if c.startswith('y')]
    
    # Skip frames for speed
    skip = 5 
    frames = range(0, len(data), skip)

    def init():
        line_bot.set_data([], [])
        line_top.set_data([], [])
        time_text.set_text('')
        return line_bot, line_top, time_text

    def update(frame_idx):
        # Bottom Chord Nodes. Even indices
        xs_b = [ANCHOR_BOT[0]] + [data[x_cols[i]].iloc[frame_idx] for i in range(0, len(x_cols), 2)]
        ys_b = [ANCHOR_BOT[1]] + [data[y_cols[i]].iloc[frame_idx] for i in range(0, len(y_cols), 2)]
        
        # Top Chord Nodes. Odd indices
        xs_t = [ANCHOR_TOP[0]] + [data[x_cols[i]].iloc[frame_idx] for i in range(1, len(x_cols), 2)]
        ys_t = [ANCHOR_TOP[1]] + [data[y_cols[i]].iloc[frame_idx] for i in range(1, len(y_cols), 2)]
        
        line_bot.set_data(xs_b, ys_b)
        line_top.set_data(xs_t, ys_t)
        
        time_text.set_text(f"Time: {data['t'].iloc[frame_idx]:.2f} s")
        return line_bot, line_top, time_text

    ani = animation.FuncAnimation(fig, update, frames=frames,
                                  init_func=init, blit=True, interval=20)
    
    plt.legend(loc='lower right')

    print("Saving to crane.gif...")
    ani.save('crane.gif', writer='pillow', fps=30)
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