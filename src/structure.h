#ifndef STRUCTURE_H
#define STRUCTURE_H
#include <unordered_map>
#include <string>

//queue class
template <class U>
class queue {
    private:
        struct node {
                U data;
                node* next;
                node() : next(nullptr) {}
                node(const U& d) : data(d), next(nullptr) {}
        };
        node* head;
    public:
        queue() : head(new node()) {}
        ~queue();
        bool is_empty();
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
bool queue<U>::is_empty() {
    if (this->head->next) return false; 
    else return true; 
}

template <class U>
queue<U>::~queue() {
    try {
        while (true) this->dequeue();
    } catch(const std::domain_error& msg) {}
    delete this->head;
}

//graph class as represtented by an adjacency list
class graph {
    private:
        std::unordered_map<std::string, std::unordered_map<std::string, int>> adjacency_list;
    protected:
        std::unordered_map<std::string, std::string> bfs(const std::string&, const std::string& = std::string());
    public:
        void insert(const std::string&);
        void print();
        void find_path(const std::string&, const std::string&);
};
 
#endif