import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

import numpy as np

font = {
        'size'     : 15
       }

fig = plt.figure( figsize = (6,4) )
ax = fig.add_subplot( 1 , 1 , 1 )

x = np.linspace(1, 100, 200)
train_loss = np.divide(1, np.power(x, 0.3)/2) + x/2000 + np.random.rand( *x.shape )/20
test_loss  = np.divide(1, np.power(x, 0.3)/2) + np.power(x, 1.2)/500 + np.random.rand( *x.shape )/20

ax.plot(x, train_loss, color = 'b', linewidth=2, label = "Training error")
ax.plot(x, test_loss,  color = 'r', linewidth=2, label = "Testing error")

min_loc = x[np.argmin(test_loss)]

ax.axvline( x=min_loc, linestyle = '--', color = 'k' )
ax.text(min_loc+10, 1.2, "final model", fontsize=12, color = 'k', verticalalignment='center' )

arrow = mpatches.FancyArrowPatch( (min_loc+10, 1.2), (min_loc, 1.2), mutation_scale=15)
ax.add_patch(arrow)

ax.xaxis.set_ticks([])
ax.yaxis.set_ticks([])

ax.set_xlabel("Training Epochs", **font )
ax.set_ylabel("Error", **font )

plt.legend()
plt.show()
