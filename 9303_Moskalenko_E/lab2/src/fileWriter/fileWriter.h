#ifndef LAB1_PA_FILEWRITER_H
#define LAB1_PA_FILEWRITER_H
#include <string>
#include <vector>
#include <fstream>

using namespace std;

void writeToFile(const string& fileName, vector<vector<int>> &result, const string& message);

#endif //LAB1_PA_FILEWRITER_H