#include "thread.h"

void startLab3(int M, int N, int T, int count, int pc, int cc) {
	Queue createQueue;
	Queue printQueue;
    std::atomic<int> sumCount = 0;

    std::vector<std::thread*> threadsProducer;
    std::vector<std::thread*> threads;
    std::vector<std::thread*> threadsConsumer;

    auto* output = new std::fstream;
    output->open("./outputProcess.txt", std::ios_base::out);

    auto begin = std::chrono::steady_clock::now();
    for(int i = 0; i < pc; i++) {
        auto threadProduce = new std::thread(createMatrix, M, N, std::ref(createQueue), count);
        threadsProducer.push_back(threadProduce);
    }

    for(int i = 0; i < T; i++) {
        auto thread = new std::thread(sumMatrix, M, N, std::ref(createQueue), std::ref(printQueue), count, ref(sumCount));
        threads.push_back(thread);
    }

    for(int i = 0; i < pc; i++)
        threadsProducer[i]->join();

    for(int i = 0; i < T; i++)
        threads[i]->join();


    for(int i = 0; i < cc; i++) {
        auto *threadConsumer = new std::thread(printMatrix, std::ref(printQueue), T, output);
        threadsConsumer.push_back(threadConsumer);
    }

    for(int i = 0; i < cc; i++)
        threadsConsumer[i]->join();

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "Produce - " << pc << ", consume - " << T << ", time - " << duration.count() << " milliseconds\n";

    output->close();
}

void createMatrix(int M, int N, Queue& queue, int count) {
	
	while(queue.count <= count) {
        auto matrix = new Matrix(M, N);
        queue.count.fetch_add(1);

        matrix->fillMatrix();

		std::atomic<Node*> newTail = new Node(matrix);
		Node* null = nullptr;

		while (true) {
            Node *oldTail = queue.tail.load();

            if (oldTail->next.compare_exchange_weak(null, newTail)) {
                queue.tail.compare_exchange_strong(oldTail, newTail);
                break;
            } else
                queue.tail.compare_exchange_strong(oldTail, oldTail->next);
        }
	}
}
void sumMatrix(int M, int N, Queue& queue, Queue& printQueue, int count, std::atomic<int>& sumCount) {

	while(sumCount < count) {
        sumCount.fetch_add(1);
		Matrix* matrix;

		while (true) {
			Node* oldHead = queue.head.load();
			Node* oldTail = queue.tail.load();
			Node* nextHead = oldHead->next.load();

			if (oldHead == oldTail) {
				if (nextHead == nullptr)
					continue;
				else
					queue.tail.compare_exchange_strong(oldTail, nextHead);
			}
			else {
				matrix = nextHead->_data;

				if (queue.head.compare_exchange_weak(oldHead, nextHead))
                    break;
			}
		}

		auto* resultMatrix = new Matrix(M, N);

		resultMatrix->sumMatrix(matrix);

		std::atomic<Node*> newTail = new Node(resultMatrix);
		Node* null = nullptr;

		while (true) {
			Node* oldTail = printQueue.tail.load();

			if (oldTail->next.compare_exchange_weak(null, newTail)) {
				printQueue.tail.compare_exchange_strong(oldTail, newTail);
				break;
			}
			else {
				printQueue.tail.compare_exchange_strong(oldTail, oldTail->next);
			}
		}

		delete matrix;
	}
}

void printMatrix(Queue& queue, int count, std::fstream* output) {

	Matrix* matrix;

	while(queue.count < count) {
        queue.count.fetch_add(1);

		while (true) {
			Node* oldHead = queue.head.load();
			Node* oldTail = queue.tail.load();
			Node* nextHead = oldHead->next.load();

			if (oldHead == oldTail) {
				if (nextHead == nullptr)
					continue;
				else
					queue.tail.compare_exchange_strong(oldTail, nextHead);
			}
			else {
				matrix = nextHead->_data;

				if (queue.head.compare_exchange_weak(oldHead, nextHead))
					break;
			}
		}
		*output << *matrix;

        delete matrix;
	}
}