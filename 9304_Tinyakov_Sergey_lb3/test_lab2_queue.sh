#!/bin/bash
make
max_queue_size=1000
iterations=100
result_file="lab2_result.log"

rm -f $result_file

for ((i = 1; i <= $iterations; i++))
do
    echo "Iteration $i..."
    ./thread_sum 3 3 10000 1 $max_queue_size test.out blocking >> $result_file
    ./thread_sum 3 3 10000 1 $max_queue_size test.out non_blocking >> $result_file
done
