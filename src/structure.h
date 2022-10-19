#ifndef STRUCTURE_H
#define STRUCTURE_H
#include <unordered_map>

//queue class
template <class U>
class queue {
    private:
        struct node {
            private:
                U data;
                node* next;
            public:
                node() : next(nullptr) {}
                node(const U& d) : data(d), next(nullptr) {}
        };
        node* head;
    public:
        queue() : head(new node()) {}
        ~queue();
        void enqueue(const U&);
        U dequeue();
};

template <class U>
void queue<U>::enqueue(const U& value) {
    node* curr = this->head;
    while (curr->next) curr = curr->next;
    curr->next = new node(value);
}

template <class U>
U queue<U>::dequeue() {
    node* to_dequeue = this->head->next;
    if (to_dequeue) {
        this->head->next = to_dequeue->next;
        U value = to_dequeue->data;
        delete to_dequeue;
        return value;
    } else throw std::domain_error("Out of bounds, queue is empty.");
}

template <class U>
queue<U>::~queue() {
    try {
        while () this->dequeue();
    } catch(const std::domain_error& msg)
    delete this->head;
}

//graph class as represtented by an adjacency list
template <class T>
class graph {
    private:
        std::unordered_map<T, std::unordered_map<T, int>> adjacency_list;
    protected:
        std::unordered_map<T, T> bfs();
    public:
        void insert(const int&);
        void print();
        void find_path(const T&, const T&);
};

#endif