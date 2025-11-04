import benchmark_functions as bf
import matplotlib.pyplot as plt
import numpy as np
from typing import List

def add(a: float, b: float) -> float:
    return a + b

def fun():
    x = np.linspace(0, 10, 100)
    y = np.sin(x)

    plt.plot(x, y)
    plt.savefig("plot.png")
    plt.show()

def f(args: List[float]) -> float:
    func = bf.Hypersphere(n_dimensions=2)
    func.show(showPoints=[args])
    return func(args)

def make_hypersphere(dim: int) -> bf.BenchmarkFunction:
    return bf.Hypersphere(n_dimensions=dim)

def make_schwefel(dim) -> bf.BenchmarkFunction:
    return bf.Schwefel(n_dimensions=dim)

def call_func(fobj: bf.BenchmarkFunction, args: List[float]) -> float:
    return fobj(args)

def show_char(fobj: bf.BenchmarkFunction, args: List[float] = None) -> None:
    if fobj.n_dimensions() < 3:
        fobj.show(showPoints=args, savePath="plot.png", show=True)
