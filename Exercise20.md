# Exercise 20: Mechanical Systems Simulation Suite

## Overview

This package provides a set of numerical tools for simulating complex mechanical systems using the Generalized-Alpha method. The suite covers both Ordinary Differential Equations (ODEs) modeled via mass-spring systems and Differential-Algebraic Equations (DAEs) modeled via distance constraints.

The solver implements exact analytical differentiation for the Jacobian matrix to ensure stability in non-linear implicit time-stepping.

## Prerequisites

C++ Environment:

* C++20 compliant compiler
* CMake (Version 3.20 or higher)

Python Environment (for visualization):

* Python 3.x
* Required packages: pandas, matplotlib, pillow (for GIF generation)

`pip install pandas matplotlib pillow`

## Installation and Build

1. Clone the repository and navigate to the project root.
2. Create a build directory and compile the executables:

`mkdir build
cd build
cmake ..
make`

This will generate the following executables in the bin/ directory:

`chain
crane
spinning_top
double_pendulum`

## Simulation Modules

### 1. Mass-Spring Chain

Extends the concept of a double pendulum to a multi-body chain. The system consists of 3 masses connected in series to a fixed anchor point under the influence of gravity.

Source Code: src/exercise20_chain.cpp

Run Simulation:

`./bin/chain`

As an output, it generates chain_simulation.tsv.

Visualization:

`python exercise20_plot_chain_results.py`

As an output, it generates time-history plots, trajectory plots, and chain.gif animation.

![chain.gif animation](https://github.com/e1529095/ASC-ODE-Team06/blob/main/docs/assets/exercise20/plots/chain.gif "Chain animation example")

### 2. Vibrating Crane Structure

Simulates a complex truss structure using beams. The structure is modeled using a lattice of horizontal and vertical springs to provide shear stiffness. The tip of the crane is loaded with a heavier mass to analyze vertical vibration damping.

Source Code: src/exercise20_crane.cpp

Run Simulation:

`./bin/crane`

As an output, it generates crane_simulation.tsv

Visualization:

`python exercise20_plot_crane_results.py`

As an output, it generates time-history plots, trajectory plots, and animation of the structure bending and oscillating under gravity.

![crane.gif animation](https://github.com/e1529095/ASC-ODE-Team06/blob/main/docs/assets/exercise20/plots/crane.gif "Crane animation example")

### 3. Spinning Top

Models a spinning top as a rigid body created by connecting three masses in an equilateral triangle with stiff springs. The system is initialized with velocity vectors consistent with rotational motion around a central pivot.

Source Code: src/exercise20_spinning_top.cpp

Run Simulation:

`./bin/spinning_top`

As an output, it generates spinning_top_simulation.tsv.

Visualization:

`python exercise20_plot_spinning_top_results.py`

As an output, it generates spinning_top.gif, visualizing the rotation of the triangular body.

![spinning_top.gif animation](https://github.com/e1529095/ASC-ODE-Team06/blob/main/docs/assets/exercise20/plots/spinning_top.gif "Spinning top animation example")

### 4. Double Pendulum

Replaces the stiff springs of the pendulum with distance constraints. The system is solved as a Differential-Algebraic Equation (DAE) using Lagrange multipliers Lambda. The state vector is extended to size 6.

Source Code: src/exercise20_double_pendulum.cpp

Run Simulation:

`./bin/double_pendulum`

As an output, it generates double_pendulum_simulation.tsv containing coordinates and Lagrange multipliers.

Visualization:

`python exercise20_plot_double_pendulum_results.py`

As an output, it plots the evolution of tension forces Lambda in the rods and the animation of the chaotic motion double_pendulum.gif.

![spinning_top.gif animation](https://github.com/e1529095/ASC-ODE-Team06/blob/main/docs/assets/exercise20/plots/double_pendulum.gif "Double pendulum animation example")
