#include "table.h"
#include <vector>

Table::Table()
{

}

void Table::add_gloabals(LexClass lc, string name,LexClass lc_type, string type, string expression)
{
    MyVariable* variable = new MyVariable(varGlobal,this);
    variable->set_lex(lc);
    variable->set_name(name);
    variable->set_lex_type(lc_type);
    variable->set_type(type);
    variable->set_expression(expression);

    globals_.push_back(variable);
}

void Table::add_hard_types(HardType *new_type)
{
    types_.push_back(new_type);
}

void Table::add_func_types(FuncType *new_func)
{
    functions_.push_back(new_func);
}

void Table::add_record_hard_type_types(RecordHardType *new_record)
{
    records_.push_back(new_record);
}

HardType *Table::create_object_hardType(string name_hardType)
{
    for(auto item: types_)
    {

        if(item->getNameHardType() == name_hardType)
        {
            return item->clone();
        }
    }

    return nullptr;
}

void Table::add_gloabals(LexClass lc, string name, LexClass lc_type, string type, HardType *item)
{
    MyVariable* variable = new MyVariable(varGlobal,this);
    variable->set_lex(lc);
    variable->set_name(name);
    variable->set_lex_type(lc_type);
    variable->set_type(type);
    variable->set_expression("");
    variable->set_variable_struct(item);

    globals_.push_back(variable);
}

MyVariable *Table::get_variable_in_globals(string name)
{
    for(auto item: globals_)
    {
        if(item->get_name() == name)
        {
            return item;
        }
    }

    return nullptr;
}

//функция для поиска уже объявленной переменной в локальной таблици функции
MyVariable *Table::get_variable_in_local_func(FuncType *func, string name)
{
    return func->get_variable_at(name);
}


//Подумать над созданием функции которая возвращает переменную по точккам где то уже такое было
MyVariable *Table::find_object(MyVariable *object, string name)
{
   return  object->get_variable(name);
}

bool Table::find_name_variable(FuncType *func, vector<map<LexClass, string> > &find)
{

    int start_position = -1;
    int stop_position = 0;
    string name = "";

    bool flag_find = false;
    HardType* object = nullptr;
    MyVariable* object_variable = nullptr;

    for(auto it=find.begin() ; it < find.end();it++)
    {
        auto data_lex = it->begin()->first;
        auto data_name = it->begin()->second;

        LexClass data_lex_prev = lcNoValue;

        if (data_lex == lcId && start_position == -1)
        {
            start_position = distance(find.begin(),it);
            stop_position = start_position;

            //поиск переменной в глобальной таблице
            object_variable = get_variable_in_globals(data_name);
            if(object_variable == nullptr)
            {
                //поиск переменной в локальной таблице
                object_variable = get_variable_in_local_func(func,data_name);
            }
        }

        if(it > find.begin())
        {
            data_lex_prev = (it-1)->begin()->first;

            if(data_lex == lcPoint && data_lex_prev == lcId)
            {
                stop_position++;
            }
            else
            {
                if(data_lex == lcId && data_lex_prev == lcPoint)
                {
                    stop_position++;

                    if(object_variable != nullptr)
                    {
                        object_variable = find_object(object_variable,data_name);

                        if(object_variable == nullptr)
                        {
                            //Это ошибка не известная переменная
                            //Дописать обработку ошибок
                            cout << "Error in expression, don't find variable"<<endl;
                            break;
                        }
                    }
                }
                else
                {
                    if(data_lex_prev == lcId && data_lex != lcId)
                    {
                        stop_position++;
                        flag_find = true;

                        //теоритически можно зациклить внутри функции рекурсией чтоб не проходить по массиву лишний раз
                        //но так наглядней,потом может подправлю

                        break;
                    }
                }
            }
        }
    }

    if(flag_find)
    {
        auto iterator_start_erase = find.begin() + start_position;
        auto iterator_end_erase = find.begin() + stop_position;
        find.erase(iterator_start_erase,iterator_end_erase);

        map<LexClass, string> newItem;
        auto newItem_type = object_variable->get_lex_type();
        auto newItem_name = object_variable->get_expression();
        newItem.insert(make_pair(newItem_type,newItem_name));

        find.insert(iterator_start_erase,newItem);

        return true;
    }

    return false;

}

//функция для поиска существования имени функции в массиве функций
FuncType *Table::find_name_func(string name)
{
    for(int i=0 ; i < functions_.size();i++)
    {
        if(functions_[i]->getNameFuncType() == name)
        {
            return functions_[i];
        }
    }

    return nullptr;
}


//переменные
//1)массивы
//2)

//Нужно массивы типов данных в которых храняться имена переменных
// List<string> list_int;
// List<string> list_float;


//Переменная состоит из
//1)имя (string)
//
//2)значение (конечное число определнного типа)


//1) Нужен какой то тип данных
// map<string,lexClass> buf_name_veriable


