/***************************************************************************
 *            linear-programming/linear_programming.hpp
 *
 *  Copyright  2005-20  Alberto Casagrande, Pieter Collins
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

/** @file linear-programming/linear_programming.hpp
 *  @brief Common numerical traits and error types for linear programming.
 */

#ifndef ARIADNE_LINEAR_PROGRAMMING_HPP
#define ARIADNE_LINEAR_PROGRAMMING_HPP

#include <stdexcept>

#include "numeric/numeric.hpp"

namespace Ariadne {

template<class X>
struct RigorousNumericsTraits {
    typedef X Type;
};
template<>
struct RigorousNumericsTraits<FloatDP> {
    typedef Bounds<FloatDP> Type;
};
template<>
struct RigorousNumericsTraits<FloatMP> {
    typedef Bounds<FloatMP> Type;
};
template<class X>
using RigorousNumericType = typename RigorousNumericsTraits<X>::Type;

template<class X>
struct ApproximateNumericsTraits {
    typedef X Type;
};
template<>
struct ApproximateNumericsTraits<FloatDP> {
    typedef Approximation<FloatDP> Type;
};
template<>
struct ApproximateNumericsTraits<FloatMP> {
    typedef Approximation<FloatMP> Type;
};
template<class X>
using ApproximateNumericType = typename ApproximateNumericsTraits<X>::Type;

/** @brief Report degeneracy during a feasibility computation. */
class DegenerateFeasibilityProblemException : public std::runtime_error {
  public:
    /** @brief Construct a degeneracy exception without a message. */
    DegenerateFeasibilityProblemException() : std::runtime_error("") {}
    /** @brief Construct a degeneracy exception with explanatory text. */
    DegenerateFeasibilityProblemException(const StringType& what) : std::runtime_error(what) {}
};

/** @brief Report a singular linear programming basis. */
struct SingularLinearProgram : std::runtime_error {
    /** @brief Construct an error identifying a singular basis. */
    SingularLinearProgram(const StringType& what) : std::runtime_error(what) {};
};
/** @brief Report an unbounded linear programming problem. */
struct UnboundedLinearProgram : std::runtime_error {
    /** @brief Construct an error identifying an unbounded objective. */
    UnboundedLinearProgram(const StringType& what) : std::runtime_error(what) {}
};
/** @brief Report an infeasible linear programming problem. */
struct InfeasibleLinearProgram : std::runtime_error {
    /** @brief Construct an error identifying infeasibility. */
    InfeasibleLinearProgram(const StringType& what) : std::runtime_error(what) {}
};

} // namespace Ariadne

#endif
