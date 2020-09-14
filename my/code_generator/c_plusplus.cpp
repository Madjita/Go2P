#include "c_plusplus.h"

C_PlusPlus::C_PlusPlus(CodeGenerator* parent)
{
    fileOut2.open("C_PlusPlus.txt", ios::binary | ios::trunc);
    fileOut2.close();

    fileOut2.open("C_PlusPlus.txt", ios::binary | ios::app); //| ios::trunc
    str = "";
}
/*
bool C_PlusPlus::generate(vector<INSTRUCTION *> vector_polish)
{
   int tabs = 0;
   vector<vector<string>> row_str;

   string str;


   //
   vector<INSTRUCTION*> vector_func_args;

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

               vector<string> column;
               column.push_back(str);
               row_str.push_back(column);
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
       default:break;
       }

   }
}
*/

bool C_PlusPlus::generate() //vector<INSTRUCTION *> vector_polish
{

    vector<INSTRUCTION*> vector_return;
    stack<INSTRUCTION*> stack_expression;

    stack<stack<INSTRUCTION*>*> loop;

    position = 0;

    while (vector_polish.size() > position) {
        str += worker(&stack_expression,position);
    }


    /*
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
        case loop_short_init:
        {
            int j = str.size();
            stack<char> find_str;
            while(1)
            {

                if(str[j] == ' ')
                {
                    while(!find_str.empty())
                    {
                        find_str.pop();
                    }

                    j--;
                    continue;
                }

                if(reverse(find_str) == "for")
                {
                    j += 5; // позиция вставки левой скобки
                    str.insert(j,"( ");
                    j += 2; // позиция удаления фигурной скобки и переноса строки
                    str.erase(j,2);

                    //Удаление лишних табуляций
                    while(str[j] == '\t')
                    {
                        str.erase(j,1);
                    }

                    break;
                }
                find_str.push(str[j]);

                j--;

            }
            break;
        }
        case gotoOpcFor:
        {

            if(!stack_expression.empty())
            {
                auto prev_item = vector_polish[i-1];
                str += expression(prev_item);
                str += " ;";
                str += ") {";
                str += "\n";
            }

            tabs--;

            for(int i=0; i < tabs; i++)
            {
                str+= "\t";
            }

            str += "}\n";

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

        case ifZOpc_for:
        {
            str.erase(str.size()-1,1);
            str +=" ";
            auto prev_item = vector_polish[i-1];
            str += expression(prev_item);
            str += ";";
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
        case incrimentOpc:
        case minusUnOpc:
        case plusUnOpc:
        {
            stack_expression.push(item);
            break;
        }

        case equalOpc:
        {
            for(int i=0; i < tabs; i++)
            {
                str+= "\t";
            }

            str += item->rez->val.var->get_name()+" ";
            str += "= ";

            if(stack_expression.empty())
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
                default:break;
                }

            }
            else
            {
                //expression
                expression(str,stack_expression,item->arg1->val.varTmp->name);
            }



            str += ";";
            str += "\n";

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

            if(stack_expression.empty())
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
                default:break;
                }

            }
            else
            {
                //expression
                expression(str,stack_expression,item->arg1->val.varTmp->name);
            }



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

        case var_statement_one:{

            for(int i=0; i < tabs; i++)
            {
                str+= "\t";
            }

            string type = item->rez->val.var->get_type();

            if(type == "int")
            {
                str += "int ";
            }

            if(type == "string")
            {
                str += "string ";
            }

            str += item->rez->val.var->get_name()+" ";

            if(type == "int")
            {
                str +="= 0;";
            }

            if(type == "string")
            {
                str +="= \" \";";
            }

            str +="\n";

            break;
        }
        default:
            break;
        }

    }
   */

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

    if(item->arg2 != nullptr)
    {
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
    }
    return str;
}

string C_PlusPlus::reverse(stack<char> stroka)
{
    string str = "";

    while(!stroka.empty())
    {
        str += stroka.top();
        stroka.pop();
    }

    return str;
}

