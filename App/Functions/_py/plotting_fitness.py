import json
import matplotlib.pyplot as plt
import numpy as np
from typing import List
import csv

def plot_best_fitness_history(
        num_iterations: int,
        best_values: List[float],
        plot_path: str = "best_fitness.png",
        csv_path: str = "best_fitness.csv",
        show: bool = False,
        points: List[List[float]] = None
) -> None:

    plt.close('all')

    if num_iterations <= 0:
        raise ValueError("num_iterations must be greater than 0")
    if len(best_values) != num_iterations:
        raise ValueError(f"best_values length {len(best_values)} != num_iterations {num_iterations}")
    if not best_values:
        raise ValueError("best_values list is empty")
    if points is not None and len(points) != num_iterations:
        raise ValueError("points length must equal num_iterations")

    iterations = list(range(1, num_iterations + 1))

    with open(csv_path, 'w', newline='', encoding='utf-8') as f:
        writer = csv.writer(f)
        writer.writerow(['iteration', 'best_value', 'points'])
        for i, best in zip(iterations, best_values):
            vec = ''
            if points is not None:
                vec = json.dumps(points[i-1])
            writer.writerow([i, best, vec])

    plt.figure(figsize=(10, 6))
    plt.plot(iterations, best_values, marker='o', linewidth=2, markersize=4, color='blue', label='Best Value')

    plt.xlabel('Iteration', fontsize=12)
    plt.ylabel('Function Value', fontsize=12)
    plt.title('Best Fitness vs Iteration', fontsize=14)
    plt.grid(True, alpha=0.3)
    plt.legend(fontsize=10)

    plt.savefig(plot_path, dpi=150, bbox_inches='tight')

    if show:
        plt.show()

    plt.close()


def plot_avg_fitness_history(
        num_iterations: int,
        avg_values: List[float],
        std_values: List[float],
        plot_path: str = "avg_fitness.png",
        csv_path: str = "avg_fitness.csv",
        show: bool = False
) -> None:

    plt.close('all')

    if num_iterations <= 0:
        raise ValueError("num_iterations must be greater than 0")
    if len(avg_values) != num_iterations:
        raise ValueError(f"avg_values length {len(avg_values)} != num_iterations {num_iterations}")
    if len(std_values) != num_iterations:
        raise ValueError(f"std_values length {len(std_values)} != num_iterations {num_iterations}")
    if not avg_values or not std_values:
        raise ValueError("avg_values and std_values lists cannot be empty")

    iterations = list(range(1, num_iterations + 1))

    with open(csv_path, 'w', newline='', encoding='utf-8') as f:
        writer = csv.writer(f)
        writer.writerow(['iteration', 'avg_value', 'std_deviation'])
        for i, avg, std in zip(iterations, avg_values, std_values):
            writer.writerow([i, avg, std])

    avg_arr = np.array(avg_values)
    std_arr = np.array(std_values)

    fig, ax1 = plt.subplots(figsize=(10, 6))

    color_avg = 'tab:blue'
    ax1.set_xlabel('Iteration', fontsize=12)
    ax1.set_ylabel('Average Function Value', fontsize=12, color=color_avg)
    line1 = ax1.plot(iterations, avg_arr, marker='o', linewidth=2, markersize=4,
                     color=color_avg, label='Average Value')
    ax1.tick_params(axis='y', labelcolor=color_avg)
    ax1.grid(True, alpha=0.3)

    ax2 = ax1.twinx()
    color_std = 'tab:orange'
    ax2.set_ylabel('Standard Deviation', fontsize=12, color=color_std)
    line2 = ax2.plot(iterations, std_arr, marker='s', linewidth=2, markersize=4,
                     color=color_std, label='Std Deviation')
    ax2.tick_params(axis='y', labelcolor=color_std)

    lines = line1 + line2
    labels = [l.get_label() for l in lines]
    ax1.legend(lines, labels, loc='upper right', fontsize=10)

    plt.title('Average Fitness and Standard Deviation vs Iteration', fontsize=14)
    fig.tight_layout()

    plt.savefig(plot_path, dpi=150, bbox_inches='tight')

    if show:
        plt.show()

    plt.close()