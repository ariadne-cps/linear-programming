/***************************************************************************
 *            tutorial/tutorial_linear_programming.cpp
 *
 *  Copyright  2026  Luca Geretti
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne Linear Programming.
 *
 *  Ariadne Linear Programming is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Ariadne Linear Programming is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne Linear Programming. If not, see <https://www.gnu.org/licenses/>.
 */

/** @file tutorial/tutorial_linear_programming.cpp
 *  @brief Demonstrate linear programming optimization and feasibility APIs.
 */

#include <iostream>
#include <string>

#include "ariadne-linear-programming.hpp"

using namespace Ariadne;
using std::cout;

namespace {
void section(const std::string& title) {
    cout << "\n== " << title << " ==\n";
}

template<class T>
void show(const std::string& label, const T& value) {
    cout << label << ": " << value << "\n";
}
} // namespace

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
    Matrix<FloatDP> equality_matrix({
                                        {1.0_x, 1.0_x}
    },
                                    dp);
    Vector<FloatDP> equality_rhs({1.0_x}, dp);

    ValidatedKleenean lp_feasible =
        simplex.feasible(lower, upper, equality_matrix, equality_rhs);
    FloatDPBoundsVector lp_solution =
        simplex.minimise(
            objective, lower, upper, equality_matrix, equality_rhs);

    show("LP feasible", lp_feasible);
    show("LP solution", lp_solution);

    section("Linear programming with the interior-point method");
    // Same bounded linear program, using a primal-dual interior-point algorithm.
    LinearInteriorPointSolver interior_point;
    auto interior_result = interior_point.minimise(
        objective, lower, upper, equality_matrix, equality_rhs);
    show("Interior-point objective", std::get<0>(interior_result));
    show("Interior-point primal vector", std::get<1>(interior_result));
    show("Interior-point dual vector", std::get<2>(interior_result));
    show("Interior-point feasibility",
         interior_point.feasible(lower, upper, equality_matrix, equality_rhs));

    return 0;
}
