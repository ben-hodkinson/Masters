import torch

if torch.cuda.is_available():
    n_gpus = torch.cuda.device_count()
    
    print("{} GPU{} detected.".format( n_gpus, 's' if n_gpus>1 else '' ) )
    
    for i in range(n_gpus):
        print("_______")
        print( "Name:                ", torch.cuda.get_device_name( i ) )
        print( "Compute Capability:  ", torch.cuda.get_device_capability( i ) )
        print( "Memroy Allocated:    ", torch.cuda.max_memory_allocated( i ) )
        print("_______")
        
else:
    print("No GPUs detected")