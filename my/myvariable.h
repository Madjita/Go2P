#ifndef MYVARIABLE_H
#define MYVARIABLE_H

#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

#include "alien/scanner.h"

#include "my/hardtype.h"
#include "my/recordhardtype.h"

//Объект переменной
class MyVariable
{
public:
    MyVariable();
    ~MyVariable();

    void set_lex(LexClass lc);
    void set_name(string name);
    void set_lex_type(LexClass lc_type);
    void set_type(string type);
    void set_expression(string expression);
    void set_variable_struct(HardType* newObject);
    void set_variable_record(RecordHardType* newObject);

    MyVariable* get_variable(string name);

    LexClass get_lex();
    string get_name();
    LexClass get_lex_type();
    string get_type();
    string get_expression();

    MyVariable* clone();

    //функция для записи кода
    void add_vvMap_KeyData(vector<vector<map<LexClass,string>>>& set_vvMap_KeyData,int start);

private:
    //Лексема переменной
    LexClass lc_;
    //Лексема типа
    LexClass lc_type_;

    //имя переменной
    string name_;
    //тип переменной
    string type_;
    //результат переменной
    string expression_;

    HardType* variable_struct_;
    RecordHardType* variable_record_;


    //вектор строк хранящий в себе вектор лексем (ключ значение)
    vector<vector<map<LexClass,string>>> vvMap_KeyData;

};

#endif // MYVARIABLE_H
