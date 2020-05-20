#include "recordhardtype.h"
#include "my/myvariable.h"

RecordHardType::RecordHardType()
{
    //vectorMap_variables_nameType.clear();
    nameHardType = "";
    //vectorNameThisLabel.clear();
    //nameThisType = "";
    //unionNameThisType = "";
    local = "";

    //новое
    flag_local_ = false;
}

void RecordHardType::addNameHardType(string name)
{
    nameHardType = name;
}

//void RecordHardType::addVectorNameThisLabel(vector<string> data)
//{
//    //дописать проверку на уже имеющуюся переменую в данном типе

//    for(auto item : data)
//    {
//        vectorNameThisLabel.push_back(item);
//    }
//}

//bool RecordHardType::addVectorNameThisLabel(string data)
//{
//    if(vectorNameThisLabel.size() > 0)
//    {
//        for(auto item : vectorNameThisLabel)
//        {
//            if(item == data)
//            {
//                return false;
//            }
//        }
//    }
//    vectorNameThisLabel.push_back(data);

//    return true;
//}

//void RecordHardType::addNameThisType(string data)
//{
//    nameThisType = data;
//}

//void RecordHardType::addNewVariables()
//{
//    for(int i=0; i < vectorNameThisLabel.size();i++)
//    {
//        map<string,string> map_nameType;
//        map_nameType.insert(make_pair(vectorNameThisLabel[i],nameThisType));
//        vectorMap_variables_nameType.push_back(map_nameType);
//    }

//    nameThisType = "";
//    vectorNameThisLabel.clear();
//}

//void RecordHardType::addUnionVectorNameThisLabel(vector<string> data)
//{
//    //дописать проверку на уже имеющуюся переменую в данном типе

//    for(auto item : data)
//    {
//        union_vectorNameThisLabel.push_back(item);
//    }
//}

//bool RecordHardType::addUnionVectorNameThisLabel(string data)
//{
//    if(union_vectorNameThisLabel.size() > 0)
//    {
//        for(auto item : union_vectorNameThisLabel)
//        {
//            if(item == data)
//            {
//                return false;
//            }
//        }
//    }
//    union_vectorNameThisLabel.push_back(data);

//    return true;
//}

//void RecordHardType::addUnionNewVariables()
//{
//    for(int i=0; i < union_vectorNameThisLabel.size();i++)
//    {
//        map<string,string> map_nameType;
//        map_nameType.insert(make_pair(union_vectorNameThisLabel[i],unionNameThisType));
//        union_variables_nameType.push_back(map_nameType);
//    }

//    unionNameThisType = "";
//    union_vectorNameThisLabel.clear();
//}

//void RecordHardType::addUnioNameThisType(string data)
//{
//    unionNameThisType = data;
//}


string RecordHardType::getNameHardType()
{
    return nameHardType;
}

string RecordHardType::getLocal()
{
    return local;
}

//vector<map<string, string> > RecordHardType::getVariables()
//{
//    return vectorMap_variables_nameType;
//}

//vector<map<string, string> > RecordHardType::getVariablesRecord()
//{
//    return union_variables_nameType;
//}

void RecordHardType::setLocal(string data)
{
    local = data;
    flag_local_ = true;
}


/////

void RecordHardType::add_variable()
{
    variables_.push_back(new MyVariable);
}

MyVariable *RecordHardType::get_variable_at(int position)
{
    return variables_[position];
}

//новая функция проверяет существует ли Данная локальная переменная уже в списке или нет
//Если нет то создает локальную переменную с именем
bool RecordHardType::add_variables_to_vector(string name)
{
    if(!variables_.empty())
    {
        for(auto item: variables_)
        {
            if(item->get_name() == name)
            {
                return false; // уже есть данная переменная
            }
        }
    }

    add_variable();
    get_variable_at(variables_.size()-1)->set_name(name); //установка имени

    return true;
}

bool RecordHardType::add_variables_type(string type)
{
    get_variable_at(variables_.size()-1)->set_type(type); //установка типа
}

bool RecordHardType::add_variables_expression(string expression)
{
    get_variable_at(variables_.size()-1)->set_expression(expression); //установка результата
}

void RecordHardType::add_variable_union()
{
    variables_union_.push_back(new MyVariable);
}

MyVariable *RecordHardType::get_variable_union_at(int position)
{
    return variables_union_[position];
}

bool RecordHardType::add_variables_union_to_vector(string name)
{
    if(!variables_union_.empty())
    {
        for(auto item: variables_union_)
        {
            if(item->get_name() == name)
            {
                return false; // уже есть данная переменная
            }
        }
    }

    add_variable_union();
    get_variable_union_at(variables_union_.size()-1)->set_name(name); //установка имени

    return true;
}

bool RecordHardType::add_variables_union_type(string type)
{
  get_variable_union_at(variables_union_.size()-1)->set_type(type); //установка типа
}

bool RecordHardType::add_variables_union_expression(string expression)
{
    get_variable_union_at(variables_union_.size()-1)->set_expression(expression); //установка результата
}

bool RecordHardType::find_init_variables_union(string name)
{
    if(!variables_union_.empty())
    {
        for(auto item: variables_union_)
        {
            if(item->get_name() == name)
            {
                return true; // уже есть данная переменная
            }
        }
    }

    return false;
}

int RecordHardType::get_count_union()
{
     return variables_union_.size();
}

bool RecordHardType::find_init_variables(string name)
{
    if(!variables_.empty())
    {
        for(auto item: variables_)
        {
            if(item->get_name() == name)
            {
                return true; // уже есть данная переменная
            }
        }
    }

    return false;
}

int RecordHardType::get_count()
{
    return variables_.size();
}

