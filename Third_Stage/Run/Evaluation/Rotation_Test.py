import numpy as np
import torch
import time


Z_events = 16836855.898109462
NonZ_events = 40018.28559402158
Real = 18356995.0

print( (Real - NonZ_events) / Z_events )

## Zmumu
Z_events = 19907253.84590653
NonZ_events = 42151.175749949165
Real = 22759438.0 

print( (Real - NonZ_events) / Z_events )

# batch_size = 1000000
# theta = torch.empty(batch_size).uniform_(-np.pi, np.pi)
# 
# dummy = torch.empty( 1, dtype = torch.float32, device = "cuda" )
# 
# start = time.time()
# matrix = torch.empty( batch_size, 2, 2, dtype = torch.float32, device = "cuda" )
# matrix[:,0,0] =  torch.cos(theta)
# matrix[:,0,1] = -torch.sin(theta)
# matrix[:,1,0] =  torch.sin(theta)
# matrix[:,1,1] =  torch.cos(theta)
# print( time.time() - start )
# 
# start = time.time()
# other_matrix = torch.empty( batch_size, 2, 2, dtype = torch.float32, device = "cuda" )
# other_matrix[:,0,0] =  torch.cos(theta)
# other_matrix[:,1,0] =  torch.sin(theta)
# other_matrix[:,1,1] =  other_matrix[:,0,0]
# other_matrix[:,0,1] = -other_matrix[:,1,0]
# print( time.time() - start )
# 
# print(matrix - other_matrix)
