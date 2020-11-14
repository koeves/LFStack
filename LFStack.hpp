#include <atomic>
#include <unistd.h>
#include <iostream>
#include "Node.hpp"

template <class T> 
class LFStack {
    
public:
    LFStack() : top(nullptr) {}
    ~LFStack() {
        Node<T> *node = top;
        while (node) {
            std::cout << "Destroying node " << node->value << std::endl;
            Node<T> *old = node;
            node = node->next;
            delete old;
        }
    }
    void push(T value) {
        Node<T> *new_node = new Node<T>(value);
        while (true) {
            if (try_push(new_node)) return;
            else sleep(random() % 5);
        }
    }
    void pop() {}

protected:
    bool try_push(Node<T> *node) {
        Node<T> *old_top = std::atomic_load(&top);
        node->next = old_top;
        return std::atomic_compare_exchange_strong(&top, &old_top, node);
    }
    bool try_pop();

private:
    std::atomic<Node<T> *> top;

};