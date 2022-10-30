CXX ?= g++
CXXFLAGS := -pthread -std=c++2a -Wall

.PHONY: threads
threads:
	$(CXX) $(CXXFLAGS) threads/main.cpp -o threads.out && ./threads.out

.PHONY: p_threads
p_threads:
	$(CXX) $(CXXFLAGS) p_threads/main.cpp -o p_threads.out && ./p_threads.out

clean:
	rm -f result.txt *.out
