#!/bin/bash
sizes="512 1024 1536 2048 4096"
threads=7
block_size=128
iters=5

for size in $sizes
do
    for ((j = 0; j <= $iters; j++))
    do
        echo "Iteration $j"
        echo "Runnig blocks..."
        ./pmm $size $size $threads out_matrix.out blocks $block_size
        echo "Runnig strassen..."
        ./pmm $size $size $threads out_matrix.out strassen $block_size
        echo "Runnig gpu..."
        ./pmm $size $size $threads out_matrix.out gpu $block_size
    done
done
