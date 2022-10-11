#include <thread>
#include <fstream>
#include <vector>
#include <iostream>
#include <random>

typedef std::vector<std::vector<int>> Matrix;

void printMatrix(Matrix matrix) {
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[0].size(); ++j) {
            std::cout<<matrix[i][j]<<" ";
        }
        std::cout<<"\n";
    }
}

std::vector<std::vector<int>> generateMatrix(int m_width, int m_height) {
    std::vector<int> init_vec(m_height, 0);
    Matrix gen_matrix(m_width, init_vec);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(-500, 500);
    for (int i = 0; i < m_width; ++i) {
        for (int j = 0; j < m_height; ++j) {
            gen_matrix[i][j] = distr(gen);
        }
    }
    return gen_matrix;
}

void sumMatrix(Matrix &m1, Matrix &m2, Matrix &res, int elemStart, int size_of_one) {
    for (unsigned int elemIndex = elemStart; elemIndex < elemStart + size_of_one; elemIndex++) {
        int i = elemIndex / m1[0].size();
        int j = elemIndex % m1[0].size();
        res[i][j] = m1[i][j] + m2[i][j];
    }
}

std::vector<int> get_vector_of_elements(std::string str){

//    std::string::iterator it = std::remove(str.begin(), str.end(), '\n');
//    str.erase(it, str.end());
    std::vector<std::string> arr;
    std::string delim(" ");
    size_t prev = 0;
    size_t next;
    size_t delta = delim.length();

    while( ( next = str.find( delim, prev ) ) != std::string::npos ){
//Отладка-start
        std::string tmp = str.substr( prev, next-prev );
        std::cout << tmp << std::endl;
        //Отладка-end
        arr.push_back( str.substr( prev, next-prev ) );
        prev = next + delta;
    }
//Отладка-start
    std::string tmp = str.substr( prev );
    std::cout << tmp << std::endl;
    //Отладка-end
    arr.push_back( str.substr( prev ) );

    std::vector<int> res;
    for (int i = 0; i < arr.size(); ++i) {
        res[i] = std::stoi(arr[i]);
    }
    return res;
}

Matrix readMatrixFromFile(std::string path){
    Matrix matrix;
    std::ifstream input(path);
    std::string line;
    
    if (input.is_open())
    {
        int i = 0;
        while (getline(input, line)){
            i++;

            std::vector<int> matrix_line = get_vector_of_elements(line);

            for (int j = 0; j < matrix_line.size(); ++j) {
                matrix[i][j] = matrix_line[j];
            }
        }
    }
    input.close();


    return matrix;
}


void * read(Matrix & m1, Matrix & m2, int width, int height){
    char flag = 'n';
//    std::cout<<"\nDo you want to read from file? (y/n)\n";
//    std::cin>>flag;

    if (flag =='y'){
        m1 = readMatrixFromFile("m1.txt");
        m2 = readMatrixFromFile("m2.txt");

        printMatrix(m1);
        std::cout<<"\n123";
        printMatrix(m2);
        std::cout<<"\n";
    } else{
        m1 = generateMatrix(width, height);
        m2 = generateMatrix(width, height);
    }
    return nullptr;
}


Matrix sumMatrixMulti(Matrix & res, Matrix & m1, Matrix & m2, int count_threads) {

    int total_elems = m1.size() * m1[0].size();

    if (count_threads <= 0 || count_threads > total_elems) {
        std::cout << "Error: incorrect threads counter";
        exit(EXIT_FAILURE);
    }

    std::vector<std::thread> threads;
    threads.reserve(count_threads);

    int size_of_one = total_elems / count_threads;

    for (int thread_number = 0; thread_number < count_threads; thread_number++) {
        int elemStart = thread_number * size_of_one;

        if (thread_number == count_threads - 1) {
            size_of_one = total_elems - (size_of_one * thread_number);
        }

        std::thread sumMatrixThread(sumMatrix, std::ref(m1), std::ref(m2), std::ref(res), elemStart, size_of_one);
        threads.push_back(std::move(sumMatrixThread));
    }

    for (auto &thread : threads) {
        thread.join();
    }

    return res;
}


void * write(Matrix & m, std::string & file_name){

    std::ofstream out;
    out.open(file_name, std::fstream::out );

    for (int i = 0; i < m.size(); ++i) {
        for (int j = 0; j < m[i].size(); ++j) {
            out << m[i][j]<<" ";
        }
        out <<"\n";
    }
    out.close();
    return nullptr;
}

int main(){

    int m_width = 15;
    int m_height = 15;
    int threads_counter = 4;

    std::string file_name = "m3.txt";

    Matrix m1, m2;

    std::vector<int> init_vec(m_height, 0);
    Matrix res(m_width, init_vec);

    std::thread generateMatrixThread(read, std::ref(m1), std::ref(m2), m_width, m_height);
    generateMatrixThread.join();

    auto start = std::chrono::steady_clock::now();

    std::thread sumMatrixMultiThread(sumMatrixMulti, std::ref(res), std::ref(m1), std::ref(m2), std::ref(threads_counter));
    sumMatrixMultiThread.join();

    auto finish = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);

    std::cout << "Elapsed time: " << (double )duration.count()/ 1000000 <<"\n";


    std::thread writeMatrixThread(write, std::ref(res), std::ref(file_name));
    writeMatrixThread.join();

//    printMatrix(m1);
//    std::cout<<"\n";
//    printMatrix(m2);
//    std::cout<<"\n";
//    printMatrix(res);
}