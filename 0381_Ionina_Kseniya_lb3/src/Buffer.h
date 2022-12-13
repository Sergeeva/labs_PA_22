#include <memory>
#include "Matrix.h"
using namespace std;


template<typename T>
class Node{
public:

    shared_ptr<T> matrix;
    shared_ptr<Node<T>> next; //указатель на следующий узел


    Node() : matrix(nullptr){}

    Node(shared_ptr<T> matrix) : matrix(matrix){}

};

template<typename T>
class Buffer{

private:
    shared_ptr<Node<T>> head_;

public:

Buffer(){

    // указатель на голову стека
    this->head_ = nullptr;

}

//Производитель
//Добавление
void produce(T matrix){

    //Запомнили, куда указывает голова стека
    shared_ptr<Node<T>> node = this->head_;//скопировали H  
    //указатель на узел

    //Создаем новый элемент, который хотим добавить в начало стека.
    shared_ptr<Node<T>> newHead = make_shared<Node<T>>(Node<T>(make_shared<T>(matrix))); 
    //указатель на след.значение для него - node

   //Он создает объект типа Node, передавая аргументы своему конструктору, и возвращает объект типа shared_ptr.который владеет и хранит указатель на него.


//Пробуем переместить H на новый элемент, при помощи cas
    do {

        newHead->next = node;//след - next

    } while (!atomic_compare_exchange_weak(&this->head_, &node, newHead)); //21
//Если удалось - добавление прошло успешно.
//Если нет - кто-то другой изменил стек, пока мы пытались добавить элемент -Ю начинаем сначала
}


//Потребитель
//Удаление
shared_ptr<T> consume(){

     //Запомнили, куда указывает голова стека.  *Нужно вернуть*
    shared_ptr<Node<T>> node = this->head_;

//Пробуем переместить голову стека casОМ

//*(1 = стек не пуст и когда никто другой ничего не добавил)*
//пока *(0)* 
//пока стек пустой или кто-то что-то добавил, меняем...
    while (!(node && atomic_compare_exchange_weak(&this->head_, &node, node->next))) 
        node = this->head_;
    return node->matrix;
}
};