#include "strassen.h"
#include "../threads/threads.h"

void makeTemps(vector<Matrix> &temps, vector<Matrix> &AMatrices, vector<Matrix> &BMatrices) {
    makeAddition(AMatrices[0], AMatrices[3], temps[0]); // T1
    makeAddition(BMatrices[0], BMatrices[3], temps[1]);   // T2
    makeAddition(AMatrices[2], AMatrices[3], temps[2]);
    subtract(BMatrices[1], BMatrices[3], temps[3]);   // T4
    subtract(BMatrices[2], BMatrices[0], temps[4]);   // T5
    makeAddition(AMatrices[0], AMatrices[1], temps[5]);   // T6
    subtract(AMatrices[2], AMatrices[0], temps[6]);   // T7
    makeAddition(BMatrices[0], BMatrices[1], temps[7]);   // T8
    subtract(AMatrices[1], AMatrices[3], temps[8]);   // T9
    makeAddition(BMatrices[2], BMatrices[3], temps[9]);
}


void makeSTemps(vector<Matrix> &temps, vector<Matrix> &AMatrices,
                vector<Matrix> &BMatrices, vector<Matrix> &sBlocks, int size) {
    if (size * 4 >= N) {
        std::vector<std::thread> vecOfThreads;
        vecOfThreads.emplace_back(
                std::thread(strassenMultiply, std::ref(temps[0]), std::ref(temps[1]), std::ref(sBlocks[0])));
        vecOfThreads.emplace_back(
                std::thread(strassenMultiply, std::ref(temps[2]), std::ref(BMatrices[0]), std::ref(sBlocks[1]))); // S2
        vecOfThreads.emplace_back(
                std::thread(strassenMultiply, std::ref(AMatrices[0]), std::ref(temps[3]), std::ref(sBlocks[2])));
        vecOfThreads.emplace_back(
                std::thread(strassenMultiply, std::ref(AMatrices[3]), std::ref(temps[4]), std::ref(sBlocks[3])));
        vecOfThreads.emplace_back(
                std::thread(strassenMultiply, std::ref(temps[5]), std::ref(BMatrices[3]), std::ref(sBlocks[4])));
        vecOfThreads.emplace_back(
                std::thread(strassenMultiply, std::ref(temps[6]), std::ref(temps[7]), std::ref(sBlocks[5])));
        vecOfThreads.emplace_back(
                std::thread(strassenMultiply, std::ref(temps[8]), std::ref(temps[9]), std::ref(sBlocks[6])));
        for (auto &vecOfThread: vecOfThreads) {
            vecOfThread.join();
        }
    } else {
        strassenMultiply(temps[0], temps[1], sBlocks[0]);
        strassenMultiply(temps[2], BMatrices[0], sBlocks[1]); // S2
        strassenMultiply(AMatrices[0], temps[3], sBlocks[2]);
        strassenMultiply(AMatrices[3], temps[4], sBlocks[3]);
        strassenMultiply(temps[5], BMatrices[3], sBlocks[4]);
        strassenMultiply(temps[6], temps[7], sBlocks[5]);
        strassenMultiply(temps[8], temps[9], sBlocks[6]);
    }
}

void STempsAdditions(vector<Matrix> &STemps, vector<Matrix> &T11_17) {
    makeAddition(STemps[0], STemps[3], T11_17[0]);
    makeAddition(STemps[2], STemps[5], T11_17[1]);
    subtract(STemps[6], STemps[4], T11_17[2]);
    subtract(STemps[0], STemps[1], T11_17[3]);
}

void makeAB(vector<Matrix> &AMatrices, vector<Matrix> &BMatrices,
            Matrix &first, Matrix &second, int halfSize) {
    for (int i = 0; i < halfSize; i++)
        for (int j = 0; j < halfSize; j++) {
            AMatrices[0][i][j] = first[i][j];
            AMatrices[1][i][j] = first[i][halfSize + j];
            AMatrices[2][i][j] = first[halfSize + i][j];
            AMatrices[3][i][j] = first[halfSize + i][halfSize + j];
            BMatrices[0][i][j] = second[i][j];
            BMatrices[1][i][j] = second[i][halfSize + j];
            BMatrices[2][i][j] = second[halfSize + i][j];
            BMatrices[3][i][j] = second[halfSize + i][halfSize + j];
        }
}

void makePartsOfC(vector<Matrix> &STemps, vector<Matrix> &T11_17, vector<Matrix> &partsOfC) {
    makeAddition(STemps[2], STemps[4], partsOfC[1]);
    makeAddition(STemps[1], STemps[3], partsOfC[2]);
    makeAddition(T11_17[0], T11_17[2], partsOfC[0]);
    makeAddition(T11_17[1], T11_17[3], partsOfC[3]);
}

void fillResult(int halfSize, Matrix &result, vector<Matrix> &partsOfC) {
    for (int i = 0; i < halfSize; i++)
        for (int j = 0; j < halfSize; j++) {
            result[i][j] = partsOfC[0][i][j];
            result[i][j + halfSize] = partsOfC[1][i][j];
            result[halfSize + i][j] = partsOfC[2][i][j];
            result[halfSize + i][halfSize + j] = partsOfC[3][i][j];
        }
}

void strassenMultiply(Matrix &first, Matrix &second, Matrix &result) {
    int n = result.size();
    if (n == 1) {
        result[0][0] = first[0][0] * second[0][0];
        return;
    }
    int halfSize = n / 2;
    if (halfSize < 64) {
        tileMatrixMultiplication(first, second, result);
        return;
    }
    vector<Matrix> AMatrices(4, Matrix(halfSize, vector<int>(halfSize, 0)));
    vector<Matrix> BMatrices(4, Matrix(halfSize, vector<int>(halfSize, 0)));
    makeAB(AMatrices, BMatrices, first, second, halfSize);

    vector<Matrix> TTemps(10, Matrix(halfSize, vector<int>(halfSize, 0)));
    makeTemps(TTemps, AMatrices, BMatrices);

    vector<Matrix> STemps(7, Matrix(halfSize, vector<int>(halfSize, 0)));
    makeSTemps(TTemps, AMatrices, BMatrices, STemps, halfSize);

    vector<Matrix> T11_17(4, Matrix(halfSize, vector<int>(halfSize, 0)));
    STempsAdditions(STemps, T11_17);

    vector<Matrix> partsOfC(4, Matrix(halfSize, vector<int>(halfSize, 0)));
    makePartsOfC(STemps, T11_17, partsOfC);
    fillResult(halfSize, result, partsOfC);
}