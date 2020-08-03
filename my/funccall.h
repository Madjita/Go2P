#ifndef FUNCCALL_H
#define FUNCCALL_H



#include <iostream>
#include <vector>
#include <string.h>

#include <map>

using namespace std;

//#include "alien/scanner.h"
//#include "my/variableexeption.h"

class FuncType;
class MyVariable;

class FuncCall
{
public:
    FuncCall();
    FuncCall(FuncType* call_func_type);

    string get_name_call_func();

    string nameCallFunc_;
    vector<MyVariable*> vector_variable_;

    FuncType* call_func_type_;
};

#endif // FUNCCALL_H
