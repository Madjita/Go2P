#include "myvariable.h"

#include "my/table.h"

MyVariable::MyVariable(varType var, void* parent)
{
    parent_ = nullptr;
    variable_struct_ = nullptr;
    variable_record_ = nullptr;
    expression_ = "undef";
}

MyVariable::MyVariable(MyVariable* parent)
{
//    parent_.var = nullptr;
//    parent_.hardType = nullptr;
//    parent_.recordType = nullptr;
//    parent_.varGlobal = nullptr;

//    switch (var)
//    {
//        case varGlobal:
//                parent_.var = static_cast<MyVariable*>(parent);
//        break;
//        case varLocal:
//                parent_.var = nullptr;
//        break;
//        case varHardType:
//            parent_.hardType = static_cast<HardType*>(parent);
//        break;
//        case varRecordType:
//            parent_.recordType = static_cast<RecordHardType*>(parent);
//        break;
//    }

    parent_ = parent;

    variable_struct_ = nullptr;
    variable_record_ = nullptr;

    expression_ = "undef";
}

MyVariable::~MyVariable()
{
    cout << "DELETE : " << name_ <<endl;
    delete this;
}

void MyVariable::set_lex(LexClass lc)
{
    lc_ = lc;
}

void MyVariable::set_name(string name) {
    name_ = name;
}

void MyVariable::set_lex_type(LexClass lc_type)
{
    lc_type_ = lc_type;
}

void MyVariable::set_type(string type) {
    type_ = type;

    if(type_ == "int")
    {
        expression_ = "0";
    }
    if(type_ == "float")
    {
        expression_ = "0.0";
    }
}

void MyVariable::set_expression(string expression) {
    expression_ = expression;
}

void MyVariable::set_variable_struct(HardType *newObject)
{
    variable_struct_ = newObject;
}

void MyVariable::set_variable_record(RecordHardType *newObject)
{
    variable_record_ = newObject;
}

MyVariable *MyVariable::get_variable(string name)
{
    auto object_struct = variable_struct_->get_variable_at(name);
    if(object_struct != nullptr)
    {
        return object_struct;
    }

//    auto object_record = variable_record_->get_variable_at(name);
//    if(object_record != nullptr)
//    {
//        return object_record;
//    }

    return nullptr;
}


LexClass MyVariable::get_lex()
{
    return lc_;
}

string MyVariable::get_name() {
    return name_;
}


LexClass MyVariable::get_lex_type() {
    return lc_type_;
}

string MyVariable::get_type() {
    return type_;
}

string MyVariable::get_expression() {
    return expression_;
}

MyVariable *MyVariable::clone()
{
    return new MyVariable(*this);
}

bool MyVariable::set_parent_var(MyVariable *parent)
{
    parent_ = parent;
}

bool MyVariable::parent_empty()
{
    if(parent_ == nullptr)
    {
        return true;
    }

    return false;
}

MyVariable *MyVariable::parent_get()
{
    return parent_;
}

void MyVariable::add_vvMap_KeyData(vector<vector<map<LexClass, string> > >& set_vvMap_KeyData, int start)
{
    for( auto it = set_vvMap_KeyData.begin()+start; it != set_vvMap_KeyData.end();it++)
    {
           vvMap_KeyData.push_back(*it);
    }
}


