
#ifndef H_STACK_CPP
#define H_STACK_CPP

#include<stdio.h>

#include "stack.h"


template <class T> 
Stack<T>::Stack() {
    size = 0;
    head = nullptr;
    top  = nullptr;
}

template <class T> 
Stack<T>::~Stack() {
    while(top != nullptr) {
        T *obj = pop();
        delete obj;
    }
}

template <class T> 
T *Stack<T>::peek() {
    return top->element;
}

template <class T> 
void Stack<T>::push(T *obj) {
    StackNode *newNode = new StackNode(obj);
    if (head == nullptr) {
        head = newNode;
        top  = newNode;
    } else {
        top->next = newNode;
        newNode->prev = top;
        top = newNode;
    }
    size++;
}

template <class T> 
T *Stack<T>::pop() {

    if (top == nullptr) {
        printf("[WARNING in stack.cpp]: Tried to pop a stack off when the head is null!\n");
    }

    T *obj = top->element;
    StackNode *newTop = top->prev;
    delete top;

    top = newTop;
    size--;

    return obj;
}

#endif
