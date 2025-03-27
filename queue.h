#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <stdexcept>

// 链队列节点结构体
template <typename T>
struct QueueNode {
    T data;
    QueueNode<T>* next;
    
    QueueNode(const T& item) : data(item), next(nullptr) {}
};

// 链队列类定义
template <typename T>
class Queue {
private:
    QueueNode<T>* front;  // 队头指针
    QueueNode<T>* rear;   // 队尾指针
    int size;             // 队列大小
    
public:
    // 构造函数
    Queue() : front(nullptr), rear(nullptr), size(0) {}
    
    // 析构函数
    ~Queue() {
        clear();
    }
    
    // 入队操作
    void enqueue(const T& item) {
        QueueNode<T>* newNode = new QueueNode<T>(item);
        
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        
        size++;
    }
    
    // 出队操作
    T dequeue() {
        if (isEmpty()) {
            throw std::runtime_error("队列为空");
        }
        
        QueueNode<T>* temp = front;
        T value = temp->data;
        
        front = front->next;
        delete temp;
        
        if (front == nullptr) {
            rear = nullptr;  // 队列变空
        }
        
        size--;
        return value;
    }
    
    // 获取队头元素
    T& peek() {
        if (isEmpty()) {
            throw std::runtime_error("队列为空");
        }
        return front->data;
    }
    
    // 判断队列是否为空
    bool isEmpty() const {
        return front == nullptr;
    }
    
    // 获取队列大小
    int getSize() const {
        return size;
    }
    
    // 清空队列
    void clear() {
        while (!isEmpty()) {
            dequeue();
        }
    }
};

#endif // QUEUE_H