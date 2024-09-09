CPP=g++
SRC_DIR=../../src
INCLUDE_DIR=../../include
CFLAGS=-Wall -Wconversion -Wsign-conversion -O3 -I$(INCLUDE_DIR)
ROOTFLAGS=-isystem$(shell root-config --incdir) -L$(shell root-config --libdir) -lCore -lRIO -lHist -lTree -lMathCore -lVc

all: angdisttest

AngularDistribution.o: $(SRC_DIR)/AngularDistribution.cc $(INCLUDE_DIR)/AngularDistribution.hh
	$(CPP) -c -o $@ $< $(CFLAGS)

angdisttest: AngularDistribution.o AngularDistributionGenerator.cpp
	$(CPP) -o $@ $^ $(CFLAGS) $(ROOTFLAGS)
	cp $@ ../../

.PHONY: all clean

clean:
	rm -f angdisttest
	rm -f AngularDistribution.o
	rm -f ../../angdisttest
