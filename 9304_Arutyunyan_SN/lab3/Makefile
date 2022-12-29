CXX ?= g++
CXXFLAGS := -pthread --std=c++17 -Wall

.PHONY: threads
threads:
	$(CXX) $(CXXFLAGS) threads/main.cpp -o threads.out

clean:
	rm -f result.txt *.out
