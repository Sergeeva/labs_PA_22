generator: matrix_generate.cpp
	g++ matrix_generate.cpp -o ./generator.out

processes: processes.cpp
	g++ -lm -pthread processes.cpp -o ./processes.out

threads: threads.cpp
	g++ -lm -pthread threads.cpp -o ./threads.out
	
Pthreads: Pthreads.cpp
	g++ -lm -pthread Pthreads.cpp -o ./Pthreads.out

cleartxtfiles:
	rm -rf *.txt

clearobjectsfiles:
	rm -rf *.o

clear:
	rm -rf *.o *.out *.txt
