#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

#include "my/myvariable.h"
#include "my/hardtype.h"
#include "my/functype.h"
#include "my/recordhardtype.h"

#include "my/variableexeption.h"


//таблица существующих переменных
class Table
{
public:
    Table();

    void add_gloabals(LexClass lc, string name,LexClass lc_type,string type,string expression);
    //Глобальные переменные
    vector<MyVariable*> globals_;

    void add_hard_types(HardType* new_type);
    //Структуры (сложные типы данных)
    // Должны храниться все инициализированные сткрутуры (внутри должны быть данные на MyVariable)
    vector<HardType*> types_;

    void add_func_types(FuncType* new_func);
    //Функции
    // Должны храниться все инициализированные функции (внутри должны быть данные на локальные MyVariable)
    vector<FuncType*> functions_;

    void add_record_hard_type_types(RecordHardType* new_record);
    //Обобщения
    // Должны храниться все инициализированные Обобщения (внутри должны быть данные на локальные MyVariable)
    vector<RecordHardType*> records_;


    //результат переменных
    VariableExeption expression;


    //функция для нахождения структуры
    HardType* create_object_hardType(string name_hardType);
    //функция для добавления структуры в таблицу с глобальными переменными
    void add_gloabals(LexClass lc, string name,LexClass lc_type,string type,HardType* item);

    //функция для поиска уже объявленной переменной в глобальной таблици
    MyVariable* get_variable_in_globals(string name);
    //функция для поиска уже объявленной переменной в локальной таблици функции
    MyVariable* get_variable_in_local_func(FuncType* func,string name);


    //Поиск переменной
    MyVariable* find_object(MyVariable *object, string name);
    //функция для поиска сложных переменных и их замена
    bool find_name_variable(FuncType* func,vector<map<LexClass,string>> &find);

    //функция для поиска существования имени функции в массиве функций
    FuncType *find_name_func(string name);

};

#endif // TABLE_H
