all: libcompute.a

file.o: file.c file.h
	gcc -O -c -std=c99 file.c file.h

computation_functions.o: computation_functions.c computation_functions.h
	gcc -O -c -std=c99 computation_functions.c computation_functions.h

libcompute.a: file.o computation_functions.o 
	ar rcs libcompute.a file.o computation_functions.o
clean:
	rm -f *.o *.a *.gch