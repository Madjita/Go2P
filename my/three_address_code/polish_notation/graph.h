#ifndef GRAPH_H
#define GRAPH_H

#include "my/three_address_code/polish_notation/polish_notation.h"

#include <iostream>
using namespace std;

#include <list>


class Graph
{
public:
    Graph(INSTRUCTION V);
    void addEdge(int src, int dest);


private:
    opcType opc; //оператор
    string opc_name; // оператор в виде строки
    list<opcType> *adjLists;
};

vector<Graph> vector_modules; //вектор кода графоф

#endif // GRAPH_H

