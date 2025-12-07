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
  cout << "Simulating Crane Structure -> " << filename << endl;
    
  MassSpringSystem<D> mss;
  mss.setGravity({0.0, -9.81});

  // ------------------ Configuration of the Crane
  double  seg_length  = 1.0;    // Horizontal length of one segment
  double  seg_height  = 1.0;    // Vertical height of the truss
  int     n_segments  = 8;      // Number of segments (length of crane)
  double  k           = 5000.0; // Stiffness. Needs to be high
  double  mass_val    = 1.0;    // Mass of structural nodes

  // Anchor Points (two fixed points)
  auto fix_bottom = mss.addFix({ {0.0, 0.0} });
  auto fix_top    = mss.addFix({ {0.0, seg_height} });

  // Arrays to keep track of the created nodes
  vector<Connector> bottom_nodes;
  vector<Connector> top_nodes;
  bottom_nodes.push_back(fix_bottom);
  top_nodes.push_back(fix_top);

  // Building the Truss
  for (int i = 1; i <= n_segments; i++)
  {
    double x = i * seg_length;
        
    // Making the tip heavier to simulate a load
    double m = (i == n_segments) ? 10.0 : mass_val;

    // Adding masses for this segment (top and bottom)
    // pos={x, 0} and {x, h}, vel={0,0}, acc={0,0}
    auto node_bot = mss.addMass({ m, {x, 0.0},        {0.0, 0.0}, {0.0, 0.0} });
    auto node_top = mss.addMass({ m, {x, seg_height}, {0.0, 0.0}, {0.0, 0.0} });

    // Getting previous nodes to connect to
    auto prev_bot = bottom_nodes.back();
    auto prev_top = top_nodes.back();

    // ------------------ The springs
        
    // Horizontal (Top and Bottom Chords)
    mss.addSpring({ seg_length, k, {prev_bot, node_bot} });
    mss.addSpring({ seg_length, k, {prev_top, node_top} });

    // Vertical (Strut)
    mss.addSpring({ seg_height, k, {node_bot, node_top} });

    // Diagonals (X-Bracing)
    double diag_len = sqrt(seg_length*seg_length + seg_height*seg_height);
    mss.addSpring({ diag_len, k, {prev_bot, node_top} });
    mss.addSpring({ diag_len, k, {prev_top, node_bot} });

    // Storing current nodes for the next iteration
    bottom_nodes.push_back(node_bot);
    top_nodes.push_back(node_top);
  }

  // Initial State
  Vector<> state(mss.masses().size()*D), v(mss.masses().size()*D), a(mss.masses().size()*D);
  mss.getState(state, v, a);
    
  mss.setState(state, v, a);

  // Solver Setup
  auto rhs = std::make_shared<ExactMSSFunction>(mss);
  auto mass_matrix = std::make_shared<IdentityFunction>(rhs->dimX());

  std::ofstream outfile(filename);
  outfile << "t";
  // Header for TSV x0 y0 x1 y1 and so on
  for(size_t i=0; i<mss.masses().size(); ++i) outfile << "\tx" << i << "\ty" << i;
  outfile << endl;

  auto callback = [&](double t, VectorView<double> x) {
    outfile << t;
    for (int i = 0; i < x.size(); i++) outfile << "\t" << x(i);
    outfile << endl;
  };

  SolveODE_Alpha(20.0, int(20.0/dt), 0.8, state, v, a, rhs, mass_matrix, callback);
}

// ------------------ Main procedure
int main(int argc, char *argv[])
{
    string output_dir = ".";
    if (argc > 1) output_dir = argv[1];
    
    RunSimulation(output_dir + "/crane_simulation.tsv", 0.01);
    return 0;
}