#include "args.h"

void parse_args(int c, char** v, int* params, int params_len,
                const char* warn_template) {
    for(int i = 1; i < min(c,params_len+1); ++i) {
        int val = atoi(v[i]);
        if(val < 1) {
           puts("Warning: non-positive argument. Using default.");
           continue;
        }
        if(val > (numeric_limits<int>::max() - 1)/2) {
            puts("Warning: number too large - possible overflow. Using default.");
            continue;
        }
        params[i-1] = val;
    }
    if(c < params_len+1) {
       puts("Info: not enough arguments. Using default values.");
       printf(warn_template, v[0]);
    }
    if(c > params_len+1) {
        puts("Warning: extra arguments ignored.");
        printf(warn_template, v[0]);
    }
}
