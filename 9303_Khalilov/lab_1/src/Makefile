all: gen_matrix

gen_matrix: generate.cpp
	g++ generate.cpp -o ./gen_matrix.out && ./gen_matrix.out $(n) $(m) 

processes: processes.cpp gen_matrix
	g++ -lm -pthread processes.cpp -o ./processes.out  && ./processes.out

threads: threads.cpp gen_matrix
	g++ -lm -pthread threads.cpp -o ./threads.out && ./threads.out
	
p_threads: p_threads.cpp gen_matrix
	g++ -lm -pthread p_threads.cpp -o ./p_threads.out  && ./p_threads.out $(p)

clean_files:
	rm -rf *.txt

clean:
	rm -rf *.o *.out *.txt

