#!/bin/bash

make

max_threads=100
sizes="10 100 1000 10000"
matrix1="matrix1.bin"
matrix2="matrix2.bin"
res_matrix="matrix_sum.bin"

for size in $sizes
do
    echo "Generating matrices with size $size x $size"
    ./generate_matrix $size $size $matrix1 bin
    ./generate_matrix $size $size $matrix2 bin
    for ((i = 1; i <= $max_threads; i++))
    do
        echo "Runnig with $i threads..."
        ./thread_sum $matrix1 $matrix2 $res_matrix $i bin >> result.log
    done
done
