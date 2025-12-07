import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Configuration
FILENAME = "spinning_top_simulation.tsv"

def load_data(filename):
    try:
        return pd.read_csv(filename, sep='\t')
    except FileNotFoundError:
        print("File not found.")
        exit()

def animate_top(data):
    print("Generating animation...")
    fig, ax = plt.subplots(figsize=(8, 8))
    ax.set_aspect('equal')
    ax.grid(True)
    ax.set_xlim(-2, 2)
    ax.set_ylim(-2, 2)
    ax.set_title('Spinning Top Animation')

    # Pivot point
    ax.plot(0, 0, 'kx', markersize=10, label='Pivot')

    # Line drawing triangle M0 -> M1 -> M2 -> M0 and spokes
    line_rim, = ax.plot([], [], 'o-', lw=2, color='blue')
    line_spokes, = ax.plot([], [], '-', lw=1, color='gray') # Spokes
    
    time_text = ax.text(0.05, 0.95, '', transform=ax.transAxes)

    # Column indices
    x0, y0 = data.columns[1], data.columns[2]
    x1, y1 = data.columns[3], data.columns[4]
    x2, y2 = data.columns[5], data.columns[6]

    skip = 2
    frames = range(0, len(data), skip)

    def init():
        line_rim.set_data([], [])
        line_spokes.set_data([], [])
        time_text.set_text('')
        return line_rim, line_spokes, time_text

    def update(i):
        # Mass coordinates
        mx = [data[x0].iloc[i], data[x1].iloc[i], data[x2].iloc[i], data[x0].iloc[i]]
        my = [data[y0].iloc[i], data[y1].iloc[i], data[y2].iloc[i], data[y0].iloc[i]]
        
        # Triangle
        line_rim.set_data(mx, my)
        
        # Line of spokes
        sx = [0, mx[0], 0, mx[1], 0, mx[2]]
        sy = [0, my[0], 0, my[1], 0, my[2]]
        line_spokes.set_data(sx, sy)

        time_text.set_text(f"Time: {data['t'].iloc[i]:.2f} s")
        return line_rim, line_spokes, time_text

    ani = animation.FuncAnimation(fig, update, frames=frames, init_func=init, blit=True, interval=20)
    
    print("Saving to spinning_top.gif...")
    ani.save('spinning_top.gif', writer='pillow', fps=30)
    print("Animation saved!")
    
    plt.show()

if __name__ == "__main__":
    # Load Data
    df = load_data(FILENAME)
    
    # Animation
    animate_top(df)