TARGET = monitor
GCC = g++
CFLAGS = -Wall -O2 -c
CFLAGS += $(shell root-config --cflags)
LDFLAGS = $(shell root-config --ldflags)
LIBS    = $(shell root-config --libs)
LIBS   += $(shell root-config --glibs)

#ROOTFLAG = `root-config --cflags`
#ROOTLIBS = `root-config --libs --glibs`
#ROOTLIBS += -lRooFit -lRooFitCore -lMinuit
#ROOTLIBS += -lMinuit
#LDFLAGS = $(shell root-config --ldflags)


main	:src/main.o
	$(GCC) -o ${TARGET} $^ $(LIBS) $(LDFLAGS)
#$(LDFLAGS)


src/main.o	:src/main.cxx
	$(GCC) $(CFLAGS)  -c $^ -o $@

clean:
	rm -f src/*.o
	rm -f *~

