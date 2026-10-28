#include <iostream>
#include <string>
#include "ariadne-linear-programming.hpp"

using namespace Ariadne;
using std::cout;

int main() {
    section("Linear programming with the simplex solver");

    // Solve
    //   minimise x0 + 2*x1
    //   subject to x0 + x1 = 1
    //              0 <= x0,x1 <= 1.
    // The optimum is attained at (1,0).
    LinearSimplexSolver<FloatDP> simplex;
    Vector<FloatDP> objective({1.0_x, 2.0_x}, dp);
    Vector<FloatDP> lower({0.0_x, 0.0_x}, dp);
    Vector<FloatDP> upper({1.0_x, 1.0_x}, dp);
    Matrix<FloatDP> equality_matrix({{1.0_x, 1.0_x}}, dp);
    Vector<FloatDP> equality_rhs({1.0_x}, dp);

    ValidatedKleenean lp_feasible =
        simplex.feasible(lower, upper, equality_matrix, equality_rhs);
    FloatDPBoundsVector lp_solution =
        simplex.minimise(
            objective, lower, upper, equality_matrix, equality_rhs);

    show("LP feasible", lp_feasible);
    show("LP solution", lp_solution);

    return 0;
}
