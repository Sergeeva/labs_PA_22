#include "fileWriter.h"
#include "../matrixFunc/matrixFunc.h"
#include <iostream>

mutex obj;

void writeToFile(const string &fileName, vector<vector<int>> &result, const string &message) {
    unique_lock<mutex> lock(obj);
    ofstream resultFile;
    resultFile.open("./" + fileName + ".txt");
    if (resultFile.is_open()) {
        if (!message.empty()) {
            resultFile << message;
        }
        for (auto &i: result) {
            resultFile << endl;
            for (int &j: i) {
                resultFile << j << " ";
            }
        }
        resultFile.close();
    }
    lock.unlock();
}
