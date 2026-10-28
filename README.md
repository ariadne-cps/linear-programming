# Ariadne Linear Programming

A standalone C++20 linear-programming component of Ariadne. Provides validated simplex methods and a linear interior-point solver, with C++ and Python interfaces.

## Repository layout

- `include/solving/`: public linear-programming headers (preserved for source compatibility).
- `src/`: implementations directly under this directory, without a nested `solving/`.
- `tests/`: simplex and linear interior-point regression suites.
- `python/bindings/`, `python/tests/`, `python/tutorials/`: Python integration and examples.
- `tutorials/tutorial_linear_programming/`: standalone installed C++ consumer.

The umbrella header is `ariadne-linear-programming.hpp`.

## Dependencies

- [ariadne-cps/function](https://github.com/ariadne-cps/function), direct submodule dependency, matching the source solver build. It transitively provides Algebra and Numeric.
- [ariadne-cps/configuration](https://github.com/ariadne-cps/configuration), common CMake build infrastructure.

No dependency is introduced on nonlinear programming, constraint solving, equation solving, Set, Paving, or SMT.

## Build and test

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel --target tests
ctest --test-dir build --output-on-failure
```

To install the standalone C++ library and Python module (with compatible Python 3.11+ development headers):

```sh
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release --parallel --target ariadne-linear-programming bindings
cmake --install build-release --prefix /path/to/install
```

C++ tutorial: `tutorials/tutorial_linear_programming/tutorial_linear_programming.cpp`. Python tutorial: `python/tutorials/tutorial_linear_programming.py`.

## Origin

The linear-programming implementation and tests were extracted from `ariadne-cps/solving` (`main`); `linear_program_state.hpp` originated from `ariadne-cps/kernel` (`split`). Behavioral changes to the algorithms were intentionally avoided during extraction.

## License

GNU General Public License v3.0 (or later where noted in source files).
