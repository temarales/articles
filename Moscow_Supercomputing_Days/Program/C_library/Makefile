INCLUDE_DIRS = -L./lib
LIBS       = -lcompute

default: all

all: program 

program: library
	gcc -lm -std=c99 -o program program_signals.o $(INCLUDE_DIRS) $(LIBS)

program_signals.o: program_signals.c
	gcc -O -c -std=c99 program_signals.c

library: program_signals.o
	@$(MAKE) -C lib all

clean:
	rm -f program *.o 
	@$(MAKE) -C lib clean