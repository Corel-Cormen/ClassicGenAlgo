import benchmark_functions as bf
from opfunu.cec_based.cec2014 import F72014
import numpy as np

def make_hypersphere(dim: int) -> bf.BenchmarkFunction:
    return bf.Hypersphere(n_dimensions=dim)

def make_schwefel(dim: int) -> bf.BenchmarkFunction:
    return bf.Schwefel(n_dimensions=dim)

def make_styblinski_tang(dim: int) -> bf.BenchmarkFunction:
    return bf.StyblinskiTang(n_dimensions=dim)

def make_griewank_cec2014(dim: int, highlight_point=None):
    if dim not in [10, 30, 50]:
        raise ValueError(f"F72014 supports only ndim in [10, 30, 50], got {dim}")
    func = F72014(ndim=dim)

    if highlight_point is not None:
        if isinstance(highlight_point, (int, float)):
            hp = np.full(dim, float(highlight_point), dtype=float)
        elif isinstance(highlight_point, dict):
            hp = np.zeros(dim, dtype=float)
            for k, v in highlight_point.items():
                idx = int(k)
                if idx < 0 or idx >= dim:
                    raise ValueError(f"highlight_point index {idx} out of range for dim {dim}")
                hp[idx] = float(v)
        else:
            try:
                lp = list(highlight_point)
            except TypeError:
                raise TypeError("highlight_point must be scalar, iterable or dict")
            if len(lp) == dim:
                hp = np.array(lp, dtype=float)
            elif len(lp) < dim:
                hp = np.zeros(dim, dtype=float)
                hp[:len(lp)] = np.array(lp, dtype=float)
            else:
                raise ValueError(f"highlight_point length {len(lp)} > dim {dim}")

        if hasattr(func, "lb") and hasattr(func, "ub"):
            try:
                hp = np.clip(hp, func.lb, func.ub)
            except Exception:
                hp = np.clip(hp, np.array(func.lb, dtype=float), np.array(func.ub, dtype=float))

        func.highlight_point = hp

    return func