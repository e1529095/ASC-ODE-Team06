#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <memory>
#include <vector>
#include <mass_spring.hpp>
#include <Newmark.hpp>

using namespace ASC_ode;
using namespace std;

// Physical model of a double pendulum
class DoublePendulumDAE : public NonlinearFunction
{
    double L1, L2; // Lengths of the rods
    double m1, m2; // Masses
    double g;      // Gravity

public:
    DoublePendulumDAE(double _L1, double _L2, double _m1, double _m2)
        : L1(_L1), L2(_L2), m1(_m1), m2(_m2), g(9.81) { }

    // State vector: [x1, y1, x2, y2, lambda1, lambda2]
    virtual size_t dimX() const override { return 6; }
    virtual size_t dimF() const override { return 6; }

    virtual void evaluate(VectorView<double> x, VectorView<double> f) const override
    {
        // Unpack state
        Vec<2> p1 = {x(0), x(1)};
        Vec<2> p2 = {x(2), x(3)};
        double lam1 = x(4);
        double lam2 = x(5);

        // Anchor point is at (0,0)
        Vec<2> p0 = {0.0, 0.0};

        f = 0.0;

        // ------------------ Forces on Mass 1
        // Gravity
        f(1) -= m1 * g; 
        
        // Constraint 1: Fix(p0) <-> Mass1(p1)
        Vec<2> d1 = p1 - p0;
        f(0) -= lam1 * d1(0);
        f(1) -= lam1 * d1(1); 

        // Constraint 2: Mass1(p1) <-> Mass2(p2)
        Vec<2> d2 = p2 - p1;
        f(0) += lam2 * d2(0);
        f(1) += lam2 * d2(1);

        // ------------------ Forces on Mass 2
        // Gravity
        f(3) -= m2 * g;

        // Constraint 2: Mass1(p1) <-> Mass2(p2)
        f(2) -= lam2 * d2(0);
        f(3) -= lam2 * d2(1);

        // ------------------ Constraint Equations (Inner product)
        double d1_sq = d1(0)*d1(0) + d1(1)*d1(1);
        double d2_sq = d2(0)*d2(0) + d2(1)*d2(1);

        f(4) = 0.5 * (d1_sq - L1 * L1);
        f(5) = 0.5 * (d2_sq - L2 * L2);     
    }

    // ------------------ Calculating the Jacobian Matrix
    virtual void evaluateDeriv(VectorView<double> x, MatrixView<double> df) const override
    {
        
        Vec<2> p1 = {x(0), x(1)};
        Vec<2> p2 = {x(2), x(3)};
        double lam1 = x(4);
        double lam2 = x(5);
        Vec<2> p0 = {0.0, 0.0};

        df = 0.0;

        Vec<2> d1 = p1 - p0;
        Vec<2> d2 = p2 - p1;

        // Term: -lambda1 * (p1 - p0)  -> deriv w.r.t p1 is -lambda1 * I
        df(0, 0) -= lam1; 
        df(1, 1) -= lam1;

        // Term: +lambda2 * (p2 - p1) -> deriv w.r.t p1 is -lambda2 * I
        df(0, 0) -= lam2;
        df(1, 1) -= lam2;
        
        // deriv w.r.t p2 is +lambda2 * I
        df(0, 2) += lam2;
        df(0, 3) += 0.0;
        df(1, 3) += lam2;

        // Term (Force on M2): -lambda2 * (p2 - p1)
        
        // deriv w.r.t p2 is -lambda2 * I
        df(2, 2) -= lam2;
        df(3, 3) -= lam2;
        
        // deriv w.r.t p1 is +lambda2 * I
        df(2, 0) += lam2;
        df(3, 1) += lam2;


        // ------------------ Gradient of constraints
        
        // Constraint 1 acting on M1
        df(0, 4) = -d1(0); 
        df(1, 4) = -d1(1);

        // Constraint 2 acting on M1
        df(0, 5) = d2(0);
        df(1, 5) = d2(1);

        // Constraint 2 acting on M2
        df(2, 5) = -d2(0);
        df(3, 5) = -d2(1);


        // ------------------ d(Constraints)/dx

        // Constraint 1: 0.5*(|p1|^2 - L^2) -> deriv is p1^T
        df(4, 0) = d1(0);
        df(4, 1) = d1(1);

        // Constraint 2: 0.5*(|p2-p1|^2 - L^2)
        
        // deriv w.r.t p1 is -(p2-p1)^T
        df(5, 0) = -d2(0);
        df(5, 1) = -d2(1);
        
        // deriv w.r.t p2 is (p2-p1)^T
        df(5, 2) = d2(0);
        df(5, 3) = d2(1);
    }
};

// ------------------ Singular Mass Matrix for DAE
class MassMatrixDAE : public NonlinearFunction
{
    double m1, m2;
public:
    MassMatrixDAE(double _m1, double _m2) : m1(_m1), m2(_m2) {}
    
    virtual size_t dimX() const override { return 6; }
    virtual size_t dimF() const override { return 6; }

    virtual void evaluate(VectorView<double> x, VectorView<double> f) const override
    {
        f = 0.0;
        f(0) = m1 * x(0);
        f(1) = m1 * x(1);
        f(2) = m2 * x(2);
        f(3) = m2 * x(3);
    }

    virtual void evaluateDeriv(VectorView<double> x, MatrixView<double> df) const override
    {
        df = 0.0;
        df(0,0) = m1;
        df(1,1) = m1;
        df(2,2) = m2;
        df(3,3) = m2;
    }
};

// ------------------ Function for running a simulation
void RunSimulation(string filename, double dt)
{
    cout << "Simulating Double Pendulum -> " << filename << endl;

    double L1 = 1.0, L2 = 1.0;
    double m1 = 1.0, m2 = 1.0;

    auto rhs = std::make_shared<DoublePendulumDAE>(L1, L2, m1, m2);
    auto mass = std::make_shared<MassMatrixDAE>(m1, m2);

    // Initial Conditions
    Vector<> x(6), v(6), a(6);
    x = 0.0; v = 0.0; a = 0.0;

    // Start in horizontal position (90 degrees)

    // Mass 1 at (1, 0)
    x(0) = L1; x(1) = 0.0;
    
    // Mass 2 at (2, 0)
    x(2) = L1 + L2; x(3) = 0.0;

    std::ofstream outfile(filename);
    outfile << "t\tx0\ty0\tx1\ty1\tlam1\tlam2" << endl;

    auto callback = [&](double t, VectorView<double> state) {
        outfile << t 
                << "\t" << state(0) << "\t" << state(1) 
                << "\t" << state(2) << "\t" << state(3)
                << "\t" << state(4) << "\t" << state(5) << endl;
    };

    SolveODE_Alpha(10.0, int(10.0/dt), 0.9, x, v, a, rhs, mass, callback);
}

// ------------------ Main procedure
int main(int argc, char *argv[])
{
    string output_dir = ".";
    if (argc > 1) output_dir = argv[1];
    
    RunSimulation(output_dir + "/double_pendulum_simulation.tsv", 0.01);
    return 0;
}