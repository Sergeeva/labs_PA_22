CXX ?= g++
CXXFLAGS := -pthread -std=c++17 -Wall -g

.PHONY: threads
run:
	$(CXX) $(CXXFLAGS) main.cpp -o run.out && ./run.out

clean:
	rm -f result.txt *.out
