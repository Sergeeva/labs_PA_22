void kernel multiplication(global const int* A, 
                           global const int* B,
                           global int* C,
                           const int side)
{
    const int myR = get_global_id(0) / side;
    const int myX = get_global_id(0) % side;

    int sum = 0;

    for (unsigned i = 0; i < side; ++i)
        sum += B[myR * side + i] * A[i * side + myX];

    C[get_global_id(0)] = sum;
}