import benchmark_functions as bf
from opfunu.cec_based.cec2005 import F72005

def make_hypersphere(dim: int) -> bf.BenchmarkFunction:
    return bf.Hypersphere(n_dimensions=dim)

def make_schwefel(dim: int) -> bf.BenchmarkFunction:
    return bf.Schwefel(n_dimensions=dim)

def make_styblinski_tang(dim: int) -> bf.BenchmarkFunction:
    return bf.StyblinskiTang(n_dimensions=dim)

def make_griewank_cec2005(dim: int):
    if dim not in [10, 30, 50]:
        raise ValueError(f"F72005 supports only ndim in [10, 30, 50], got {dim}")
    return F72005(ndim=dim)