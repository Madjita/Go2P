#include "functype.h"

#include "my/ifelse.h"

FuncType::FuncType()
{

}

void FuncType::addNameFuncType(string name)
{
    nameFun = name;
}

void FuncType::addNameFuncReturnType(string name)
{
    if(returnType == "*" || returnType == "&")
    {
        returnType +=name;
    }
    else
    {
        returnType = name;
    }
}

void FuncType::addVectorNameThisLabel(vector<string> data)
{
    //дописать проверку на уже имеющуюся переменую в данном типе

    for(auto item : data)
    {
        vectorNameThisLabel.push_back(item);
    }
}

bool FuncType::addVectorNameThisLabel(string data)
{
    for(auto item : vectorNameThisLabel)
    {
        if(item == data)
        {
            return false;
        }
    }
    vectorNameThisLabel.push_back(data);
    return true;
}

void FuncType::addNameThisType(string data)
{
    nameThisType = data;
}

void FuncType::addNewVariables()
{
    for(int i=0; i < vectorNameThisLabel.size();i++)
    {

        if(nameThisType == "auto")
        {
            if(expression.getResult().rbegin()->first == lcIntNum)
            {
                nameThisType = "int";
            }
        }

        map<string,string> map_nameType;
        map_nameType.insert(make_pair(vectorNameThisLabel[i],nameThisType));
        vectorMap_variables_nameType.push_back(map_nameType);

        map<map<string,string>,VariableExeption> item;
        item.insert(make_pair(map_nameType,expression));
        vectorNameThisLabel_example.push_back(item);

    }

    nameThisType = "";
    vectorNameThisLabel.clear();

}

void FuncType::initVariables(string name)
{
    vector<string> split;
    string buffer = "";

    for(int i = 0; i < name.size();i++)
    {
        buffer += name[i];
        if(name[i] == '.')
        {
            split.push_back(buffer);
            buffer.clear();
        }
    }

    // думаю как лучше сделать заполнение результатов переменных в структуре или сложных типов
    if(split.empty())
    {
        for(auto& vecMap: vectorNameThisLabel_example)
        {
            for (auto& map: vecMap.begin()->first)
            {
                if(map.first == name)
                {
                    (*vecMap.begin()).second = expression;
                    break;
                }
            }
        }

    }
    else
    {
        for(auto& vecMap: vectorNameThisLabel_example)
        {
            for (auto& map: vecMap.begin()->first)
            {
                if(map.second == split[0])
                {
                    (*vecMap.begin()).second = expression;
                    break;
                }
            }
        }
    }

}

string FuncType::getNameFuncType()
{
    return nameFun;
}

string FuncType::getReturnFuncType()
{
    return returnType;
}

vector<map<string, string> > FuncType::getVariables()
{
    return vectorMap_variables_nameType;
}

//Пробую функцию
vector<map<map<string, string>, VariableExeption> > FuncType::getVariables2()
{
    return vectorNameThisLabel_example;
}

void FuncType::setExpression(vector<map<LexClass, string> > _expression)
{

    expression.setExpression(_expression);
}

bool FuncType::findInitStatment(string variable)
{
    for(auto item : vectorMap_variables_nameType)
    {
        if(item.begin()->first == variable)
        {
            return true;
        }
    }

    return false;
}


string FuncType::getInitStatment_type(string variable)
{
    for(auto item : vectorMap_variables_nameType)
    {
        if(item.begin()->first == variable)
        {
            return item.begin()->second;
        }
    }

    return "undef";
}

//новая функция проверяет существует ли Данная локальная переменная уже в списке или нет
//Если нет то создает локальную переменную с именем
bool FuncType::add_variables_to_vector(string name)
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

bool FuncType::add_variables_type(string type)
{

    for(int i=0; i < variables.size();i++)
    {
        if(get_variable_at(i)->get_type() == "")
        {
            get_variable_at(i)->set_type(type); //установка типа
        }
    }

    //get_variable_at(variables.size()-1)->set_type(type); //установка типа
}

bool FuncType::add_variables_expression(string expression)
{
    get_variable_at(variables.size()-1)->set_expression(expression); //установка результата
}

bool FuncType::set_variable_struct(HardType *newObject)
{
    get_variable_at(variables.size()-1)->set_variable_struct(newObject);
}

bool FuncType::set_variable_record(RecordHardType *newObject)
{
    get_variable_at(variables.size()-1)->set_variable_record(newObject);
}


void FuncType::add_variable()
{
    variables.push_back(new MyVariable(varLocal,this));
}

void FuncType::remove_variable(MyVariable* item)
{
    for (int i=0;i < variables.size();i++)
    {
        if(variables[i] == item)
        {
            variables.erase(variables.begin()+i);
        }
    }
}

////Мы не знаем какие локальные переменные нужно удалять (либо ввести признак локальности)
//void FuncType::variable_pop_back()
//{
//    MyVariable* p = variables[variables.size()-1];
//    //delete p;
//    variables.pop_back();
//}

MyVariable *FuncType::get_variable_at(int position)
{
    return variables[position];
}

MyVariable *FuncType::get_variable_at(string name)
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

MyVariable *FuncType::get_variable_las()
{
    return variables[variables.size()-1];
}

void FuncType::add_vvMap_KeyData(vector<vector<map<LexClass, string> > >& set_vvMap_KeyData, int start)
{
    for( auto it = set_vvMap_KeyData.begin()+start; it != set_vvMap_KeyData.end();it++)
    {
        vvMap_KeyData.push_back(*it);
    }
}

IfElse *FuncType::get_if_else(int position)
{
    return if_else[position];
}

void FuncType::add_callFun_name(FuncCall *call_func)
{
    vectorCallFun.push_back(call_func);
}

void FuncType::add_new_if_else()
{
    IfElse* last_item = nullptr;
    if(get_if_else_size() > 0)
    {
        last_item = if_else[get_if_else_size()];
    }
    if_else.push_back(new IfElse(last_item,this,0));
}

void FuncType::remove_new_if_else()
{
    if_else.pop_back();
}

