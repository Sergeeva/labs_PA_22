all: threads

threads: threads.cpp
	g++ -lm -pthread threads.cpp -o ./threads.out && ./threads.out
	
clean_files:
	rm -rf *.txt

clean:
	rm -rf *.o *.out *.txt results/*txt

