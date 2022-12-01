#include <iostream>
#include <functional>

#include "buffer/BQueue.h"
#include "matrix/MatrixHandler.h"

void initialize(BQueue<std::pair<Matrix, Matrix>>& raw, int iterations,
          int rows, int columns)
{
    for (int i = 0; i < iterations; i++)
    {

        unsigned int seed = time(nullptr) + rand();
        std::cout << "Generated:" << seed << std::endl;

        Matrix a = MatrixHandler::create(rows, columns, seed);
        Matrix b = MatrixHandler::create(rows, columns, seed + rand());

        a.set_id(seed);
        b.set_id(seed);

        MatrixHandler::output(a, Config::Data_path, "A");
        MatrixHandler::output(b, Config::Data_path, "B");

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
        std::cout << "Summed:" << a.get_id() << std::endl;
        result.set_id(a.get_id());

        result_buffer.add(result);
    }
}

void output(BQueue<Matrix>& result_buffer, int iterations)
{
    for (int i = 0; i < iterations; i++)
    {

        Matrix result = result_buffer.get();

        MatrixHandler::output(result, Config::Data_path,  Config::summed);

    }
};

int main() {
    BQueue<std::pair<Matrix, Matrix>> ab;
    BQueue<Matrix> res_buff;

    int iter = Config::iterations;

    std::thread init = std::thread(initialize, std::ref(ab), iter, Config::R, Config::C);

    std::thread sum = std::thread(sum_matrices, std::ref(ab), std::ref(res_buff),
                                  iter, Config::summator_threads);

    std::thread out = std::thread(output, std::ref(res_buff), iter);

    init.join();
    sum.join();
    out.join();

    return 0;

}
