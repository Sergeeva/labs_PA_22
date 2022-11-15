#include <vector>
#include "../headers/Matrix.h"

namespace MatrixHelper {

    template<typename T>
    void add(std::vector<std::vector<T>> &first,
             std::vector<std::vector<T>> &second,
             std::vector<std::vector<T>> &result,
             int size) {
        for (auto i = 0; i < size; i++) {
            for (auto j = 0; j < size; j++) {
                result[i][j] = first[i][j] + second[i][j];
            }
        }
    }

    template<typename T>
    void add(std::vector<std::vector<T>> &first,
             Matrix &second,
             std::vector<std::vector<T>> &result,
             int size) {
        for (auto i = 0; i < size; i++) {
            for (auto j = 0; j < size; j++) {
                result[i][j] = first[i][j] + second.get(i, j);
            }
        }
    }

    template<typename T>
    void add(Matrix &first,
             Matrix &second,
             std::vector<std::vector<T>> &result,
             int size) {
        for (auto i = 0; i < size; i++) {
            for (auto j = 0; j < size; j++) {
                result[i][j] = first.get(i, j) + second.get(i, j);
            }
        }
    }

    template<typename T>
    void subtract(std::vector<std::vector<T>> &first,
                  std::vector<std::vector<T>> &second,
                  std::vector<std::vector<T>> &result,
                  int size) {
        for (auto i = 0; i < size; i++) {
            for (auto j = 0; j < size; j++) {
                result[i][j] = first[i][j] - second[i][j];
            }
        }
    }

    template<typename T>
    void subtract(std::vector<std::vector<T>> &first,
                  Matrix &second,
                  std::vector<std::vector<T>> &result,
                  int size) {
        for (auto i = 0; i < size; i++) {
            for (auto j = 0; j < size; j++) {
                result[i][j] = first[i][j] - second.get(i, j);
            }
        }
    }

}

