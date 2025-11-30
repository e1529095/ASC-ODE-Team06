#include <iostream>
#include <fstream>
#include <nonlinfunc.hpp>
#include <timestepper.hpp>

using namespace ASC_ode;


class Circuit : public NonlinearFunction
{
private:
    double Resistance;
    double Capacity;
    double Voltage;

public:
    Circuit(double r, double c, double vs) : Resistance(r), Capacity(c), Voltage(vs) {}

    size_t dimX() const override { return 1; }
    size_t dimF() const override { return 1; }

    void evaluate(VectorView<double> x, VectorView<double> f) const override
    {
        f(0) = (Voltage - x(0)) / (Resistance * Capacity);
    }

    void evaluateDeriv(VectorView<double> x, MatrixView<double> df) const override
    {
        df = 0.0;
        df(0,0) = -1.0 / (Resistance * Capacity);
    }
};


int main()
{
    double R = 1000.0;
    double C = 1e-6;
    double V = 5.0;
    double tend = 0.5;
    int steps = 100;
    double tau = tend / steps;

    auto rhs = std::make_shared<Circuit>(R, C, V);


  // ===== Improved Euler =====
  {
    Vector<> y = { 1, 0 };
    ImprovedEuler stepper(rhs);

    std::ofstream outfile("CircuitImprovedEuler.txt");
    std::cout << 0.0 << "  " << y(0) << " " << y(1) << std::endl;
    outfile  << 0.0 << "  " << y(0) << " " << y(1) << std::endl;

    for(int i = 0; i < steps; i++)
    {
      stepper.DoStep(tau, y);
      std::cout << (i+1) * tau << "  " << y(0) << " " << y(1) << std::endl;
      outfile  << (i+1) * tau << "  " << y(0) << " " << y(1) << std::endl;
    }
  }

  // ===== Implicit Euler =====
  {
    Vector<> y = { 1, 0 };
    ImplicitEuler stepper(rhs);

    std::ofstream outfile("CircuitImplicitEuler.txt");
    std::cout << 0.0 << "  " << y(0) << " " << y(1) << std::endl;
    outfile  << 0.0 << "  " << y(0) << " " << y(1) << std::endl;

    for(int i = 0; i < steps; i++)
    {
      stepper.DoStep(tau, y);
      std::cout << (i+1) * tau << "  " << y(0) << " " << y(1) << std::endl;
      outfile  << (i+1) * tau << "  " << y(0) << " " << y(1) << std::endl;
    }
  }

  // ===== Crank-Nicolson =====
  {
    Vector<> y = { 1, 0 };
    CrankNicolson stepper(rhs);

    std::ofstream outfile("CircuitCrankNicolson.txt");
    std::cout << 0.0 << "  " << y(0) << " " << y(1) << std::endl;
    outfile  << 0.0 << "  " << y(0) << " " << y(1) << std::endl;

    for(int i = 0; i < steps; i++)
    {
      stepper.DoStep(tau, y);
      std::cout << (i+1) * tau << "  " << y(0) << " " << y(1) << std::endl;
      outfile  << (i+1) * tau << "  " << y(0) << " " << y(1) << std::endl;
    }
  }

  return 0;
}
