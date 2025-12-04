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

**Notes for the Report and Plots for Exercise 17.4.1**

In the exercise 17.4.1 we did the simulation of the Mass-Spring system, and it clearly demonstrates the stability characteristics of the three methods applied to a conservative problem.

We implemented the methods in exercise17_massspring.cpp file and put results and plots in the appropriate folders.

**Results and discussion**

The Improved Euler method (red line), being an explicit scheme, exhibits a lack of long-term stability for this oscillatory system. As seen in the phase portraits (especially with steps = 200), the solution spirals outward and the total energy grows unbounded.

This confirms that explicit methods can introduce artificial energy into a system, making them unsuitable for long-term simulations of orbital or frictionless mechanics without extremely small time steps.

In contrast, the Implicit Euler method (green line) demonstrates strong L-stability. While it is unconditionally stable and does not "explode" like the explicit method, it introduces significant numerical damping.

The phase portrait spirals inward rapidly, and the energy plot shows a decay to zero, effectively acting as if the system had heavy friction. While this method is safe from instability, it is physically inaccurate for modeling energy-preserving systems because it kills the oscillations too quickly.

The Crank-Nicolson method (blue line) proves to be the optimal choice for the Mass-Spring system. It is A-stable but does not possess the strong damping properties of L-stable methods. Consequently, it perfectly preserves the periodic nature of the system: the phase portrait remains a closed circle, and the total energy remains constant at 0.5 over time.

This illustrates why Crank-Nicolson is the standard for conservative problems where preserving the system's energy is a priority over damping out high frequencies.

**Notes for the Report and Plots for Exercise 17.4.2**

In the exercise 17.4.2 we did the RC Circuit simulation, and highlights the concept of stiffness in ODEs.

We implemented the methods in exercise17_circuit.cpp file and put results and plots in the appropriate folders.

**Results and discussion**

With the chosen parameters (R = 1000, C = 10 to the power of -6), the system has a very fast reaction time. When using a time step that is large relative to this reaction time, the Improved Euler method fails catastrophically, with the voltage crashing to negative infinity. This "explosion" occurs because the explicit method's stability domain was violated by the large step size.

**Summary for exercise 17**

In summary, these exercises confirm that while explicit methods like Improved Euler are simple to implement, they are dangerous for stiff systems (like the circuit) and conservative systems (like the spring) unless the time step is vanishingly small.

Implicit methods require solving a system of equations at every step (using Newton's method), but they offer superior stability. Specifically, Implicit Euler is best for damping out noise in stiff systems, while Crank-Nicolson is superior for preserving the physics of oscillating systems.

----------------------------------------------------------------------------------------

**Notes for the Report and Plots for Exercise 18**

Exercise 18 introduced and built on an automatic differentiation framework in C++

The following operators and functions were added to the autodiff class: Subtraction, division, scalar multiplication, cosine, log and exponential.


The newly included features were applied it to:

1. Legendre polynomials to obtain function values and derivatives simultaneously. The plot of Legendre-polynomials up to order 5, in the interval [-1,1] were created, including their derivatives

2. A nonlinear ODE system (pendulum) to automatically generate the Jacobian used in implicit time integration.


Th key takeaway for this exercise is that once the autodiff infrastructure (AutoDiff + operators) is in place, derivative information becomes easy for any function expressed in terms of the supported operations, significantly simplifying the implementation of nonlinear solvers and time-stepping schemes.

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
