import os
import csv
import sys
import time
import random
import shutil
import argparse
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from collections import OrderedDict

import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import Dataset, DataLoader

import MyPackage
import MyPackage.Network as mn

######## The data set class must be defined for PyTorch to be able to read the data ########
class EventsDataset(Dataset):
    def __init__( self, root_folder, data_set_name, numrows = None, out_type = "XY", sample_from = None ):
        print( data_set_name )

        self.out_type = out_type
        my_file_name = os.path.join( root_folder, data_set_name )


        if sample_from is None:
            print( "-- converting csv to pandas" )
            file_data = pd.read_csv( my_file_name, nrows = numrows, dtype=np.float32, header = None )

            print( "-- converting pandas to tensor" )
            self.tensor_data = torch.as_tensor( file_data.values.astype(np.float32), dtype = torch.float )
            del file_data

        else:
            print( "-- sampling from previously generated dataset" )
            random.seed(100)
            indicies = random.sample( range( len(sample_from) ), numrows )
            self.tensor_data = sample_from.tensor_data[indicies]

        print( "-- done\n" )

    def __len__(self):
        return len(self.tensor_data)

    def __getitem__(self, idx):
        truth_data  = self.tensor_data[idx, 1:3]
        recon_data  = self.tensor_data[idx, 4:69]
        weight_data = self.tensor_data[idx, 69:70]

        # True_MET = ( self.tensor_data[idx, 0].item() * 5.35681798e+01 ) + 6.45477052e+01
        # if True_MET < 37.97468354:
            # weight_data[0] = 1.0

        return recon_data, truth_data, weight_data
None

## An alternative weight initialisation useful for SELU and deeper networks
def alternative_weight_init(m):
    if isinstance(m, nn.Linear):
        print( m.weight.size()[1] )
        m.weight.data.normal_( 0.0, 1.0/np.sqrt(m.weight.size()[1]) )
        m.bias.data.fill_(0.0)



## Training the Neural Network
def train(train_loss, activated_ann, device, data_loader, loss_fn, running_diag, optimiser, weighted_learning):
    activated_ann.train() # Tells PyTorch that we are in training mode for dropout
    tot_loss = torch.tensor(0, requires_grad=False, dtype=torch.float).to(device)
    new_p = 0
    old_p = -1

    for batch_idx, (recon, truth, weight) in enumerate(data_loader):
        recon, truth = recon.to(device), truth.to(device)

        optimiser.zero_grad()
        outputs = activated_ann(recon)

        if weighted_learning:
            weight = weight.to(device)
            loss = loss_fn(outputs, truth) * weight
        else:
            loss = loss_fn(outputs, truth)

        ave_loss = loss.mean()

        ave_loss.backward()
        optimiser.step()

        tot_loss += loss.detach().sum()

        # For printing percentages
        if running_diag:
            new_p = int( 100*batch_idx/len(data_loader) )
            if (new_p > old_p):
                print( "Training --- {:.0%} complete         \r".format( new_p/100 ), end="" )
                sys.stdout.flush()
                old_p = new_p


    print( "Training --- {100%} complete         " )

    train_loss.append( tot_loss.item() / len(data_loader.dataset) / 2 )

    return 0


# Evaluating the Neural Network
def evaluate(test_loss, activated_ann, device, data_loader, loss_fn, running_diag, weighted_learning):
    activated_ann.eval() # Tells PyTorch that we are in evaluation mode for dropout
    tot_loss = torch.tensor(0, requires_grad=False, dtype=torch.float).to(device)
    new_p = 0
    old_p = -1

    with torch.no_grad():
        for batch_idx, (recon, truth, weight) in enumerate(data_loader):
            recon, truth = recon.to(device), truth.to(device)

            outputs = activated_ann(recon)

            if weighted_learning:
                weight = weight.to(device)
                loss = loss_fn(outputs, truth) * weight
            else:
                loss = loss_fn(outputs, truth)

            tot_loss += loss.detach().sum()

            # For printing percentages
            if running_diag:
                new_p = int( 100*batch_idx/len(data_loader) )
                if (new_p > old_p):
                    print( "Testing  --- {:.0%} complete     \r".format( new_p/100 ), end="" )
                    sys.stdout.flush()
                    old_p = new_p

        print( "Testing  --- {100%} complete         " )

        test_loss.append( tot_loss.item() / len(data_loader.dataset) / 2 )

        return 0


