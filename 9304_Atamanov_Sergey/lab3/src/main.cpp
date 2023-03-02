#include "thread.h"

#define M 5000
#define N 5000
#define T 1
#define count 12
#define producerCounter 2
#define consumerCounter 1

int main() {
	startLab3(M, N, T, count, producerCounter, consumerCounter);

	return 0;
}