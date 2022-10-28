#include "structure.h"
#include <sstream>
#include <iostream>

//priority queue methods
p_queue::p_queue() {
    this->size = 0;
    this->max_heap.push_back(std::make_pair(std::string(), 0));
}

void p_queue::swap(int index1, int index2) {
    std::pair to_swap = this->max_heap[index1];
    this->max_heap[index1] = this->max_heap[index2];
    this->max_heap[index2] = to_swap;
}

void p_queue::heapify(int i) {
    unsigned int child_index = 0;
    if (2*i <= this->size && this->max_heap[i].second >= this->max_heap[2*i].second) child_index = 2*i;
    if (2*i+1 <= this->size && this->max_heap[i].second >= this->max_heap[2*i+1].second) child_index = 2*i+1;
    if (!child_index) return; //we do not heapify if we are at a valid heap/leaf node.
    child_index = this->max_heap[2*i] > this->max_heap[2*i+1] ? 2 * i : 2 * 1 + 1;
    this->swap(child_index, i);
    this->heapify(child_index);
}

std::pair<std::string, int> p_queue::extract_max() {
    if (this->size < 1) return this->max_heap[0];
    std::pair<std::string, int> return_value = this->max_heap[1];
    this->max_heap[1] = this->max_heap[this->size];
    this->size--;
    this->max_heap.pop_back();
    this->heapify(1);
    return return_value;
}

void p_queue::increase_key(int i, int weight) {
    this->max_heap[i].second = weight;
    while (i != 1 && this->max_heap[i/2].second < this->max_heap[i].second) {
        this->swap(i, i/2);
        i = i/2;
    }
}

void p_queue::insert(std::pair<std::string, int> new_insert) {
    this->max_heap.push_back(std::make_pair(new_insert.first, INT_MIN));
    this->size+=1;
    this->increase_key(this->size, new_insert.second);
}

//graph methods
//all values on the line are treated as neighbors of one another.
void graph::insert(const std::string& line) {
    std::string node, neighbor, parse_line;
    std::stringstream ssone;
    ssone.str(line); 

    while (std::getline(ssone, node, ' ')) {
        std::stringstream sstwo;
        sstwo.str(ssone.str());
        while (std::getline(sstwo, neighbor, ' ')) {
            //check i think is unecessary beacuse unordered map handles duplicate cases. though duplicates will overwrite.
            //however running into issues with stringstream and modifying the stream mid loop. therefore we perform the check.
            if (node.compare(neighbor) == 0) continue;
            if (this->adjacency_list[node].find(neighbor) == adjacency_list[node].end()) { 
                this->adjacency_list[node].insert({neighbor, 0});
                //this->adjacency_list[neighbor].insert({node, 0});
            }
        }
    }
}

void graph::print() {
    for (auto& pairone : this->adjacency_list) {
        std::cout << pairone.first << ": ";
        for (auto& pairtwo : pairone.second) {
            std::cout << pairtwo.first << ", ";
        }
        std::cout << std::endl;
    }
}

//we use default string value if no end is specified.
std::unordered_map<std::string, std::string> graph::bfs(const std::string& start, const std::string& end) {
    //check if value even exists
    if (this->adjacency_list.count(start) == 0) return std::unordered_map<std::string, std::string>();
    queue<std::string>* to_visit = new queue<std::string>();
    std::unordered_map<std::string, std::string> pred;
    std::unordered_map<std::string, bool> visited;
    for (auto& pair : this->adjacency_list) {
        visited.insert({pair.first, false});
        pred.insert({pair.first, std::string()});
    }
    to_visit->enqueue(start);
    while (!to_visit->is_empty()) {
        std::string curr = to_visit->dequeue();
        if (curr.compare(end) == 0) break;
        visited[curr] = true;
        for (auto& pair : this->adjacency_list[curr]) {
            if (!visited[pair.first] && pred[pair.first].compare(std::string()) == 0) {
                to_visit->enqueue(pair.first);
                pred[pair.first] = curr;
            }
        }
    }
    delete to_visit;
    return pred;
}

//throws error of node does not exist in graph
void graph::find_path(const std::string& start, const std::string& end) {
    auto x = this->bfs(start);
    std::string result = "";
    std::string value = end;
    while (value != std::string()) {
        result = ":" + value + result;
        value = x.at(value);
    }
    std::cout << result << std::endl;
}