# A check on the number of bad epochs on evaluation data to prevent overfitting
def early_stop_check(patience, bad_epochs, best_loss, this_loss):
    stop_training = False

    if this_loss < best_loss:
        bad_epochs = 0
        best_loss = this_loss

    else:
        bad_epochs = bad_epochs + 1
        print("Number of bad epochs = {}/{}".format(bad_epochs, patience) )

        if bad_epochs == patience:
            print("Patience has been exceeded. Stopping the training process.")
            stop_training = True

    return stop_training, bad_epochs, best_loss


def check_arugments(params):
    parser = argparse.ArgumentParser()
    parser.add_argument( "--depth", type=int, default=0, metavar='D' )
    parser.add_argument( "--width", type=int, default=0, metavar='W' )
    parser.add_argument( "--activation", default='', metavar='A' )
    parser.add_argument( "--optimiser", default='', metavar='O' )
    parser.add_argument( "--learning_rate", type=float, default=0, metavar='LR' )
    parser.add_argument( "--dropout", type=float, default=0, metavar='DO' )
    parser.add_argument( "--loss_fn", default='', metavar='LS' )
    args = parser.parse_args()

    if args.depth != 0:
        params["depth"] = args.depth
    if args.width != 0:
        params["width"] = args.width
    if args.activation != '':
        params["activation"] = args.activation
    if args.optimiser != '':
        params["optimiser"] = args.optimiser
    if args.learning_rate != 0:
        params["learning_rate"] = args.learning_rate
    if args.dropout != 0:
        params["dropout"] = args.dropout
    if args.loss_fn != '':
        params["loss_fn"] = args.loss_fn

    return params


def create_net_name( params ):
    network_name = params["network_prefix"]

    if params["weighted_learning"]:
        network_name += "WL_"

    network_name += ( params["activation"] + '_' +
                      str(params["depth"]) + 'x' + str(params["width"]) + '_' +
                      params["network"]    + '_' + params["loss_fn"]    + '_' +  params["optimiser"] + '_' +
                      params["out_type"]   + '_' + "{:.0e}".format(params["learning_rate"]) )

    if params["dropout"] != 0:
        network_name += "_Drop" + str( int( params["dropout"]*10 ) )

    if params["L2_regularisation"] != 0:
        network_name += "_L2" + str(params["L2_regularisation"])

    if params["batchnorm"]:
        network_name += "_BN"

    if params["load_previous"] != "":
        network_name = params["load_previous"] + params["change_name"]

    return network_name


# Saving the neural network and its definitions as it improves
def save( params, network_name, epoch, activated_ann, optimiser, train_loss, test_loss, check_loss, train_speed, bad_epochs, best_epoch):

    output_spec_dir = params["network_base_dir"] + network_name + '/'

    if not os.path.exists( output_spec_dir ):
        os.system( "mkdir -p {}".format(output_spec_dir) )

    ## Saving the current form of the network and optimiser to continue training
    network_file = output_spec_dir + "network_model_current"
    opt_file = output_spec_dir + "optimiser_state"
    torch.save( activated_ann.state_dict(), network_file )
    torch.save( optimiser.state_dict(), opt_file )

    ## On the first epoch we save a copy of the data stats file for future use and normalisation
    if epoch == 1:
        srcpath = os.path.join(params["data_base_dir"], params["stats_dataset"] )
        dstpath = os.path.join(output_spec_dir, "data_stats.csv" )
        shutil.copyfile(srcpath, dstpath)

    ## Saving the best version of the network
    if bad_epochs == 0:
        network_file = output_spec_dir + "network_model_optimal"
        torch.save( activated_ann.state_dict(), network_file )
        best_epoch = epoch

    ## Saving legacy versions of the network
    if epoch%params["save_every"] == 0 and epoch != 0 and params["save_every"] != 0:
        network_file = output_spec_dir + "network_model_at_{}".format(epoch)
        torch.save( activated_ann.state_dict(), network_file )

    ## Saving the running loss data
    loss_file = output_spec_dir + "loss.csv"
    with open( loss_file , 'w' ) as outfile:
        wr = csv.writer(outfile)
        wr.writerow(train_loss)
        wr.writerow(test_loss)
        wr.writerow(check_loss)
        wr.writerow(train_speed)

    ## Saving the information file
    info_file = output_spec_dir + "info.txt"
    with open( info_file , 'w') as outfile:
        for param in params.keys():
            print("{}:  {}".format(param, params[param]), file=outfile)

        print("", file=outfile)
        print("Training speed:  {:.3e}s".format(np.average(train_speed)/params["train_set_size"]), file=outfile)
        print("Epochs trained:  {}".format(epoch), file=outfile)
        print("Best epoch:      {}".format(best_epoch), file=outfile)
        print("Best test loss:  {}".format(test_loss[best_epoch]), file=outfile)

        print("", file=outfile)
        print(activated_ann, file=outfile)

    return best_epoch


