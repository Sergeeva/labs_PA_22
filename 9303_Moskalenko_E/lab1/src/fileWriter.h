//
// Created by Елизавета Москаленко on 01.10.2022.
//

#ifndef LAB1_PA_FILEWRITER_H
#define LAB1_PA_FILEWRITER_H

#include <string>
#include <vector>
#import <fstream>

using namespace std;

void writeToFile(const string &fileName, vector<vector<int>> &result, const string& message = "");

#endif //LAB1_PA_FILEWRITER_H