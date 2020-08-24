import numpy as np

class Variable(object):
    def __init__(self, name, nbins, xmin, xmax, tree = None, branch = None, reso = False, tail = False ):

        self.name = name
        self.nbins = nbins
        self.xmin = xmin
        self.xmax = xmax

        self.tree = tree
        self.branch = branch
        self.tail = tail
        self.reso = reso

    def TH2D_params( self, nbins2d, xmin2d, xmax2d ):

        self.xmin2d  = xmin2d
        self.xmax2d  = xmax2d
        self.nbins2d = nbins2d



class WorkingPoint(object):
    def __init__(self, name, tree = None, branch = None ):

        self.name = name

        if tree == None:
            if name == "Network":
                self.tree = "ann_tree"
            else:
                self.tree = "wpt_tree"

        if branch == None:
            if name == "Truth":   branch = "WP_Truth"
            if name == "Network": branch = "WP_ANN"
            if name == "Tight":   branch = "WP_TST_Tight"
            if name == "Loose":   branch = "WP_TST_Loose"
            if name == "FJVT":    branch = "WP_TST_FJVT"
            if name == "Calo":    branch = "WP_CST_Tight" 
            if name == "Track":   branch = "WP_Track"

        self.Et = branch + "_ET"
        self.Ex = branch + "_X"
        self.Ey = branch + "_Y"
        self.Phi = branch + "_Phi"




def angle_between( x1, y1, x2, y2 ):
    vec1 = [x1, y1]
    vec2 = [x2, y2]

    phi = np.arccos( np.dot(vec1, vec2) / np.linalg.norm(vec1) / np.linalg.norm(vec2) )
    cross = np.cross( vec1, vec2 )

    if cross < 0:
        return phi
    else:
        return -phi



def anticlockwise_rotate( vec, angle):
    x = vec[0]
    y = vec[1]
    new_x =   x*np.cos(angle) - y*np.sin(angle)
    new_y =   x*np.sin(angle) + y*np.cos(angle)
    return [ new_x, new_y ]




def TH1D_Maths_String( var_name, Rec_Et, Rec_Ex, Rec_Ey, Rec_Phi, True_Et, True_Ex, True_Ey, True_Phi ):

    if var_name == "R":
        return "{rt} - {tt}".format( rt = Rec_Et, tt = True_Et )

    if var_name == "D":
        return "TMath::Sqrt( ( {rx} - {tx} )^2 + ( {ry} - {ty} )^2 )".format( tx = True_Ex, ty = True_Ey, rx = Rec_Ex, ry = Rec_Ey )

    if var_name == "Lin":
        return "( {tt} > 1 ) * ( {rt} / ( {tt} + 0.000001 ) - 1 )".format( tt = True_Et, rt = Rec_Et )

    if var_name == "Phi":
        return "({rp}-{tp}) + 2*pi*(({rp}-{tp})<-pi) - 2*pi*(({rp}-{tp})>pi)".format( tp = True_Phi, rp = Rec_Phi )

    if var_name == "X":
        return "{rx} - {tx}".format( rx = Rec_Ex, tx = True_Ex )

    if var_name == "Y":
        return "{ry} - {ty}".format( ry = Rec_Ey, ty = True_Ey )

    if var_name == "Truth":
        return "{tt}".format( tt = True_Et )

    if var_name == "WPmag":
        return "{rt}".format( rt = Rec_Et )









































