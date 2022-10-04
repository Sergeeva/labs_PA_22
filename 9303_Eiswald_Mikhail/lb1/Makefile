processes:
	g++ processes.cpp matrix/matrix.cpp args/args.cpp -o processes
threads:
	g++ threads.cpp matrix/matrix.cpp args/args.cpp -o threads -lpthread
clean:
	touch threads && rm threads
	touch processes && rm processes
