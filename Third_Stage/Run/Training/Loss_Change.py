import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os.path

def ccm( c, n_classes ):
    total_maps = [ "summer", "PRGn", "autumn", "winter", "cool", "hot"]
    idx_map = int(c * n_classes)
    val = ( c * n_classes - idx_map  ) / 2
    cmap = plt.get_cmap(total_maps[idx_map])
    return cmap(val)

folder = os.path.join( os.environ["HOME_DIRECTORY"], "Saved_Networks" )

net_names = [

        "Main_Swish_5x1000_AM_SL_AA_XY_6e-07_Drop2",
        # "Main_WL_Swish_5x1000_AM_SL_AA_XY_5e-08_Drop2",
        # "Main_SMP_WL_Swish_5x1000_AM_SL_AA_XY_5e-08_Drop2",
        # "Main_Swish_10x200_AM_SL_SG_XY_1e-06_BN",

        ]

fig = plt.figure( figsize = (10,5) )
ax_loss = fig.add_subplot( 1 , 1 , 1 )
ax_loss.set_xlabel('Epoch')
ax_loss.set_ylabel('Average Loss')

for name in net_names:

    full_name = os.path.join( folder, name, "loss.csv" )

    data = pd.read_csv( full_name, header = None )
    train_loss = data.iloc[0, :].to_numpy()
    test_loss  = data.iloc[1, :].to_numpy()
    check_loss = data.iloc[2, :].to_numpy()
    time       = data.iloc[3, :].to_numpy()

    ax_loss.plot( test_loss,  '-', label = "Testing Loss" )
    ax_loss.plot( check_loss,  '-', label = "Training Loss" )
    # ax_loss.plot( train_loss,  '-', label = "Training Loss" )

ax_loss.legend()





















plt.show()



















