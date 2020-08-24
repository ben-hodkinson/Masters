import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

import numpy as np

import torch
import torch.nn as nn
import torch.nn.functional as F

L1Loss = nn.L1Loss( reduction = 'none')
MSELoss = nn.MSELoss( reduction = 'none')
SmoothL1Loss = nn.SmoothL1Loss( reduction = 'none')


fig = plt.figure()
ax = fig.add_subplot( 1 , 1 , 1 )
    
x = torch.linspace(-3,3, 101)
z = torch.zeros( x.shape )
l1 = L1Loss(x, z)
l2 = MSELoss(x, z)
hu = SmoothL1Loss(x, z)

x  = x.numpy().squeeze()
l1 = l1.numpy().squeeze()
l2 = l2.numpy().squeeze()
hu = hu.numpy().squeeze()

plt.grid(True)

plt.plot(x, l1, color = 'red',  label = "Mean Absolute Error")
plt.plot(x, l2, color = 'green', label = "Mean Squared Error")
plt.plot(x, hu, color = 'blue', label = "Huber Loss" )

plt.ylim([0,np.max(hu)])

plt.ylabel("Cost")
plt.xlabel(r"$\hat{y}_i - y_i$")

plt.legend()
plt.show()
