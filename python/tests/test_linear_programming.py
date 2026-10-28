from pyariadne import FloatDPLinearSimplexSolver, LinearInteriorPointSolver, Slackness


def test_solver_exports():
    assert FloatDPLinearSimplexSolver() is not None
    assert LinearInteriorPointSolver() is not None
    assert Slackness.BASIS is not None
    assert Slackness.FIXED is not None
