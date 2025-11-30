#include <iostream>
#include "pendulum_ad.hpp"

using namespace ASC_ode;

int main()
{
    PendulumAD pend(1.0, 9.81);

    Vector<double> x(2);
    x(0) = 0.5;
    x(1) = 0.0;

    Vector<double> f(2);
    Matrix<double> df(2,2);

    pend.evaluate(x, f);
    pend.evaluateDeriv(x, df);

    std::cout << "f(x) = (" << f(0) << ", " << f(1) << ")\n";
    std::cout << "Df(x) =\n";
    std::cout << df(0,0) << " " << df(0,1) << "\n";
    std::cout << df(1,0) << " " << df(1,1) << "\n";

    return 0;
}
