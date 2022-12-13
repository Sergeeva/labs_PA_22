#include <random>

#include "./matrix/MatrixHandler.h"

#include "./buffers/Treiber_stack.h"
#include "./buffers/BQueue.h"

#include "./utilities/session_timer.h"


// Atomic id counter
std::atomic<int> count{};

int make_id() {
    return count++;
}

template <class Buff>
void initialize(Buff* raw, int iterations,
                int rows, int columns)
{
    for (int i = 0; i < iterations; i++)
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> dist(1.0, 10.0);

        int seed = int(dist(mt) * 100);

//        std::cout << "Generated:" << seed << std::endl;

        Matrix a = MatrixHandler::create(rows, columns, seed);
        Matrix b = MatrixHandler::create(rows, columns, seed + rand());

        int id = make_id();

        a.set_id(id);
        b.set_id(id);

        MatrixHandler::output(a, Config::Data_path, "A");
        MatrixHandler::output(b, Config::Data_path, "B");

        std::pair pr = std::make_pair(a, b);

        raw->push(pr);
    }
}

template <class Buff, class buff>
void sum_matrices(Buff* raw, buff* result_buffer,
                  int iterations, int threads)
{
    for (int i = 0; i < iterations; i++)
    {

        std::pair <Matrix, Matrix> matrices = raw->pop();

        Matrix a = matrices.first;
        Matrix b = matrices.second;

        Matrix result =  MatrixHandler::parallel_sum(a, b, threads);
//        std::cout << "Summed:" << a.get_id() << std::endl;
        result.set_id(a.get_id());

        result_buffer->push(result);
    }
}

template <class Buff>
void output(Buff* result_buffer, int iterations)
{
    for (int i = 0; i < iterations; i++)
    {

        Matrix result = result_buffer->pop();

        MatrixHandler::output(result, Config::Data_path,  Config::summed);

    }
};

template <class Buff, class buff>
long long int experiment() {
    Buff* raw = new Buff();
    buff* result = new buff();

    SessionTimer tm = SessionTimer();

    std::vector<std::thread> producers(Config::threads);
    std::vector<std::thread> consumers(Config::threads);

    tm.start_session();

    for (int i = 0; i < Config::threads; i ++) {
        producers[i] = std::thread(initialize<Buff>, raw, Config::iterations, Config::R, Config::C);

        consumers[i] = std::thread (sum_matrices<Buff, buff>,
                raw, result, Config::iterations, Config::summator_threads);
    }

    std::thread out (output<buff>, result, Config::iterations * Config::threads);

    for (int i = 0; i < Config::threads; i++) {
        producers[i].join();
        consumers[i].join();
    }

    tm.finish_session();

    out.join();

    delete raw;
    delete result;

    return tm.last();
}

void log_experiment(const std::string name, const std::string structure, long long int _duration) {
    std::cout << name << std::endl;
    std::cout << "{" + structure + "} ";
    std::cout << "(" + std::to_string(Config::threads) + ") threads, with [" + std::to_string(Config::iterations) +
                 "] iterations each, finished within: " + std::to_string(_duration) + "ms" << std::endl << std::endl;
}

int main() {

    Config::log_config();

    log_experiment("Lab 2. Fat-lock data-structure:", "Blocking queue",
                   experiment<BQueue<std::pair<Matrix, Matrix>>,
                           BQueue<Matrix>>());

    log_experiment("Lab 3. Lock-free data-structure:", "Traiber stack",
                   experiment<TRStack<std::pair<Matrix, Matrix>>,
                           TRStack<Matrix>>());

    return 0;
}
