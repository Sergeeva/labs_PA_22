#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>     
#include <time.h>   
using namespace std;


void writeMatrix(ofstream& output, int height, int width); 

int main() {
    int height = 10;
    int width = 10;

    ofstream output("input.txt");
    vector<vector<int>> matrix;

    writeMatrix(output, height, width);
    writeMatrix(output, height, width);

    output.close();
    return 0;
}

void writeMatrix(ofstream& output, int height, int width) {

    srand (time(NULL));

    output << height << ' ' << width << '\n';
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            output << rand()%101;
            if (j < width - 1) {
                output << ' ';
            }
        }
        output << '\n';
    }
}