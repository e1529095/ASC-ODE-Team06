#include <iostream>
#include <autodiff.hpp>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>

using namespace ASC_ode;
using namespace std;

template <typename T>
T func1(T x, T y)
{
    return x * sin(y);
    // return 1e6 + y;
}

template <typename T>
void LegendrePolynomials(int n, T x, std::vector<T> &P)
{
    if (n < 0)
    {
        P.clear();
        return;
    }
    P.resize(n + 1);
    P[0] = T(1);
    if (n == 0)
        return;
    P[1] = x;
    for (int k = 2; k <= n; ++k)
    {
        P[k] = ((T(2 * k - 1) * x * P[k - 1]) - T(k - 1) * P[k - 2]) / T(k);
    }
}

int main(int argc, char *argv[])
{
    // EXERCISE 18.4 - added the legendre polynomial template for the autodiff

    string output_dir = argv[1];
    std::ofstream outfile(output_dir + "/legendre.tsv");

    const int nmax = 5;   // highest order P_n
    const int npts = 201; // number of x-points in [-1,1]

    for (int i = 0; i < npts; ++i)
    {
        double xv = -1.0 + 2.0 * i / double(npts - 1);

        // AutoDiff with 1 independent variable
        AutoDiff<1> x_ad = Variable<0>(xv);

        std::vector<AutoDiff<1>> P;
        LegendrePolynomials(nmax, x_ad, P);

        // Print: x, then for k=0..nmax: P_k(x), P_k'(x)
        outfile << xv;
        for (int k = 0; k <= nmax; ++k)
        {
            double val = P[k].value();
            double deriv = P[k].deriv()[0]; // derivative wrt x
            outfile << "\t" << val << "\t" << deriv;
        }
        outfile << "\n";
    }

    return 0;
}
