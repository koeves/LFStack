#include <atomic>
#include <unistd.h>
#include <iostream>
#include "Node.hpp"

template <class T> class LFStack {
    
public:
    LFStack() : top(nullptr) {}
    ~LFStack() {
        Node<T> *node = top;
        while (node) {
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
    T pop() {
        while (true) {
            Node<T> *return_node = try_pop();
            if (return_node) {
                T return_value = return_node->value;
                delete return_node;
                return return_value;
            }
            else {
                sleep(random() % 5);
            }
        }
    }

    struct EmptyException : public std::exception {
        const char *what() const throw() {
            return "Stack is empty";
        }
    };

protected:
    bool try_push(Node<T> *node) {
        Node<T> *old_top = std::atomic_load(&top);
        node->next = old_top;
        return std::atomic_compare_exchange_strong(&top, &old_top, node);
    }
    Node<T> *try_pop() {
        try {
            Node<T> *old_top = std::atomic_load(&top);
            if (!old_top) throw EmptyException();
            Node<T> *new_top = old_top->next;
            return (std::atomic_compare_exchange_strong(&top, &old_top, new_top)) ? old_top : nullptr;
        }
        catch(EmptyException &e) {
            std::cerr << e.what() << std::endl;
            return nullptr;
        }
    }

private:
    std::atomic<Node<T> *> top;
};