#ifndef STACK_H
#define STACK_H

#include <iostream>

// 链栈节点结构体
template <typename T>
struct StackNode {
    T data;
    StackNode<T>* next;
    
    StackNode(const T& item) : data(item), next(nullptr) {}
};

// 链栈类定义
template <typename T>
class Stack {
private:
    StackNode<T>* top; // 栈顶指针
    int size;          // 栈大小
    
public:
    // 构造函数
    Stack() : top(nullptr), size(0) {}
    
    // 析构函数
    ~Stack() {
        clear();
    }
    
    // 入栈操作
    void push(const T& item) {
        StackNode<T>* newNode = new StackNode<T>(item);
        newNode->next = top;
        top = newNode;
        size++;
    }
    
    // 出栈操作
    T pop() {
        if (isEmpty()) {
            throw std::runtime_error("栈为空");
        }
        
        StackNode<T>* temp = top;
        T value = temp->data;
        top = top->next;
        delete temp;
        size--;
        return value;
    }
    
    // 获取栈顶元素
    T& peek() {
        if (isEmpty()) {
            throw std::runtime_error("栈为空");
        }
        return top->data;
    }
    
    // 判断栈是否为空
    bool isEmpty() const {
        return top == nullptr;
    }
    
    // 获取栈大小
    int getSize() const {
        return size;
    }
    
    // 清空栈
    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }
};

#endif // STACK_H