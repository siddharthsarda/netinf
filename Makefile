#
# Makefile for non-Microsoft compilers
#

## Linux  (uncomment the 2 lines below for compilation on Linux)
CXXFLAGS += -std=c++98 -Wall -ggdb
LDFLAGS += -lrt

## CygWin (uncomment the 2 lines below for compilation on CygWin)
#CXXFLAGS += -Wall
#LDFLAGS += 

all: netinf generate_nets
 
opt: CXXFLAGS += -O4
opt: LDFLAGS += -O4
opt: netinf generate_nets

# COMPILE

generate_nets: generate_nets.cpp Snap.o
	g++ -o generate_nets generate_nets.cpp cascinf.cpp Snap.o -I./glib -I./snap $(LDFLAGS) 

netinf: netinf.cpp Snap.o
	g++ -o netinf netinf.cpp cascinf.cpp Snap.o -I./glib -I./snap $(LDFLAGS) 
	
Snap.o: 
	g++ -c $(CXXFLAGS) ./snap/Snap.cpp -I./glib -I./snap

clean:
	rm -f   generate_nets Snap.o  $(MAIN).exe generate_nets.exe
