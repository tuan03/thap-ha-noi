#pragma once
#include <iostream>
using namespace std;
namespace Tuan{
template <class T>
class Node {
public:
    T data;
    Node<T>* next;
    Node(T element) {
        data = element;
        next = NULL;
    }
};

template <class T>
class Stack {
private:
    Node<T>* top_node;
public:
    Stack() {
        top_node = NULL;
    }

    void push(T element) {
        Node<T>* new_node = new Node<T>(element);
        new_node->next = top_node;
        top_node = new_node;
    }

    T pop() {
        if (empty()) throw "Stack is Empty";
        T temp = top_node->data;
        Node<T>* node_to_delete = top_node;
        top_node = top_node->next;
        delete node_to_delete;
        return temp;
    }

    bool empty() {
        return top_node == NULL;
    }
    ~Stack() {
            while (top_node != nullptr) {
                Node<T>* temp = top_node;
                top_node = top_node->next;
                delete temp;
            }
        }
};





template <class T>
class Queue {
private:
    Node<T>* head;
    Node<T>* tail;
public:
    Queue() {
        head = NULL;
        tail = NULL;
    }

    void push(T element) {
        Node<T>* new_node = new Node<T>(element);
        if (tail == nullptr) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
     }

    T pop() {
        if (this->empty()) throw "Queue is empty";
            T temp = head->data;
            Node<T>* node_to_delete = head;
            head = head->next;
            delete node_to_delete;
            if (head == nullptr) {
                tail = nullptr;
            }
            return temp;
    }

    bool empty() {
        return head == NULL;
    }
    ~Queue() {
            while (head != nullptr) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        }
};





}