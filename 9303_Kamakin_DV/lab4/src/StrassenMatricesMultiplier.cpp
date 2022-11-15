#include "../headers/StrassenMatricesMultiplier.h"
#include "./MatrixHelper.cpp"

void StrassenMatricesMultiplier::SimplifiedCalculatingTask::run() {
    auto first = target->get_first();
    auto second = target->get_second();

    for (int i = 0; i < first.get_height(); i++) {
        std::vector<int> resultRow(first.get_height(), 0);

        for (int j = 0; j < first.get_width(); j++) {
            for (int k = 0; k < first.get_width(); k++) {
                resultRow[j] += first.get(i, k) * second.get(k, j);
            }
        }

        result->set(i, resultRow);
    }

    result->mark_changed();

    (*previous_level_group_action)();
}

StrassenMatricesMultiplier::SimplifiedCalculatingTask::~SimplifiedCalculatingTask() {
    delete target;
}

std::function<void()> *StrassenMatricesMultiplier::StrassenCalculatingTask::get_group_action() {
    return new std::function<void()>([new_size = new_size, result = result, action = previous_level_group_action,
                                             p_matrices = p_matrices, task_consumer = task_consumer, p_size = p_size,
                                             action_not_called = action_not_called]() {
        if (!action_not_called->load()) {
            return;
        }

        for (int i = 0; i < p_size; i++) {
            if ((*p_matrices)[i]->is_not_changed()) {
                return;
            }
        }

        auto not_called = true;

        if (action_not_called->compare_exchange_weak(not_called, false)) {
            task_consumer->push(new GroupingTask(new_size, result, action, p_matrices));
        }
    });
}

void StrassenMatricesMultiplier::StrassenCalculatingTask::run() {
    new_size /= 2;

    std::vector<int> zeroes(new_size);
    std::vector<std::vector<int>>
            a12(new_size, zeroes), a21(new_size, zeroes),
            b12(new_size, zeroes), b21(new_size, zeroes);

    auto b11 = new std::vector<std::vector<int>>(new_size, zeroes);
    auto a11 = new std::vector<std::vector<int>>(new_size, zeroes);
    auto a22 = new std::vector<std::vector<int>>(new_size, zeroes);
    auto b22 = new std::vector<std::vector<int>>(new_size, zeroes);

    auto first = target->get_first();
    auto second = target->get_second();

    for (auto i = 0; i < new_size; i++) {
        for (auto j = 0; j < new_size; j++) {
            (*a11)[i][j] = first.get(i, j);
            a12[i][j] = first.get(i, j + new_size);
            a21[i][j] = first.get(i + new_size, j);
            (*a22)[i][j] = first.get(i + new_size, j + new_size);

            (*b11)[i][j] = second.get(i, j);
            b12[i][j] = second.get(i, j + new_size);
            b21[i][j] = second.get(i + new_size, j);
            (*b22)[i][j] = second.get(i + new_size, j + new_size);
        }
    }

    auto group_action = get_group_action();

    // p1:
    auto a_calculated_for_p1 = new std::vector<std::vector<int>>(new_size, zeroes);
    auto b_calculated_for_p1 = new std::vector<std::vector<int>>(new_size, zeroes);

    MatrixHelper::add(*a11, *a22, *a_calculated_for_p1, new_size); // a11 + a22
    MatrixHelper::add(*b11, *b22, *b_calculated_for_p1, new_size); // b11 + b22

    multiplier->recursive_walk(new MatricesHolder(a_calculated_for_p1, b_calculated_for_p1, new_size),
                               (*p_matrices)[0],
                               task_consumer,
                               group_action); // p1 = (a11+a22) * (b11+b22)

    // p2:
    auto a_calculated_for_p2 = new std::vector<std::vector<int>>(new_size, zeroes);

    MatrixHelper::add(a21, *a22, *a_calculated_for_p2, new_size); // a21 + a22

    multiplier->recursive_walk(new MatricesHolder(a_calculated_for_p2, b11, new_size),
                               (*p_matrices)[1],
                               task_consumer,
                               group_action); // p2 = (a21+a22) * (b11)

    // p3:
    auto b_calculated_for_p3 = new std::vector<std::vector<int>>(new_size, zeroes);

    MatrixHelper::subtract(b12, *b22, *b_calculated_for_p3, new_size);      // b12 - b22

    multiplier->recursive_walk(new MatricesHolder(a11, b_calculated_for_p3, new_size),
                               (*p_matrices)[2],
                               task_consumer,
                               group_action); // p3 = (a11) * (b12 - b22)

    // p4:
    auto b_calculated_for_p4 = new std::vector<std::vector<int>>(new_size, zeroes);

    MatrixHelper::subtract(b21, *b11, *b_calculated_for_p4, new_size);       // b21 - b11

    multiplier->recursive_walk(new MatricesHolder(a22, b_calculated_for_p4, new_size),
                               (*p_matrices)[3],
                               task_consumer,
                               group_action); // p4 = (a22) * (b21 - b11)

    // p5:
    auto a_calculated_for_p5 = new std::vector<std::vector<int>>(new_size, zeroes);

    MatrixHelper::add(*a11, a12, *a_calculated_for_p5, new_size);  // a11 + a12

    auto b22Matrix = Matrix(b22, new_size);

    multiplier->recursive_walk(new MatricesHolder(a_calculated_for_p5, b22, new_size),
                               (*p_matrices)[4],
                               task_consumer,
                               group_action); // p5 = (a11+a12) * (b22)

    // p6:
    auto a_calculated_for_p6 = new std::vector<std::vector<int>>(new_size, zeroes);
    auto b_calculated_for_p6 = new std::vector<std::vector<int>>(new_size, zeroes);

    MatrixHelper::subtract(a21, *a11, *a_calculated_for_p6, new_size); // a21 - a11
    MatrixHelper::add(*b11, b12, *b_calculated_for_p6, new_size); // b11 + b12

    multiplier->recursive_walk(new MatricesHolder(a_calculated_for_p6, b_calculated_for_p6, new_size),
                               (*p_matrices)[5],
                               task_consumer,
                               group_action);    // p6 = (a21-a11) * (b11+b12)

    // p7:
    auto a_calculated_for_p7 = new std::vector<std::vector<int>>(new_size, zeroes);
    auto b_calculated_for_p7 = new std::vector<std::vector<int>>(new_size, zeroes);

    MatrixHelper::subtract(a12, *a22, *a_calculated_for_p7, new_size); // a12 - a22
    MatrixHelper::add(b21, *b22, *b_calculated_for_p7, new_size);     // b21 + b22

    multiplier->recursive_walk(new MatricesHolder(a_calculated_for_p7, b_calculated_for_p7, new_size),
                               (*p_matrices)[6],
                               task_consumer,
                               group_action);    // p7 = (a12-a22) * (b21+b22)

    (*group_action)();

    delete b11;
    delete a11;
    delete a22;
    delete b22;
}

