/***************************************************************************
 *            linear-programming/linear_program_state.hpp
 *
 *  Copyright  2026  Luca Geretti
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

#ifndef ARIADNE_LINEAR_PROGRAMMING_LINEAR_PROGRAM_STATE_HPP
#define ARIADNE_LINEAR_PROGRAMMING_LINEAR_PROGRAM_STATE_HPP

#include "linear-programming/linear_simplex_algorithm.hpp"
#include "utility/array.hpp"
#include "algebra/vector.hpp"
#include "algebra/matrix.hpp"

namespace Ariadne {

template<class X, class XX>
struct LinearProgram {
    LinearProgram() : LinearProgram(0u,0u,X(dp)) { }
    LinearProgram(SizeType m, SizeType n, X const& z);
    Matrix<X> A;
    Vector<X> b;
    Vector<X> c;
    Vector<X> l;
    Vector<X> u;
    Array<Slackness> vt;
    Array<SizeType> p;
    Matrix<XX> B;
    Vector<XX> x;
    Vector<XX> y;
    Vector<XX> z;
};

template<class X, class XX> LinearProgram<X,XX>::LinearProgram(SizeType m, SizeType n, X const& zero)
    : A(m,n,zero), b(m,zero), c(n,zero), l(n,zero), u(n,zero)
    , vt(n), p(n), B(m,m,zero)
    , x(n,zero), y(m,zero),z(n,zero) { }

} // namespace Ariadne

#endif
