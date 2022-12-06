#define GROUP_DIM_0 8
#define GROUP_DIM_1 8
#define GROUP_SIZE (GROUP_DIM_0 * GROUP_DIM_1)
#define BANK_SIZE 32

__kernel void matrix_mul(__global const int* a, __global const int* b, __global int* c, unsigned long matrix_height, unsigned long matrix_width){
    int local_id0 = get_local_id(0);
    int local_id1 = get_local_id(1);
    int global_id0 = get_global_id(0);
    int global_id1 = get_global_id(1);
    int group_id0 = get_group_id(0);
    int group_id1 = get_group_id(1);
    __local int matrix1[GROUP_SIZE];
    __local int matrix2[GROUP_SIZE];
    __local int matrix_res[GROUP_SIZE];
    int my_row = local_id0 * GROUP_DIM_1;
    int my_local_res_id = local_id0 * GROUP_DIM_1 + local_id1;
    int my_global_res_id = global_id0 * matrix_width + global_id1;
    int matrix_row = global_id0 * matrix_width;
    matrix_res[my_local_res_id] = 0;
    
    for(int j = 0; j < matrix_width; j += GROUP_DIM_1){
        matrix1[my_local_res_id] = a[matrix_row + j + local_id1];
        matrix2[my_local_res_id] = b[(j + local_id0) * matrix_width + global_id1];
        
        barrier(CLK_LOCAL_MEM_FENCE);

        int sum = 0;

        for(int i = 0; i < GROUP_DIM_1; i++){
            sum += matrix1[my_row + (i + my_local_res_id) % GROUP_DIM_1] * matrix2[((i + my_local_res_id) % GROUP_DIM_0) * GROUP_DIM_1 + local_id1];
        }

        matrix_res[my_local_res_id] += sum;

        barrier(CLK_LOCAL_MEM_FENCE);
    }

    c[my_global_res_id] = matrix_res[my_local_res_id];
    
}
