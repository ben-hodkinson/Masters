# Root variables
ROOTCFLAGS   = $(shell root-config --cflags)
ROOTLIBS     = $(shell root-config --nonew --libs)
ROOTGLIBS    = $(shell root-config --nonew --glibs)
# rootcint
ROOTCINT= rootcint
# rootcint flags
RCINCL  = -I.
RCFLAGS = -c -p
# Programs
CXX          = g++
CXXFLAGS     = -O2 -Wall -fPIC
# for code profiling
LD          = g++
LDFLAGS      = -O
SOFLAGS      = -shared
# fortran stuff
F77          = f77
F77FLAGS     = -c 
RM           = rm -f 
MV           = mv 
ECHO         = echo
CINT         = rootcint 
TCLIBS +=$(ROOTGLIBS)
USERLIBS    = /usr/local
MINUIT2_LIB_DIR := $(ROOTHOME)/lib
MINUIT2_INC_DIR := $(ROOTHOME)/include
CXXFLAGS    += $(ROOTCFLAGS) -I../inc -I$(MINUIT2_INC_DIR) 
LIBS        += $(ROOTLIBS) -L. -lana 
GLIBS       += $(ROOTGLIBS) -L. -lana -L/usr/lib -lnsl  -lm -lg2c -L/usr/lib/root $(MINUIT_LIB_DIR) -lMinuit2 -lGraf -lRIO -lHist -lMatrix -lCore -lCint -lMathCore 
CXXSRCS      = MyEvent.C 
CXXOBJS      = MyEvent.o MyEventDict.o
EXEOBJS       = Analysis.o
GARBAGE      = $(CXXOBJS) $(EXEOBJS) libana.so Analysis *Dict.h *Dict.C *~
.PHONY : all
%.o : %.C 
	$(CXX) $(CXXFLAGS) -c $<
%Dict.C : ../inc/%.h
	$(CINT) -f $@ -c  $< ../inc/$*LinkDef.h 
all : Analysis
libana : $(CXXOBJS)
	$(CXX) $(SOFLAGS) $(LDFLAGS) $^ -o $@.so 
libFast : $(EXEOBJS)
	$(CXX) $(SOFLAGS) $(LDFLAGS) $^ -o $@.so

Analysis : Analysis.o libana 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ Analysis.C $(ROOTGLIBS) -L. -lana 
clean :
	$(RM) $(GARBAGE)
print :
	echo compiler  : $(CXX)
	echo c++ srcs  : $(CXXSRCS)
	echo c++ objs  : $(CXXOBJS)
	echo c++ flags : $(CXXFLAGS)
	echo libs      : $(LIBS)
	echo so flags  : $(SOFLAGS)
