import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os.path
from scipy.ndimage.filters import gaussian_filter1d

folder = os.path.join( os.environ["HOME_DIRECTORY"], "Saved_Networks/Blitz/Search" )

list_depth = [ 3, 5 ] # 3, 5, 7
list_width = [ 100, 200, 500, 1000 ] # 100, 200, 500, 1000
list_activation = [ "ELU" ] # "ReLU", "ELU", "SELU", "Swish", "PReLU"
list_optimiser = [ "AA", "AX", "AD", "SG" ] # "AA", "AX", "AD", "SG"
list_learning_rate = [ 0.01, 0.001, 0.0001 ]

net_names = []

# Creating More names
for d in list_depth:
    for w in list_width:
        for o in list_optimiser:
            for a in list_activation:
                for l in list_learning_rate:

                    full_name = os.path.join( folder, "{}_{}x{}_AM_L1_{}_XY_{:.0e}_Loss.csv".format(a,d,w,o,l) )

                    if os.path.isfile(full_name):

                        min_loss = min( np.genfromtxt( full_name, max_rows=1, delimiter = ',' ) )

                        if min_loss < 1.5:
                            net_names.append(full_name)
                            print( "{}_{}x{}_AM_L1_{}_XY_{:.0e}_Loss.csv".format(a,d,w,o,l), min_loss )


fig = plt.figure( figsize = (10,5) )
ax_loss = fig.add_subplot( 1 , 1 , 1 )
ax_loss.set_xlabel('Epoch')
ax_loss.set_ylabel('Average Loss')

colour_space = np.linspace( 0, 1, len(net_names), endpoint=False)


for i, (c, name) in enumerate(zip(colour_space, net_names)):
    col = plt.cm.jet(c)

    data = pd.read_csv( name, header = None )

    train_loss = data.iloc[0, :].to_numpy()

    ax_loss.plot( train_loss, '-', color = col, label = "{} (test)".format(name) )


ax_loss.legend()
plt.show()
