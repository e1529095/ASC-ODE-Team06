#include <iostream>
#include <string>
#include <fstream>
#include "pendulum_ad.hpp"

using namespace ASC_ode;
using namespace std;

int main(int argc, char *argv[])
{
    string output_dir = argv[1];
    std::ofstream outfile(output_dir + "/pendulum.tsv");

    PendulumAD pend(1.0, 9.81);

    Vector<double> x(2);
    x(0) = 0.5;
    x(1) = 0.0;

    Vector<double> f(2);
    Matrix<double> df(2, 2);

    pend.evaluate(x, f);
    pend.evaluateDeriv(x, df);

    outfile << "f(x) = (" << f(0) << ", " << f(1) << ")\n";
    outfile << "Df(x) =\n";
    outfile << df(0, 0) << "\t" << df(0, 1) << "\n";
    outfile << df(1, 0) << "\t" << df(1, 1) << "\n";

    return 0;
}
