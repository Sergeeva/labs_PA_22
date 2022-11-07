#include <unistd.h>
#include <sys/wait.h>

#include <limits>
#include <iostream>
#include <fstream>

#include "args/args.h"
#include "matrix/matrix.h"

using namespace std;

int main(int c, char** v) {
    int params[] = {5, 5, (numeric_limits<int>::max() - 1)/2};
    parse_args(c, v, params, 3);
    printf("Generating and adding %dx%d matrices with elements up to %d\n",
           params[0], params[1], params[2]);

    int pipe_to_counter[2];
    constexpr int read_index = 0;
    constexpr int write_index = 1;
    int buf_size = (2 + params[0]*params[1])*sizeof(int);
    if (pipe(pipe_to_counter) < 0) {
        perror(v[0]);
        return 1;
    }
    pid_t pid = fork();
    if(pid == -1) {
        perror(v[0]);
        return 2;
    }
    if(pid == 0) {
        close(pipe_to_counter[read_index]);
        Matrix matrix1 = Matrix(params[0], params[1], params[2]);
        Matrix matrix2 = Matrix(params[0], params[1], params[2]);
        if(params[0] <= 10 && params[1] <= 10) {
            cout << matrix1 << "\n" << matrix2;
            cout.flush();
        }
        int* data1 = matrix1.serialize();
        write(pipe_to_counter[write_index], (void*)data1, buf_size);
        int* data2 = matrix2.serialize();
        write(pipe_to_counter[write_index], (void*)data2, buf_size);
        close(pipe_to_counter[write_index]);
        delete [] data1;
        delete [] data2;
        _exit(0);
    }
    close(pipe_to_counter[write_index]);
    char* buf = new char[buf_size];
    int bytes_read = 0;
    while(bytes_read < buf_size) {
        bytes_read += read(pipe_to_counter[read_index],
                           buf+bytes_read, buf_size-bytes_read);
    }
    Matrix read_matrix1 = Matrix((int*)buf);
    bytes_read = 0;
    while(bytes_read < buf_size) {
        bytes_read += read(pipe_to_counter[read_index],
                           buf+bytes_read, buf_size-bytes_read);
    }
    Matrix read_matrix2 = Matrix((int*)buf);
    wait(&pid);
    close(pipe_to_counter[read_index]);
    delete [] buf;
    int pipe_from_counter[2];
    if (pipe(pipe_from_counter) < 0) {
        perror(v[0]);
        return 1;
    }
    pid = fork();
    if(pid == -1) {
        perror(v[0]);
        return 2;
    }
    if(pid == 0) {
        close(pipe_from_counter[write_index]);
        char* buf = new char[buf_size];
        for(int bytes_read = 0; bytes_read < buf_size;) {
            bytes_read += read(pipe_from_counter[read_index],
                               buf+bytes_read, buf_size-bytes_read);
        }
        close(pipe_from_counter[read_index]);
        Matrix parsed_result = Matrix((int*)buf);
        delete [] buf;
        ofstream out;
        out.open("output.txt");
        out << parsed_result;
        out.close();
        _exit(0);
    }

    close(pipe_from_counter[read_index]);
    Matrix result = read_matrix1 + read_matrix2;
    int* data = result.serialize();
    write(pipe_from_counter[write_index], (void*)data, buf_size);
    close(pipe_from_counter[write_index]);
    delete [] data;
    wait(&pid);
    cout << "Done. Check output.txt for results.\n";
    return 0;
}
