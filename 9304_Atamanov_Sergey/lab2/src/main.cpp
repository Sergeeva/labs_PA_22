#include "thread.h"

#define M 5000
#define N 5000
#define count 12
#define producerCounter 1
#define consumerCounter 2
#define T 1

int main() {
	startLab2(M, N, count, producerCounter, consumerCounter, T);

	return 0;
}