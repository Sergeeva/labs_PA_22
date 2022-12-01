#include "Task1.h"
using namespace std;
int main()
 {
    StartProcess(MatrixRead);
    StartProcess(MatrixSum);
    StartProcess(MatrixWrite);

    return 0;
}

void StartProcess(const function<void()> &go) 
{

// pid - определяет идентификатор процесса 
    auto pid = fork();

    switch (pid) {
        case 0:
            go();
            exit(0);

        default: // parent
            wait(&pid);
    }
}

void MatrixRead()
{//читаем из файла в память

    ifstream inputMatrix("input.txt"); //файловый поток

    auto Matrix1 = Matrix();
    auto Matrix2 = Matrix();

    inputMatrix >> Matrix1;//считали матрицу1
    inputMatrix >> Matrix2;//считали матрицу2

    //Матрица загружена

    inputMatrix.close();
    //файл закрыли


//     cout<<"прочитали из файла"<<endl;
//     cout<<Matrix1<<endl;
//     cout<<"Matrix1"<<endl;
//     cout<<Matrix2<<endl;

    auto memShared = (int *)getSharedPointer();

    auto pointerAfterMatrix1 = Matrix1.writeByPtr(memShared);
    Matrix2.writeByPtr(pointerAfterMatrix1);

}


void MatrixSum() 
{

    auto memShared = (int *) getSharedPointer();

    auto nextPointer1 = Matrix::readByPtr(memShared);
    auto const &Matrix1 = nextPointer1.first;

    auto nextPointer2 = Matrix::readByPtr(nextPointer1.second);
    auto const &Matrix2 = nextPointer2.first;

    auto summedMatrix = Matrix1 + Matrix2;
    summedMatrix.writeByPtr(memShared);


//cout<<"прочитали из памяти"<<endl;

//cout<<Matrix1<<endl;
//cout<<"Matrix1"<<endl;
//cout<<Matrix2<<endl;
//cout<<"сложили"<<endl;

//cout<<summedMatrix<<endl;
    
}



void *getSharedPointer() 
{
    const int size = 1024;
//создает
    auto id = shmget(1, size, 0644 | IPC_CREAT); // присваиваем идентификатор разделяемому сегменту памяти 

//возвращает
    //работа с разделяемой памятью  
    auto pointer = shmat(id, nullptr, 0);//указатель, на память, куда запишутся матрицы из файла

    return pointer;//указатель на начало общей памяти
}


void MatrixWrite() 
{
    //читаем из памяти, записываем в файл

    auto memShared = (int *) getSharedPointer();

    auto matrixWithNextPointer = Matrix::readByPtr(memShared);
    auto const &matrix = matrixWithNextPointer.first;

    ofstream outputMatrix("output.txt");

    outputMatrix << matrix;
    outputMatrix.close();

}
