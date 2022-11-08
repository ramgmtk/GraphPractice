#ifndef STRUCTURE_H
#define STRUCTURE_H
#include <unordered_map>
#include <vector>
#include <string>

#define INT_MAX 2147483647

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
        std::unordered_map<std::string, std::string> dijkstra(const std::string&, const std::string&);
        std::string super_dijkstra(const std::string&, std::vector<std::string>);
    public:
        void insert(const std::string&);
        void print();
        void find_path(const std::string&, const std::string&);
        void map_path(const std::string&, const std::string&);
};

//priotity queue implemented using a mix heap
template <class T>
class p_queue {
    private:
        std::vector<std::pair<T, int>> heap;
        unsigned int size;
    protected:
        void insert(std::pair<T, int>);
        std::pair<T, int> extract_min();
    public:
        p_queue();
        std::pair<T, int> dequeue() { return this->extract_min(); }
        void heapify(int);
        void enqueue(std::pair<T, int> new_insert) { this->insert(new_insert);};
        void increase_key(int, int);
        void swap(int, int);
        bool is_empty() { if (this->size == 0) return true; else return false; }
        //void print();
        friend class graph;
};

//priority queue methods
template <class T>
p_queue<T>::p_queue() {
    this->size = 0;
    this->heap.push_back(std::make_pair(T(), 0));
}

template <class T>
void p_queue<T>::swap(int index1, int index2) {
    std::pair<T, int> to_swap = this->heap[index1];
    this->heap[index1] = this->heap[index2];
    this->heap[index2] = to_swap;
}

template <class T>
void p_queue<T>::heapify(int i) {
    unsigned int child_index = i;
    if (2*i <= this->size && this->heap[child_index].second > this->heap[2*i].second) child_index = 2*i;
    if (2*i+1 <= this->size && this->heap[child_index].second > this->heap[2*i+1].second) child_index = 2*i+1;
    if (child_index == i) return; //we do not heapify if we are at a valid heap/leaf node.
    //child_index = this->heap[2*i] > this->heap[2*i+1] ? 2 * i : 2 * 1 + 1;
    this->swap(child_index, i);
    this->heapify(child_index);
}

template <class T>
std::pair<T, int> p_queue<T>::extract_min() {
    if (this->size < 1) return this->heap[0];
    std::pair<T, int> return_value = this->heap[1];
    this->heap[1] = this->heap[this->size];
    this->size--;
    this->heap.pop_back();
    this->heapify(1);
    return return_value;
}

template <class T>
void p_queue<T>::increase_key(int i, int weight) {
    this->heap[i].second = weight;
    while (i != 1 && this->heap[i/2].second > this->heap[i].second) {
        this->swap(i, i/2);
        i = i/2;
    }
}

template <class T>
void p_queue<T>::insert(std::pair<T, int> new_insert) {
    this->heap.push_back(std::make_pair(new_insert.first, INT_MAX));
    this->size+=1;
    this->increase_key(this->size, new_insert.second);
}

/*template <class T>
void p_queue<T>::print() {
    for (int i = 1; i <= this->size; i++) {
        std::cout << "(" << this->heap[i].first << "," << this->heap[i].second << "):";
    }
    std::cout << std::endl;
}*/
 
#endif