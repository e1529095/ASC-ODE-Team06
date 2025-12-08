# Exercise 20: Mechanical Systems Simulation Suite
## Overview
This package provides a set of numerical tools for simulating complex mechanical
systems using the Generalized-Alpha method. The suite covers both
**Ordinary Differential Equations (ODEs)** modeled via mass-spring systems and
**Differential-Algebraic Equations (DAEs)** modeled via distance constraints.

The solver implements exact analytical differentiation for the Jacobian matrix
to ensure stability in non-linear implicit time-stepping.

## Prerequisites
### C++
- `C++20` compliant compiler
- `CMake` (Version `3.20` or higher)

### Python (for visualization):
- `Python 3.x`
- Required packages: `pandas`, `matplotlib`, `pillow` (for GIF generation)
  ```shell
  pip install pandas matplotlib pillow
  ```

## Installation and Build
1. Clone the repository and navigate to the project root.
2. Create a build directory and compile the executables:
   ```shell
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

This will generate the following executables in the `bin` directory:

```shell
chain
crane
spinning_top
double_pendulum
```
## Simulation Modules
### Mass-Spring Chain
Extends the concept of a double pendulum to a multi-body chain. The system
consists of 3 masses connected in series to a fixed anchor point under the
influence of gravity.

#### Simulation
##### Source Code
- [`exercise20_chain.cpp`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/src/exercise20_chain.cpp)
```shell
./bin/chain
```

##### Output
- [`chain_simulation.tsv`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/docs/assets/exercise20/data/crane_simulation.tsv).

#### Visualization
##### Source Code
- [`exercise20_plot_chain_results.py`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/src/plot/exercise20_plot_chain_results.py)
```shell
python exercise20_plot_chain_results.py
```

##### Output
- [`chain_plot_time_history.png`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/docs/assets/exercise20/plots/chain_plot_time_history.png)
- [`chain_plot_trajectories.png`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/docs/assets/exercise20/plots/chain_plot_trajectories.png)
- [`chain.gif`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/docs/assets/exercise20/plots/chain.gif)

```{figure} ./../assets/exercise20/plots/chain.gif
:label: chain_gif
Chain animation example
```

### Vibrating Crane Structure
Simulates a complex truss structure using beams. The structure is modeled using
a lattice of horizontal and vertical springs to provide shear stiffness. The tip
of the crane is loaded with a heavier mass to analyze vertical vibration damping.

#### Simulation
##### Source Code
- [`exercise20_crane.cpp`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/src/exercise20_crane.cpp)
```shell
./bin/crane
```
##### Output
- [`crane_simulation.tsv`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/docs/assets/exercise20/data/crane_simulation.tsv)

#### Visualization
##### Source code
- [`exercise20_plot_crane_results.py`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/src/plot/exercise20_plot_crane_results.py)
```shell
python exercise20_plot_crane_results.py
```
##### Output
- [`crane_time_history.png`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/docs/assets/exercise20/plots/crane_time_history.png)
- [`crane_trajectories.png`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/docs/assets/exercise20/plots/crane_trajectories.png)
- [`crane.gif`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/docs/assets/exercise20/plots/crane.gif)

```{figure} ./../assets/exercise20/plots/crane.gif
:label: cran_gif
Crane animation example
```

### Spinning Top
Models a spinning top as a rigid body created by connecting three masses in an
equilateral triangle with stiff springs. The system is initialized with velocity
vectors consistent with rotational motion around a central pivot.

#### Simulation
##### Source Code
- [`exercise20_spinning_top.cpp`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/src/exercise20_spinning_top.cpp)
```shell
./bin/spinning_top
```
##### Output
- [`spinning_top_simulation.tsv`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/docs/assets/exercise20/data/spinning_top_simulation.tsv).

#### Visualization
##### Source Code
- [`exercise20_plot_spinning_top_results.py`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/src/plot/exercise20_plot_spinning_top_results.py)
```shell
python exercise20_plot_spinning_top_results.py
```
##### Output
- [`spinning_top.gif`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/docs/assets/exercise20/plots/spinning_top.gif)

```{figure} ./../assets/exercise20/plots/spinning_top.gif
:label: spining_top_gif
Spinning top animation example
```

### Double Pendulum
Replaces the stiff springs of the pendulum with distance constraints. The system
is solved as a **Differential-Algebraic Equation (DAE)** using Lagrange
multipliers Lambda. The state vector is extended to size `6`.

#### Simulation
##### Source Code
- [`exercise20_double_pendulum.cpp`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/src/exercise20_double_pendulum.cpp)
```shell
./bin/double_pendulum
```
##### Output
- [`double_pendulum_simulation.tsv`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/docs/assets/exercise20/data/double_pendulum_simulation.tsv)

#### Visualization
##### Source Code
- [`exercise20_plot_double_pendulum_results.py`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/src/plot/exercise20_plot_double_pendulum_results.py)
```shell
python exercise20_plot_double_pendulum_results.py
```

##### Output
- [`double_pendulum_constraint_forces.png`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/docs/assets/exercise20/plots/double_pendulum_constraint_forces.png)
- [`double_pendulum.gif`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/docs/assets/exercise20/plots/double_pendulum.gif).

```{figure} ./../assets/exercise20/plots/double_pendulum.gif
:label: double_pendulum_gif
Double pendulum animation example
```
