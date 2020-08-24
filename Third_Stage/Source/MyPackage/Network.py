import math

import torch
import torch.nn as nn
import torch.nn.functional as F
from collections import OrderedDict

class AMANNDA(nn.Module):
    def __init__(self, act = "ELU", depth = 2, width = 2000, dropout_p = 0.0, out_type = "XY", batch_norm = False ):
        super(AMANNDA, self).__init__()

        self.out_type = out_type
        self.ls = width


        ####### Defining input and output type #######
        self.input = 65

        if out_type == "XY":
            self.output = 2
        elif out_type == "R":
            self.output = 1
        else:
            print( "\n\nWarning: XY and R are the only co-ordinates available right now!!!\n\n" )
            return 1


        ####### Defining the activation function #######
        if   act == "ELU":   activ = nn.ELU()
        elif act == "ReLU":  activ = nn.ReLU()
        elif act == "SELU":  activ = nn.SELU()
        elif act == "Swish": activ = Swish()
        elif act == "PReLU": activ = nn.PReLU()
        else:
            print( "\n\nWarning: unrecognised activation function!!!\n\n" )
            return 1


        ####### Defining the normalisation method #######

        if dropout_p > 0.0 and not batch_norm:
            BN_DP_split = 0

        if dropout_p == 0.0 and batch_norm:
            BN_DP_split = depth

        if dropout_p > 0.0 and batch_norm:
            BN_DP_split = math.ceil(depth/2)

        ####### Defining the layer structure #######
        layers = []
        for l_num in range(1, depth+1):
            inpt = self.input if l_num == 1 else self.ls

            layers.append(( "lin_{}".format(l_num), nn.Linear(inpt, self.ls) ))

            layers.append(( "act_{}".format(l_num), activ ))

            if dropout_p > 0.0 and l_num > BN_DP_split: layers.append(( "dro_{}".format(l_num), nn.Dropout(dropout_p) ))

            if batch_norm and l_num <= BN_DP_split: layers.append(( "btn_{}".format(l_num), nn.BatchNorm1d( self.ls, momentum=0.01 ) ))


        layers += [ ( "lin_out", nn.Linear(self.ls, self.output, bias = True) ) ]
        self.fc = nn.Sequential(OrderedDict(layers))
        ############################################



    def forward(self, data):
        return self.fc(data)

    def print_weight_distributions(self):
        i = 1

        print(" ___________________________________________________ ")
        print("| {:5} {:>10} {:>10} {:>10} {:>10} |".format( "Layer", "Mean", "StdDev", "Min", "Max" ) )
        print("|                                                   |")

        for layer in self.fc:
            if isinstance(layer, nn.Linear):
                size = layer.weight.size()[1]
                mean = layer.weight.mean().item()
                std = layer.weight.std().item()
                min = layer.weight.min().item()
                max = layer.weight.max().item()
                print( "| {:<5} {:10.4f} {:10.4f} {:10.4f} {:10.4f} |".format( i, mean, math.sqrt(size) * std, min, max ) )
                i += 1

        print("|___________________________________________________|")




class CaSANNDRA(nn.Module):
    def __init__(self, act = "ELU", depth = 2, width = 1000, dropout_p = 0.0, out_type = "XY", batch_norm = False ):
        super(CaSANNDRA, self).__init__()

        self.out_type = out_type
        self.ls = width


        ####### Defining input and output type #######
        self.input = 65

        if not out_type == "RSC":
            print( "\n\n\nWarning: CaSANNDRA only works for RSC regression!!!\n\n\n"  )
            return 0


        ####### Defining the activation function #######
        if   act == "ELU":   activ = nn.ELU()
        elif act == "ReLU":  activ = nn.ReLU()
        elif act == "SELU":  activ = nn.SELU()
        elif act == "Swish": activ = Swish()
        elif act == "PReLU": activ = nn.PReLU()
        else:
            print( "\n\nWarning: unrecognised activation function!!!\n\n" )
            return 1


        ####### Defining the layer structure #######
        if (width%2) != 0:
            print( "\n\n\nWarning: CaSANNDRA needs an even number of neurons per layer!!!\n\n\n" )
        half_width = self.ls//2

        if depth < 3:
            print( "\n\n\nWarning: CaSANNDRA needs a depth greater than 3!!!\n\n\n" )

        full_layers = []
        for l_num in range(1, depth-1):
            inpt = self.input if l_num == 1 else self.ls
            full_layers.append(( "lin_{}".format(l_num), nn.Linear(inpt, self.ls) ))
            full_layers.append(( "act_{}".format(l_num), activ ))
            if batch_norm > 0:
                full_layers.append(( "btn_{}".format(l_num), nn.BatchNorm1d(self.ls) ))

        top_layers = []
        for l_num in range(depth-1, depth+1):
            top_layers.append(( "top_lin_{}".format(l_num), nn.Linear(half_width, half_width) ))
            top_layers.append(( "top_act_{}".format(l_num), activ ))
            if dropout_p > 0:
                top_layers.append(( "top_dro_{}".format(l_num), nn.Dropout(dropout_p) ))
        top_layers += [ ( "top_lin_out", nn.Linear(half_width, 1, bias = True) ) ]

        bot_layers = []
        for l_num in range(depth-1, depth+1):
            bot_layers.append(( "bot_lin_{}".format(l_num), nn.Linear(half_width, half_width) ))
            bot_layers.append(( "bot_act_{}".format(l_num), activ ))
            if dropout_p > 0:
                bot_layers.append(( "bot_dro_{}".format(l_num), nn.Dropout(dropout_p) ))
        bot_layers += [ ( "bot_lin_out", nn.Linear(half_width, 2, bias = True) ) ]

        self.fc      = nn.Sequential(OrderedDict(full_layers))
        self.top_seg = nn.Sequential(OrderedDict(top_layers))
        self.bot_seg = nn.Sequential(OrderedDict(bot_layers))


    def forward(self, data):
        data = self.fc(data)

        t_data, b_data = data.split(self.ls//2, 1)

        t_data = self.top_seg(t_data)
        b_data = self.bot_seg(b_data)

        all_data = torch.cat( ( t_data , b_data ), 1 )

        return all_data




class EU_Loss(nn.Module):
    def __init__(self, out_type):
        super(EU_Loss, self).__init__()

        self.out_type = out_type

    def forward(self, output, truth):

        if self.out_type == "XY":
            loss = ( output - truth )**2
            loss = torch.sum( loss, dim = 1 )
            loss = torch.sqrt( loss + 0.01 ) - 0.1

        else:
            loss = torch.abs( output - truth )

        return loss



class Swish(nn.Module):
    def __init__(self):
        super(Swish, self).__init__()

    def forward(self, input):
        return ( input * torch.sigmoid(input) )
