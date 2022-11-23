#include <iostream>
#include <functional>

#include "buffer/BQueue.h"
#include "matrix/MatrixHandler.h"

void initialize(BQueue<std::pair<Matrix, Matrix>>& raw, int iterations,
          int rows, int columns)
{
    for (int i = 0; i < iterations; i++)
    {

//        unsigned int seed1 = time(nullptr);
//        unsigned int seed2 = seed1 + 7;

        unsigned int seed1 = 1;
        unsigned int seed2 = 2;

        Matrix a = MatrixHandler::create(rows, columns, seed1);
        Matrix b = MatrixHandler::create(rows, columns, seed2);

//        size_t id = std::hash<std::thread::id>{} (std::this_thread::get_id());
//
//        a.set_id(id);
//        b.set_id(id);

        MatrixHandler::output(a, std::to_string(a.get_id()), "A");
        MatrixHandler::output(b, std::to_string(b.get_id()), "B");

        raw.add(std::pair<Matrix, Matrix>(a, b));
    }
}

void sum_matrices(BQueue<std::pair<Matrix, Matrix>>& raw, BQueue<Matrix>& result_buffer,
                  int iterations, int threads)
{
    for (int i = 0; i < iterations; i++)
    {

        std::pair <Matrix, Matrix> matrices = raw.get();
        
        Matrix a = matrices.first;
        Matrix b = matrices.second;

        Matrix result =  MatrixHandler::parallel_sum(a, b, threads);
        result.set_id(a.get_id());

        result_buffer.add(result);
    }
}

void output(BQueue<Matrix>& result_buffer, int iterations)
{
    for (int i = 0; i < iterations; i++)
    {

        Matrix result = result_buffer.get();

        std::string path = std::to_string(result.get_id());

        MatrixHandler::output(result, path,  Config::summed);

        std::cout << result;

    }
};

int main() {
    BQueue<std::pair<Matrix, Matrix>> ab;
    BQueue<Matrix> res_buff;

    int iter = Config::iterations;

    std::thread init = std::thread(initialize, std::ref(ab), iter, Config::R, Config::C);

    std::thread sum = std::thread(sum_matrices, std::ref(ab), std::ref(res_buff),
                                  iter, Config::execution_threads);

    std::thread out = std::thread(output, std::ref(res_buff), iter);

    init.join();
    sum.join();
    out.join();

    return 0;

}