string C_PlusPlus::worker(stack<INSTRUCTION *>* stack_expression, int n)
{
    vector<INSTRUCTION*> vector_func_args;
    vector<INSTRUCTION*> vector_return;


    string code = "";
    int position_loop_begin = 0;

    INSTRUCTION* item = vector_polish[n];

    switch (item->opc)
    {
    case callFunc_Begin:{
        //Берем возвращяемое значение функции
        auto return_type = item->rez->val.fanc->getReturnFuncType();

        vector<INSTRUCTION*> vector_func_args;

        if(return_type != ""){
            code += return_type+" ";
        }
        else {
            code += "void ";
        }

        //Вставляем имя функции
        code += item->rez->val.fanc->getNameFuncType()+" ";
        code += "(";

        position++;
        code += processing_func_arg(position);
        code.erase(code.size()-1,code.size());

        code+= " ";
        code+= ")";
        code+= "\n";
        code+= "{";
        code+= "\n";

        tabs++;

        break;
    }
    case returnOpc:
    {
        for(int i=0; i < tabs; i++)
        {
            code+= "\t";
        }


        code+= "return ";

        switch (item->rez->typ)
        {
        case constOpd:
            switch (item->rez->val.cons->typ)
            {
            case INTTYP:
                code+= to_string(item->rez->val.cons->val.unum);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        code += ";";
        code += "\n";
        position++;
        break;
    }
    case callFunc_End:{
        position++;

        tabs--;
        code += "}\n";
        code += "\n";
        break;
    }
    case loop_begin:{
        stack<INSTRUCTION *>* stack_expression = new stack<INSTRUCTION *>();
        position_loop_begin = position;

        for(int i=0; i < tabs; i++)
        {
            code+= "\t";
        }

        code += "for ";
        code += "( ";

        position++;

        tabs++;



        int position_str_add = 0;
        string code_add = "";
        while(item->opc != loop_end)
        {
            loop(stack_expression,position,position_str_add,code_add);
            item = vector_polish[position];
        }

        code += code_add;

        tabs--;
        code += "\n";
        for(int i=0; i < tabs; i++)
        {
            code+= "\t";
        }

        code += "}";
        code += "\n";


        position++;

        delete stack_expression;

        break;
    }

        ////
    case LCircleOpc:
    case RCircleOpc:
    case slashOpc:
    case starOpc:
    case minusOpc:
    case plusOpc:
    case incrimentOpc:
    case minusUnOpc:
    case plusUnOpc:
    {

        code += expression(stack_expression,position);

        //stack_expression->push(item);
        position++;
        break;
    }
        ////
    case assignOpc:
    {
        for(int i=0; i < tabs; i++)
        {
            str+= "\t";
        }

        code += "auto ";
        code += item->rez->val.var->get_name()+" ";
        code += "= ";

        if(stack_expression->empty())
        {
            switch (item->arg1->typ)
            {
            case nameVarOpd:
            {
                code += item->arg1->val.var->get_name();
                break;
            }
            case constOpd:
            {

                switch (item->arg1->val.cons->typ)
                {
                case INTTYP:
                    code += to_string(item->arg1->val.cons->val.unum);
                    break;

                default:break;
                }
                break;
            }
            default:break;
            }

        }
        else
        {
            //expression
            expression(code,*stack_expression,item->arg1->val.varTmp->name);
        }

        code += ";";
        code += "\n";

        position++;

        break;
    }
        ///
    default:
        position++;
        break;
    }


    return code;
}

string C_PlusPlus::processing_func_arg(int n)
{
    stack<INSTRUCTION*> stack_expression;

    string code = "";

    INSTRUCTION* item = vector_polish[position];

    switch (item->opc)
    {
    case param:{

        code += " ";

        auto item_arg = item->rez->val.var;
        code += item_arg->get_type();
        code += " ";
        code += item_arg->get_name();

        position++;
        code +=",";
        code += processing_func_arg(position);

        break;
    }

    default:
        break;
    }

    return code;
}

string C_PlusPlus::loop(stack<INSTRUCTION*>* stack_expression,int n,int& position_str_add,string& code)
{


    int position_loop_begin = 0;

    INSTRUCTION* item = vector_polish[n];

    switch (item->opc)
    {
    case loop_begin:{
        stack<INSTRUCTION*> stack_expression_2;
        position_loop_begin = position;
        int position_str_add_2 = 0;

        code += "for ";
        code += "( ";


        position++;
        tabs++;

        string code_add = "";

        int position_str_add = 0;
        while(item->opc != loop_end)
        {
            loop(&stack_expression_2,position,position_str_add_2,code_add);
            item = vector_polish[position];
        }

        tabs--;
        code_add += "\n";
        for(int i=0; i < tabs; i++)
        {
            code_add+= "\t";
        }

        code_add += "}";
        code_add += "\n";

        code += code_add;
        position++;
        break;
    }
    case loop_end:{
        position++;
        break;
    }
        ////
    case largerEQOpc:
    case smallerEQOpc:
    case largerOpc:
    case smallerOpc:
    case LCircleOpc:
    case RCircleOpc:
    case slashOpc:
    case starOpc:
    case minusOpc:
    case plusOpc:
    case incrimentOpc:
    case minusUnOpc:
    case plusUnOpc:
    {
        code += expression(stack_expression,position);

        if(code != "")
        {
            code += "\n";
            for(int i=0; i < tabs; i++)
            {
                code+= "\t";
            }
        }

        //position++;
        break;
    }
        ////
    case equalOpc:
    {
        code += expression(stack_expression,position);
        //stack_expression->push(item);
        position++;
        break;
    }
    case assignOpc:
    {

        code += expression(stack_expression,position);

        position++;
        break;
    }
        ///
    case loop_short_init:
    {
        position++;
        code += expression(stack_expression,position);
        code += " ";
        break;
    }

    case ifZOpc_for:
    {

        if(stack_expression->empty())
        {
            switch (item->arg1->typ)
            {
            case nameVarOpd:
            {
                code += item->arg1->val.var->get_name();
                break;
            }
            case constOpd:
            {

                switch (item->arg1->val.cons->typ)
                {
                case INTTYP:
                    code += to_string(item->arg1->val.cons->val.unum);
                    break;

                default:break;
                }
                break;
            }
            default:break;
            }

        }
        else
        {
            //expression
            expression(code,*stack_expression,item->arg1->val.varTmp->name);
        }

        code += ";";
        code += " ";
        position_str_add = code.size();
        code += ")";
        code += "\n";
        for(int i=0; i < tabs-1; i++)
        {
            code+= "\t";
        }
        code += "{";
        code += "\n";

        for(int i=0; i < tabs; i++)
        {
            code+= "\t";
        }
        position++;
        break;
    }
    case gotoOpcFor:
    {

        string code_add = "";

        if(stack_expression->empty())
        {
            int find_position = 0;
            int find_semicolon = 0;

            for(int i= code.size(); i > 0; i--)
            {
                if(code[i] ==';' || code[i] =='}')
                {
                    find_semicolon++;
                    if(find_semicolon == 2)
                    {
                        find_position = i+1;
                        break;
                    }
                }

                if(code[i] == '{')
                {
                    break;
                }
            }

            if(find_position > 0)
            {
                for(int i=find_position; i < code.size();i++)
                {
                    if(code[i] == '\n' || code[i] == '\t' )
                    {

                    }
                    else
                    {
                        code_add +=code[i];
                    }
                }
                code.erase(find_position);
            }
        }
        else
        {
            if(stack_expression->size() == 1)
            {
                auto it = stack_expression[0].top();
                if ( it->opc == incrimentOpc)
                {
                    code_add += it->arg1->val.var->get_name();
                    code_add += "++";
                    code_add += "; ";
                }
            }
            else
            {
                code_add += expression(stack_expression,position);
            }

        }


        code.insert(position_str_add,code_add);
        position++;
        break;
    }

    default:{
        position++;
        break;
    }
    }

    return code;
}



string C_PlusPlus::expression(stack<INSTRUCTION*>* stack_expression,int n)
{
    string code = "";

    INSTRUCTION* item = vector_polish[n];

    switch (item->opc)
    {
    ////
    case smallerOpc:
    case LCircleOpc:
    case RCircleOpc:
    case slashOpc:
    case starOpc:
    case minusOpc:
    case plusOpc:
    case incrimentOpc:
    case minusUnOpc:
    case plusUnOpc:
    {
        stack_expression->push(item);
        position++;

        code += expression(stack_expression,position);

        break;
    }
        ///
    case equalOpc:
    {
        code += item->rez->val.var->get_name()+" ";
        code += "= ";

        if(stack_expression->empty())
        {
            switch (item->arg1->typ)
            {
            case nameVarOpd:
            {
                code += item->arg1->val.var->get_name();
                break;
            }
            case constOpd:
            {

                switch (item->arg1->val.cons->typ)
                {
                case INTTYP:
                    code += to_string(item->arg1->val.cons->val.unum);
                    break;

                default:break;
                }
                break;
            }
            default:break;
            }

        }
        else
        {
            //expression
            expression(code,*stack_expression,item->arg1->val.varTmp->name);
        }

        code += ";";
        position++;

        break;
    }
        ///
        ////
    case assignOpc:
    {


        code += "auto ";
        code += item->rez->val.var->get_name()+" ";
        code += "= ";

        if(stack_expression->empty())
        {
            switch (item->arg1->typ)
            {
            case nameVarOpd:
            {
                code += item->arg1->val.var->get_name();
                break;
            }
            case constOpd:
            {

                switch (item->arg1->val.cons->typ)
                {
                case INTTYP:
                    code += to_string(item->arg1->val.cons->val.unum);
                    break;

                default:break;
                }
                break;
            }
            default:break;
            }

        }
        else
        {
            //expression
            expression(code,*stack_expression,item->arg1->val.varTmp->name);
        }

        code += ";";


        break;
    }
        ///
    case gotoOpcFor:
    {
        item = stack_expression->top();
        code += item->rez->val.var->get_name()+" ";
        code += "= ";

        if(stack_expression->empty())
        {
            switch (item->arg1->typ)
            {
            case nameVarOpd:
            {
                code += item->arg1->val.var->get_name();
                break;
            }
            case constOpd:
            {

                switch (item->arg1->val.cons->typ)
                {
                case INTTYP:
                    code += to_string(item->arg1->val.cons->val.unum);
                    break;

                default:break;
                }
                break;
            }
            default:break;
            }

        }
        else
        {
            //expression
            expression(code,*stack_expression,item->arg1->val.varTmp->name);
        }

        code += ";";

        break;
    }
    default:
        break;
    }

    return code;
}

bool C_PlusPlus::get_opcType(INSTRUCTION *item)
{
    switch (item->opc)
    {
    case starOpc:            // умножение '*'
    case plusOpc:            //плюс '+'
    case minusOpc:           //минус '-'
    case minusUnOpc:         //унарный минус '-'
    case plusUnOpc:         //унарный плюс '+'
    case dicrimentOpc:       // декремент '−−'
    case incrimentOpc:       //инкремент '++'
    case modOpc:             //остаток от деления '%'
    case slashOpc:           // деление '/'
    case assignOpc:			// авто присваивание ':='
    case equalOpc:			// присвоение '='
    case notEqualOpc:		// не равно '#'
    case smallerOpc:			// меньше '<'
    case largerOpc:			// больше '>'
    case smallerEQOpc:		// меньше или равно '<='
    case largerEQOpc:		// больше или равно '>='
    case eqOpc:              // равенство '=='
    case noValueOpc:			// нет значения ''
    case impersantOpc:		// имперсант '&'
    case LCircleOpc:			// круглые скобки '('
    case RCircleOpc:			// круглые скобки ')'
    {
        return true;
    }
    default:
        return false;
    }
}

