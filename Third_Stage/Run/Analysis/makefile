# Defining the variables
finalname = Run_SRs

CXX = g++
ROOTFlags = `root-config --cflags --glibs`

includes = lester_mt2_bisect.h

all: SUSY_Plots.cpp
	$(CXX) -o $(finalname) SUSY_Plots.cpp $(includes) $(ROOTFlags)