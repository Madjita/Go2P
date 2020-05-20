#ifndef RECORDHARDTYPE_H
#define RECORDHARDTYPE_H

#include <iostream>
#include <vector>
#include <string.h>


//#include "my/myvariable.h"

#include <map>

class MyVariable;

using namespace std;

class RecordHardType
{
public:
    RecordHardType();

    void addNameHardType(string name);
//    void addVectorNameThisLabel(vector<string> data);
//    bool addVectorNameThisLabel(string data);
//    void addNameThisType(string data);
//    void addNewVariables();

//    //
//    void addUnionVectorNameThisLabel(vector<string> data);
//    bool addUnionVectorNameThisLabel(string data);
//    void addUnionNewVariables();
//    void addUnioNameThisType(string data);

//    //



//    vector<map<string,string>> getVariables();
//    vector<map<string,string>> getVariablesRecord();

    string getNameHardType();
    string getLocal();
    void setLocal(string data);

    //новое
    void add_variable();
    MyVariable* get_variable_at(int position);
    bool add_variables_to_vector(string name);
    bool add_variables_type(string type);
    bool add_variables_expression(string expression);
    bool find_init_variables(string name);
    int  get_count();

    void add_variable_union();
    MyVariable *get_variable_union_at(int position);
    bool add_variables_union_to_vector(string name);
    bool add_variables_union_type(string type);
    bool add_variables_union_expression(string expression);
    bool find_init_variables_union(string name);
    int  get_count_union();

private:
    string nameHardType;

    string local; // какая область видимости (глобальная или локальная) ?

//    vector<map<string,string>> vectorMap_variables_nameType;
//    vector<string> vectorNameThisLabel;

//    vector<map<string,string>> union_variables_nameType;
//    vector<string> union_vectorNameThisLabel;



//    string nameThisType;
//    string unionNameThisType;

    //вектор переменных (полей структуры)
    vector<MyVariable*> variables_;
    //вектор переменных (полей union)
    vector<MyVariable*> variables_union_;
    //призннак локального обобщения или глобального
    bool flag_local_; // true is local

};

#endif // RECORDHARDTYPE_H
