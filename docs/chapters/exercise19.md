# Exercise 19
To complete the assignment, we added the file
[`exercise19_runge_kutta.cpp`](https://github.com/e1529095/ASC-ODE-Team06/blob/main/src/exercise19_runge_kutta.cpp)
to the project, in which we reproduced the **Gaussian** and **Rado**
implementations for the **Runge-Kutta** methods.
The experiment showed that the results of highly accurate methods on short time
scales are very difficult to visually distinguish.

This occurs for a number of reasons:
1. **Rado** methods are **L-stable**. This means that they introduce
_numerical damping_ (artificial viscosity). The system loses energy, but very
slowly because the time step is quite small.
2. **Gaussian** methods are symplectic (conservative). They preserve the
system's energy. Even at a scale of $0.00001$, they show no damping.
**Gauss-Legendre** (3 stages) has a 6th-order accuracy. The error decays as `N`
to the minus sixth power. For $100$ steps, the error is negligible (about $10$
to the minus 12th power), so the graph looks like an analytical solution.
3. The 3-stage **Radau** method has a 5th-order accuracy. It is so accurate that
the decay over such a short time interval is negligible and merges with the
energy conservation line.

## Visually demonstration of the error
When choosing $100$ steps, all the lines in the phase portrait merge into a
perfect circle. Since the energy loss at the green Radau IIA line (2 stages) was
only $0.07%$, the radius of the circle has decreased by a microscopic amount,
imperceptible to the naked eye on a graph with a scale of $-1$ to $1$. The
spiral is there, but the turns are too close together.

Higher-order methods (5th and 6th) are so good that at $100$ steps they provide
a nearly analytical solution. Therefore, we reduced the precision (by reducing
the number of steps) to see the differences in the nature of the methods.

By reducing the number of steps to $25$, we can visually detect at least a
minimal difference in the results of these highly accurate methods from the
ideal analytical solution.

```{figure} ./../assets/exercise19/plot/E19_Energy_Conservation_Check_25_steps.png
:label: energy_conservative_25
Energy Conservation check with 25 steps.
```

```{figure} ./../assets/exercise19/plot/E19_Energy_Conservation_Check_100_steps.png
:label: energy_conservative_100
Energy Conservation check with 100 steps.
```

```{figure} ./../assets/exercise19/plot/E19_Phase_Space_Comparison_25_steps.png
:label: phase_space_25
Phase Space comparison with 25 steps.
```

```{figure} ./../assets/exercise19/plot/E19_Phase_Space_Comparison_100_steps.png
:label: phase_space_100
Phase Space comparison with 100 steps.
```