def main():

    ####### ADJUSTABLE PARAMETERS ########
    params = [

        ( "network_prefix",       "Final_" ), # A prefix for the network name

        ( "activation",           "Swish"),
        ( "depth",                10 ),
        ( "width",                200 ),
        ( "dropout",              0.0 ),
        ( "batchnorm",            True ),

        ( "weighted_learning",    False ),

        ( "network",             "AM" ),         # Either CA or AM (Casanndra, Amannda)
        ( "loss_fn",             "SL" ),         # Either L1 or L2 or SL or EU
        ( "optimiser",           "SG" ),         # Either AA or SG or AX or AD (ADAM, SGD, AdaMax, AdaDelta)
        ( "out_type",            "XY" ),         # Either XY, R

        ( "train_set_size",       None ),       # None = all
        ( "test_set_size",        None ),       # None = all

        ( "train_dataset",       "combined_train.csv" ),
        ( "test_dataset",        "combined_test.csv" ),
        ( "stats_dataset",       "combined_stats.csv" ),    # This file is not used in this script but will be copied and saved alongside the network

        ( "load_previous",       "Final_Swish_10x200_AM_SL_SG_XY_1e-06_BN" ),
        ( "change_name",         "" ),

        ( "learning_rate",       1e-03 ),
        ( "momentum",            0.9 ),  # Only if optimiser is SG
        ( "L2_regularisation",   0.0 ),
        ( "alternative_weight",  0 ),
        ( "scheduler_on",        0 ),

        ( "mini_batch_size",     5000 ),
        ( "number_of_workers",   12 ),

        ( "early_stop_patience", 500 ),             # 0 for no stopping
        ( "max_epochs",          10000 ),
        ( "save_every",          1000 ),

        ( "print_running",       True ),            # % updates during epochs. Bad if outputing to text file.

        ( "network_base_dir",    "/home/matthew/Documents/Masters/Met_network/Saved_Networks/" ),
        ( "data_base_dir",       "/home/matthew/Documents/Masters/Met_network/Data/Learning/" ),


        ]

    params = OrderedDict( params )
    network_name = create_net_name( params )
    print("")
    ######################################

    """ ___           _   _     _   _             _   _                 _     _
       |_ _|  _ __   (_) | |_  (_) | |_    __ _  | | (_)  ___    __ _  | |_  (_)   ___    _ __
        | |  | '_ \  | | | __| | | | __|  / _` | | | | | / __|  / _` | | __| | |  / _ \  | '_ \
        | |  | | | | | | | |_  | | | |_  | (_| | | | | | \__ \ | (_| | | |_  | | | (_) | | | | |
       |___| |_| |_| |_|  \__| |_|  \__|  \__,_| |_| |_| |___/  \__,_|  \__| |_|  \___/  |_| |_|
    """

    ####### Device initialisation #######
    device_name = "GPU" if torch.cuda.is_available() else "CPU"
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

    print( "Executing training operations on the {}:".format(device_name) )
    if device_name == "GPU":
        print( torch.cuda.get_device_name( torch.cuda.current_device() ) )
        torch.cuda.empty_cache()
    print("")



    ####### Neural Network initialisation ########
    print( "Initialising network: {}\n".format( network_name ) )

    if   params["network"] == "AM":
        activated_ann = mn.AMANNDA(   params["activation"], params["depth"], params["width"], params["dropout"], params["out_type"], params["batchnorm"] )
    elif params["network"] == "CA":
        activated_ann = mn.CaSANNDRA( params["activation"], params["depth"], params["width"], params["dropout"], params["out_type"], params["batchnorm"] )

    else:
        print( "\n\nWarning: unrecognised network type!!!\n\n" )
        return 1

    if params["alternative_weight"]:    activated_ann.apply(alternative_weight_init)


    if params["load_previous"] != "":
        print( "Loading previous state of network\n" )
        activated_ann.load_state_dict(torch.load( params["network_base_dir"] + params["load_previous"] + "/network_model_current" ) )

    print( activated_ann )
    print('')

    activated_ann = activated_ann.to(device)



    ####### Dataset initialisation #######
    if params["out_type"] not in ["XY", "R"]:
        print( "\n\nWarning: unrecognised output type!!!\n\n" )
        return 1
    print( "Initialising the datasets from: {} and {} \n".format( params["train_dataset"], params["test_dataset"] ) )

    train_set = EventsDataset( params["data_base_dir"], params["train_dataset"], numrows = params["train_set_size"], out_type = params["out_type"] )
    test_set  = EventsDataset( params["data_base_dir"], params["test_dataset"],  numrows = params["test_set_size"] , out_type = params["out_type"] )
    check_set = EventsDataset( params["data_base_dir"], params["train_dataset"], numrows = len(test_set)           , out_type = params["out_type"], sample_from = train_set )

    if params["train_set_size"] == None: params["train_set_size"] = len(train_set)
    if params["test_set_size"]  == None: params["test_set_size"]  = len(test_set)

    train_loader = DataLoader(train_set, batch_size = params["mini_batch_size"], num_workers = params["number_of_workers"], pin_memory=True, shuffle = True )
    test_loader  = DataLoader(test_set,  batch_size = params["mini_batch_size"], num_workers = params["number_of_workers"], pin_memory=True )
    check_loader = DataLoader(check_set, batch_size = params["mini_batch_size"], num_workers = params["number_of_workers"], pin_memory=True )

    print( '==>> Total number of training events:     {}'.format( len(train_set) ) )
    print( '==>> Total number of testing events:      {}'.format( len(test_set) ) )
    print( '==>> Total number of train check events:  {}'.format( len(check_set) ) )
    print('')


    ####### Loss Function initialisation #######
    if   params["loss_fn"] == "L1":   loss_fn = nn.L1Loss( reduction='none' )
    elif params["loss_fn"] == "L2":   loss_fn = nn.MSELoss( reduction='none' )
    elif params["loss_fn"] == "SL":   loss_fn = nn.SmoothL1Loss( reduction='none' )
    elif params["loss_fn"] == "EU":   loss_fn = mn.EU_Loss( params["out_type"] )

    else:
        print( "\n\nWarning: unrecognised loss function!!!\n\n" )
        return 1



    ####### Optimiser initialisation #######
    if   params["optimiser"] == "AA":   optimiser = optim.Adam( activated_ann.parameters(), lr = params["learning_rate"], weight_decay = params["L2_regularisation"] )
    elif params["optimiser"] == "SG":   optimiser = optim.SGD( activated_ann.parameters(), lr = params["learning_rate"], weight_decay = params["L2_regularisation"], momentum = params["momentum"], nesterov=True)
    elif params["optimiser"] == "AX":   optimiser = optim.Adamax( activated_ann.parameters(), lr = params["learning_rate"], weight_decay = params["L2_regularisation"] )
    elif params["optimiser"] == "AD":   optimiser = optim.Adadelta( activated_ann.parameters(), lr = 5*params["learning_rate"], weight_decay = params["L2_regularisation"] )

    else:
        print("\n\nWarning: unrecognised optimiser!!!\n\n")
        return 1

    # if params["load_previous"] != "":
        # print( "Loading previous state of optimiser\n" )
        # optimiser.load_state_dict(torch.load( params["network_base_dir"] + params["load_previous"] + "/optimiser_state" ) )

    if params["scheduler_on"]:
        scheduler = optim.lr_scheduler.ReduceLROnPlateau(optimiser, factor=0.9, patience=10, verbose=True, threshold=0.0)



    ######################################

    """ ____    _             _
       / ___|  | |_    __ _  | |_    ___
       \___ \  | __|  / _` | | __|  / _ \
        ___) | | |_  | (_| | | |_  |  __/
       |____/   \__|  \__,_|  \__|  \___|
    """

    ####### The current state of the network is evaluated ########
    train_loss  = []
    test_loss   = []
    check_loss  = []
    train_speed = []

    if params["load_previous"] != "":
        previous_data = pd.read_csv( params["network_base_dir"] + params["load_previous"] + "/loss.csv" , header = None )
        print(previous_data)

        train_loss  = previous_data.iloc[0, :].tolist()
        test_loss   = previous_data.iloc[1, :].tolist()
        check_loss  = previous_data.iloc[2, :].tolist()
        train_speed = previous_data.iloc[3, :].tolist()

        best_loss = min(test_loss)
        best_epoch = np.argmin(test_loss)

        print( "Best epoch was at: {}".format(best_epoch) )
        print( "Previous loss on all training Data:     {:.5} ".format( train_loss[-1] ) )
        print( "Previous loss on all testing Data:      {:.5} ".format( test_loss[-1]  ) )
        print( "Previous loss on check training Data:   {:.5} ".format( check_loss[-1] ) )
        activated_ann.print_weight_distributions()

    else:
        e_start_time = time.time()

        evaluate( train_loss, activated_ann, device, train_loader, loss_fn, params["print_running"], params["weighted_learning"] )
        evaluate( test_loss,  activated_ann, device, test_loader,  loss_fn, params["print_running"], params["weighted_learning"] )
        evaluate( check_loss, activated_ann, device, check_loader, loss_fn, params["print_running"], params["weighted_learning"] )
        best_loss = test_loss[0]
        best_epoch = 0

        print( "Initial loss on all training Data:     {:.5} ".format( train_loss[0] ) )
        print( "Initial loss on all testing Data:      {:.5} ".format( test_loss[0]  ) )
        print( "Initial loss on check training Data:   {:.5} ".format( check_loss[0] ) )

        train_time = time.time() - e_start_time
        train_speed.append(train_time)
        print( "Epoch Time: {:.5}".format(train_time) )
        activated_ann.print_weight_distributions()



    ######################################

    """ _____                  _           _
       |_   _|  _ __    __ _  (_)  _ __   (_)  _ __     __ _
         | |   | '__|  / _` | | | | '_ \  | | | '_ \   / _` |
         | |   | |    | (_| | | | | | | | | | | | | | | (_| |
         |_|   |_|     \__,_| |_| |_| |_| |_| |_| |_|  \__, |
                                                        |___/
    """

    ####### Some initial values before training loop ########
    stop_training = False
    bad_epochs = 0

    ####### Learning process commences #######
    print("\nStarting to learn:")
    for epoch in range(len(train_loss), params["max_epochs"]+1):
        print("")
        print( "Epoch: {}".format(epoch) )

        e_start_time = time.time()

        train   ( train_loss, activated_ann, device, train_loader, loss_fn, params["print_running"], optimiser, params["weighted_learning"] )
        evaluate( test_loss,  activated_ann, device, test_loader,  loss_fn, params["print_running"], params["weighted_learning"] )
        evaluate( check_loss, activated_ann, device, check_loader, loss_fn, params["print_running"], params["weighted_learning"] )

        print("Average loss on all training Data:     {:.5} ".format( train_loss[-1] ) )
        print("Average loss on all testing Data:      {:.5} ".format( test_loss[-1]  ) )
        print("Average loss on check training Data:   {:.5} ".format( check_loss[-1] ) )

        if np.isnan( train_loss[-1] ):
            print("\n\nNOT A NUMBER DETECTED!!!")
            print("ABORTING LEARNING!!! \n\n")
            break

        train_time = time.time() - e_start_time
        train_speed.append(train_time)
        print( "Epoch Time: {:.5}".format(train_time) )

        if params["scheduler_on"]:
            scheduler.step(train_loss[-1])

        stop_training, bad_epochs, best_loss = early_stop_check(params["early_stop_patience"], bad_epochs, best_loss, test_loss[-1])
        if stop_training:
            break

        best_epoch = save( params, network_name, epoch, activated_ann, optimiser, train_loss, test_loss, check_loss, train_speed, bad_epochs, best_epoch )
        # activated_ann.print_weight_distributions()

    return 0

if __name__ == '__main__':
    main()
