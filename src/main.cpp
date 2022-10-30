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
    g.print();
    g.find_path("1", "9");

    system("pause");
    return 0;
}