#include "../common/matrix.hpp"
#include <type_traits>
#include <thread>
#include <cassert>
#include <future>

constexpr std::size_t STRASSEN_MIN_SIZE = 128;
constexpr std::size_t MAX_STRASSEN_DEPTH = 1;
constexpr std::size_t THREADS_COUNT = 4;

template <typename T, typename U = T, typename R = typename std::common_type<T, U>::type>
Matrix<R> StrassenMultiply(const Matrix<T>& left, const Matrix<U>& right, std::size_t current_depth);

template <typename T, typename U = T, typename R = typename std::common_type<T, U>::type>
void MultiplyPart(int start, int end, const Matrix<T>& left, const Matrix<U>& right, Matrix<R>& result) {
    for (int op = start; op < end; ++op) {
        auto row = op % result.height;
        auto column = op / result.width;
        auto result_element = 0;

        for (std::size_t i = 0; i < std::min(result.height, result.width); ++i) {
            auto first = left[row][i];
            auto second = right[i][column];
            result_element += first * second;
        }

        result[row][column] = result_element;
    }
}

template <typename T, typename U = T, typename R = typename std::common_type<T, U>::type>
Matrix<R> ParallelMultiply(const Matrix<T>& left, const Matrix<U>& right) {
    assert(left.width == right.height);

    auto result = Matrix<R>(left.height, right.width);
    std::vector<std::thread> threads;

    auto elements_count = left.height * left.width;
    auto operations_count = elements_count / THREADS_COUNT;
    auto rest_operations = elements_count % THREADS_COUNT;

    for (std::size_t i = 0; i < THREADS_COUNT; i++) {
        int start;
        int end;

        if (i == 0) {
            start = operations_count * i;
            end = (operations_count * (i + 1)) + rest_operations;
        } else {
            start = operations_count * i + rest_operations;
            end = (operations_count * (i + 1)) + rest_operations;
        }

        std::thread thread(MultiplyPart<T, U, R>, start, end, std::cref(left), std::cref(right), std::ref(result));
        threads.emplace_back(std::move(thread));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return result;
}

template <typename T>
Matrix<T> CreateMatrixFromBlocks(const Matrix<T> C11, const Matrix<T> C12,
                                 const Matrix<T> C21, const Matrix<T> C22,
                                 std::size_t n) {
    Matrix<T> result(n, n);
    std::size_t mid = n / 2;

    for (std::size_t x = 0; x < mid; ++x) {
        for (std::size_t y = 0; y < mid; ++y) {
            result[y][x] = C11[y][x];
        }
    }
    for (std::size_t x = mid; x < n; ++x) {
        for (std::size_t y = 0; y < mid; ++y) {
            result[y][x] = C12[y][x - mid];
        }
    }
    for (std::size_t x = 0; x < mid; ++x) {
        for (std::size_t y = mid; y < n; ++y) {
            result[y][x] = C21[y - mid][x];
        }
    }
    for (std::size_t x = mid; x < n; ++x) {
        for (std::size_t y = mid; y < n; ++y) {
            result[y][x] = C22[y - mid][x - mid];
        }
    }

    return result;
}

template <typename T, typename U = T, typename R = typename std::common_type<T, U>::type>
Matrix<R> StrassenParallelMultiplyPart(const Matrix<T>& left, const Matrix<U>& right, std::size_t current_depth) {
    std::size_t n = left.width;
    std::size_t mid = n / 2; // у нас N == M

    auto A11 = left.GetBlock(0, 0, mid, mid);
    auto A12 = left.GetBlock(mid, 0, n, mid);
    auto A21 = left.GetBlock(0, mid, mid, n);
    auto A22 = left.GetBlock(mid, mid, n, n);

    auto B11 = right.GetBlock(0, 0, mid, mid);
    auto B12 = right.GetBlock(mid, 0, n, mid);
    auto B21 = right.GetBlock(0, mid, mid, n);
    auto B22 = right.GetBlock(mid, mid, n, n);

    // std::cout << "strassen multiply... " << n << " " << mid << std::endl;
    auto future_P1 = std::async(StrassenMultiply<R>, A11 + A22, B11 + B22, current_depth + 1);
    auto future_P2 = std::async(StrassenMultiply<R>, A21 + A22, B11.GetMatrix(), current_depth + 1);
    auto future_P3 = std::async(StrassenMultiply<R>, A11.GetMatrix(), B12 - B22, current_depth + 1);
    auto future_P4 = std::async(StrassenMultiply<R>, A22.GetMatrix(), B21 - B11, current_depth + 1);
    auto future_P5 = std::async(StrassenMultiply<R>, A11 + A12, B22.GetMatrix(), current_depth + 1);
    auto future_P6 = std::async(StrassenMultiply<R>, A21 - A11, B11 + B12, current_depth + 1);
    auto future_P7 = std::async(StrassenMultiply<R>, A12 - A22, B21 + B22, current_depth + 1);

    auto P1 = future_P1.get();
    auto P2 = future_P2.get();
    auto P3 = future_P3.get();
    auto P4 = future_P4.get();
    auto P5 = future_P5.get();
    auto P6 = future_P6.get();
    auto P7 = future_P7.get();

    return CreateMatrixFromBlocks(
        BaseSumAllMatrixes<R>(mid, SignedMatrix<R>(P1), SignedMatrix<R>(P4), SignedMatrix<R>(P5, -1), SignedMatrix<R>(P7)),
        BaseSumAllMatrixes<R>(mid, SignedMatrix<R>(P3), SignedMatrix<R>(P5)),
        BaseSumAllMatrixes<R>(mid, SignedMatrix<R>(P2), SignedMatrix<R>(P4)),
        BaseSumAllMatrixes<R>(mid, SignedMatrix<R>(P1), SignedMatrix<R>(P2, -1), SignedMatrix<R>(P3), SignedMatrix<R>(P6)),
        n
    );
}

template <typename T, typename U = T, typename R = typename std::common_type<T, U>::type>
Matrix<R> StrassenMultiplyPart(const Matrix<T>& left, const Matrix<U>& right, std::size_t current_depth) {
    std::size_t n = left.width;
    std::size_t mid = n / 2; // у нас N == M
    
    auto A11 = left.GetBlock(0, 0, mid, mid);
    auto A12 = left.GetBlock(mid, 0, n, mid);
    auto A21 = left.GetBlock(0, mid, mid, n);
    auto A22 = left.GetBlock(mid, mid, n, n);

    auto B11 = right.GetBlock(0, 0, mid, mid);
    auto B12 = right.GetBlock(mid, 0, n, mid);
    auto B21 = right.GetBlock(0, mid, mid, n);
    auto B22 = right.GetBlock(mid, mid, n, n);

    auto P1 = StrassenMultiply(A11 + A22, B11 + B22, current_depth + 1);
    auto P2 = StrassenMultiply(A21 + A22, B11.GetMatrix(), current_depth + 1);
    auto P3 = StrassenMultiply(A11.GetMatrix(), B12 - B22, current_depth + 1);
    auto P4 = StrassenMultiply(A22.GetMatrix(), B21 - B11, current_depth + 1);
    auto P5 = StrassenMultiply(A11 + A12, B22.GetMatrix(), current_depth + 1);
    auto P6 = StrassenMultiply(A21 - A11, B11 + B12, current_depth + 1);
    auto P7 = StrassenMultiply(A12 - A22, B21 + B22, current_depth + 1);

    return CreateMatrixFromBlocks(
        P1 + P4 - P5 + P7,
        P3 + P5,
        P2 + P4,
        P1 - P2 + P3 + P6,
        n
    );
}

template <typename T, typename U = T, typename R = typename std::common_type<T, U>::type>
Matrix<R> StrassenMultiply(const Matrix<T>& left, const Matrix<U>& right, std::size_t current_depth) {
    assert(left.width == right.height);
    assert((left.width & (left.width - 1)) == 0);

    if (left.width <= STRASSEN_MIN_SIZE) {
        return ParallelMultiply(left, right);
    } else if (current_depth < MAX_STRASSEN_DEPTH) {
        return StrassenParallelMultiplyPart(left, right, current_depth);
    }

    return StrassenMultiplyPart(left, right, current_depth);
}