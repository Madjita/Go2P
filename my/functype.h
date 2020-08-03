#ifndef FUNCTYPE_H
#define FUNCTYPE_H

#include <iostream>
#include <vector>
#include <string.h>

#include <map>

using namespace std;


#include "funccall.h"

#include "alien/scanner.h"
#include "my/variableexeption.h"

#include "my/myvariable.h"

class FuncCall;
class IfElse;

class FuncType
{
public:
    FuncType();


    void addNameFuncType(string name);
    void addNameFuncReturnType(string name);
    void addVectorNameThisLabel(vector<string> data);
    bool addVectorNameThisLabel(string data);
    void addNameThisType(string data);
    void addNewVariables();
    void initVariables(string name);

    string getNameFuncType();
    string getReturnFuncType();
    vector<map<string,string>> getVariables();


    //Пробую потом удалить
    void setExpression(vector<map<LexClass,string>> _expression);
    vector<map<map<string,string>,VariableExeption>> getVariables2();

    //Проверить объявленна ли переменная
    bool findInitStatment(string variable);
    string getInitStatment_type(string variable);

    void add_local_variable(string name,string type);

    //новое пробую
    void add_variable();
    void remove_variable(MyVariable *item);
    MyVariable* get_variable_at(int position);
    MyVariable *get_variable_at(string name);
    MyVariable *get_variable_las();
    bool add_variables_to_vector(string name);
    bool add_variables_type(string type);
    bool add_variables_expression(string expression);

    bool set_variable_struct(HardType* newObject);
    bool set_variable_record(RecordHardType* newObject);

    //функция для записи кода
    void add_vvMap_KeyData(vector<vector<map<LexClass,string>>>& set_vvMap_KeyData,int start);


    //вектор локальных переменных функции
    vector<MyVariable*> variables;

    void add_new_if_else();
    void remove_new_if_else();
    IfElse* get_if_else(int position);
    int get_if_else_size(){return if_else.size()-1;}
    //вектор условий с локальными переменными в if и else
    vector<IfElse*> if_else;

//    //вектор локальных переменных в if
//    vector<MyVariable*>  if_variables;
//    //вектор локальных переменных в case
//    vector<MyVariable*>  case_variables;
//    //вектор локальных переменных в switch
//    vector<MyVariable*>  switch_variables;
//    //вектор локальных переменных в for
//    vector<MyVariable*>  for_variables;


    //vectorCallFun
    void add_callFun_name(FuncCall* call_func);
private:
    string nameFun;
    string returnType;

    //вектор локально объявленных переменных
    vector<map<string,string>> vectorMap_variables_nameType;
    vector<string> vectorNameThisLabel;

    //результат локально объявленных переменных
    VariableExeption expression;
    vector<map<map<string,string>,VariableExeption>> vectorNameThisLabel_example; //Пробую потом удалить
    string nameThisType;

    //вектор строк хранящий в себе вектор лексем (ключ значение)
    vector<vector<map<LexClass,string>>> vvMap_KeyData;


    vector<FuncCall*> vectorCallFun;
};

#endif // FUNCTYPE_H
