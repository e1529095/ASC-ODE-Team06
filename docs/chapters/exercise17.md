# Exercise 17
## [Exercise 17.2.2](https://jschoeberl.github.io/IntroSC/ODEs/implementation_ee.html#exercise)
In this exercise, we implemented and analyzed time-stepping methods for solving
**Ordinary Differential Equations (ODE)**. In particular, we modeled a
mass-spring system governed by the simple harmonic motion equation. This
second-order equation was reduced to a first-order system to be solved
numerically.

### Implementation
Two distinct methods were implemented in `C++`:
1. [`ExplicitEuler`](https://github.com/e1529095/ASC-ODE-Team06/blob/5a7110b6f9ebedb97474925afe85e126745d23c4/src/timestepper.hpp#L23)
Method: A first-order method.
2. [`ImprovedEuler`](https://github.com/e1529095/ASC-ODE-Team06/blob/5a7110b6f9ebedb97474925afe85e126745d23c4/src/timestepper.hpp#L57)
Method: A second-order method defined by the two-stage
process. We used the Midpoint rule.

### The code
The implementation was carried out in the
[timestepper.hpp](https://github.com/e1529095/ASC-ODE-Team06/blob/main/src/timestepper.hpp)
file. A base class `TimeStepper` was used to define the interface. We also used
a Python script to plot the calculation results.

The `ExplicitEuler` class updates the state vector by evaluating the
**Right-Hand Side (RHS)** at the current state and adding the scaled derivative.

The `ImprovedEuler` class was added as an extension. It requires an
intermediate vector storage (`m_ytilde`) to hold the half-step prediction.
The **RHS** is evaluated twice per step: once to find the midpoint state, and
again at the midpoint to determine the final slope for the full time-step
update.

### Results and Discussion
We performed simulations using both methods with varying time-steps, represented
by the number of steps `N` over the simulation period. The results were
visualized using time evolution plots and phase-space plots
(Velocity vs Position).

#### Explicit Euler Analysis
The **Explicit Euler** method showed significant limitations characteristic of
first-order explicit schemes applied to oscillatory systems:
1. The method fails to conserve energy. As seen in the phase plots (spiraling
outwards), the system artificially gains energy over time.
2. With smaller time-steps (500 or 1000 steps), the orbit is roughly elliptical
but still exhibits distortion over long periods. With larger time-steps
(e.g., 100 steps), the method becomes unstable, and the phase plot spirals
outward rapidly.

#### Improved Euler Analysis
The **Improved Euler** method demonstrated superior performance:
1. Even with fewer time-steps, the method produced a nearly perfect circular
phase plot, which aligns with the theoretical conservation of energy for a
harmonic oscillator.
2. The oscillations remained stable over the simulation time, avoiding the
spiraling artifact seen in the standard Euler method.

### Conclusion
The comparison highlights the importance of choosing an appropriate integration
scheme for **ODE**s. While the **Explicit Euler** method is simple to implement,
it is unreliable for conservative systems like the mass-spring model due to
energy drift.

We should also note that the **Improved Euler** method, by evaluating the
derivative at the midpoint, achieves much higher accuracy and preserves the
geometric properties (circular phase orbits) of the system significantly better.

## [Exercise 17.4.1](https://jschoeberl.github.io/IntroSC/ODEs/implementation_ie.html#excercises)
In this exercise we did the simulation of the Mass-Spring system, and it
clearly demonstrates the stability characteristics of the three methods applied
to a conservative problem.

We implemented the methods in
[`exercise17_massspring.cpp`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/src/exercise17_massspring.cpp).

### Results and discussion
The **Improved Euler** method (red line), being an explicit scheme, exhibits a
lack of long-term stability for this oscillatory system. As seen in the phase
portraits (especially with steps = 200), the solution spirals outward and the
total energy grows unbounded.

This confirms that explicit methods can introduce artificial energy into a
system, making them unsuitable for long-term simulations of orbital or
frictionless mechanics without extremely small time steps.

In contrast, the **Implicit Euler** method (green line) demonstrates strong
**L-stability**. While it is unconditionally stable and does not _explode_ like
the explicit method, it introduces significant numerical damping.

The phase portrait spirals inward rapidly, and the energy plot shows a decay to
zero, effectively acting as if the system had heavy friction. While this method
is safe from instability, it is physically inaccurate for modeling
energy-preserving systems because it kills the oscillations too quickly.

The **Crank-Nicolson** method (blue line) proves to be the optimal choice for
the Mass-Spring system. It is **A-stable** but does not possess the strong
damping properties of **L-stable** methods. Consequently, it perfectly preserves
the periodic nature of the system: the phase portrait remains a closed circle,
and the total energy remains constant at `0.5` over time.

This illustrates why **Crank-Nicolson** is the standard for conservative
problems where preserving the system's energy is a priority over damping out
high frequencies.

## [Exercise 17.4.2](https://jschoeberl.github.io/IntroSC/ODEs/implementation_ie.html#stability-function)
In this exercise we did the RC Circuit simulation, and highlights the
concept of stiffness in ODEs.

We implemented the methods in [`exercise17_circuit.cpp`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/src/exercise17_circuit.cpp).

### Results and discussion
With the chosen parameters (`R = 1000`, `C = 10` to the power of `-6`), the
system has a very fast reaction time. When using a time step that is large
relative to this reaction time, the **Improved Euler** method fails
catastrophically, with the voltage crashing to negative infinity. This
_explosion_ occurs because the explicit method's stability domain was violated
by the large step size.

## Summary for exercise 17
In summary, these exercises confirm that while explicit methods like
**Improved Euler** are simple to implement, they are dangerous for stiff systems
(like the circuit) and conservative systems (like the spring) unless the time
step is vanishingly small.

Implicit methods require solving a system of equations at every step (using
Newton's method), but they offer superior stability. Specifically, Implicit
Euler is best for damping out noise in stiff systems, while Crank-Nicolson is
superior for preserving the physics of oscillating systems.
