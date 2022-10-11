#include "../headers/SumTask.h"


void SumTask::run() {
    for (int i = offset; i < offset + batch_size && i < total_size; i++) {
        int height = i / target.get_width();
        int width = i % target.get_width();
        int sum = holder.get_first().get(height, width) + holder.get_second().get(height, width);

        target.set(height, width, sum);
    }
}