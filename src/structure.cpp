#include "structure.h"
#include <sstream>
#include <iostream>

#define INT_MAX 2147483647
//priority queue methods
p_queue::p_queue() {
    this->size = 0;
    this->heap.push_back(std::make_pair(std::string(), 0));
}

void p_queue::swap(int index1, int index2) {
    std::pair<std::string, int> to_swap = this->heap[index1];
    this->heap[index1] = this->heap[index2];
    this->heap[index2] = to_swap;
}

void p_queue::heapify(int i) {
    unsigned int child_index = i;
    if (2*i <= this->size && this->heap[child_index].second > this->heap[2*i].second) child_index = 2*i;
    if (2*i+1 <= this->size && this->heap[child_index].second > this->heap[2*i+1].second) child_index = 2*i+1;
    if (child_index == i) return; //we do not heapify if we are at a valid heap/leaf node.
    //child_index = this->heap[2*i] > this->heap[2*i+1] ? 2 * i : 2 * 1 + 1;
    this->swap(child_index, i);
    this->heapify(child_index);
}

std::pair<std::string, int> p_queue::extract_min() {
    if (this->size < 1) return this->heap[0];
    std::pair<std::string, int> return_value = this->heap[1];
    this->heap[1] = this->heap[this->size];
    this->size--;
    this->heap.pop_back();
    this->heapify(1);
    this->print();
    return return_value;
}

void p_queue::increase_key(int i, int weight) {
    this->heap[i].second = weight;
    while (i != 1 && this->heap[i/2].second > this->heap[i].second) {
        this->swap(i, i/2);
        i = i/2;
    }
}

void p_queue::insert(std::pair<std::string, int> new_insert) {
    this->heap.push_back(std::make_pair(new_insert.first, INT_MAX));
    this->size+=1;
    this->increase_key(this->size, new_insert.second);
    this->print();
}
void p_queue::print() {
    for (int i = 1; i <= this->size; i++) {
        std::cout << "(" << this->heap[i].first << "," << this->heap[i].second << "):";
    }
    std::cout << std::endl;
}

//graph methods
//all values on the line are treated as neighbors of one another.
void graph::insert(const std::string& line) {
    std::string node, neighbor, weight;
    std::stringstream ssone;
    ssone.str(line); 
    std::getline(ssone, weight, ' ');
    while (std::getline(ssone, node, ' ')) {
        std::stringstream sstwo;
        sstwo.str(ssone.str());
        sstwo.rdbuf()->pubseekpos(2);
        while (std::getline(sstwo, neighbor, ' ')) {
            //check i think is unecessary beacuse unordered map handles duplicate cases. though duplicates will overwrite.
            //however running into issues with stringstream and modifying the stream mid loop. therefore we perform the check.
            if (node.compare(neighbor) == 0) continue;
            if (this->adjacency_list[node].find(neighbor) == adjacency_list[node].end()) { 
                this->adjacency_list[node].insert({neighbor, std::stoi(weight)});
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

//INCOMPLETE
//LOGIC ERROR. WHEN USING 1 -> {2, 8}  WE FIND THE SHORTEST PATH FROM 1 TO 2 AND 1 TO 8. NOT THE PATH
//WHICH INCLUDES 1, 2, 8;
std::unordered_map<std::string, std::string> graph::dijkstra(const std::string& start, const std::string& goal) {
    if (this->adjacency_list.count(start) == 0) std::unordered_map<std::string, std::string>();
    p_queue to_visit;
    std::unordered_map<std::string, std::string> pred;
    std::unordered_map<std::string, int> path_cost;
    for (auto& pair : this->adjacency_list) {
        pred.insert({pair.first, std::string()});
        path_cost.insert({pair.first, INT_MAX}); //max value for nodes we have yet to visit.
    }

    path_cost[start] = 0;
    to_visit.enqueue(std::make_pair(start, path_cost[start]));
    while (!to_visit.is_empty()) {
        std::pair<std::string, int> curr = to_visit.dequeue();
        //perform check to see if we have visited all goals.
        if (curr.first.compare(goal) == 0) return pred;

        for (auto& neighbor : this->adjacency_list[curr.first]) {
            //logic that follows is smaller weights on a path means the path is more desirable.
            int cost = curr.second + neighbor.second;
            //have we found a faster path?
            if (path_cost[neighbor.first] > cost) {
                pred[neighbor.first] = curr.first;
                path_cost[neighbor.first] = cost;
                to_visit.enqueue(std::make_pair(neighbor.first, cost));
            }
        }
    }
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

void graph::map_path(const std::string& start, const std::string& goal) {
    auto x = this->dijkstra(start, goal);
    std::string result = "";
    std::string value = goal;
    while (value != std::string()) {
        result = ":" + value + result;
        value = x.at(value);
    }
    std::cout << result << std::endl;
}