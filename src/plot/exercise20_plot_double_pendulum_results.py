import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation

FILENAME = "double_pendulum_simulation.tsv"
ANCHOR = (0.0, 0.0) 

def load_data(filename):
    try:
        return pd.read_csv(filename, sep='\t')
    except FileNotFoundError:
        print(f"File '{filename}' not found.")
        exit()

def plot_constraints(data): # Plots the Lagrange multipliers (Tension forces) over time
    
    time = data['t']
    
    plt.figure(figsize=(10, 5))
    plt.plot(time, data['lam1'], label='Lambda 1 (Top Rod)')
    plt.plot(time, data['lam2'], label='Lambda 2 (Bottom Rod)')
    
    plt.title('Lagrange Multipliers (Constraint Forces)')
    plt.xlabel('Time [s]')
    plt.ylabel('Force / Length')
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.savefig('double_pendulum_constraint_forces.png')
    plt.show()

def animate_pendulum(data):
    print("Generating animation...")
    
    fig, ax = plt.subplots(figsize=(8, 8))
    ax.set_aspect('equal')
    ax.grid(True)
    ax.set_xlim(-2.5, 2.5)
    ax.set_ylim(-2.5, 2.5)
    ax.set_title('Double Pendulum Simulation')

    # Rods and Masses
    line, = ax.plot([], [], 'o-', lw=3, markersize=8, color='black')
    # Trace for the tip
    trace, = ax.plot([], [], '-', lw=1, color='red', alpha=0.5)
    
    time_text = ax.text(0.05, 0.95, '', transform=ax.transAxes)

    trace_x, trace_y = [], []
    
    skip = 2
    frames = range(0, len(data), skip)

    def init():
        line.set_data([], [])
        trace.set_data([], [])
        time_text.set_text('')
        return line, trace, time_text

    def update(i):
        # Anchor -> Mass1 -> Mass2
        xs = [ANCHOR[0], data['x0'].iloc[i], data['x1'].iloc[i]]
        ys = [ANCHOR[1], data['y0'].iloc[i], data['y1'].iloc[i]]
        
        line.set_data(xs, ys)
        
        # Update trace
        trace_x.append(data['x1'].iloc[i])
        trace_y.append(data['y1'].iloc[i])
        # Limit trace length
        if len(trace_x) > 100:
            trace_x.pop(0)
            trace_y.pop(0)
        trace.set_data(trace_x, trace_y)
        
        time_text.set_text(f"Time: {data['t'].iloc[i]:.2f} s")
        return line, trace, time_text

    ani = animation.FuncAnimation(fig, update, frames=frames,
                                  init_func=init, blit=True, interval=20)
    
    print("Saving to double_pendulum.gif...")
    ani.save('double_pendulum.gif', writer='pillow', fps=30)
    print("Animation saved!")
    
    plt.show()

if __name__ == "__main__":
    # Load Data
    df = load_data(FILENAME)
    
    # Static Plot
    plot_constraints(df)
    
    # Animation
    animate_pendulum(df)