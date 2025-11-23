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
