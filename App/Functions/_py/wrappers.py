from typing import List, Literal
from .plotting_fitness import plot_best_fitness_history, plot_avg_fitness_history
from .plotting_3d import plot_and_save_3d

def plot_fitness_summary(
        num_iterations: int,
        best_values: List[float],
        avg_values: List[float] = None,
        std_values: List[float] = None,
        output_dir: str = ".",
        show: bool = False
) -> None:
    import os
    os.makedirs(output_dir, exist_ok=True)
    
    plot_best_fitness_history(
        num_iterations=num_iterations,
        best_values=best_values,
        plot_path=os.path.join(output_dir, "best_fitness.png"),
        csv_path=os.path.join(output_dir, "best_fitness.csv"),
        show=show
    )
    
    if avg_values is not None and std_values is not None:
        plot_avg_fitness_history(
            num_iterations=num_iterations,
            avg_values=avg_values,
            std_values=std_values,
            plot_path=os.path.join(output_dir, "avg_fitness.png"),
            csv_path=os.path.join(output_dir, "avg_fitness.csv"),
            show=show
        )


def plot_3d_simple(
        points: List[List[float]],
        values: List[float],
        plot_type: Literal["scatter", "surface", "contour"] = "scatter",
        output_dir: str = ".",
        show: bool = False
) -> None:
    import os
    os.makedirs(output_dir, exist_ok=True)
    
    plot_and_save_3d(
        points=points,
        values=values,
        csv_path=os.path.join(output_dir, f"data_{plot_type}.csv"),
        plot_path=os.path.join(output_dir, f"plot_{plot_type}.png"),
        show=show,
        plot_type=plot_type
    )