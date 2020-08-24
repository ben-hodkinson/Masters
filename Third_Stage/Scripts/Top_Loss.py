import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os.path
from scipy.ndimage.filters import gaussian_filter1d
from matplotlib.patches import Rectangle

folder = os.path.join( os.environ["HOME_DIRECTORY"], "Saved_Networks/Blitz/Search" )

exist_nets =    [
                    ( "ELU_5x1000_AM_L1_AA_XY_1e-02_Loss.csv",    0.60, "Swish 5x2000 Adam Huber", 10, 2 ),
                    ( "Swish_5x1000_AM_L1_AA_XY_1e-02_Loss.csv",  0.61, "Swish 5x1000 Adam Huber", 3, 1 ),
                    ( "PReLU_3x1000_AM_L1_AA_XY_1e-02_Loss.csv",  0.63, "Swish 3x2000 Adam Huber", 10, 2 ),
                    ( "Swish_3x1000_AM_L1_AA_XY_1e-02_Loss.csv",  0.64, "Swish 3x1000 Adam Huber", 3, 1 ),
                    ( "Swish_7x200_AM_L1_AA_XY_1e-02_Loss.csv",   0.65, "Swish 9x500   Adam Huber", 1, 1 ),
                    ( "Swish_7x500_AM_L1_AA_XY_1e-02_Loss.csv",   0.66, "Swish 7x500   Adam Huber", 3, 1 ),
                    ( "Swish_5x500_AM_L1_AA_XY_1e-02_Loss.csv",   0.71, "Swish 5x500   Adam Huber", 3, 1 ),
                    ( "ELU_5x500_AM_L1_AA_XY_1e-02_Loss.csv",     0.99, "ELU    5x2000 Adam Huber", 3, 1 ),
                ]

deleted_nets =  [
                ]

fig = plt.figure( figsize = (10,5) )
ax_loss = fig.add_subplot( 1 , 1 , 1 )
ax_loss.set_xlabel('Epoch')
ax_loss.set_ylabel('Average RMSE')

for (name, correction, title, filter, squish) in exist_nets + deleted_nets:
    full_name = os.path.join( folder, name )

    data = pd.read_csv( full_name, header = None )

    train_loss = data.iloc[0, :].to_numpy()

    smoothed = gaussian_filter1d(train_loss, sigma=5)
    smoothed = smoothed - ( min( smoothed ) - correction )

    xspace = np.arange(0, len(smoothed))

    xspace = xspace / squish

    ax_loss.plot( xspace, smoothed, '-', label = title + r"$\rightarrow {:.2f}$".format(min(smoothed)) )

ax_loss.set_ylim([0,15.8])
ax_loss.set_xlim([80,4000])
ax_loss.legend()
plt.show()


