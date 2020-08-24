import matplotlib.pyplot as plt

import numpy as np

import torch
import torch.nn as nn
import torch.nn.functional as F

def Swish(x):
    return x * torch.sigmoid(x)

def plot_this( func, name ):
    fig = plt.figure()
    ax = fig.add_subplot( 1 , 1 , 1 )
    
    x_space = torch.linspace(-5,5, 101)
    y_space = func(x_space)

    x = x_space.numpy().squeeze()
    y = y_space.numpy().squeeze()
    
    plt.grid(True)
    plt.axhline(0, color='grey')
    plt.axvline(0, color='grey')

    plt.plot(x, y, color = 'red', linewidth=3)
    
    # plt.text(-4, 2, name, fontsize=25, color = 'red')
    
    plt.ylim([-2,3])
    plt.xlim([-5,5])
    plt.savefig( name + ".pdf" )

plot_this( nn.Sigmoid(), "Sigmoid" )
plot_this( nn.ReLU(), "ReLU" )
plot_this( nn.LeakyReLU( negative_slope=0.1 ), "Leaky_ReLU" )
plot_this( nn.ELU(), "ELU" )
plot_this( nn.SELU(), "SELU" )
plot_this( Swish, "Swish" )
