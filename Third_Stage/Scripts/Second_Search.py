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

    ( "No Regularisor",                 "ALL_Swish_5x1000_AM_L1_AA_XY_5e-06" ),

    ( "Dropout $p=0.1$",                "ALL_Swish_5x1000_AM_L1_AA_XY_5e-06_Drop1" ),
    ( "Dropout $p=0.2$",                "ALL_Swish_5x1000_AM_L1_AA_XY_5e-06_Drop2" ),
    ( "Dropout $p=0.3$",                "ALL_Swish_5x1000_AM_L1_AA_XY_5e-06_Drop3" ),
    ( "Dropout $p=0.5$",                "ALL_Swish_5x1000_AM_L1_AA_XY_5e-06_Drop5" ),

    ( "BatchNorm & Dropout: $p=0.3$",   "ALL_Swish_5x1000_AM_L1_AA_XY_5e-06_Drop3_BN" ),

    ( "BatchNorm",                      "ALL_Swish_5x1000_AM_L1_AA_XY_5e-06_BN" ),

        ]

fig = plt.figure( figsize = (10,5) )
ax_loss = fig.add_subplot( 1 , 1 , 1 )
ax_loss.set_xlabel('Epoch')
ax_loss.set_ylabel('Testing Loss')

mins = []
for label, name in net_names:

    full_name = os.path.join( folder, name, "loss.csv" )

    data = pd.read_csv( full_name, header = None )
    train_loss = data.iloc[0, :].to_numpy()
    test_loss  = data.iloc[1, :].to_numpy()
    check_loss = data.iloc[2, :].to_numpy()
    time       = data.iloc[3, :].to_numpy()

    val = 13.952914999999999 / 0.201766828125

    minl = min(test_loss)

    if minl > 1:
        test_loss = test_loss / val

    mins.append( min(test_loss) )

    print( label )
    print( np.argmin(test_loss) )
    print( "" )

    ax_loss.plot( test_loss,  '-', label = label )

inds = np.argsort(mins)

for i in inds:
    print( net_names[i] )
    print( "{:.5f}".format( mins[i] ) )
    print( "" )

ax_loss.legend()





















plt.show()



















