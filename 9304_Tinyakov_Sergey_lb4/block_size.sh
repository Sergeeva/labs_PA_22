#!/bin/bash
#sizes="10 100 1000 10000"
matrix_size=1024
threads=6
max_block_size=512
iters=5

for ((i = 64; i <= $max_block_size; i += 64))
do
    for ((j = 0; j <= $iters; j++))
    do
        echo "Iteration $j"
        echo "Blocks, block size is $i"
        ./pmm $matrix_size $matrix_size $threads out_matrix.out blocks $i
        echo "Strassen, block size is $i"
        ./pmm $matrix_size $matrix_size $threads out_matrix.out strassen $i
    done
done
