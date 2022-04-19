import subprocess
import numpy as np
import pandas as pd
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.animation import FuncAnimation
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import sys

if __name__ == "__main__":
    file_data = None
    with open(sys.argv[1]) as f:
        file_data = [list(map(float, row.split())) for row in f.readlines()]

    x_max = file_data[0][0]
    h = file_data[0][1]
    t_max = file_data[0][2]
    tau = file_data[0][3]

    X = np.arange(0, x_max + h / 2, h)
    Y = np.arange(0, t_max + tau / 2, tau)
    X, Y = np.meshgrid(X, Y)
    matrix = np.matrix(file_data[1:])

    hf = plt.figure(figsize=[18, 10])
    ax = hf.add_subplot(111, projection='3d')
    ax.set_xlabel('X', fontsize=15)
    ax.set_ylabel('t', fontsize=15)
    ax.set_zlabel('u', fontsize=15)

    ax.plot_surface(X, Y, matrix, cmap='viridis', linewidths=0.2);
    plt.show()

