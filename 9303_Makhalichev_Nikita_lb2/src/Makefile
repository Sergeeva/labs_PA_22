OBJ = Buffer.o Matrix.o main.o
H = ./Headers
S = ./Sources
NAME = a.out
FLAGS = -c -std=c++14 -pthread -lm

all: a.out

a.out: $(OBJ)
	g++ $(OBJ) -o $(NAME)

main.o: $(S)/main.cpp
	g++ $(FLAGS) $(S)/main.cpp
	
Buffer.o: $(S)/Buffer.cpp $(H)/Buffer.h
	g++ $(FLAGS) $(S)/Buffer.cpp

Matrix.o: $(S)/Matrix.cpp $(H)/Matrix.h
	g++ $(FLAGS) $(S)/Matrix.cpp

clearobj:
	rm -rf *.o

clear:
	rm -rf *.o *.out *.txt
