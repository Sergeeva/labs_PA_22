#pragma once

#include <vector>
#include <utility>
#include <memory>

using Row = std::vector<int>;
using Matrix = std::vector<Row>;
using MatrixPair = std::pair<Matrix, Matrix>;
using MatrixValueType = Row::value_type;

using MatrixPtr = std::shared_ptr<Matrix>;
using MatrixPtrPair = std::pair<MatrixPtr, MatrixPtr>;
