# Defining the variables
finalname = Run_MT2

CXX = g++
ROOTFlags = `root-config --cflags --glibs`

includes = lester_mt2_bisect.h

all: Add_MT2_tree.cpp
	$(CXX) -o $(finalname) Add_MT2_tree.cpp $(includes) $(ROOTFlags)