StrassenMatricesMultiplier::StrassenCalculatingTask::~StrassenCalculatingTask() {
    if (!keep_target) {
        delete target;
    }

    delete multiplier;
}

void StrassenMatricesMultiplier::GroupingTask::run() {
    std::vector<int> zeroes(new_size);
    std::vector<std::vector<int>>
            c11(new_size, zeroes), c12(new_size, zeroes), c21(new_size, zeroes),
            c22(new_size, zeroes), a_result(new_size, zeroes), b_result(new_size, zeroes);

    MatrixHelper::add(*(*p_matrices)[2], *(*p_matrices)[4], c12, new_size); // c12 = p3 + p5
    MatrixHelper::add(*(*p_matrices)[1], *(*p_matrices)[3], c21, new_size); // c21 = p2 + p4

    MatrixHelper::add(*(*p_matrices)[0], *(*p_matrices)[3], a_result, new_size); // p1 + p4
    MatrixHelper::add(a_result, *(*p_matrices)[6], b_result, new_size);  // p1 + p4 + p7
    MatrixHelper::subtract(b_result, *(*p_matrices)[4], c11, new_size); // c11 = p1 + p4 - p5 + p7

    MatrixHelper::add(*(*p_matrices)[0], *(*p_matrices)[2], a_result, new_size); // p1 + p3
    MatrixHelper::add(a_result, *(*p_matrices)[5], b_result, new_size);  // p1 + p3 + p6
    MatrixHelper::subtract(b_result, *(*p_matrices)[1], c22, new_size); // c22 = p1 + p3 - p2 + p6

    for (auto i = 0; i < new_size; i++) {
        for (auto j = 0; j < new_size; j++) {
            result->set(i, j, c11[i][j]);
            result->set(i, j + new_size, c12[i][j]);
            result->set(i + new_size, j, c21[i][j]);
            result->set(i + new_size, j + new_size, c22[i][j]);
        }
    }

    result->mark_changed();

    (*action)();
}

StrassenMatricesMultiplier::GroupingTask::~GroupingTask() {
    delete p_matrices;
}

void StrassenMatricesMultiplier::recursive_walk(MatricesHolder *matrices,
                                                Matrix *result,
                                                SynchronizedQueue<Runnable> *task_consumer,
                                                std::function<void()> *action) {
    auto first = matrices->get_first();
    auto size = first.get_width();

    if (size == 1) {
        result->set(0, 0, first.get(0, 0) * matrices->get_second().get(0, 0));
        result->mark_changed();
    } else if (size < 64) {
        task_consumer->push(new SimplifiedCalculatingTask(matrices, action, result));
    } else {
        task_consumer->push(new StrassenCalculatingTask(matrices, size, task_consumer, action, result));
    }
}

std::promise<Matrix *> *StrassenMatricesMultiplier::multiply(MatricesHolder *matrices,
                                                             SynchronizedQueue<Runnable> *task_consumer) {
    auto promise = new std::promise<Matrix *>();
    auto is_result_not_ready = new std::atomic<bool>(true);

    auto size = matrices->get_first().get_width();
    auto result = new Matrix(size, size);

    task_consumer->push(new StrassenCalculatingTask(matrices,
                                                    size,
                                                    task_consumer,
                                                    new std::function<void()>([result, promise, is_result_not_ready]() {
                                                        if (result->is_changed() && is_result_not_ready->load()) {
                                                            promise->set_value(result);
                                                            is_result_not_ready->store(false);
                                                        }
                                                    }),
                                                    result,
                                                    true));

    return promise;
}
