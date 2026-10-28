#include "pybind11.hpp"

void linear_programming_submodule(pybind11::module& module);

void linear_programming_module(pybind11::module& module) {
    linear_programming_submodule(module);
}
