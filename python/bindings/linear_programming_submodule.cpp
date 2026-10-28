#include "pybind11.hpp"
#include "linear-programming-utilities.hpp"
#include "numeric/numeric.hpp"
#include "algebra/vector.hpp"
#include "algebra/matrix.hpp"
#include "linear-programming/linear_programming.hpp"
#include "linear-programming/linear_simplex_algorithm.hpp"
#include "linear-programming/linear_interior_point_method.hpp"

using namespace Ariadne;

Void export_slackness(pybind11::module& module)
{
    pybind11::enum_<Slackness> variable_enum(module,"Slackness");
    variable_enum.value("BASIS", Slackness::BASIS);
    variable_enum.value("LOWER", Slackness::LOWER);
    variable_enum.value("UPPER", Slackness::UPPER);
    variable_enum.value("FIXED", Slackness::FIXED);
}

Void export_interior_point_solver(pybind11::module& module)
{
    pybind11::class_<LinearInteriorPointSolver> interior_point_solver_class(module,"LinearInteriorPointSolver");
    interior_point_solver_class.def(pybind11::init<>());
    interior_point_solver_class.def("minimise", &LinearInteriorPointSolver::minimise);
    interior_point_solver_class.def("feasible", (ValidatedKleenean(LinearInteriorPointSolver::*)(const Vector<FloatDP>&,const Vector<FloatDP>&, const Matrix<FloatDP>&,const Vector<FloatDP>&)const) &LinearInteriorPointSolver::feasible);
    interior_point_solver_class.def("validate_feasibility", &LinearInteriorPointSolver::validate_feasibility);
}


template<class X>
Void export_simplex_solver(pybind11::module& module)
{
    typedef Array<SizeType> SizeArray;

    typedef RigorousNumericType<X> XX;

    pybind11::class_< LinearSimplexSolver<X> > simplex_solver_class(module,(class_name<X>()+"LinearSimplexSolver").c_str());
    simplex_solver_class.def(pybind11::init<>());
    simplex_solver_class.def("lpstep",(Bool(LinearSimplexSolver<X>::*)(const Vector<X>&,const Vector<X>&,const Vector<X>&,const Matrix<X>&,const Vector<X>&,Array<Slackness>& ,SizeArray&,Matrix<XX>&,Vector<XX>&)const) &LinearSimplexSolver<X>::lpstep);


    simplex_solver_class.def("minimise",(Vector<XX>(LinearSimplexSolver<X>::*)(const Vector<X>&,const Vector<X>&,const Vector<X>&,const Matrix<X>&,const Vector<X>&)const) &LinearSimplexSolver<X>::minimise);
    simplex_solver_class.def("feasible",(ValidatedKleenean(LinearSimplexSolver<X>::*)(const Vector<X>&,const Vector<X>&,const Matrix<X>&,const Vector<X>&)const) &LinearSimplexSolver<X>::feasible);

    simplex_solver_class.def("verify_feasibility",(ValidatedKleenean(LinearSimplexSolver<X>::*)(const Vector<X>&,const Vector<X>&,const Matrix<X>&,const Vector<X>&,const Array<Slackness>&)const) &LinearSimplexSolver<X>::verify_feasibility);

    simplex_solver_class.def("compute_basis",(Pair< SizeArray, Matrix<XX> >(LinearSimplexSolver<X>::*)(const Matrix<X>&)const) &LinearSimplexSolver<X>::compute_basis);

}



void linear_programming_submodule(pybind11::module& module) {
    export_slackness(module);
    export_simplex_solver<FloatDPApproximation>(module);
    export_simplex_solver<FloatDP>(module);
    export_simplex_solver<Rational>(module);
    export_interior_point_solver(module);
}
