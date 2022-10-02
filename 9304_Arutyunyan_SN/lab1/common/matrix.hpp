#pragma once

#include <vector>
#include <utility>

using Row = std::vector<int>;
using Matrix = std::vector<Row>;
using MatrixPair = std::pair<Matrix, Matrix>;
using MatrixValueType = Row::value_type;
