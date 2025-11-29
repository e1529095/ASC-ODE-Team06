**Numerical Simulation and Scientific Computing I - Exercise 3.1**

WS 2025

**Team 6**

Moritz Horina (e01529095)\
Lorenz Friessenbichler (e12102558)\
Tinashe Chipako (e12533643)\
Vladimir Toropov (e12451260)

In this exercise, we implemented and analyzed time-stepping methods for solving Ordinary Differential Equations. In particular, we modeled a mass-spring system governed by the simple harmonic motion equation. This second-order equation was reduced to a first-order system to be solved numerically.

**Implementation**

Two distinct methods were implemented in C++:

1. Explicit Euler Method: A first-order method.
2. Improved Euler Method: A second-order method defined by the two-stage process. We used the Midpoint rule.

**The code**

The implementation was carried out in the timestepper.hpp file. A base class TimeStepper was used to define the interface. We also used a Python script to plot the calculation results. They can be found in the Results folder. The corresponding graphs, plotted based on the calculation results, are located in the Plots folder.

The Explicit Euler class updates the state vector by evaluating the right-hand side (RHS) at the current state and adding the scaled derivative.

The Improved Euler class was added as an extension. It requires an intermediate vector storage (m_ytilde) to hold the half-step prediction. The RHS is evaluated twice per step: once to find the midpoint state, and again at the midpoint to determine the final slope for the full time-step update.

**Results and Discussion**

We performed simulations using both methods with varying time-steps, represented by the number of steps N over the simulation period. The results were visualized using time evolution plots and phase-space plots (Velocity vs Position).

**Explicit Euler Analysis**

The Explicit Euler method showed significant limitations characteristic of first-order explicit schemes applied to oscillatory systems:

1. The method fails to conserve energy. As seen in the phase plots (spiraling outwards), the system artificially gains energy over time.

2. With smaller time-steps (500 or 1000 steps), the orbit is roughly elliptical but still exhibits distortion over long periods. With larger time-steps (e.g., 100 steps), the method becomes unstable, and the phase plot spirals outward rapidly.

**Improved Euler Analysis**

The Improved Euler method demonstrated superior performance:

1. Even with fewer time-steps, the method produced a nearly perfect circular phase plot, which aligns with the theoretical conservation of energy for a harmonic oscillator.

2. The oscillations remained stable over the simulation time, avoiding the spiraling artifact seen in the standard Euler method.

**Conclusion**

The comparison highlights the importance of choosing an appropriate integration scheme for ODEs. While the Explicit Euler method is simple to implement, it is unreliable for conservative systems like the mass-spring model due to energy drift.

We should also note that the Improved Euler method, by evaluating the derivative at the midpoint, achieves much higher accuracy and preserves the geometric properties (circular phase orbits) of the system significantly better.

----------------------------------------------------------------------------------------

**Notes for the Report and Plots for Exercise 19**

To complete the assignment, we added the file exercise19_runge_kutta.cpp to the project, in which we reproduced the Gaussian and Rado implementations for the Runge-Kutta methods. The calculation results are placed in the Results folder, and the plots are in the Plots folder.
The experiment showed that the results of highly accurate methods on short time scales are very difficult to visually distinguish.

This occurs for a number of reasons:

1. Rado methods are L-stable. This means that they introduce "numerical damping" (artificial viscosity). The system loses energy, but very slowly because the time step is quite small.

2. Gaussian methods are symplectic (conservative). They preserve the system's energy. Even at a scale of 0.00001, they show no damping. Gauss-Legendre (3 stages) has a 6th-order accuracy. The error decays as N to the minus sixth power. For 100 steps, the error is negligible (about 10 to the minus 12th power), so the graph looks like an analytical solution.

3. The 3-stage Radau method has a 5th-order accuracy. It is so accurate that the decay over such a short time interval is negligible and merges with the energy conservation line.

**Our solution for visually demonstrating the error**

When choosing 100 steps, all the lines in the phase portrait merge into a perfect circle. Since the energy loss at the green Radau IIA line (2 stages) was only 0.07%, the radius of the circle has decreased by a microscopic amount, imperceptible to the naked eye on a graph with a scale of -1 to 1. The spiral is there, but the turns are too close together.

Higher-order methods (5th and 6th) are so good that at 100 steps they provide a nearly analytical solution. Therefore, we reduced the precision (by reducing the number of steps) to see the differences in the nature of the methods.

By reducing the number of steps to 25, we can visually detect at least a minimal difference in the results of these highly accurate methods from the ideal analytical solution.