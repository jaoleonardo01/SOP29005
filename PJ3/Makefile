# TerminALL Main Makefile

include Makedefs

LIBDIR := fs

all: FILESYSTEM.o
	(cd $(LIBDIR) && make all)
	g++ FILESYSTEM.o $(LDFLAGS) -o FILESYSTEM

clean: FORCE
	(cd $(LIBDIR) && make clean)
	rm -f *.o FILESYSTEM core*

FORCE: 	
