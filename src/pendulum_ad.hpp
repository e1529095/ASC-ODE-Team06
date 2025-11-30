#pragma once

#include "autodiff.hpp"
#include "nonlinfunc.hpp"

namespace ASC_ode {

    class PendulumAD : public NonlinearFunction
{
    private:
    double m_length;
    double m_gravity;

    public:
    PendulumAD(double length, double gravity=9.81) : m_length(length), m_gravity(gravity) {}

    size_t dimX() const override { return 2; }
    size_t dimF() const override { return 2; }

    void evaluate (VectorView<double> x, VectorView<double> f) const override
    {
        T_evaluate<double>(x, f);
    }

    void evaluateDeriv (VectorView<double> x, MatrixView<double> df) const override
    {
        Vector<AutoDiff<2>> x_ad(2);
        Vector<AutoDiff<2>> f_ad(2);

        x_ad(0) = Variable<0>(x(0));
        x_ad(1) = Variable<1>(x(1));
        T_evaluate<AutoDiff<2>>(x_ad, f_ad);

        for (size_t i = 0; i < 2; i++)
        for (size_t j = 0; j < 2; j++)
            df(i,j) = f_ad(i).deriv()[j];
    }

    template <typename T>
    void T_evaluate (VectorView<T> x, VectorView<T> f) const

    {
        f(0) = x(1);
        f(1) = -m_gravity/m_length*sin(x(0));
    }
    };

}
