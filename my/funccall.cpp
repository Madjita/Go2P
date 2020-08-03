#include "funccall.h"


#include "my/functype.h"

FuncCall::FuncCall()
{

}

FuncCall::FuncCall(FuncType *call_func_type)
{
    call_func_type_ = call_func_type;
}

string FuncCall::get_name_call_func()
{
    return call_func_type_->getNameFuncType();
}
