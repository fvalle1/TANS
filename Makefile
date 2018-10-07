CFLAGS = $(shell root-config --cflags) -qopenmp
LIBS   = $(shell root-config --libs)
GLIBS  = $(shell root-config --glibs)

CXX = icpc
CC = icc

all: central

central: main.cxx
	$(CXX) $^ $(CFLAGS) $(LIBS) $(GLIBS) -o central.out

decay: main.cxx
	$(CXX) $^ $(CFLAGS) $(LIBS) $(GLIBS) -o decay.out
.PHONY:
clean:
	rm -rf *.o *.d *.so *.pcm
