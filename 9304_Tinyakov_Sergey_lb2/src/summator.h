#ifndef SUMMATOR_H
#define SUMMATOR_H

#include <stdlib.h>

#include "data.h"
#include "matrix_operations.h"

void *summator_main_thread(void *arg);

void *summator_thread(void *arg);

#endif // SUMMATOR_H
