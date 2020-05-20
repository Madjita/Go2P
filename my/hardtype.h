#ifndef HARDTYPE_H
#define HARDTYPE_H

#include <iostream>
#include <vector>
#include <string.h>

#include <map>

#include "alien/scanner.h"

class MyVariable;

using namespace std;

//Сложный тип данных
class HardType
{
public:
    HardType();

    void addNameHardType(string name);
    void addVectorNameThisLabel(vector<string> data);
    bool addVectorNameThisLabel(string data);
    string getNameHardType();

    void add_variable();
    MyVariable* get_variable_at(int position);
    MyVariable* get_variable_at(string name);
    bool add_variables_to_vector(string name);
    bool add_variables_type(string type);
    bool add_variables_expression(string expression);
    bool find_init_variables(string name);
    int  get_count();

    bool set_variable_struct(HardType* newObject);
    //bool set_variable_record(RecordHardType* newObject);

    HardType* clone();

    //функция для записи кода
    void add_vvMap_KeyData(vector<vector<map<LexClass,string>>>& set_vvMap_KeyData,int start);

private:
    string nameHardType; //имя

    //вектор переменных (полей структуры)
    vector<MyVariable*> variables;

    vector<vector<map<LexClass,string>>> vvMap_KeyData; //вектор строк хранящий в себе вектор лексем (ключ значение)


};

#endif // HARDTYPE_H
