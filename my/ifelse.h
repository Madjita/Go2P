#ifndef IFELSE_H
#define IFELSE_H


#include <iostream>
#include <vector>
#include <string.h>

#include <map>
#include "alien/scanner.h"


#include "my/functype.h"

class MyVariable;


using namespace std;

class IfElse : public FuncType
{
public:
    IfElse(IfElse* parent = nullptr,FuncType* parent_func = nullptr,unsigned int pos = 0);

    void set_result_if(bool result){result_if = result;};
    void add_new_if_else();
    void remove_new_if_else();
    IfElse* get_if_else(int position);
    int get_if_else_size(){return if_else.size()-1;}

    //функция для записи кода
    void add_vvMap_KeyData(vector<vector<map<LexClass,string>>>& set_vvMap_KeyData,int start);


    //variabls
    void add_variable();
    void add_variable(MyVariable* point);
    void variable_clear();
    MyVariable* get_variable_at(int position);
    MyVariable *get_variable_at(string name);
    bool add_variables_to_vector(string &name);
    bool add_variables_type(string type);
    bool add_variables_expression(string expression);

private:
    FuncType* func_;
    IfElse* parent_;

    //вложенность для установки числа после @
    unsigned int pos_;

    //результат условия if
    bool result_if;

    //вектор локальных переменных условия
    vector<MyVariable*> variables;

    //вложенные условия в данное условие
    vector<IfElse*> if_else;

    //вектор строк хранящий в себе вектор лексем (ключ значение)
    vector<vector<map<LexClass,string>>> vvMap_KeyData;


};

#endif // IFELSE_H
