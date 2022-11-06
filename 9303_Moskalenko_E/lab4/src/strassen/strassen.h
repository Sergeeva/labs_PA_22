//
// Created by Елизавета Москаленко on 05.11.2022.
//

#ifndef LAB1_PA_STRASSEN_H
#define LAB1_PA_STRASSEN_H

#include "../matrixFunc/matrixFunc.h"
#include <thread>

using namespace std;

void makeTemps(vector<Matrix> &temps, vector<Matrix> &AMatrices, vector<Matrix> &BMatrices);

void makeSTemps(vector<Matrix> &temps, vector<Matrix> &AMatrices,
                vector<Matrix> &BMatrices, vector<Matrix> &sBlocks);

void STempsAdditions(vector<Matrix> &STemps, vector<Matrix> &T11_17);

void makeAB(vector<Matrix> &AMatrices, vector<Matrix> &BMatrices,
            Matrix &first, Matrix &second, int halfSize);

void makePartsOfC(vector<Matrix> &STemps, vector<Matrix> &T11_17, vector<Matrix> &partsOfC);

void fillResult(int halfSize, Matrix &result, vector<Matrix> &partsOfC);

void strassenMultiply(Matrix &first, Matrix &second, Matrix &result);

#endif //LAB1_PA_STRASSEN_H
