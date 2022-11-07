#include "definitions.h"
#include "readingprocess.h"
#include "calculationprocess.h"
#include "writingprocess.h"

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        std::cout << "Usage:"
            << "\tprocesses <input_file_1> <input_file_2>"
            << "\tprocesses <input_file_1> <input_file_2> <output_file>"
            << "\n";
        return -1;
    }

    pid_t processReadID, processCalculateID, processWriteID;
    ReadingProcess *readingProcess;
    CalculationProcess *calculationProcess;
    WritingProcess *writingProcess;

    processReadID = fork();
    switch (processReadID) {
        case -1:
            std::cerr << "Error: could not create read process\n";
            return -1;
        case 0:
            processCalculateID = fork();
            switch (processCalculateID) {
                case -1:
                    std::cerr << "Error: could not create read process\n";
                    return -1;
                case 0:
                    processWriteID = fork();
                    switch (processWriteID) {
                        case -1:
                            std::cerr << "Error: could not create read process\n";
                            return -1;
                        case 0:
                            writingProcess = new WritingProcess(argc == 4 ? argv[3] : nullptr);
                            writingProcess->run();
                            delete writingProcess;
                            break;
                        default:
                            calculationProcess = new CalculationProcess();
                            calculationProcess->run();
                            wait(&processWriteID);
                            delete calculationProcess;
                    }
                    break;
                default:
                    readingProcess = new ReadingProcess(argv[1], argv[2]);
                    readingProcess->run();
                    wait(&processCalculateID);
                    delete readingProcess;
            }
            break;
        default:
            wait(&processReadID);
    }
    return 0;
}