TARGET = monitor
GCC = g++
ROOTFLAG = `root-config --cflags`
ROOTLIBS = `root-config --libs --glibs`
#ROOTLIBS += -lRooFit -lRooFitCore -lMinuit
#ROOTLIBS += -lMinuit

main	:src/main.o
	$(GCC) $(ROOTLIBS) $^ -o ${TARGET}

src/main.o	:src/main.cxx
	$(GCC) $(ROOTFLAG) -c $^ -o $@

clean:
	rm -f src/*.o
	rm -f *~

