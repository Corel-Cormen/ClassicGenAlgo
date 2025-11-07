import benchmark_functions as bf
from opfunu.cec_based.cec2014 import F72014
import numpy as np

def make_hypersphere(dim: int) -> bf.BenchmarkFunction:
    return bf.Hypersphere(n_dimensions=dim)

def make_hyperellipsoid(dim: int) -> bf.BenchmarkFunction:
    return bf.Hyperellipsoid(n_dimensions=dim)

def make_rosenbrock(dim: int) -> bf.BenchmarkFunction:
    return bf.Rosenbrock(n_dimensions=dim)

def make_rastrigin(dim: int) -> bf.BenchmarkFunction:
    return bf.Rastrigin(n_dimensions=dim)

def make_schwefel(dim: int) -> bf.BenchmarkFunction:
    return bf.Schwefel(n_dimensions=dim)

def make_griewank(dim: int) -> bf.BenchmarkFunction:
    return bf.Griewank(n_dimensions=dim)

def make_ackley(dim: int) -> bf.BenchmarkFunction:
    return bf.Ackley(n_dimensions=dim)

def make_michalewicz(dim: int) -> bf.BenchmarkFunction:
    return bf.Michalewicz(n_dimensions=dim)

def make_egg_holder(dim: int) -> bf.BenchmarkFunction:
    return bf.EggHolder(n_dimensions=dim)

def make_keane(dim: int) -> bf.BenchmarkFunction:
    return bf.Keane(n_dimensions=dim)

def make_rana(dim: int) -> bf.BenchmarkFunction:
    return bf.Rana(n_dimensions=dim)

def make_easom(dim: int) -> bf.BenchmarkFunction:
    return bf.Easom(n_dimensions=dim)

def make_de_jong_3(dim: int) -> bf.BenchmarkFunction:
    return bf.DeJong3(n_dimensions=dim)

def make_de_jong_5(dim: int) -> bf.BenchmarkFunction:
    return bf.DeJong5(n_dimensions=dim)

def make_goldstein_and_price(dim: int) -> bf.BenchmarkFunction:
    return bf.GoldsteinAndPrice(n_dimensions=dim)

def make_picheny_goldstein_and_price(dim: int) -> bf.BenchmarkFunction:
    return bf.PichenyGoldsteinAndPrice(n_dimensions=dim)

def make_styblinski_tang(dim: int) -> bf.BenchmarkFunction:
    return bf.StyblinskiTang(n_dimensions=dim)

def make_mc_cormick(dim: int) -> bf.BenchmarkFunction:
    return bf.McCormick(n_dimensions=dim)

def make_schaffer_2(dim: int) -> bf.BenchmarkFunction:
    return bf.Schaffer2(n_dimensions=dim)

def make_Himmelblau(dim: int) -> bf.BenchmarkFunction:
    return bf.Himmelblau(n_dimensions=dim)

def make_pits_and_holes(dim: int) -> bf.BenchmarkFunction:
    return bf.PitsAndHoles(n_dimensions=dim)

def make_bc_1(dim: int) -> bf.BenchmarkFunction:
    return bf.BC_1(n_dimensions=dim)

def make_bc_2(dim: int) -> bf.BenchmarkFunction:
    return bf.BC_2(n_dimensions=dim)

def make_bc_35(dim: int) -> bf.BenchmarkFunction:
    return bf.BC_35(n_dimensions=dim)

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