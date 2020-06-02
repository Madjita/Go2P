#include "ifelse.h"
#include "my/myvariable.h"


IfElse::IfElse(IfElse *parent, FuncType *parent_func, unsigned int pos)
{
    parent_ = parent;
    func_ = parent_func;
    pos_ = pos;
}

void IfElse::add_vvMap_KeyData(vector<vector<map<LexClass, string> > >& set_vvMap_KeyData, int start)
{
    for( auto it = set_vvMap_KeyData.begin()+start; it != set_vvMap_KeyData.end();it++)
    {
        vvMap_KeyData.push_back(*it);
    }

    vvMap_KeyData.pop_back(); //убрать последнию троку так как она явно лишняя
}

IfElse *IfElse::get_if_else(int position)
{
    return if_else[position];
}

void IfElse::add_new_if_else()
{
    pos_++;
    if_else.push_back(new IfElse(this,func_,pos_));
}

void IfElse::remove_new_if_else()
{
    if_else.pop_back();
}


void IfElse::add_variable()
{
    variables.push_back(new MyVariable(varLocal,this));
}

void IfElse::add_variable(MyVariable *point)
{
    //Указатель на переменную в списке переменных функции
    variables.push_back(point);
}

void IfElse::variable_clear()
{
    for(auto item: variables)
    {
        func_->remove_variable(item);
    }
    variables.clear();
}

MyVariable *IfElse::get_variable_at(int position)
{
    return variables[position];
}

MyVariable *IfElse::get_variable_at(string name)
{
    for(auto item: variables)
    {
        if(item->get_name() == name)
        {
           return item;
        }
    }
   return nullptr;
}

//новая функция проверяет существует ли Данная локальная переменная уже в списке или нет
//Если нет то создает локальную переменную с именем
bool IfElse::add_variables_to_vector(string& name)
{


    MyVariable* item_find = nullptr;
    do{
        item_find = func_->get_variable_at(name);
        if(item_find != nullptr)
        {
            name+="@"+to_string(pos_);
        }
    }while(item_find != nullptr);

    func_->add_variable();

    auto variable = func_->get_variable_at(func_->variables.size()-1);
    variable->set_name(name); //установка имени


//    if(!variables.empty())
//    {
//        for(auto item: variables)
//        {
//            if(item->get_name() == name)
//            {
//                return false; // уже есть данная переменная
//            }
//        }
//    }

    add_variable(variable);
    //get_variable_at(variables.size()-1)->set_name(name); //установка имени

    return true;
}

bool IfElse::add_variables_type(string type)
{
    get_variable_at(variables.size()-1)->set_type(type); //установка типа
}

bool IfElse::add_variables_expression(string expression)
{
    get_variable_at(variables.size()-1)->set_expression(expression); //установка результата
}


