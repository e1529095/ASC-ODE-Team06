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

constexpr int D = 2; // Dimensionality (2D)

class ExactMSSFunction : public MSS_Function<D>
{
private:
  MassSpringSystem<D> & mss_ref; 

public:
  ExactMSSFunction(MassSpringSystem<D> & _mss) 
    : MSS_Function<D>(_mss), mss_ref(_mss) { }

  virtual void evaluateDeriv(VectorView<double> x, MatrixView<double> df) const override
  {
    // Checking dimensions
    size_t n_masses = mss_ref.masses().size();
    df = 0.0;
    auto xmat = x.asMatrix(n_masses, D);

    for (const auto& spring : mss_ref.springs())
    {
      auto [c1, c2] = spring.connectors;
            
      if (c1.type == Connector::FIX && c2.type == Connector::FIX) continue;

        // Getting positions
        Vec<D> p1 = (c1.type == Connector::FIX) ? mss_ref.fixes()[c1.nr].pos : xmat.row(c1.nr);
        Vec<D> p2 = (c2.type == Connector::FIX) ? mss_ref.fixes()[c2.nr].pos : xmat.row(c2.nr);

        Vec<D> d = p2 - p1;
        double r = norm(d);
        if (r < 1e-12) continue;

        Vec<D> u = (1.0 / r) * d;

        double k = spring.stiffness;
        double l0 = spring.length;

        double alpha = k * (1.0 - l0 / r); 
        double beta  = k * (l0 / r);      

        double K_local[D][D];
        for (int i = 0; i < D; i++)
          for (int j = 0; j < D; j++)
            K_local[i][j] = (i == j ? alpha : 0.0) + beta * u(i) * u(j);

        // Filling the global matrix
        if (c1.type == Connector::MASS)
        {
          double invm1 = 1.0 / mss_ref.masses()[c1.nr].mass;
          size_t idx1 = c1.nr;
                
          for (int i=0; i<D; i++) for (int j=0; j<D; j++) {
            df(idx1*D + i, idx1*D + j) -= invm1 * K_local[i][j];
                    
            if (c2.type == Connector::MASS)
              df(idx1*D + i, c2.nr*D + j) += invm1 * K_local[i][j];
            }
        }

        if (c2.type == Connector::MASS)
        {
          double invm2 = 1.0 / mss_ref.masses()[c2.nr].mass;
          size_t idx2 = c2.nr;

          for (int i=0; i<D; i++) for (int j=0; j<D; j++) {
            df(idx2*D + i, idx2*D + j) -= invm2 * K_local[i][j];

            if (c1.type == Connector::MASS)
              df(idx2*D + i, c1.nr*D + j) += invm2 * K_local[i][j];
            }
        }
    }
  }
};

// ------------------ Function for running a simulation
void RunSimulation(string filename, double dt)
{
    cout << "Simulating: " << filename << endl;
    
    MassSpringSystem<D> mss;
    mss.setGravity({0.0, -9.81});

    // Creating a chain
    auto fix = mss.addFix({ {0.0, 0.0} });
    
    double m = 1.0;
    // Adding the mass 'm' as the first argument
    // Structure: { mass, pos, vel, acc }
    // pos={x,y}, vel={0,0}, acc={0,0}
    auto m1 = mss.addMass({ m, {1.0, 0.0}, {0.0, 0.0}, {0.0, 0.0} }); 
    auto m2 = mss.addMass({ m, {2.0, 0.0}, {0.0, 0.0}, {0.0, 0.0} });
    auto m3 = mss.addMass({ m, {3.0, 0.0}, {0.0, 0.0}, {0.0, 0.0} });

    // Springs
    double k = 1000.0;
    mss.addSpring({ 1.0, k, {fix, m1} });
    mss.addSpring({ 1.0, k, {m1, m2} });
    mss.addSpring({ 1.0, k, {m2, m3} });

    // Initial disturbance of the system
    Vector<> state(mss.masses().size()*D), v(mss.masses().size()*D), a(mss.masses().size()*D);
    mss.getState(state, v, a);
    state(4) = 2.5; state(5) = -1.0; 
    mss.setState(state, v, a);

    auto rhs = std::make_shared<ExactMSSFunction>(mss);
    auto mass_matrix = std::make_shared<IdentityFunction>(rhs->dimX());

    std::ofstream outfile(filename);
    outfile << "t";
    for(size_t i=0; i<mss.masses().size(); ++i) outfile << "\tx" << i << "\ty" << i;
    outfile << endl;

    auto callback = [&](double t, VectorView<double> x) {
        outfile << t;
        for (int i = 0; i < x.size(); i++) outfile << "\t" << x(i);
        outfile << endl;
    };

    SolveODE_Alpha(10.0, int(10.0/dt), 0.8, state, v, a, rhs, mass_matrix, callback);
}

// ------------------ Main procedure
int main(int argc, char *argv[])
{
    string output_dir = ".";
    if (argc > 1) output_dir = argv[1];
    
    RunSimulation(output_dir + "/chain_simulation.tsv", 0.01);
    return 0;
}