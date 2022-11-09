#ifndef UNTITLED_STRASSENMATRICESMULTIPLIER_H
#define UNTITLED_STRASSENMATRICESMULTIPLIER_H


#include "MatricesMultiplier.h"
#include <future>
#include <utility>

class StrassenMatricesMultiplier : public MatricesMultiplier {

    class SimplifiedCalculatingTask : public Runnable {

    private:
        MatricesHolder *target;
        std::function<void()> *previous_level_group_action;
        Matrix *result;

    public:
        SimplifiedCalculatingTask(MatricesHolder *target,
                                  std::function<void()> *action,
                                  Matrix *result) : target(target),
                                                    previous_level_group_action(action),
                                                    result(result) {}

        void run() override;

        ~SimplifiedCalculatingTask() override;

    };

    class StrassenCalculatingTask : public Runnable {

    private:
        int p_size;
        int new_size;
        bool keep_target;
        MatricesHolder *target;
        SynchronizedQueue<Runnable> *task_consumer;
        StrassenMatricesMultiplier *multiplier;
        std::function<void()> *previous_level_group_action;
        std::vector<Matrix *> *p_matrices;
        Matrix *result;
        std::atomic<bool> *action_not_called;

        std::function<void()> *get_group_action();

    public:
        StrassenCalculatingTask(
                MatricesHolder *target,
                int new_size,
                SynchronizedQueue<Runnable> *task_consumer,
                std::function<void()> *action,
                Matrix *result,
                bool keep_target = false) : p_size(7),
                                            new_size(new_size),
                                            target(target),
                                            action_not_called(new std::atomic<bool>(true)),
                                            task_consumer(task_consumer),
                                            multiplier(new StrassenMatricesMultiplier()),
                                            previous_level_group_action(action),
                                            result(result),
                                            p_matrices(new std::vector<Matrix *>(std::vector<Matrix *>(p_size))),
                                            keep_target(keep_target) {
            p_matrices->reserve(p_size);

            for (auto i = 0; i < p_size; i++) {
                (*p_matrices)[i] = new Matrix(new_size, new_size);
            }
        }

        void run() override;

        ~StrassenCalculatingTask() override;

    };

    class GroupingTask : public Runnable {

    private:
        int new_size;
        Matrix *result;
        std::function<void()> *action;
        std::vector<Matrix *> *p_matrices;

    public:
        GroupingTask(int new_size,
                     Matrix *result,
                     std::function<void()> *action,
                     std::vector<Matrix *> *p_matrices) : new_size(new_size),
                                                          result(result),
                                                          action(action),
                                                          p_matrices(p_matrices) {}

        void run() override;

        ~GroupingTask() override;

    };

    void recursive_walk(MatricesHolder *matrices, Matrix *result, SynchronizedQueue<Runnable> *task_consumer,
                        std::function<void()> *action);

public:

    std::promise<Matrix *> *multiply(MatricesHolder *matrices, SynchronizedQueue<Runnable> *task_consumer) override;

};


#endif //UNTITLED_STRASSENMATRICESMULTIPLIER_H
