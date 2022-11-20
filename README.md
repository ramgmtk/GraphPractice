This project demonstrates the use of a graph and methods that can be utilized to traverse the graph. <br />
The graph structure is represented as an adjacency list made of two unordered maps. Each node on the graph is aware of it's neighbors and the weight of edges between their neighbors. <br />
I implemented a simple BFS (breadth first search) function to travel through all nodes in the graph. The bfs makes use of a queue data structure to in order to manage what nodes we have visited. <br />
Additionally I wrote a dijkstra function which finds the shortest path between two points by making use of the information provided by the edge values between nodes. <br />
The dijkstra function makes use of a priority queue instead of a regular queue to take into account the cost to traverse from a node towards a goal. I made the priority queue by writing a min heap data structure. The min heap is demonstrated through the use of a vector in this case. <br />
<br />
To add my own challenge to my graph practice I wrote an additional function that tracks the path through multiple goals. <br />
I brute forced the solution by performing a nested dijkstra's call wihin an existing dijkstra's call. We find the shortest path from one node to the next and add up the total values. e.g. a->b a->c, ab->d ab->c, so on so forth.
The idea is that given a starting node it will find a path that traverses through all designated nodes in the shortest time. <br />
The practical use of this implementation would be something like giving a delivery man a path to take for all the homes they need to reach on their delivery route. <br />
This project was a good opporunity for me to review how graph data structures work, as well as writing my own supporting data structures to implement path finding functions. <br />


