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
    // Voltage is not a constant, it depends on time

public:
    Circuit(double r, double c) : Resistance(r), Capacity(c) {}

    size_t dimX() const override { return 2; }   // System dimension: 2 (voltage and time)
    size_t dimF() const override { return 2; }

    void evaluate(VectorView<double> x, VectorView<double> f) const override
    {
        double Uc = x(0);
        double t = x(1);
        double U_source = cos(100 * M_PI * t); // Voltage source
        
        f(0) = (U_source - Uc) / (Resistance * Capacity);
        f(1) = 1.0; // Derivative of time with respect to time
    }

    void evaluateDeriv(VectorView<double> x, MatrixView<double> df) const override
    {
        double t = x(1);
        
        df = 0.0;
        df(0,0) = -1.0 / (Resistance * Capacity);
        df(0,1) = (-100 * M_PI * sin(100 * M_PI * t)) / (Resistance * Capacity);
    }
};


int main()
{
    double R = 1000.0;    
    double C = 1e-6;           
    double tend = 0.5;  
    //int steps = 100;
    int steps = 80;
    double tau = tend / steps;

    auto rhs = std::make_shared<Circuit>(R, C);


  // ===== Improved Euler =====
  {
    Vector<> y = { 0.0, 0.0 };  // Initial condition for voltage at capacitor 0,0
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
    Vector<> y = { 0.0, 0.0 };
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
    Vector<> y = { 0.0, 0.0 };
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
