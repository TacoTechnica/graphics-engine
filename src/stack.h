#ifndef H_STACK
#define H_STACK

//#define STACK_SIZE 2

template <class T> 
class Stack {
    private:
        int size;
//        template <class T>
        class StackNode {
            public:
                T *element;
                StackNode *next;
                StackNode *prev;
                // Adds a node on top of this one
                /*void addNode(StackNode<T> *node) {
                    next = node;
                    node->prev = this;
                }*/
                StackNode(T *element) {
                    this->element = element;
                    prev = nullptr;
                    next = nullptr;
                }
                StackNode() : StackNode(nullptr) {}
        };

        StackNode *head;
        StackNode *top;
    public:
        Stack();
        ~Stack();
        T *peek();
        void push(T *obj);
        T *pop();
        //void print();
        int getSize() { return size; } 
};

#include "stack.cpp" // Solve linking issue with templates
#endif
