#!/bin/bash

make

threads=8
types="blocking non_blocking non_blocking_cycled"
iterations=100
prog_iter_count=1000000
result_file="time_result.log"

rm -f $result_file

for ((i = 1; i <= $iterations; i++))
do
    echo "Iteration $i..."
    for type in $types
    do
        ./test_time $type $threads $prog_iter_count >> $result_file
        ./test_time $type $threads $prog_iter_count >> $result_file
        ./test_time $type $threads $prog_iter_count >> $result_file
    done
done
