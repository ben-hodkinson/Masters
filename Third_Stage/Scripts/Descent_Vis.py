import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np

from matplotlib.patches import Circle, PathPatch
from mpl_toolkits.mplot3d import Axes3D
import mpl_toolkits.mplot3d.art3d as art3d

font = {'weight' : 'bold',
        'size'   : 22}


def func(x, y):
    return + np.cos(3*x+1) + np.cos(2*y-1)

# Make data.
grid = np.arange(-1, 1, 0.1)

x, y = np.meshgrid( grid, grid )
z = func(x, y)

## 3D axes stuff
fig3d = plt.figure( figsize = (10,5) )
ax3d = fig3d.gca(projection='3d')

surf = ax3d.plot_surface(x, y, z, cmap=cm.cubehelix, linewidth=0, antialiased=False, vmin=-3, vmax=3)

ax3d.xaxis.set_rotate_label(False)
ax3d.yaxis.set_rotate_label(False)
ax3d.zaxis.set_rotate_label(False)

ax3d.xaxis.set_ticks([-1,-0,1])
ax3d.yaxis.set_ticks([-1,-0,1])
ax3d.zaxis.set_ticks([])

ax3d.set_xlabel(r"$\theta_1$", **font)
ax3d.set_ylabel(r"$\theta_2$", **font)
ax3d.set_zlabel(r"$C$", **font)

ax3d.set_zticklabels([])
plt.show()

