#include "structure.h"
#include <sstream>
#include <iostream>

//path_map methods
bool pred_map::contains_same(const pred_map& rhs) const {
    std::cout << "using overloaded pred_map comparison" << std::endl;
    for (auto& kv : rhs) {
        if (this->count(kv.first) == 0) return false;
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

//VERY INTENSIVE B/C LOTS OF MAPS BEING STORED. WOULD BE WORTHWHILE TO MAKE AN INHEIRITED MAP CLASS TO 
//HAVE OUR OWN VERSION  OF OPERATOR== FOR MAPS BECAUSE WE ARE ONLY CONCERNED WITH COMPARING KEYS NOT
//NOT BOTH IF MAPS ARE PERMUTATIONS OF OTHER MAPS
std::string graph::super_dijkstra(const std::string& start, std::vector<std::string> goals) {
    /*if (this->adjacency_list.count(start) == 0) return std::string();
    p_queue<std::pair<std::string, pred_map>> v;
    std::unordered_map<pred_map, pred_map> pred;
    std::unordered_map<pred_map, int> path_cost;

    v.enqueue(std::make_pair(make_pair(start, pred_map()), 0));
    while (!v.is_empty()) {
        auto curr = v.dequeue();

        //check if done going through all goals.
        bool finished = true;
        for (auto& goal : goals) {
            if (curr.first.second.count(goal) == 0) finished = false;
        }
        if (finished) {
            //IMPORTANT LOGIC ERROR. PRED DOES NOT ALLOW FOR A PATH TO HAVE TWO PREDECESSORS.
            return std::string();
        }

        //perform goals run
        for (auto& goal : goals) {
            //if we are a goal trying to reach itself or a goal we have already reached
            if (curr.first.first == goal || curr.first.second.count(goal) == 0) continue;
            dijkstra_return value = this->dijkstra(curr.first.first, goal);
            int cost = value.second + curr.second;
            if (path_cost.count(value.first) == 0 || path_cost[value.first] > cost) {
                pred[value.first] = curr.first.second;
                path_cost[value.first] = cost;
                v.enqueue(std::make_pair(std::make_pair(goal, value.first), cost));
            }
        }

    }*/
    return std::string();
}

/*
function header(start, goals) {
    check empty adjacency list
    priority<pair(new_start, predecessor)> v

    //do not initialize these like idjkstra
    pred which points to the current predecessor array being worked on
    cost which will contain costs of paths not individual nodes

    enqueue(<start, pred>, 0)
    while v not empty
        curr = dequeue;
        for (all goal in goals) if all goals exists in curr.pred
            calculate path and rteurn string?
        for (all goals in goal)
            if (curr.new_start == goal || curr.pred[goal] exists) continue;
            value = dijstra's on curr.new_start(goal)
            if cost[value.pred] does not exist or cost[value.pred] > value.cost + curr.second
                cost[value.pred] = value.cost + curr.second
                v.enqueue(<goal, value.pred>, cost[value.pred])
    
    return default std::string?
    
}
*/