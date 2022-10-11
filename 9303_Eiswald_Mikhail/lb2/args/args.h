#ifndef ARGS_H
#define ARGS_H

#include <limits>
#include <iostream>

using namespace std;

void parse_args(int c, char** v, int* params, int params_len, const char* warn_template);

#endif // ARGS_H
