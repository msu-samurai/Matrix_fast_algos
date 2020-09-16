all: fast

fast: mult_checking.o multiplication.o output.o initialize.o
	gcc mult_checking.o multiplication.o output.o initialize.o -o fast

mult_checking.o: mult_checking.c
	gcc -c mult_checking.c -o mult_checking.o  

multiplication.o: multiplication.c multiplication.h
	gcc -c multiplication.c -o multiplication.o

output.o: output.c output.h 
	gcc -c output.c -o output.o 
initialize.o: initialize.c initialize.h
	gcc -c initialize.c -o initialize.o 

clean:
	rm -rf *.o fast  