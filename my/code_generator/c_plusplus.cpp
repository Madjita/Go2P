#include "c_plusplus.h"

C_PlusPlus::C_PlusPlus(CodeGenerator* parent)
{
    fileOut2.open("C_PlusPlus.txt", ios::binary | ios::trunc);
    fileOut2.close();

    fileOut2.open("C_PlusPlus.txt", ios::binary | ios::app); //| ios::trunc
    str = "";
}

bool C_PlusPlus::generate(vector<INSTRUCTION *> vector_polish)
{

    vector<INSTRUCTION*> vector_func_args;
    vector<INSTRUCTION*> vector_return;
    stack<INSTRUCTION*> stack_expression;

    stack<stack<INSTRUCTION*>*> loop;


    int tabs = 0;

    for(int i =0 ; i < vector_polish.size();i++)
    {
        auto item = vector_polish[i];

        switch (item->opc)
        {
        case param:{
            vector_func_args.push_back(item);
            break;
        }


        case callFunc_Begin:{
            //Берем возвращяемое значение функции
            auto return_type = item->rez->val.fanc->getReturnFuncType();

            if(return_type != ""){
                str += return_type+" ";
            }
            else {
                str += "void ";
            }

            //Вставляем имя функции
            str += item->rez->val.fanc->getNameFuncType()+" ";
            str += "(";

            if(!vector_func_args.empty())
            {
                str += " ";

                for(int i=0; i < vector_func_args.size();i++)
                {
                    auto item_arg = vector_func_args[i]->rez->val.var;
                    str += item_arg->get_type();
                    str += " ";
                    str += item_arg->get_name();

                    if(i+1 != vector_func_args.size())
                        str += ", ";

                    //Находим локальную переменную
                    //auto item_local = item->rez->val.fanc->get_variable_at();
                }

                vector_func_args.clear();
            }

            str+= ")";
            str+= "\n";
            str+= "{";
            str+= "\n";

            tabs++;


            break;
        }
        case returnOpc:
        {
            for(int i=0; i < tabs; i++)
            {
                str+= "\t";
            }


            str+= "return ";

            switch (item->rez->typ)
            {
            case constOpd:
                switch (item->rez->val.cons->typ)
                {
                case INTTYP:
                    str+= to_string(item->rez->val.cons->val.unum);
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
            str += ";";
            str += "\n";

            break;
        }
        case callFunc_End:{
            tabs--;
            str+= "}\n";
            str+= "\n";
            break;
        }
        case loop_begin:{

            for(int i=0; i < tabs; i++)
            {
                str+= "\t";
            }

            str += "for {";
            str += "\n";
            tabs++;


            break;
        }
        case gotoOpcFor_infinity:
        {
            tabs--;

            for(int i=0; i < tabs; i++)
            {
                str+= "\t";
            }

            str += "}\n";

            break;
        }
        case ifZOpc:
        {
            for(int i=0; i < tabs; i++)
            {
                str+= "\t";
            }

            str += "if ( ";

            auto prev_item = vector_polish[i-1];

            str += expression(prev_item);

            str += " )";
            str += "\n";

            for(int i=0; i < tabs; i++)
            {
                str+= "\t";
            }

            str += "{";
            str += "\n";
            tabs++;

            break;
        }
        case gotoOpcIfZ_false:
        case gotoOpcIfZ_true:
        {
            tabs--;
            for(int i=0; i < tabs; i++)
            {
                str+= "\t";
            }
            str += "}";
            str += "\n";

            break;
        }
        case elseOpc:
        {
            for(int i=0; i < tabs; i++)
            {
                str+= "\t";
            }

            str += "else";
            str += "\n";

            for(int i=0; i < tabs; i++)
            {
                str+= "\t";
            }

            str += "{";
            str += "\n";
            tabs++;


            break;
        }
        case smallerOpc:
        {
            break;
        }
        case LCircleOpc:
        case RCircleOpc:
        case slashOpc:
        case starOpc:
        case minusOpc:
        case plusOpc:
        case minusUnOpc:
        case plusUnOpc:
        {
            stack_expression.push(item);
            break;
        }
        case assignOpc:
        {
            for(int i=0; i < tabs; i++)
            {
                str+= "\t";
            }

            str += "auto ";
            str += item->rez->val.var->get_name()+" ";
            str += "= ";
            //expression
            expression(str,stack_expression,item->arg1->val.varTmp->name);


            str += ";";
            str += "\n";

            break;
        }

        case gotoOpcIf_falseBreak:
        case gotoOpcIf_trueBreak:
        case gotoOpcFor_break:
        {
            for(int i=0; i < tabs; i++)
            {
                str+= "\t";
            }

            str += "break;";
            str += "\n";

            break;
        }
        default:
            break;
        }
    }

    fileOut2 << str;

    fileOut2.close();
}

void C_PlusPlus::expression(string &str, stack<INSTRUCTION *> &stack_expression, string tmp)
{
    stack<INSTRUCTION *> stack_;


    while(!stack_expression.empty())
    {
        auto item = stack_expression.top();


        stack<INSTRUCTION *> copy = stack_expression;


        if(item->rez != nullptr)
        {
            if(item->rez->val.varTmp->name != tmp)
            {
                stack_.push(stack_expression.top());
                stack_expression.pop();
                continue;
            }
        }


        if(item->arg1 != nullptr && item->arg2 != nullptr)
        {
            if(item->arg1->typ == tmpVarOpd && item->arg2->typ == tmpVarOpd)
            {

                copy.pop();

                bool flag = false;
                int priority = item->priority_rang;
                if(priority < copy.top()->priority_rang)
                {
                    str += "(";
                    flag = true;
                }


                expression(str,copy,item->arg1->val.varTmp->name);

                if(flag)
                {
                    str += ")";
                }

            }
            else
            {
                switch (item->arg1->typ)
                {
                case nameVarOpd:
                {
                    str += item->arg1->val.var->get_name();
                    break;
                }
                case constOpd:
                {

                    switch (item->arg1->val.cons->typ)
                    {
                    case INTTYP:
                        str += to_string(item->arg1->val.cons->val.unum);
                        break;

                    default:break;
                    }
                    break;
                }
                case tmpVarOpd:
                {

                    int priority = item->priority_rang;
                    stack_expression.pop();

                    bool flag = false;
                    if(priority < stack_expression.top()->priority_rang)
                    {
                        str += "(";
                        flag = true;
                    }


                    expression(str,stack_expression,item->arg1->val.varTmp->name);

                    if(flag)
                    {
                        str += ")";
                    }

                    break;
                }
                default:break;
                }

            }

        }

        str += Polish_notation::opc(item->opc);

        if(item->arg2 != nullptr)
        {
            if(item->arg1->typ == tmpVarOpd && item->arg2->typ == tmpVarOpd)
            {
                bool flag = false;
                int priority = item->priority_rang;
                if(priority < copy.top()->priority_rang)
                {
                    str += "(";
                    flag = true;
                }

                expression(str,copy,item->arg2->val.varTmp->name);

                if(flag)
                {
                    str += ")";
                }

                stack_expression = copy;
            }
            else
            {
                switch (item->arg2->typ)
                {
                case nameVarOpd:
                    str += item->arg2->val.var->get_name();
                    break;
                case constOpd:
                {

                    switch (item->arg2->val.cons->typ)
                    {
                    case INTTYP:
                        str += to_string(item->arg2->val.cons->val.unum);
                        break;

                    default:break;
                    }
                    break;
                }
                case tmpVarOpd:
                {

                    int priority = item->priority_rang;
                    stack_expression.pop();

                    bool flag = false;
                    if(priority < stack_expression.top()->priority_rang)
                    {
                        str += "(";
                        flag = true;
                    }

                    expression(str,stack_expression,item->arg2->val.varTmp->name);

                    if(flag)
                    {
                        str += ")";
                    }

                    break;
                }
                default:break;
                }
            }

        }

        if(item->arg2 != nullptr && item->arg1 != nullptr)
        {
            if(item->arg1->typ != tmpVarOpd && item->arg2->typ != tmpVarOpd)
            {
                stack_expression.pop();
            }
        }


        while(!stack_.empty())
        {
            stack_expression.push(stack_.top());
            stack_.pop();
        }

        return;
    }
}

string C_PlusPlus::expression(INSTRUCTION *item)
{
    string str = "";

    switch (item->arg1->typ)
    {
    case nameVarOpd:
    {
        str += item->arg1->val.var->get_name();
        break;
    }
    case constOpd:
    {
        switch (item->arg1->val.cons->typ)
        {
        case INTTYP:
            str += to_string(item->arg1->val.cons->val.unum);
            break;
        default:break;
        }
        break;
    }
    case tmpVarOpd:
    {
        break;
    }
    default:break;
    }

    str += Polish_notation::opc(item->opc);

    switch (item->arg2->typ)
    {
    case nameVarOpd:
    {
        str += item->arg2->val.var->get_name();
        break;
    }
    case constOpd:
    {
        switch (item->arg2->val.cons->typ)
        {
        case INTTYP:
            str += to_string(item->arg2->val.cons->val.unum);
            break;
        default:break;
        }
        break;
    }
    case tmpVarOpd:
    {
        break;
    }
    default:break;
    }

    return str;
}

