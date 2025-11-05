import benchmark_functions as bf
import matplotlib.pyplot as plt
import numpy as np
from typing import List, Union

def show_char(fobj: Union[bf.BenchmarkFunction, object], args: List[float] = None) -> None:
    if isinstance(fobj, bf.BenchmarkFunction):
        if args is not None:
            fobj.show(showPoints=args)
        else:
            fobj.show()

    elif hasattr(fobj, 'evaluate') and hasattr(fobj, 'lb') and hasattr(fobj, 'ub'):
        dim1, dim2 = 0, 1
        n_points = 50

        x = np.linspace(fobj.lb[dim1], fobj.ub[dim1], n_points)
        y = np.linspace(fobj.lb[dim2], fobj.ub[dim2], n_points)
        X, Y = np.meshgrid(x, y)

        Z = np.zeros_like(X)
        fixed_point = np.zeros(fobj.ndim)

        for i in range(n_points):
            for j in range(n_points):
                fixed_point[dim1] = X[i, j]
                fixed_point[dim2] = Y[i, j]
                Z[i, j] = fobj.evaluate(fixed_point)

        fig = plt.figure(figsize=(12, 10))
        ax = fig.add_subplot(111, projection='3d')

        surf = ax.plot_surface(X, Y, Z, cmap='viridis', alpha=0.9,
                               edgecolor='none', linewidth=0, antialiased=True)

        ax.set_xlabel(f'Dimension {dim1}', fontsize=12)
        ax.set_ylabel(f'Dimension {dim2}', fontsize=12)
        ax.set_zlabel('Function Value', fontsize=12)
        ax.set_title(f'{fobj.__class__.__name__} (3D Surface)', fontsize=14)

        cbar = fig.colorbar(surf, ax=ax, shrink=0.5, aspect=10)
        cbar.set_label('Function Value', fontsize=12)

        plt.show()
    else:
        print(f"Function {type(fobj).__name__} does not support visualization")