#include <iostream>
#include <fstream> 
#include <string>
#include <cmath>
#include <memory>

#include <nonlinfunc.hpp>
#include <timestepper.hpp>
#include <implicitRK.hpp> 

using namespace ASC_ode;
using namespace std;

// ------------------ Physical model
class MassSpring : public NonlinearFunction
{
private:
  double mass;
  double stiffness;

public:
  MassSpring(double m, double k) : mass(m), stiffness(k) {}

  size_t dimX() const override { return 2; }
  size_t dimF() const override { return 2; }
  
  void evaluate (VectorView<double> x, VectorView<double> f) const override
  {
    f(0) = x(1);
    f(1) = -stiffness/mass*x(0);
  }
  
  void evaluateDeriv (VectorView<double> x, MatrixView<double> df) const override
  {
    df = 0.0;
    df(0,1) = 1;
    df(1,0) = -stiffness/mass;
  }
};

// ------------------ Function for running a simulation
void RunSimulation(string filename, int stages, bool useRadau)
{
    cout << "Running: " << filename << " (Stages: " << stages << ", Type: " 
         << (useRadau ? "Radau IIA" : "Gauss-Legendre") << ")" << endl;

    // Setting up time and model
    double tend = 4 * M_PI;
    //int steps = 100;
    int steps = 25; // To see the error better
    double tau = tend / steps;
    
    Vector<> y = { 1, 0 }; // Initial conditions: displacement 1, velocity 0
    auto rhs = std::make_shared<MassSpring>(1.0, 1.0);

    // Node generation (c)
    Vector<> c(stages), w(stages);
    if (useRadau) {
        GaussRadau(c, w);
    } else {
        GaussLegendre(c, w);
    }

    // Calculation of coefficients (A, b)
    auto [A, b] = ComputeABfromC(c);

    // Creating a solver
    ImplicitRungeKutta stepper(rhs, A, b, c);

    // Saving to file
    std::ofstream outfile(filename);
    outfile << 0.0 << "  " << y(0) << " " << y(1) << std::endl;

    for (int i = 0; i < steps; i++)
    {
       stepper.DoStep(tau, y); 
       outfile << (i+1) * tau << "  " << y(0) << " " << y(1) << std::endl;
    }
    cout << "  -> Saved to " << filename << endl;
}

// ------------------ Main procedure (Running 4 simulations one after another)
int main()
{
  // Gauss-Legendre (2 stages) -> Order 4
  RunSimulation("results_GL_2.txt", 2, false);

  // Gauss-Legendre (3 stages) -> Order 6
  RunSimulation("results_GL_3.txt", 3, false);

  // Radau IIA (2 stages) -> Order 3, L-stable
  RunSimulation("results_Radau_2.txt", 2, true);

  // Radau IIA (3 stages) -> Order 5, L-stable
  RunSimulation("results_Radau_3.txt", 3, true);

  return 0;
}