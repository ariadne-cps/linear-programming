# Ariadne Linear Programming

[![Format](https://github.com/ariadne-cps/linear-programming/actions/workflows/format.yml/badge.svg)](https://github.com/ariadne-cps/linear-programming/actions/workflows/format.yml)
[![macOS Status](https://github.com/ariadne-cps/linear-programming/actions/workflows/macos.yml/badge.svg)](https://github.com/ariadne-cps/linear-programming/actions/workflows/macos.yml)
[![Ubuntu Status](https://github.com/ariadne-cps/linear-programming/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/ariadne-cps/linear-programming/actions/workflows/ubuntu.yml)
[![Windows Status](https://github.com/ariadne-cps/linear-programming/actions/workflows/win.yml/badge.svg)](https://github.com/ariadne-cps/linear-programming/actions/workflows/win.yml)
[![Sanitizers Status](https://github.com/ariadne-cps/linear-programming/actions/workflows/sanitizers.yml/badge.svg)](https://github.com/ariadne-cps/linear-programming/actions/workflows/sanitizers.yml)
[![Coverage Status](https://github.com/ariadne-cps/linear-programming/actions/workflows/coverage.yml/badge.svg)](https://github.com/ariadne-cps/linear-programming/actions/workflows/coverage.yml)
[![codecov](https://codecov.io/gh/ariadne-cps/linear-programming/branch/main/graph/badge.svg)](https://codecov.io/gh/ariadne-cps/linear-programming)

A standalone C++20 linear-programming component of Ariadne. Provides validated simplex methods and a linear interior-point solver, with C++ and Python interfaces.
Although independently maintained and usable, this repository is also part of the [Ariadne repository structure](https://github.com/ariadne-cps/ariadne/blob/main/doc/structure.md).

## Repository layout

- `include/linear-programming/`: public linear-programming headers.
- `src/`: implementation files.
- `tests/`: simplex and linear interior-point regression suites.
- `python/bindings/`, `python/tests/`, `tutorial/`: Python integration and examples.
- `tutorial/`: standalone installed C++ consumer.

The umbrella header is `ariadne-linear-programming.hpp`.

## Dependencies

- [ariadne-cps/algebra](https://github.com/ariadne-cps/algebra), direct submodule dependency. It transitively provides Interval and Numeric.
- [ariadne-cps/build](https://github.com/ariadne-cps/build), common CMake build infrastructure.

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

