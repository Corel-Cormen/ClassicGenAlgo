import matplotlib.pyplot as plt
import numpy as np
from typing import List, Literal
import csv

def plot_and_save_3d(
        points: List[List[float]],
        values: List[float],
        csv_path: str = "data.csv",
        plot_path: str = "plot_3d.png",
        show: bool = False,
        plot_type: Literal["scatter", "surface", "contour"] = "scatter"
) -> None:
    
    plt.close('all')

    if len(points) != len(values):
        raise ValueError(f"Points length {len(points)} != values length {len(values)}")
    if not points:
        raise ValueError("Points list is empty")
    if any(len(p) != 3 for p in points):
        raise ValueError("All points must have exactly 3 coordinates [x, y, z]")

    with open(csv_path, 'w', newline='', encoding='utf-8') as f:
        writer = csv.writer(f)
        writer.writerow(['x', 'y', 'z', 'value'])
        for point, value in zip(points, values):
            writer.writerow([point[0], point[1], point[2], value])

    x = np.array([p[0] for p in points])
    y = np.array([p[1] for p in points])
    z = np.array([p[2] for p in points])
    vals = np.array(values)

    fig = plt.figure(figsize=(10, 8))

    if plot_type == "scatter":
        ax = fig.add_subplot(111, projection='3d')
        scatter = ax.scatter(x, y, z, c=vals, cmap='viridis', s=50, alpha=0.8, edgecolors='k', linewidth=0.5)

        ax.set_xlabel('X', fontsize=12)
        ax.set_ylabel('Y', fontsize=12)
        ax.set_zlabel('Z', fontsize=12)
        ax.set_title('3D Scatter Plot', fontsize=14)

        cbar = plt.colorbar(scatter, ax=ax, shrink=0.5, aspect=10)
        cbar.set_label('Function Value', fontsize=12)

    elif plot_type == "surface":
        from scipy.interpolate import griddata

        xi = np.linspace(x.min(), x.max(), 50)
        yi = np.linspace(y.min(), y.max(), 50)
        Xi, Yi = np.meshgrid(xi, yi)

        Zi = griddata((x, y), z, (Xi, Yi), method='cubic', fill_value=z.mean())
        Vi = griddata((x, y), vals, (Xi, Yi), method='cubic', fill_value=vals.mean())

        ax = fig.add_subplot(111, projection='3d')
        surf = ax.plot_surface(Xi, Yi, Zi, facecolors=plt.cm.viridis((Vi - Vi.min()) / (Vi.max() - Vi.min())),
                               alpha=0.8, edgecolor='none', shade=True)

        ax.set_xlabel('X', fontsize=12)
        ax.set_ylabel('Y', fontsize=12)
        ax.set_zlabel('Z', fontsize=12)
        ax.set_title('3D Surface Plot', fontsize=14)

        m = plt.cm.ScalarMappable(cmap='viridis')
        m.set_array(Vi)
        m.set_clim(Vi.min(), Vi.max())
        cbar = plt.colorbar(m, ax=ax, shrink=0.5, aspect=10)
        cbar.set_label('Function Value', fontsize=12)

    elif plot_type == "contour":
        from scipy.interpolate import griddata

        xi = np.linspace(x.min(), x.max(), 100)
        yi = np.linspace(y.min(), y.max(), 100)
        Xi, Yi = np.meshgrid(xi, yi)

        Zi = griddata((x, y), z, (Xi, Yi), method='cubic', fill_value=z.mean())

        ax = fig.add_subplot(111)

        contourf = ax.contourf(Xi, Yi, Zi, levels=20, cmap='viridis', alpha=0.8)
        contour = ax.contour(Xi, Yi, Zi, levels=20, colors='black', linewidths=0.5, alpha=0.4)
        ax.clabel(contour, inline=True, fontsize=8)

        scatter = ax.scatter(x, y, c=vals, cmap='plasma', s=30, edgecolors='k', linewidth=0.5, zorder=5)

        ax.set_xlabel('X', fontsize=12)
        ax.set_ylabel('Y', fontsize=12)
        ax.set_title('2D Contour Plot (Z values as colors)', fontsize=14)
        ax.set_aspect('equal', adjustable='box')

        cbar = plt.colorbar(contourf, ax=ax)
        cbar.set_label('Z Coordinate', fontsize=12)

    else:
        raise ValueError(f"Invalid plot_type: {plot_type}. Choose 'scatter', 'surface', or 'contour'")

    plt.savefig(plot_path, dpi=150, bbox_inches='tight')

    if show:
        plt.show()

    plt.close()