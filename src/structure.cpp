#include "structure.h"
#include <sstream>
#include <iostream>

//path_map methods
bool pred_map::contains_same(const pred_map& rhs) const {
    std::cout << "using overloaded pred_map comparison" << std::endl;
    for (auto& kv : rhs) {
        std::cout << "rhskey:value :: " << kv.first << ":" << kv.second << std::endl;
        std::cout << "lhskey:value :: " << kv.first << ":" << this->at(kv.first) << std::endl;
        if (this->count(kv.first) == 0 || this->at(kv.first) != kv.second) 
            return false;
    }
    return true; 
}

bool operator==(const pred_map& lhs, const pred_map& rhs) {
    return lhs.contains_same(rhs);
}

bool operator!=(const pred_map& lhs, const pred_map& rhs) {
    return !lhs.contains_same(rhs);
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
pred_map graph::bfs(const std::string& start, const std::string& end) {
    //check if value even exists
    if (this->adjacency_list.count(start) == 0) return pred_map();
    queue<std::string>* to_visit = new queue<std::string>();
    pred_map pred;
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
dijkstra_return graph::dijkstra(const std::string& start, const std::string& goal) {
    if (this->adjacency_list.count(start) == 0) return make_pair(pred_map(), INT_MAX);
    p_queue<std::string> to_visit;
    pred_map pred;
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
        if (curr.first.compare(goal) == 0) return std::make_pair(pred, curr.second);

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
    return std::make_pair(pred, INT_MAX);
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
        value = x.first.at(value);
    }
    std::cout << result << std::endl;
}

//super dijkstra's for finding multiple paths
std::string graph::super_dijkstra(const std::string& start, std::vector<std::string> goals) {
    if (this->adjacency_list.count(start) == 0) return std::string();
    p_queue<std::pair<std::string, std::pair<pred_map, visited_map>>> v;
    //note a map using a map has a key requires the creation of a hashing function for said map:
    //https://stackoverflow.com/questions/51220257/compilation-error-related-to-map-and-unordered-map-attempting-to-reference-a-d 
    std::unordered_map<pred_map, pred_map> pred;
    std::unordered_map<pred_map, int> path_cost;

    v.enqueue(std::make_pair(std::make_pair(start, std::make_pair(pred_map(), visited_map())), 0));
    while (!v.is_empty()) {
        auto curr = v.dequeue();

        //check if done going through all goals.
        bool finished = true;
        for (auto& goal : goals) {
            if (curr.first.second.first.count(goal) == 0) finished = false;
        }
        if (finished) {
            pred_map& curr_pred = curr.first.second.first;
            std::string value = curr.first.first;
            std::string result = value + result;
            while (curr_pred != pred_map()) {
                while (curr_pred[value] != std::string()) {
                    value = curr_pred[value];
                    result = value + result;
                }
                curr_pred = pred[curr_pred];
                value = curr_pred[value];
            }
            return result;
        }

        //perform goals run
        for (auto& goal : goals) {
            visited_map curr_visited = curr.first.second.second;
            //if we are a goal trying to reach itself or a goal we have already reached along the path to another goal
            //previos error now resolved: 
            //EACH PRED DOES NOT NECESSARY INCLUDE THE PREVIOUS GOALS TRAVERSED. THOSE GOALS ARE UNKOWN TO THE PRED ARRAY EVEN IF THEY ARE ON THE PREDICTED PATH.
            //so we might have to pass a visited map. curr.first.second.second references visited.
            if (curr.first.first == goal || curr_visited.count(goal) != 0) continue;
            dijkstra_return value = this->dijkstra_returning_visited(curr.first.first, goal, curr_visited);
            int cost = value.second + curr.second;
            //issues comes herre the pred map evaluated from 1->2 is being considered the same as 1->8
            //consider changing indexing from pred map to the visited map instead.
            if (path_cost.count(value.first) == 0 || path_cost[value.first] > cost) {
                pred[value.first] = curr.first.second.first;
                path_cost[value.first] = cost;
                v.enqueue(std::make_pair(std::make_pair(goal, std::make_pair(value.first, curr_visited)), cost));
            }
        }

    }
    return std::string();
}

dijkstra_return graph::dijkstra_returning_visited
(const std::string& start, const std::string& goal, visited_map& visited) {
    if (this->adjacency_list.count(start) == 0) return make_pair(pred_map(), INT_MAX);
    p_queue<std::string> to_visit;
    pred_map pred;
    std::unordered_map<std::string, int> path_cost;
    for (auto& pair : this->adjacency_list) {
        pred.insert({pair.first, std::string()});
        path_cost.insert({pair.first, INT_MAX}); //max value for nodes we have yet to visit.
    }

    path_cost[start] = 0;
    to_visit.enqueue(std::make_pair(start, path_cost[start]));
    while (!to_visit.is_empty()) {
        std::pair<std::string, int> curr = to_visit.dequeue();
        visited[curr.first] = true;
        //perform check to see if we have visited all goals.
        if (curr.first.compare(goal) == 0) return std::make_pair(pred, curr.second);
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
    return std::make_pair(pred, INT_MAX);
}

void graph::map_whole_path(const std::string& start, std::vector<std::string> goals) {
    std::string result = this->super_dijkstra(start, goals);
    std::cout << result << std::endl;
}