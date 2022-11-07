#include <thread>
#include <future>

int* multiplyShtrassen(int* first, int* second, int size, int depth);

int* divideIntoParts(int* matrix, int start, int size) {
  int* result = new int[size * size];
  int newSize = size / 2;
  for (int i = 0; i < newSize; i++) {
    for (int j = 0; j < newSize; j++) {
      result[j * newSize + i] = matrix[(start + i) + size * j];
    }
  }
  return result;
}

int* multiply_matrixes(int* first, int* second, int size) {
  int* result = new int[size * size];
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      int res = 0;
      for (int k = 0; k < size; k++) {
          res += first[i * size + k] * second[j + k * size];
      }
      result[i * size + j] = res;
    }
  }
  return result;
}

int* sum(int* first, int* second, int size) {
  int* result = new int[size];
  for (int i = 0; i < size; i++) {
    result[i] = first[i] + second[i];
  }
  return result;
}

int* substr(int* first, int* second, int size) {
  int* result = new int[size];
  for (int i = 0; i < size; i++) {
    result[i] = first[i] - second[i];
  }
  return result;
}

int* m1(int* first, int* second, int*third, int size, int depth) {
  int* summed = substr(second, third, size * size);
  return multiplyShtrassen(first, summed, size, depth + 1);
}

int* m2(int* first, int* second, int* third, int size, int depth) {
  int* summed = sum(first, second, size * size);
  return multiplyShtrassen(summed, third, size, depth + 1);
}

int* m5(int* first, int* second, int* third, int* forth, int size, int depth) {
  int* summed_1 = sum(first, second, size * size);
  int* summed_2 = sum(third, forth, size * size);
  return multiplyShtrassen(summed_1, summed_2, size, depth + 1);
}

int* m6(int* first, int* second, int* third, int* forth, int size, int depth) {
  int* summed_1 = substr(first, second, size * size);
  int* summed_2 = sum(third, forth, size * size);
  return multiplyShtrassen(summed_1, summed_2, size, depth + 1);
}

void merge(int* result, int* matrix, int start, int newSize) {
  int size = 2 * newSize;
  for (int i = 0; i < newSize; i++) {
    for (int j = 0; j < newSize; j++) {
      result[(start + i) + size * j] = matrix[j * newSize + i];
    }
  }
}


int* threadMultiplyShtrassen(int* first, int* second, int size, int depth, std::promise<int*>&& promise) {
  int newSize = size / 2;

  int* first_first = divideIntoParts(first, 0, size);
  int* first_second = divideIntoParts(second, 0, size);
  int* second_first = divideIntoParts(first, newSize, size);
  int* second_second = divideIntoParts(second, newSize, size);
  int* third_first = divideIntoParts(first, size * newSize, size);
  int* third_second = divideIntoParts(second, size * newSize, size);
  int* forth_first = divideIntoParts(first, (size + 1) * newSize , size);
  int* forth_second = divideIntoParts(second, (size + 1) * newSize, size);

  int* res_m1 = m1(first_first, second_second, forth_second, newSize, depth);
  int* res_m2 = m2(first_first, second_first, forth_second, newSize, depth);
  int* res_m3 = m2(third_first, forth_first, first_second, newSize, depth);
  int* res_m4 = m1(forth_first, third_second, first_second, newSize, depth);
  int* res_m5 = m5(first_first, forth_first, first_second, forth_second, newSize, depth);
  int* res_m6 = m6(second_first, forth_first, third_second, forth_second, newSize, depth);
  int* res_m7 = m6(first_first, third_first, first_second, second_second, newSize, depth);

  int* c_11_1 = sum(res_m5, res_m4, newSize * newSize);
  int* c_11_2 = substr(c_11_1, res_m2, newSize * newSize);
  int* c_11 = sum(c_11_2, res_m6, newSize * newSize);

  int* c_12 = sum(res_m1, res_m2, newSize * newSize);

  int* c_21 = sum(res_m3, res_m4, newSize * newSize);

  int* c_22_1 = sum(res_m5, res_m1, newSize * newSize);
  int* c_22_2 = sum(res_m3, res_m7, newSize * newSize);
  int* c_22 = substr(c_22_1, c_22_2, newSize * newSize);

  int* result = new int[size * size];
  merge(result, c_11, 0, newSize);
  merge(result, c_12, newSize, newSize);
  merge(result, c_21, size * newSize, newSize);
  merge(result, c_22, (size + 1) * newSize, newSize);
  promise.set_value(result);

  delete first_first;
  delete second_first;
  delete third_first;
  delete forth_first;
  delete first_second;
  delete second_second;
  delete third_second;
  delete forth_second;
  delete res_m1;
  delete res_m2;
  delete res_m3;
  delete res_m4;
  delete res_m5;
  delete res_m6;
  delete res_m7;
  delete c_11_1;
  delete c_11_2;
  delete c_11;
  delete c_12;
  delete c_21;
  delete c_22_1;
  delete c_22_2;
  delete c_22;

  return result;
}

int* multiplyShtrassen(int* first, int* second, int size = FIRST_ROWS, int depth = 1){
  if (size <= 64) {
    return multiply_matrixes(first, second, size);
  } else if (depth > 3) {
    std::promise<int*> promise_not_needed;
    return threadMultiplyShtrassen(first, second, size, depth, std::move(promise_not_needed));
  } else {
    std::promise<int*> promise;
    auto future = promise.get_future();
    std::thread newMult = std::thread{threadMultiplyShtrassen, first, second, size, depth, std::move(promise)};
    newMult.join();
    return future.get();
  }
}
