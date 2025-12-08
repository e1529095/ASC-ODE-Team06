import numpy as np
import matplotlib.pyplot as plt
import os

# ------------------------ Mass-Spring
def plot_mass_spring():
    
    simulations = [
        ('MassspringImprovedEuler.txt', 'Improved Euler', 'red',   '-',  2), 
        ('MassspringImplicitEuler.txt', 'Implicit Euler', 'green', '--', 4), 
        ('MassspringCrankNicolson.txt', 'Crank-Nicolson', 'blue',  '-',  2)  
    ]
    
    plt.figure(figsize=(12, 6))
    
    # Phase Portrait
    plt.subplot(1, 2, 1)
    for fname, label, color, style, width in simulations:
        if os.path.exists(fname):
            data = np.loadtxt(fname)
            # data[:,1] = x (position), data[:,2] = v (velocity)
            plt.plot(data[:, 1], data[:, 2], label=label, color=color, 
                     linestyle=style, linewidth=width, alpha=0.9)
        else:
            print(f"File {fname} not found!")
            
    plt.title('Mass-Spring: Phase Portrait (Steps = 200)')
    plt.xlabel('Position (x)')
    plt.ylabel('Velocity (v)')
    plt.grid(True)
    plt.legend()
    plt.axis('equal')

    # Energy Conservation
    plt.subplot(1, 2, 2)
    for fname, label, color, style, width in simulations:
        if os.path.exists(fname):
            data = np.loadtxt(fname)
            energy = 0.5 * data[:, 1]**2 + 0.5 * data[:, 2]**2
            plt.plot(data[:, 0], energy, label=label, color=color, 
                     linestyle=style, linewidth=width, alpha=0.9)

    plt.title('Mass-Spring: Energy Conservation (Steps = 200)')
    plt.xlabel('Time (t)')
    plt.ylabel('Total Energy (E)')
    plt.grid(True)
    plt.legend()
    
    plt.tight_layout()
    plt.show()

# ------------------------ RC-Circuit
def plot_circuit():
    simulations = [
        ('CircuitImprovedEuler.txt', 'Improved Euler', 'red',   '-',  6),
        ('CircuitImplicitEuler.txt', 'Implicit Euler', 'green', '--', 4),
        ('CircuitCrankNicolson.txt', 'Crank-Nicolson', 'blue',  '-',  2)
    ]
    
    plt.figure(figsize=(10, 6))
    
    # Plotting the simulation results
    for fname, label, color, style, width in simulations:
        if os.path.exists(fname):
            data = np.loadtxt(fname)
            plt.plot(data[:, 0], data[:, 1], label=label, color=color, 
                     linestyle=style, linewidth=width, alpha=0.8)
        else:
            print(f"File {fname} not found!")

    # Plotting the exact solution
    t = np.linspace(0, 0.5, 2000)
    u_source = np.cos(100 * np.pi * t)
    plt.plot(t, u_source, 'k:', label='Source U0(t)', linewidth=1.5, alpha=0.6)

    plt.title('RC-Circuit: Capacitor Voltage vs Time (Steps = 80)')
    plt.xlabel('Time (s)')
    plt.ylabel('Voltage (V)')
    plt.grid(True)
    plt.legend()
    plt.show()

if __name__ == "__main__":
    print("Plotting Mass-Spring System results...")
    plot_mass_spring()
    
    print("Plotting Circuit results...")
    plot_circuit()