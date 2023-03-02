#include <iostream>
#include "process.h"
#include "thread.h"

#define M 15000
#define N 15000
#define T 6

int main(int argc, char** argv){

    if(argc > 2){
        std::cerr << "Too much arguments\n";
        std::exit(-1);
    }

    if(argv[1][0] == '-' && argv[1][1] == 'p')
        startProcessMode(M, N);
    else if(argv[1][0] == '-' && argv[1][1] == 't')
        startThreadMode(M, N, T);
    else{
        std::cerr << "Undefined program parameters\n";
        std::exit(-1);
    }
}