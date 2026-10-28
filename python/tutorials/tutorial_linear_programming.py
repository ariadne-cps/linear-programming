#!/usr/bin/env python3
from pyariadne import *

def tutorial_linear_programming():
    dp = DoublePrecision()
    simplex = FloatDPLinearSimplexSolver()
    objective = FloatDPVector([1, 2], dp)
    lower = FloatDPVector([0, 0], dp)
    upper = FloatDPVector([1, 1], dp)
    A = FloatDPMatrix([[1, 1]], dp)
    b = FloatDPVector([1], dp)
    print("LP feasible:", simplex.feasible(lower, upper, A, b))
    print("LP solution:", simplex.minimise(objective, lower, upper, A, b))\n    interior = LinearInteriorPointSolver()\n    print("Interior-point solution:", interior.minimise(objective, lower, upper, A, b))\n    print("Interior-point feasibility:", interior.feasible(lower, upper, A, b))

if __name__ == "__main__":
    tutorial_linear_programming()
