import numpy as np
import matplotlib.pyplot as plt

# List of files, their titles and line styles
simulations = [
    ('output_GL_2.txt',    'Gauss-Legendre (2 stages)', 'blue',   '-',  6),
    ('output_GL_3.txt',    'Gauss-Legendre (3 stages)', 'orange', '-',  4),
    ('output_Radau_2.txt', 'Radau IIA (2 stages)',      'green',  '--', 2),
    ('output_Radau_3.txt', 'Radau IIA (3 stages)',      'red',    ':',  2)
]

# Energy plot
plt.figure(figsize=(10, 6))

for filename, label, color, style, width in simulations:
    try:
        data = np.loadtxt(filename, usecols=(0, 1, 2))
        energy = 0.5 * data[:,2]**2 + 0.5 * data[:,1]**2
        plt.plot(data[:,0], energy, label=label, color=color, linestyle=style, linewidth=width)
    except OSError:
        print(f"File {filename} not found.")

plt.xlabel('Time')
plt.ylabel('Total Energy')
plt.title('Energy Conservation Check (100 steps)')
plt.legend()
plt.grid(True)
plt.show()

# Phase Portraits
plt.figure(figsize=(8, 8))

for filename, label, color, style, width in simulations:
    try:
        data = np.loadtxt(filename, usecols=(0, 1, 2))
        plt.plot(data[:,1], data[:,2], label=label, color=color, linestyle=style, linewidth=width)
    except OSError:
        pass

# Exact solution
theta = np.linspace(0, 2*np.pi, 200)
plt.plot(np.cos(theta), -np.sin(theta), 'k--', alpha=0.3, label='Exact Solution', linewidth=1)

plt.xlabel('Position')
plt.ylabel('Velocity')
plt.title('Phase Space Comparison (100 steps)')
plt.legend()
plt.grid(True)
plt.axis('equal')
plt.show()