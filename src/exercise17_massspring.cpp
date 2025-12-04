#include <iostream>
#include <fstream>
#include <nonlinfunc.hpp>
#include <timestepper.hpp>
#include <string>

using namespace ASC_ode;
using namespace std;

class MassSpring : public NonlinearFunction
{
private:
  double mass;
  double stiffness;

public:
  MassSpring(double m, double k) : mass(m), stiffness(k) {}

  size_t dimX() const override { return 2; }
  size_t dimF() const override { return 2; }

  void evaluate(VectorView<double> x, VectorView<double> f) const override
  {
    f(0) = x(1);
    f(1) = -stiffness / mass * x(0);
  }

  void evaluateDeriv(VectorView<double> x, MatrixView<double> df) const override
  {
    df = 0.0;
    df(0, 1) = 1;
    df(1, 0) = -stiffness / mass;
  }
};

void RunSimulation(string output_dir, int steps)
{
  string steps_string = to_string(steps);
  double tend = 20 * M_PI;
  double tau = tend / steps;

  auto rhs = std::make_shared<MassSpring>(1.0, 1.0);

  // ===== Improved Euler =====
  {
    Vector<> y = {1, 0};
    ImprovedEuler stepper(rhs);

    std::ofstream outfile(output_dir + "/massspring_improved_euler_" + steps_string + ".tsv");
    outfile << 0.0 << "\t" << y(0) << "\t " << y(1) << std::endl;

    for (int i = 0; i < steps; i++)
    {
      stepper.DoStep(tau, y);
      outfile << (i + 1) * tau << "\t" << y(0) << "\t" << y(1) << std::endl;
    }
  }

  // ===== Implicit Euler =====
  {
    Vector<> y = {1, 0};
    ImplicitEuler stepper(rhs);

    std::ofstream outfile(output_dir + "/massspring_implicit_euler_" + steps_string + ".tsv");
    outfile << 0.0 << "\t" << y(0) << "\t" << y(1) << std::endl;

    for (int i = 0; i < steps; i++)
    {
      stepper.DoStep(tau, y);
      outfile << (i + 1) * tau << "\t" << y(0) << "\t" << y(1) << std::endl;
    }
  }

  // ===== Crank-Nicolson =====
  {
    Vector<> y = {1, 0};
    CrankNicolson stepper(rhs);

    std::ofstream outfile(output_dir + "/massspring_crank_nicolson_" + steps_string + ".tsv");
    outfile << 0.0 << "\t" << y(0) << "\t" << y(1) << std::endl;

    for (int i = 0; i < steps; i++)
    {
      stepper.DoStep(tau, y);
      outfile << (i + 1) * tau << "\t" << y(0) << "\t" << y(1) << std::endl;
    }
  }
}

int main(int argc, char *argv[])
{
  string output_dir = argv[1];
  RunSimulation(output_dir, 200);
  RunSimulation(output_dir, 1000);

  return 0;
}
