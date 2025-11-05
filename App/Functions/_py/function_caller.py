import benchmark_functions as bf
import numpy as np
from typing import List, Union

def call_func(fobj: Union[bf.BenchmarkFunction, object], args: List[float]) -> float:
    if isinstance(fobj, bf.BenchmarkFunction):
        return fobj(args)
    elif hasattr(fobj, 'evaluate'):
        return fobj.evaluate(np.array(args))
    else:
        raise TypeError(f"Unsupported function type: {type(fobj)}")