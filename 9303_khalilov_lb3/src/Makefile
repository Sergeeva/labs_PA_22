
all: threads

threads: main.cpp
	g++ -lm -pthread main.cpp -o ./threads.out && ./threads.out
	
clean_files:
	rm -rf *.txt

clean:
	rm -rf *.o *.out *.txt results/*txt