#include <iostream>
#include <fstream>
#include "structure.h"
#include <sstream>
#include <string>

int main(int argc, char** argv) {
    std::ifstream infile;
    //if (argc < 2) //throw std::invalid_argument("./main <inputfile>.txt");
    //else infile.open(argv[1]);
    infile.open("input.txt");
    std::string line;
    graph g;

    while(std::getline(infile, line, '\n')) g.insert(line);
    infile.close();
    //g.print();
    //bfs
    //g.find_path("1", "9");
    //dijkstra's
    //g.map_path("1", "8");
    //super dijkstra's
    std::vector<std::string> x = {"2", "8"};
    g.map_whole_path("1", x);

    system("pause");
    return 0;
}