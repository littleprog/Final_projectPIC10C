output: main.o 
	g++ main.o -o output -lncurses
	
main.o: main.cpp
	g++ -c main.cpp 

clean:
	rm *.o output
	
	
