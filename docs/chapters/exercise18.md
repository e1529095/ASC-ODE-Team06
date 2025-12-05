# Exercise 18
Exercise 18 introduced and built on an automatic differentiation framework in
`C++`.

The following operators and functions were added to the
[`AutoDiff`](https://github.com/e1529095/ASC-ODE-Team06/blob/5a7110b6f9ebedb97474925afe85e126745d23c4/src/autodiff.hpp#L28)
class:
  - [`Subtraction`](https://github.com/e1529095/ASC-ODE-Team06/blame/5a7110b6f9ebedb97474925afe85e126745d23c4/src/autodiff.hpp#L99)
  - [`Division`](https://github.com/e1529095/ASC-ODE-Team06/blame/5a7110b6f9ebedb97474925afe85e126745d23c4/src/autodiff.hpp#L119)
  - [`Scalar multiplication`](https://github.com/e1529095/ASC-ODE-Team06/blame/5a7110b6f9ebedb97474925afe85e126745d23c4/src/autodiff.hpp#L135)
  - [`Cosine`](https://github.com/e1529095/ASC-ODE-Team06/blame/5a7110b6f9ebedb97474925afe85e126745d23c4/src/autodiff.hpp#L168)
  - [`Log`](https://github.com/e1529095/ASC-ODE-Team06/blame/5a7110b6f9ebedb97474925afe85e126745d23c4/src/autodiff.hpp#L188)
  - [`Exponential`](https://github.com/e1529095/ASC-ODE-Team06/blame/5a7110b6f9ebedb97474925afe85e126745d23c4/src/autodiff.hpp#L177)

The newly included features were applied it to:
1. Legendre polynomials to obtain function values and derivatives
simultaneously. The plot of Legendre-polynomials up to order 5, in the interval
$[-1,1]$ were created, including their derivatives
2. A nonlinear **ODE** system (pendulum) to automatically generate the Jacobian
used in implicit time integration.

The key takeaway for this exercise is that once the
autodiff infrastructure (AutoDiff + operators) is in place, derivative
information becomes easy for any function expressed in terms of the supported
operations, significantly simplifying the implementation of nonlinear solvers
and time-stepping schemes.

```{figure} ./../assets/exercise18/plot/Legendre_poly_plots.png
:label: normal_plot
Legendre Poly Plot
```

```{figure} ./../assets/exercise18/plot/Legendre_poly_plots_deriv_plots.png
:label: deriv_plot
Legendre Poly Plot
```
