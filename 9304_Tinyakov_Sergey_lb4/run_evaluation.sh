#!/bin/bash
sizes="128 256 512 1024 2048"
threads=6
block_size=128
iters=5

for size in $sizes
do
    for ((j = 0; j <= $iters; j++))
    do
        echo "Iteration $j"
        echo "Runnig elements..."
        ./pmm $size $size $threads out_matrix.out elements $block_size
        echo "Runnig blocks..."
        ./pmm $size $size $threads out_matrix.out blocks $block_size
        echo "Runnig strassen..."
        ./pmm $size $size $threads out_matrix.out strassen $block_size
    done
done
