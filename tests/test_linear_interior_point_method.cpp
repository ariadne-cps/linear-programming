/***************************************************************************
 *            test_linear_interior_point_method.cpp
 *
 *  Copyright  2009-20  Pieter Collins
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne.
 *
 *  Ariadne is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Ariadne is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include "macro/testing.hpp"

#include "numeric/numeric.hpp"
#include "algebra/vector.hpp"
#include "algebra/matrix.hpp"
#include "solving/linear_interior_point_method.hpp"
#include "logging/logging_command_line_interface.hpp"
#include "logging/logging.hpp"

using namespace std;
using namespace Ariadne;

double infd=inf.get_d();

class TestLinearInteriorPointSolver
{
    LinearInteriorPointSolver* optimiser;
  public:
    TestLinearInteriorPointSolver(LinearInteriorPointSolver& o) : optimiser(&o) { }

    Void test() {
        ARIADNE_TEST_CALL(test_validate_feasibility());
        ARIADNE_TEST_CALL(test_validate_feasibility_singular());
        ARIADNE_TEST_CALL(test_feasibility_after_weighted_interior_step());
        ARIADNE_TEST_CALL(test_uncertified_full_primal_step());
        ARIADNE_TEST_CALL(test_status_output());
        ARIADNE_TEST_CALL(test_primal_full_step_dual_backtracking());
        ARIADNE_TEST_CALL(test_upper_bound_primal_backtracking());
        ARIADNE_TEST_CALL(test_primal_backtracking_cutoff());
        ARIADNE_TEST_CALL(test_preconditions());
        ARIADNE_TEST_CALL(test_logging_branches());
        ARIADNE_TEST_CALL(test_quiet_uncertified_feasibility());
        ARIADNE_TEST_CALL(test_optimization());
        ARIADNE_TEST_CALL(test_mixed_bound_optimization());
        ARIADNE_TEST_CALL(test_hotstarted_optimization());
        ARIADNE_TEST_CALL(test_hotstarted_positive_gap());
        ARIADNE_TEST_CALL(test_hotstarted_boundary_optimum());
        ARIADNE_TEST_CALL(test_hotstarted_nonfinite_direction());
        ARIADNE_TEST_CALL(test_feasibility());
        ARIADNE_TEST_CALL(test_mixed_bound_feasibility());
        ARIADNE_TEST_CALL(test_feasibility_iteration_limit());
        ARIADNE_TEST_CALL(test_constrained_feasibility());
    }

    Void test_feasibility_after_weighted_interior_step() {
        // With very different coordinate widths, the unweighted
        // orthogonal correction of the midpoint violates the first bound.
        // The weighted interior-point Newton step, however, can satisfy
        // the equality while retaining strictly interior multipliers.
        Matrix<FloatDP> A({{1.0_x,1.0_x}},dp);
        Vector<FloatDP> b({7.75_x},dp);
        Vector<FloatDP> xl({0.0_x,0.0_x},dp);
        Vector<FloatDP> xu({0.25_x,16.0_x},dp);
        Vector<FloatDPApproximation> x({0.125_x,8.0_x},dp);
        Vector<FloatDPApproximation> y({0.0_x},dp);
        Vector<FloatDPApproximation> zl({1.0_x,1.0_x},dp);
        Vector<FloatDPApproximation> zu({1.0_x,1.0_x},dp);

        // This tests the step's status independently of the public wrapper.
        LinearProgramStatus status=optimiser->_feasibility_step(
            xl,xu,A,b,x,y,zl,zu);
        ARIADNE_TEST_EQUAL(status,LinearProgramStatus::PRIMAL_DUAL_FEASIBLE);
        ARIADNE_TEST_ASSERT(x[0]>FloatDPApproximation(0.0_x,dp));
        ARIADNE_TEST_ASSERT(x[0]<FloatDPApproximation(0.25_x,dp));
        ARIADNE_TEST_ASSERT(x[1]>FloatDPApproximation(0.0_x,dp));
        ARIADNE_TEST_ASSERT(x[1]<FloatDPApproximation(16.0_x,dp));
        ARIADNE_TEST_ASSERT(definitely(
            optimiser->validate_feasibility(xl,xu,A,b,x,y)));

        // The public method must not require the unweighted initial
        // feasibility projection to be inside the domain.
        ARIADNE_TEST_ASSERT(definitely(optimiser->feasible(xl,xu,A,b)));
    }

    Void test_uncertified_full_primal_step() {
        // The rows are nearly parallel. Weighting by the unequal box
        // widths permits a full primal step, but the unweighted normal
        // equations used by interval validation cannot certify it.
        // The exact solution (256,1/4) is strictly inside the box.
        Matrix<FloatDP> A({{0.000000007450580596923828125_x,1.0_x},
                           {0.0_x,1.0_x}},dp);
        Vector<FloatDP> b({0.2500019073486328125_x,0.25_x},dp);
        Vector<FloatDP> lower({-1024.0_x,-1.0_x},dp);
        Vector<FloatDP> upper({1024.0_x,1.0_x},dp);
        Vector<FloatDPApproximation> x({0.0_x,0.0_x},dp);
        Vector<FloatDPApproximation> y({0.0_x,0.0_x},dp);
        Vector<FloatDPApproximation> zl({1.0_x,1.0_x},dp);
        Vector<FloatDPApproximation> zu({1.0_x,1.0_x},dp);

        ARIADNE_TEST_ASSERT(is_indeterminate(
            optimiser->validate_feasibility(lower,upper,A,b,x,y)));
        ARIADNE_TEST_EQUAL(optimiser->_feasibility_step(
            lower,upper,A,b,x,y,zl,zu),LinearProgramStatus::PRIMAL_FEASIBLE);
        ARIADNE_TEST_ASSERT(abs(x[0]-FloatDPApproximation(256.0_x,dp))
                            <FloatDPApproximation(0.015625_x,dp));
        ARIADNE_TEST_ASSERT(abs(x[1]-FloatDPApproximation(0.25_x,dp))
                            <FloatDPApproximation(0.0001220703125_x,dp));
        ARIADNE_TEST_ASSERT(is_indeterminate(
            optimiser->validate_feasibility(lower,upper,A,b,x,y)));
        ARIADNE_TEST_ASSERT(is_indeterminate(optimiser->feasible(lower,upper,A,b)));
    }

    Void test_status_output() {
        std::ostringstream stream;
        stream << LinearProgramStatus::INDETERMINATE_FEASIBILITY << " "
               << LinearProgramStatus::PRIMAL_FEASIBLE << " "
               << LinearProgramStatus::DUAL_FEASIBLE << " "
               << LinearProgramStatus::PRIMAL_DUAL_FEASIBLE << " "
               << LinearProgramStatus::DEGENERATE_FEASIBILITY << " "
               << static_cast<LinearProgramStatus>(255);
        ARIADNE_TEST_ASSERT(stream.str().find("INDETERMINATE_FEASIBILITY")!=std::string::npos);
        ARIADNE_TEST_ASSERT(stream.str().find("PRIMAL_FEASIBLE")!=std::string::npos);
        ARIADNE_TEST_ASSERT(stream.str().find("DUAL_FEASIBLE")!=std::string::npos);
        ARIADNE_TEST_ASSERT(stream.str().find("PRIMAL_DUAL_FEASIBLE")!=std::string::npos);
        ARIADNE_TEST_ASSERT(stream.str().find("DEGENERATE_FEASIBILITY")!=std::string::npos);
        ARIADNE_TEST_ASSERT(stream.str().find("UNKNOWN_FEASIBILITY_STATUS")!=std::string::npos);
    }

    Void test_primal_full_step_dual_backtracking() {
        // Ax=b moves x from 1 to 1/2 in one full Newton step.
        // The full dual step would make the upper multiplier negative,
        // so dual backtracking is required while the primal step remains full.
        Matrix<FloatDP> A({{1.0_x}},dp);
        Vector<FloatDP> b({0.5_x},dp);
        Vector<FloatDP> c({0.0_x},dp);
        Vector<FloatDP> xl({0.0_x},dp);
        Vector<FloatDP> xu({2.0_x},dp);
        Vector<FloatDPApproximation> x({1.0_x},dp);
        Vector<FloatDPApproximation> y({0.0_x},dp);
        Vector<FloatDPApproximation> zl({1.0_x},dp);
        Vector<FloatDPApproximation> zu({1.0_x},dp);

        LinearProgramStatus status=optimiser->_minimisation_step(
            c,xl,xu,A,b,x,y,zl,zu);
        ARIADNE_TEST_EQUAL(status,LinearProgramStatus::PRIMAL_FEASIBLE);
        ARIADNE_TEST_EQUAL(x[0],FloatDPApproximation(0.5_x,dp));
        ARIADNE_TEST_EQUAL(zl[0],FloatDPApproximation(0.7890625_x,dp));
        ARIADNE_TEST_EQUAL(zu[0],FloatDPApproximation(0.2265625_x,dp));
    }

    Void test_upper_bound_primal_backtracking() {
        // A full Newton step would set x=2, exactly on the upper bound.
        // The primal line search must reject that step by its upper-bound
        // condition, even though the lower-bound condition is satisfied.
        Matrix<FloatDP> A({{1.0_x}},dp);
        Vector<FloatDP> b({2.0_x},dp);
        Vector<FloatDP> c({0.0_x},dp);
        Vector<FloatDP> xl({0.0_x},dp);
        Vector<FloatDP> xu({2.0_x},dp);
        Vector<FloatDPApproximation> x({1.0_x},dp);
        Vector<FloatDPApproximation> y({0.0_x},dp);
        Vector<FloatDPApproximation> zl({1.0_x},dp);
        Vector<FloatDPApproximation> zu({1.0_x},dp);

        LinearProgramStatus status=optimiser->_minimisation_step(
            c,xl,xu,A,b,x,y,zl,zu);
        ARIADNE_TEST_EQUAL(status,LinearProgramStatus::INDETERMINATE_FEASIBILITY);
        ARIADNE_TEST_EQUAL(x[0],FloatDPApproximation(1.75_x,dp));
        ARIADNE_TEST_ASSERT(x[0]<FloatDPApproximation(2.0_x,dp));
        ARIADNE_TEST_ASSERT(zl[0]>FloatDPApproximation(0.0_x,dp));
        ARIADNE_TEST_ASSERT(zu[0]>FloatDPApproximation(0.0_x,dp));
    }

    Void test_primal_backtracking_cutoff() {
        // The equality requires x=2^40 outside [0,2]. Even the smallest
        // permitted step from the midpoint cannot remain interior.
        // A rejected direction must leave the caller's iterate unchanged.
        Matrix<FloatDP> A({{1.0_x}},dp);
        Vector<FloatDP> b({1099511627776.0_x},dp);
        Vector<FloatDP> c({0.0_x},dp);
        Vector<FloatDP> xl({0.0_x},dp);
        Vector<FloatDP> xu({2.0_x},dp);
        Vector<FloatDPApproximation> x({1.0_x},dp);
        Vector<FloatDPApproximation> y({0.0_x},dp);
        Vector<FloatDPApproximation> zl({1.0_x},dp);
        Vector<FloatDPApproximation> zu({1.0_x},dp);

        ARIADNE_TEST_EQUAL(optimiser->_minimisation_step(
            c,xl,xu,A,b,x,y,zl,zu),LinearProgramStatus::DEGENERATE_FEASIBILITY);
        ARIADNE_TEST_EQUAL(x[0],FloatDPApproximation(1.0_x,dp));
        ARIADNE_TEST_EQUAL(y[0],FloatDPApproximation(0.0_x,dp));
        ARIADNE_TEST_EQUAL(zl[0],FloatDPApproximation(1.0_x,dp));
        ARIADNE_TEST_EQUAL(zu[0],FloatDPApproximation(1.0_x,dp));
    }

    Void test_preconditions() {
        Matrix<FloatDP> A({{1.0_x}},dp);
        Vector<FloatDP> b({1.0_x},dp);
        Vector<FloatDP> c({0.0_x},dp);
        Vector<FloatDP> xl({0.0_x},dp);
        Vector<FloatDP> xu({2.0_x},dp);
        Vector<FloatDPApproximation> x({1.0_x},dp);
        Vector<FloatDPApproximation> y({0.0_x},dp);
        Vector<FloatDPApproximation> zl({1.0_x},dp);
        Vector<FloatDPApproximation> zu({1.0_x},dp);

        Vector<FloatDP> two_exact({0.0_x,0.0_x},dp);
        Vector<FloatDPApproximation> two_approx({0.0_x,0.0_x},dp);

        ARIADNE_TEST_FAIL(optimiser->hotstarted_minimise(two_exact,xl,xu,A,b,x,y,zl,zu));
        ARIADNE_TEST_FAIL(optimiser->hotstarted_minimise(c,two_exact,xu,A,b,x,y,zl,zu));
        ARIADNE_TEST_FAIL(optimiser->hotstarted_minimise(c,xl,two_exact,A,b,x,y,zl,zu));
        ARIADNE_TEST_FAIL(optimiser->hotstarted_minimise(c,xl,xu,A,two_exact,x,y,zl,zu));
        ARIADNE_TEST_FAIL(optimiser->hotstarted_minimise(c,xl,xu,A,b,two_approx,y,zl,zu));
        ARIADNE_TEST_FAIL(optimiser->hotstarted_minimise(c,xl,xu,A,b,x,two_approx,zl,zu));
        ARIADNE_TEST_FAIL(optimiser->hotstarted_minimise(c,xl,xu,A,b,x,y,two_approx,zu));
        ARIADNE_TEST_FAIL(optimiser->hotstarted_minimise(c,xl,xu,A,b,x,y,zl,two_approx));

        Vector<FloatDPApproximation> positive_dual({1.0_x},dp);
        ARIADNE_TEST_FAIL(
            optimiser->hotstarted_minimise(c,xl,xu,A,b,x,positive_dual,zl,zu));

        Vector<FloatDP> fixed_bound({1.0_x},dp);
        ARIADNE_TEST_FAIL(optimiser->minimise(c,fixed_bound,fixed_bound,A,b));

        Vector<FloatDP> invalid_xl({1.0_x},dp);
        Vector<FloatDP> invalid_xu({0.0_x},dp);
        ARIADNE_TEST_FAIL(optimiser->feasible(invalid_xl,invalid_xu,A,b));
    }

    Void test_logging_branches() {
        auto& configuration=Logger::instance().configuration();
        unsigned int const old_verbosity=configuration.verbosity();
        configuration.set_verbosity(10u);

        Matrix<FloatDP> A({
            {1.0_x,0.0_x,1.0_x},
            {0.0_x,1.0_x,2.0_x}},dp);
        Vector<FloatDP> b({1.0_x,1.0_x},dp);
        Vector<FloatDP> c({1.0_x,0.5_x,-0.75_x},dp);
        Vector<FloatDP> xl({0.0_x,0.0_x,0.0_x},dp);
        Vector<FloatDP> xu({+inf,+inf,3.0_x},dp);
        (void)optimiser->minimise(c,xl,xu,A,b);

        Matrix<FloatDP> hot_A({{1.0_x,1.0_x}},dp);
        Vector<FloatDP> hot_b({1.0_x},dp);
        Vector<FloatDP> hot_c({0.0_x,0.0_x},dp);
        Vector<FloatDP> hot_xl({0.0_x,0.0_x},dp);
        Vector<FloatDP> hot_xu({2.0_x,2.0_x},dp);
        Vector<FloatDPApproximation> hot_x({0.5_x,0.5_x},dp);
        Vector<FloatDPApproximation> hot_y({0.0_x},dp);
        Vector<FloatDPApproximation> hot_zl({1.0_x,1.0_x},dp);
        Vector<FloatDPApproximation> hot_zu({1.0_x,1.0_x},dp);
        (void)optimiser->hotstarted_minimise(
            hot_c,hot_xl,hot_xu,hot_A,hot_b,hot_x,hot_y,hot_zl,hot_zu);

        Matrix<FloatDP> singular_A({{0.0_x}},dp);
        Vector<FloatDP> singular_b({1.0_x},dp);
        Vector<FloatDP> singular_xl({-1.0_x},dp);
        Vector<FloatDP> singular_xu({+1.0_x},dp);
        ARIADNE_TEST_ASSERT(is_indeterminate(
            optimiser->feasible(singular_xl,singular_xu,singular_A,singular_b)));

        // An equality feasible only on a bound can reach the
        // complementarity threshold without proving strict feasibility.
        Matrix<FloatDP> boundary_A({{1.0_x}},dp);
        Vector<FloatDP> boundary_b({0.0_x},dp);
        Vector<FloatDP> boundary_xl({0.0_x},dp);
        Vector<FloatDP> boundary_xu({1.0_x},dp);
        (void)optimiser->feasible(boundary_xl,boundary_xu,boundary_A,boundary_b);

        configuration.set_verbosity(old_verbosity);
    }

    Void test_quiet_uncertified_feasibility() {
        auto& configuration=Logger::instance().configuration();
        const unsigned int old_verbosity=configuration.verbosity();
        struct RestoreVerbosity {
            unsigned int value;
            ~RestoreVerbosity() {
                Logger::instance().configuration().set_verbosity(value);
            }
        } restore{old_verbosity};
        configuration.set_verbosity(0u);

        // A singular equality cannot certify feasibility; neither can an
        // equality whose only solution is on the boundary of the box.
        Matrix<FloatDP> singular_A({{0.0_x}},dp);
        Vector<FloatDP> singular_b({1.0_x},dp);
        Vector<FloatDP> lower({-1.0_x},dp);
        Vector<FloatDP> upper({1.0_x},dp);
        ARIADNE_TEST_ASSERT(is_indeterminate(
            optimiser->feasible(lower,upper,singular_A,singular_b)));

        Matrix<FloatDP> A({{1.0_x}},dp);
        Vector<FloatDP> b({0.0_x},dp);
        lower[0]=0.0_x;
        ARIADNE_TEST_ASSERT(is_indeterminate(optimiser->feasible(lower,upper,A,b)));
    }

    Void test_hotstarted_optimization() {
        Matrix<FloatDP> A({{1.0_x,1.0_x}},dp);
        Vector<FloatDP> b({1.0_x},dp);
        Vector<FloatDP> c({0.0_x,0.0_x},dp);
        Vector<FloatDP> xl({0.0_x,0.0_x},dp);
        Vector<FloatDP> xu({2.0_x,2.0_x},dp);
        Vector<FloatDPApproximation> x({0.5_x,0.5_x},dp);
        Vector<FloatDPApproximation> y({0.0_x},dp);
        Vector<FloatDPApproximation> zl({1.0_x,1.0_x},dp);
        Vector<FloatDPApproximation> zu({1.0_x,1.0_x},dp);

        auto hotstart_result=optimiser->hotstarted_minimise(
            c,xl,xu,A,b,x,y,zl,zu);
        ARIADNE_TEST_EQUAL(std::get<0>(hotstart_result),FloatDPApproximation(0.0_x,dp));
        ARIADNE_TEST_EQUAL(std::get<1>(hotstart_result).size(),2u);
        ARIADNE_TEST_EQUAL(std::get<2>(hotstart_result).size(),1u);
    }

    Void test_validate_feasibility() {

        // A feasible instance
        Vector<FloatDP> xl({0.0_x,0.0_x,0.0_x},dp);
        Vector<FloatDP> xu({inf,inf,inf},dp);
        Matrix<FloatDP> A({{1.0_x,0.0_x,1.0_x}, {0.0_x,1.0_x,2.0_x}},dp);
        Vector<FloatDP> b({1.0_x,1.0_x},dp);
        Vector<FloatDP> x({0.81_pr,0.51_pr,0.21_pr},dp);
        Vector<FloatDP> y({-1.0_x,-0.5_x},dp);
        ARIADNE_ASSERT(definitely(optimiser->validate_feasibility(xl,xu,A,b,x,y)));

        b=Vector<FloatDP>({1.0_x, -1.0_x},dp);
        y=Vector<FloatDP>({-0.5_x, -1.0_x},dp);
        ARIADNE_ASSERT(definitely(!optimiser->validate_feasibility(xl,xu,A,b,x,y)));
    }


    Void test_validate_feasibility_singular() {
        Vector<FloatDP> xl({-1.0_x,-1.0_x},dp);
        Vector<FloatDP> xu({+1.0_x,+1.0_x},dp);
        Matrix<FloatDP> A({{1.0_x,0.0_x},{2.0_x,0.0_x}},dp);
        Vector<FloatDP> b({0.0_x,0.0_x},dp);
        Vector<FloatDP> x({0.0_x,0.0_x},dp);
        Vector<FloatDP> y({0.0_x,0.0_x},dp);

        ARIADNE_TEST_ASSERT(is_indeterminate(
            optimiser->validate_feasibility(xl,xu,A,b,x,y)));
    }

    Void test_mixed_bound_optimization() {
        Matrix<FloatDP> A({
            {1.0_x,0.0_x,0.0_x,0.0_x},
            {0.0_x,1.0_x,0.0_x,0.0_x},
            {0.0_x,0.0_x,1.0_x,0.0_x},
            {0.0_x,0.0_x,0.0_x,1.0_x}},dp);
        Vector<FloatDP> b({0.0_x,1.0_x,1.0_x,1.0_x},dp);
        Vector<FloatDP> c({0.0_x,0.0_x,0.0_x,0.0_x},dp);
        Vector<FloatDP> xl({-inf,-inf,0.0_x,0.0_x},dp);
        Vector<FloatDP> xu({+inf,2.0_x,+inf,2.0_x},dp);

        auto optimum=optimiser->minimise(c,xl,xu,A,b);
        ARIADNE_TEST_EQUAL(std::get<1>(optimum).size(),4u);
        ARIADNE_TEST_EQUAL(std::get<2>(optimum).size(),4u);
    }

    Void test_hotstarted_positive_gap() {
        Matrix<FloatDP> A({{1.0_x}},dp);
        Vector<FloatDP> b({1.0_x},dp);
        Vector<FloatDP> c({1.0_x},dp);
        Vector<FloatDP> xl({0.0_x},dp);
        Vector<FloatDP> xu({2.0_x},dp);
        Vector<FloatDPApproximation> x({1.0_x},dp);
        Vector<FloatDPApproximation> y({0.0_x},dp);
        Vector<FloatDPApproximation> zl({1.0_x},dp);
        Vector<FloatDPApproximation> zu({1.0_x},dp);

        auto hotstart_result=optimiser->hotstarted_minimise(
            c,xl,xu,A,b,x,y,zl,zu);
        ARIADNE_TEST_EQUAL(std::get<1>(hotstart_result).size(),1u);
        ARIADNE_TEST_EQUAL(std::get<2>(hotstart_result).size(),1u);
    }

    Void test_hotstarted_boundary_optimum() {
        // The primal optimum is on the lower bound. A large but finite
        // objective coefficient stresses the capped primal-dual iterations.
        Matrix<FloatDP> A({{1.0_x}},dp);
        Vector<FloatDP> b({0.0_x},dp);
        Vector<FloatDP> c({8192.0_x},dp);
        Vector<FloatDP> xl({0.0_x},dp);
        Vector<FloatDP> xu({2.0_x},dp);
        Vector<FloatDPApproximation> x({1.0_x},dp);
        Vector<FloatDPApproximation> y({0.0_x},dp);
        Vector<FloatDPApproximation> zl({1.0_x},dp);
        Vector<FloatDPApproximation> zu({1.0_x},dp);

        auto boundary_run=optimiser->hotstarted_minimise(c,xl,xu,A,b,x,y,zl,zu);
        ARIADNE_TEST_EQUAL(std::get<1>(boundary_run).size(),1u);
        ARIADNE_TEST_EQUAL(std::get<2>(boundary_run).size(),1u);
    }

    Void test_hotstarted_nonfinite_direction() {
        // All public inputs are finite, but forming the normal equations
        // overflows double precision. Rejecting a NaN Newton direction
        // must leave the caller's primal and dual estimates unchanged.
        Matrix<FloatDP> A({{1e200_pr}},dp);
        Vector<FloatDP> b({0.0_x},dp);
        Vector<FloatDP> c({1.0_x},dp);
        Vector<FloatDP> xl({0.0_x},dp);
        Vector<FloatDP> xu({2e200_pr},dp);
        Vector<FloatDPApproximation> x({1e200_pr},dp);
        Vector<FloatDPApproximation> y({0.0_x},dp);
        Vector<FloatDPApproximation> zl({1.0_x},dp);
        Vector<FloatDPApproximation> zu({1.0_x},dp);

        auto result=optimiser->hotstarted_minimise(c,xl,xu,A,b,x,y,zl,zu);
        ARIADNE_TEST_EQUAL(std::get<1>(result)[0],FloatDPApproximation(1e200_pr,dp));
        ARIADNE_TEST_EQUAL(std::get<2>(result)[0],FloatDPApproximation(0.0_x,dp));
    }

    Void test_mixed_bound_feasibility() {
        Matrix<FloatDP> A({
            {1.0_x,0.0_x,0.0_x,0.0_x},
            {0.0_x,1.0_x,0.0_x,0.0_x},
            {0.0_x,0.0_x,1.0_x,0.0_x},
            {0.0_x,0.0_x,0.0_x,1.0_x}},dp);
        Vector<FloatDP> b({0.0_x,1.0_x,1.0_x,1.0_x},dp);
        Vector<FloatDP> xl({-inf,-inf,0.0_x,0.0_x},dp);
        Vector<FloatDP> xu({+inf,2.0_x,+inf,2.0_x},dp);

        ARIADNE_TEST_ASSERT(definitely(optimiser->feasible(xl,xu,A,b)));
    }


    Void test_feasibility() {
        // A feasible instance
        Vector<FloatDP> xl({0.0_x,0.0_x,0.0_x},dp);
        Vector<FloatDP> xu({inf,inf,inf},dp);
        Matrix<FloatDP> A({{1.0_x,0.0_x,1.0_x},{0.0_x,1.0_x,2.0_x}},dp);
        Vector<FloatDP> b({1.0_x,1.0_x},dp);
        ARIADNE_ASSERT(definitely(optimiser->feasible(xl,xu,A,b)));

        b=Vector<FloatDP>({1.0_x,-1.0_x},dp);
        ARIADNE_ASSERT(definitely(!optimiser->feasible(xl,xu,A,b)));
    }


    Void test_feasibility_iteration_limit() {
        // The equality is feasible only at the lower bound. A wide initial
        // interval exercises the fixed iteration budget without claiming
        // strict interior feasibility.
        Matrix<FloatDP> A({{1.0_x}},dp);
        Vector<FloatDP> b({0.0_x},dp);
        Vector<FloatDP> xl({0.0_x},dp);
        Vector<FloatDP> xu({281474976710656.0_x},dp);

        ARIADNE_TEST_ASSERT(is_indeterminate(optimiser->feasible(xl,xu,A,b)));
    }

    Void test_constrained_feasibility() {
        Matrix<FloatDP> A({{1.0_x,0.0_x,1.0_x},{0.0_x,1.0_x,2.0_x}},dp);
        Vector<FloatDP> b({1.0_x,1.0_x},dp);
        Vector<FloatDP> xl({0.0_x,0.0_x,0.0_x},dp);
        Vector<FloatDP> xu({4.0_x,2.0_x,3.0_x},dp);

        ARIADNE_ASSERT(definitely(optimiser->feasible(xl,xu,A,b)));

        b=Vector<FloatDP>({1.0_x,-1.0_x},dp);
        ARIADNE_ASSERT(definitely(!optimiser->feasible(xl,xu,A,b)));

        xu=Vector<FloatDP>({+inf,+inf,3.0_x},dp);
    }


    Void test_optimization() {
        Matrix<FloatDP> A({{1.0_x,0.0_x,1.0_x},{0.0_x,1.0_x,2.0_x}},dp);
        Vector<FloatDP> b({1.0_x,1.0_x},dp);
        Vector<FloatDP> c({1.0_x,0.5_x,-0.75_x},dp);
        Vector<FloatDP> xl({0.0_x,0.0_x,0.0_x},dp);
        Vector<FloatDP> xu({+inf,+inf,3.0_x},dp);

        ARIADNE_TEST_PRINT(optimiser->minimise(c,xl,xu,A,b));
    }

};


Int main(Int argc, const char* argv[])
{
    LoggingCommandLineInterface command_line;
    if (not command_line.acquire(argc,argv)) return -1;

    LinearInteriorPointSolver interior_point_optimiser;
    TestLinearInteriorPointSolver(interior_point_optimiser).test();

    std::cerr<<"INCOMPLETE ";
    return ARIADNE_TEST_FAILURES;
}

