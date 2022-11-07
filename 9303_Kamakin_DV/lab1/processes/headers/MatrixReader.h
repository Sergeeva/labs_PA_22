#ifndef UNTITLED_MATRIXREADER_H
#define UNTITLED_MATRIXREADER_H

#include <spdlog/spdlog.h>
#include "Matrix.h"

class MatrixReader {

public:

    template<class Supplier>
    Matrix read_from(const Supplier &value_supplier) {
        int height = value_supplier();
        int width = value_supplier();

        spdlog::info("read_from: reading height {}, width {}", height, width);

        auto matrix = Matrix(height, width);

        for (auto y = 0; y < height; y++) {
            for (auto x = 0; x < width; x++) {
                matrix.set(y, x, value_supplier());
            }
        }

        return matrix;
    }

};


#endif //UNTITLED_MATRIXREADER_H
