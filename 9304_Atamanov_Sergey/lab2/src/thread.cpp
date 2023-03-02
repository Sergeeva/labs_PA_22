#include "thread.h"

void startLab2(int M, int N, int count, int pc, int cc, int T) {
    std::vector<std::thread*> threads;
    std::vector<std::thread*> threadsProducer;
    std::vector<std::thread*> threadsConsumer;
    std::atomic<int> sumCount = 0;
    auto* output = new std::fstream;
    output->open("outputThread.txt", std::ios_base::out);

    Queue<Matrix*> createQueue;

	Queue<Matrix*> printQueue;

    auto begin = std::chrono::steady_clock::now();
    for(int i = 0; i < pc; i++) {
        auto *threadProduce = new std::thread(createMatrix, M, N, std::ref(createQueue), count);
        threadsProducer.push_back(threadProduce);
    }

    for(int i = 0; i < T; i++) {
        auto *thread = new std::thread(sumMatrix, M, N, std::ref(createQueue), std::ref(printQueue), count, ref(sumCount));
        threads.push_back(thread);
    }
	
	for(int i = 0; i < pc; i++)
        threadsProducer[i]->join();

    for(int i = 0; i < T; i++)
        threads[i]->join();

    for(int i = 0; i < cc; i++){
        auto threadConsumer = new std::thread(printMatrix, std::ref(printQueue), count, output);
        threadsConsumer.push_back(threadConsumer);
    }

    for(int i = 0; i < cc; i++)
        threadsConsumer[i]->join();

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

    std::cout << "Produce - " << pc << ", consume - " << cc << ", time - " << duration.count() << " milliseconds\n";

    output->close();
}

void createMatrix(int M, int N, Queue<Matrix*>& queue, int count){
	while(queue.count < count) {
		auto* matrix = new Matrix(M, N);
		matrix->fillMatrix();

        queue.push(matrix);
        queue.count.fetch_add(1);
	}
}

void sumMatrix(int M, int N, Queue<Matrix*>& queue, Queue<Matrix*>& printQueue, int count, std::atomic<int>& sumCount) {
	while(sumCount < count) {
        sumCount.fetch_add(1);
        auto temp = queue.pop();
        auto* resultMatrix = new Matrix(M, N);

        resultMatrix->sumMatrix(temp);

        printQueue.push(resultMatrix);
    }
}

void printMatrix(Queue<Matrix*>& queue, int count, std::fstream* output) {
	while(queue.count < count) {
        queue.count.fetch_add(1);
        auto temp = queue.pop();
        *output << temp;
    }
}