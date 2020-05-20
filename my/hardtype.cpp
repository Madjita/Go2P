#include "hardtype.h"
#include "my/myvariable.h"

HardType::HardType()
{
    nameHardType = "";
}

void HardType::addNameHardType(string name)
{
    nameHardType = name;
}

string HardType::getNameHardType()
{
    return nameHardType;
}

void HardType::add_variable()
{
    variables.push_back(new MyVariable);
}

MyVariable *HardType::get_variable_at(int position)
{
    return variables[position];
}

MyVariable *HardType::get_variable_at(string name)
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
bool HardType::add_variables_to_vector(string name)
{
    if(!variables.empty())
    {
        for(auto item: variables)
        {
            if(item->get_name() == name)
            {
                return false; // уже есть данная переменная
            }
        }
    }

    add_variable();
    get_variable_at(variables.size()-1)->set_name(name); //установка имени

    return true;
}

bool HardType::add_variables_type(string type)
{
    get_variable_at(variables.size()-1)->set_type(type); //установка типа
}

bool HardType::add_variables_expression(string expression)
{
    get_variable_at(variables.size()-1)->set_expression(expression); //установка результата
}

bool HardType::find_init_variables(string name)
{
    if(!variables.empty())
    {
        for(auto item: variables)
        {
            if(item->get_name() == name)
            {
                return true; // уже есть данная переменная
            }
        }
    }

    return false;
}

int HardType::get_count()
{
    return variables.size();
}

bool HardType::set_variable_struct(HardType *newObject)
{
    get_variable_at(variables.size()-1)->set_variable_struct(newObject);
}

HardType *HardType::clone()
{
    auto newObject = new HardType(*this);

    for(int i=0; i < newObject->variables.size();i++)
    {
        newObject->variables[i] = variables[i]->clone();
    }

    return newObject;
}

void HardType::add_vvMap_KeyData(vector<vector<map<LexClass, string> > >& set_vvMap_KeyData, int start)
{
    for( auto it = set_vvMap_KeyData.begin()+start; it != set_vvMap_KeyData.end();it++)
    {
        vvMap_KeyData.push_back(*it);
    }
}
