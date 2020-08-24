import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os.path

folder = os.path.join( os.environ["HOME_DIRECTORY"], "Saved_Networks" )

def fun( x, pow ):
    1 / np.power(x, 1.1) + np.random.randn()/20000

for i in range(1000):