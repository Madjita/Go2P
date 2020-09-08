#include "polish_notation.h"

#include <fstream>

Polish_notation::Polish_notation()
{

}

// добавить объект в стек операций
void Polish_notation::push_operation(opcType var_opc)
{
    //Создадим иснтрукцию
    INSTRUCTION* instruction_ptr = Create_new_inctruction(var_opc);


    bool flag_push = true;
    INSTRUCTION* pop_operation_top = nullptr;

    OPERAND* pop_operand_left = nullptr;
    OPERAND* pop_operand_right = nullptr;

    //Добавляем условие IfZ
    //

    switch (instruction_ptr->opc)
    {

    case gotoOpcIf_trueReturn:
    case gotoOpcIf_falseReturn:
    case gotoOpc_return:
    case gotoOpcFor_return:
    case gotoOpcIf_trueContinue:
    case gotoOpcIf_falseContinue:

        //Добавил новое 08.07.2020
        //Метка завершения положительного условия (прыгнуть на действие полсе выполнения условия)
    case gotoOpcIfZ_true:
    case gotoOpcIfZ_false:
        //Добавил новое 08.07.2020
        //Метка break из истинного условия (прыгнуть в конец цикла)
    case gotoOpcIf_trueBreak:
        //Добавил новое 08.07.2020
        //Метка break из ложного условия (прыгнуть в конец цикла)
    case gotoOpcIf_falseBreak:
        //Добавил новое 26.07.2020
    case gotoOpcFor_infinity:
        //Добавил новое 08.07.2020
        //goto метка для перехода в цикле
    case gotoOpcFor:
        //Добавил новое 08.07.2020
        //goto метка для перехода из цикла for
    case gotoOpcFor_break:
    case gotoOpcFor_continue:
        //просто метка
    case gotoOpc:
    {
        //Создаем правый операнд являющийся меткой (куда прыгнуть)
        OPERAND* tmp = Create_new_operand(labelOpd,nullptr);
        //устанавливаем в трехадресном коде инструкции Левый операнд
        add_left_operand(instruction_ptr,tmp);

        //Записываем уже сформированные трехадресные команды в стек в виде польской обратной записи
        vector_polish.push_back(instruction_ptr);
        //Записываем инструкцию в стек ожидающих определение меток goto
        stack_goto_labels.push(instruction_ptr);

        return;
    }
    case ifZOpc:
    {
        //Достаем левй операнд являющийся результатом условия
        pop_operand_left = pop_operand();
        //Создаем правый операнд являющийся меткой (куда прыгнуть)
        OPERAND* tmp = Create_new_operand(labelOpd,nullptr);
        pop_operand_right = tmp;
        //устанавливаем в трехадресном коде инструкции Левый операнд
        add_left_operand(instruction_ptr,pop_operand_left);
        //устанавливаем в трехадресном коде инструкции Правый операнд
        add_right_operand(instruction_ptr,pop_operand_right);

        //Записываем уже сформированные трехадресные команды в стек в виде польской обратной записи
        vector_polish.push_back(instruction_ptr);
        //Записываем инструкцию в стек ожидающих определение меток IfZ
        stack_goto_labels.push(instruction_ptr);

        return;
    }

    case elseOpc:
    case loop_begin:
    case loop_short_init:
    case callFunc_Begin:
    {
        //Добавим созданную инструкцию в стек операций
        stack_operations.push(instruction_ptr);
        End();
        return;
    }
    case callFunc_End:
    {


        //Заполняем переходы по return
        //В стеке есть return

        while(!stack_return.empty())
        {
            stack_return.top()->arg1->val.label->position = vector_polish.size();
            stack_return.pop();
        }

        //Добавим созданную инструкцию в стек операций
        stack_operations.push(instruction_ptr);
        End();
        return;
    }
    case param:
    {
        //Добавим созданную инструкцию в стек операций
        stack_operations.push(instruction_ptr);
        End();
        return;
    }
    case returnOpc:
    {
        //Добавим созданную инструкцию в стек операций
        stack_operations.push(instruction_ptr);
        End();
        return;
    }
    case arg_call_func:
    {
        //Добавим созданную инструкцию в стек операций
        stack_operations.push(instruction_ptr);
        End();
        return;
    }
    case call_func:{
        //Добавим созданную инструкцию в стек операций
        stack_operations.push(instruction_ptr);
        End();
        return;
    }
    case var_statement_one:{
        //Добавим созданную инструкцию в стек операций
        stack_operations.push(instruction_ptr);
        End();
        return;
    }
    default:
    {
        //Если стек операций не пустой
        if(!stack_operations.empty())
        {
            do
            {
                pop_operation_top = nullptr;
                pop_operand_left = nullptr;
                pop_operand_right = nullptr;

                //То проверяем на приоритет ( проверяет нужно ли впихивать иснтрукцию в стек )
                flag_push = check_priopity(instruction_ptr);

                if(flag_push)
                {
                    break;
                }

                //Если выявило что не нужно впихивать значит нужно вытащить
                //инструкции пока не впихнется данная иснструкция
                pop_operation_top = pop_operation();



                //Если мы хотим положить Закрывающуюся скобочку а наткнулись на открывающуюся , то убрать открывающуюся и выйти из метода
                if(pop_operation_top->opc == LCircleOpc && instruction_ptr->opc == RCircleOpc)
                {
                    delete pop_operation_top;
                    delete instruction_ptr;
                    return;
                }

                switch (pop_operation_top->opc)
                {
                case LCircleOpc:
                {

                    break;
                }

                case dicrimentOpc:
                case incrimentOpc:
                {
                    //Достаем только 1 операнд
                    pop_operand_left = pop_operand();
                    //устанавливаем в трехадресном коде инструкции Результат данную временную переменную
                    add_rezult_operand(pop_operation_top,pop_operand_left);
                    break;
                }
                case minusUnOpc:
                {
                    //Достаем только 1 операнд
                    pop_operand_left = pop_operand();
                    //Создаем временную переменную операнд и добавляемв в стек операндов
                    OPERAND* tmp = push_operand(tmpVarOpd,nullptr);
                    //устанавливаем в трехадресном коде инструкции Результат данную временную переменную
                    add_rezult_operand(pop_operation_top,tmp);
                    break;
                }
                case assignOpc:
                {
                    //Достаем левй операнд
                    pop_operand_left = pop_operand();
                    //Достаем результат операнд
                    OPERAND* tmp  = pop_operand();
                    //устанавливаем в трехадресном коде инструкции Результат являющийся переменной
                    add_rezult_operand(pop_operation_top,tmp);
                    break;
                }
                default:
                {
                    //Достаем правый операнд
                    pop_operand_right = pop_operand();
                    //Достаем левй операнд
                    pop_operand_left = pop_operand();
                    //Создаем временную переменную операнд и добавляемв в стек операндов
                    OPERAND* tmp = push_operand(tmpVarOpd,nullptr);
                    //устанавливаем в трехадресном коде инструкции Результат данную временную переменную
                    add_rezult_operand(pop_operation_top,tmp);
                    break;
                }
                }

                //устанавливаем в трехадресном коде инструкции Левый операнд
                add_left_operand(pop_operation_top,pop_operand_left);
                //устанавливаем в трехадресном коде инструкции Правый операнд
                add_right_operand(pop_operation_top,pop_operand_right);

                //Записываем уже сформированные трехадресные команды в стек в виде польской обратной записи
                vector_polish.push_back(pop_operation_top);

            }while(!flag_push);
        }

        break;
    }
    }


    //Добавим созданную инструкцию в стек операций
    stack_operations.push(instruction_ptr);
}

void Polish_notation::push_operation(opcType var_opc, label_typ typ)
{
    //Создадим иснтрукцию
    INSTRUCTION* instruction_ptr = Create_new_inctruction(var_opc);



    OPERAND* pop_operand_left = nullptr;
    OPERAND* pop_operand_right = nullptr;

    //Добавляем условие IfZ
    //
    switch (instruction_ptr->opc)
    {
    case gotoOpc:
    {
        //Создаем правый операнд являющийся меткой (куда прыгнуть)
        OPERAND* tmp = Create_new_operand(labelOpd,nullptr);
        //Задаем определенный тип метки
        tmp->val.label->typ = typ;
        //устанавливаем в трехадресном коде инструкции Левый операнд
        add_left_operand(instruction_ptr,tmp);

        //Записываем уже сформированные трехадресные команды в стек в виде польской обратной записи
        vector_polish.push_back(instruction_ptr);
        //Записываем инструкцию в стек ожидающих определение меток goto
        stack_goto_labels.push(instruction_ptr);

        break;
    }

    case ifZOpc_for:
    case ifZOpc_while:
    case ifZOpc:
    {
        //Достаем левй операнд являющийся результатом условия
        pop_operand_left = pop_operand();
        //Создаем правый операнд являющийся меткой (куда прыгнуть)
        OPERAND* tmp = Create_new_operand(labelOpd,nullptr);
        //Задаем определенный тип метки
        tmp->val.label->typ = typ;

        pop_operand_right = tmp;
        //устанавливаем в трехадресном коде инструкции Левый операнд
        add_left_operand(instruction_ptr,pop_operand_left);
        //устанавливаем в трехадресном коде инструкции Правый операнд
        add_right_operand(instruction_ptr,pop_operand_right);

        //Записываем уже сформированные трехадресные команды в стек в виде польской обратной записи
        vector_polish.push_back(instruction_ptr);
        //Записываем инструкцию в стек ожидающих определение меток IfZ
        stack_goto_labels.push(instruction_ptr);

        break;
    }
    default:
    {
        break;
    }
    }
}

// добавить объект в стек операндов
OPERAND* Polish_notation::push_operand(opdType typ, void *var_operand)
{
    //Создадим операнд
    OPERAND* operand_ptr = Create_new_operand(typ,var_operand);
    //Добавляем созданный операнд в стек операндов
    stack_operand.push(operand_ptr);

    return operand_ptr;
}

void Polish_notation::End()
{
    INSTRUCTION* pop_operation_top = nullptr;
    OPERAND* pop_operand_left = nullptr;
    OPERAND* pop_operand_right = nullptr;

    while(!stack_operations.empty())
    {

        pop_operation_top = nullptr;
        pop_operand_left = nullptr;
        pop_operand_right = nullptr;

        pop_operation_top = pop_operation();

        switch (pop_operation_top->opc)
        {
        case LCircleOpc:
        {
            cout << "Don't find RCircleOpc: '('"<<endl;
            exit(-1);
            break;
        }
        case dicrimentOpc:
        case incrimentOpc:
        {
            //Достаем только 1 операнд
            pop_operand_left = pop_operand();
            //устанавливаем в трехадресном коде инструкции Результат данную временную переменную
            add_rezult_operand(pop_operation_top,pop_operand_left);
            break;
        }
        case minusUnOpc:
        {
            //Достаем только 1 операнд
            pop_operand_left = pop_operand();
            //Создаем временную переменную операнд и добавляемв в стек операндов
            OPERAND* tmp = push_operand(tmpVarOpd,nullptr);
            //устанавливаем в трехадресном коде инструкции Результат данную временную переменную
            add_rezult_operand(pop_operation_top,tmp);
            break;
        }
        case assignOpc:
        {
            //Достаем левй операнд
            pop_operand_left = pop_operand();
            //Достаем результат операнд
            OPERAND* tmp  = pop_operand();
            //устанавливаем в трехадресном коде инструкции Результат являющийся переменной
            add_rezult_operand(pop_operation_top,tmp);
            break;
        }
        case param:{
            //устанавливаем в трехадресном коде инструкции Результат являющийся переменной
            add_rezult_operand(pop_operation_top,pop_operand());
            break;
        }

        case var_statement_one:
        {
            //устанавливаем в трехадресном коде инструкции Результат являющийся переменной
            add_rezult_operand(pop_operation_top,pop_operand());
            break;
        }
        case elseOpc:
        case loop_begin:
        case loop_short_init:
        {

            break;
        }
        case callFunc_Begin:{
            //устанавливаем в трехадресном коде инструкции Результат являющийся переменной
            add_rezult_operand(pop_operation_top,pop_operand());
            break;
        }
        case callFunc_End:{
            //устанавливаем в трехадресном коде инструкции Результат являющийся переменной
            add_rezult_operand(pop_operation_top,pop_operand());
            break;
        }
        case returnOpc:
        {
            //устанавливаем в трехадресном коде инструкции Результат являющийся переменной
            add_rezult_operand(pop_operation_top,pop_operand());
            break;
        }
        case arg_call_func:
        {
            //устанавливаем в трехадресном коде инструкции Результат являющийся переменной
            pop_operand_left = pop_operand();
            break;
        }
        case call_func:{
            //Достаем правый операнд
            pop_operand_right = pop_operand();
            //Достаем левый операнд
            pop_operand_left = pop_operand();
            //Создаем временную переменную операнд и добавляемв в стек операндов
            break;
        }
        default:
        {
            //Достаем правый операнд
            pop_operand_right = pop_operand();
            //Достаем левый операнд
            pop_operand_left = pop_operand();
            //Создаем временную переменную операнд и добавляемв в стек операндов
            OPERAND* tmp = push_operand(tmpVarOpd,nullptr);
            //устанавливаем в трехадресном коде инструкции Результат данную временную переменную
            add_rezult_operand(pop_operation_top,tmp);
            break;
        }
        }

        //устанавливаем в трехадресном коде инструкции Левый операнд
        add_left_operand(pop_operation_top,pop_operand_left);
        //устанавливаем в трехадресном коде инструкции Правый операнд
        add_right_operand(pop_operation_top,pop_operand_right);

        //Записываем уже сформированные трехадресные команды в стек в виде польской обратной записи
        vector_polish.push_back(pop_operation_top);
    }



}

//проверка приоритета
//выявляет Нужно ли добавлять инструкцию в стек ?
bool Polish_notation::check_priopity(INSTRUCTION *instruction_ptr)
{
    int priority_new = instruction_ptr->priority_rang;
    int priority_top = stack_operations.top()->priority_rang;

    if(stack_operations.empty())
    {
        if(instruction_ptr->opc == RCircleOpc)
        {
            cout << "Don't find LCircleOpc: ')'"<<endl;
            exit(-1);
        }
    }
    else
    {
        //Если мы хотим положить закрывающуюся скобку то вытаскиваем до открывающейся
        if(instruction_ptr->opc == RCircleOpc && stack_operations.top()->opc != LCircleOpc)
        {
            return false;
        }
        else
        {
            if(instruction_ptr->opc == RCircleOpc && stack_operations.top()->opc == LCircleOpc)
            {
                return false;
            }
        }



        //Если встретилась открывающая скобка то не проверять ранг
        if((instruction_ptr->opc == LCircleOpc && stack_operations.top()->opc != RCircleOpc)  || priority_top == 1)
        {
            return true;
        }

        if(priority_new > priority_top)
        {
            return false;
        }

    }


    return true;
}

//вытащить из стека операций верхушку стека
INSTRUCTION *Polish_notation::pop_operation()
{
    INSTRUCTION* item = stack_operations.top();
    stack_operations.pop();
    return item;
}

OPERAND *Polish_notation::pop_operand()
{
    OPERAND* item = stack_operand.top();
    stack_operand.pop();
    return item;
}

int positionIn_vvector_polish = 0;
void Polish_notation::out_stek_file(string fileName)
{
    ofstream fileOut2(fileName, ios::binary | ios::trunc); //| ios::trunc
    fileOut2.close();

    fileOut2.open(fileName, ios::binary | ios::app); //| ios::trunc
    string str = "\n\n";


    INSTRUCTION* top;
    //auto steck = &vector_polish;



    while(positionIn_vvector_polish < static_cast<int>(vector_polish.size()))
    {
        top = vector_polish[positionIn_vvector_polish];

        //        for(int i=0 ; i < tab;i++)
        //        {
        //            str +="\t";
        //        }

        str+=to_string(positionIn_vvector_polish)+":\t\t";
        if(top->arg1 != nullptr)
        {
            switch (top->arg1->typ)
            {
            case constOpd:
            {
                str += to_string(top->arg1->val.cons->val.unum)+"\t\t\t";
                break;
            }
            case nameVarOpd:
            {

                auto tmpItem = top->arg1->val.var;

                while(!tmpItem->parent_empty())
                {
                    str += tmpItem->parent_get()->get_name()+".";
                    tmpItem = top->arg1->val.var->parent_get();
                }

                str += top->arg1->val.var->get_name()+"\t\t\t";
                break;
            }
            case tmpVarOpd:
            {
                str += top->arg1->val.varTmp->name+"\t\t\t";
                break;
            }
            case labelOpd:
            {
                str += "_L"+to_string(top->arg1->val.label->position);

                if(str.size() > 3)
                {
                    str +="\t\t";
                }
                else
                {
                    str +="\t\t\t";
                }


                break;
            }
            case call:
            {
                str += top->arg1->val.fanc->getNameFuncType()+"\t\t\t";
                break;
            }

            default:
                str += "null\t\t";
                break;
            }

        }
        else
        {
            str += "null\t\t";
        }

        str +=opc(top->opc)+"\t\t\t\t";

        if(top->arg2 != nullptr)
        {
            switch (top->arg2->typ)
            {

            case funcVar_count:
            case constOpd:
            {
                str += to_string(top->arg2->val.cons->val.unum)+"\t";
                break;
            }
            case nameVarOpd:
            {
                auto tmpItem = top->arg2->val.var;

                while(!tmpItem->parent_empty())
                {
                    str += tmpItem->parent_get()->get_name()+".";
                    tmpItem = top->arg2->val.var->parent_get();
                }

                str += top->arg2->val.var->get_name()+"\t";
                break;
            }
            case tmpVarOpd:
            {

                str += top->arg2->val.varTmp->name+"\t";
                break;
            }
            case labelOpd:
            {
                str += "_L"+to_string(top->arg2->val.label->position)+"\t";
                break;
            }
            case call:
            {
                str += top->rez->val.fanc->getNameFuncType()+"\t";
                break;
            }

            default:
                str += "null\t";
                break;
            }
        }
        else
        {
            str += "null\t\t";
        }

        str +="\t=\t";

        if(top->rez != nullptr)
        {
            switch (top->rez->typ)
            {
            case constOpd:
            {
                str += to_string(top->rez->val.cons->val.unum)+"\t";
                break;
            }
            case nameVarOpd:
            {
                str += top->rez->val.var->get_name()+"\t";
                break;
            }
            case tmpVarOpd:
            {
                str += top->rez->val.varTmp->name+"\t";
                break;
            }
            case call:
            {
                str += top->rez->val.fanc->getNameFuncType()+"\t";
                break;
            }
            default:
                str += "null\t";
                break;
            }
        }
        else
        {
            str += "null\t";
        }

        str += "priority_rang: "+to_string(top->priority_rang)+"\t\n";
        positionIn_vvector_polish++;
    }

    fileOut2 << str;

}

vector<INSTRUCTION *> Polish_notation::get_vector_polish()
{
    return vector_polish;
}

string Polish_notation::opc(opcType typ)
{
    string str = "";
    switch (typ)
    {
    case LCircleOpc:    str = "(";                      break;
    case RCircleOpc:    str = ")";                      break;
    case starOpc:       str = "*";                      break;
    case slashOpc:      str = "/";                      break;
    case  plusOpc:      str = "+";                      break;
    case dicrimentOpc:  str = "--";                     break;
    case incrimentOpc:  str = "++";                     break;

    case minusUnOpc:
    case  minusOpc:     str = "-";                      break;

    case assignOpc:     str = ":=";                     break;
    case modOpc:        str = "%";                      break;
    case smallerOpc:    str = "<";                      break;
    case smallerEQOpc:  str = "<=";                     break;
    case largerOpc:     str = ">";                      break;
    case largerEQOpc:   str = ">=";                     break;
    case eqOpc:         str = "==";                     break;
    case ifZOpc:        str = "ifz";                    break;
    case ifZOpc_for:    str = "ifZOpc_for";             break;
    case ifZOpc_while:    str = "ifZOpc_while";         break;
    case gotoOpc:       str = "goto";                   break;

    case elseOpc:       str = "elseOpc";                break;


    case gotoOpcIfZ_false: str ="goto_ifz_f";           break;              //goto метка для перехода if после ложного условия
    case gotoOpcIfZ_true: str = "goto_ifz_t";           break;              //goto метка для перехода if после истенного условия
    case gotoOpcIf_trueBreak: str = "goto_ifz_t_B";     break;              //goto метка для выхода из цикла (for) в истенном условии
    case gotoOpcIf_falseBreak: str = "goto_ifz_f_B";    break;              //goto метка для выхода из цикла (for) в ложном условии
    case gotoOpcIf_trueContinue: str = "goto_ifz_t_C";  break;
    case gotoOpcIf_falseContinue: str = "goto_ifz_f_C"; break;
    case gotoOpcIf_trueReturn:      str = "goto_ifz_t_r"; break;
    case gotoOpcIf_falseReturn:     str = "goto_ifz_f_r"; break;
    case gotoOpcFor: str = "goto_for";                  break;               //goto метка для перехода в цикле
    case gotoOpcFor_continue: str = "goto_for_C";       break;
    case gotoOpcFor_break: str = "goto_for_B";          break;              //goto метка для перехода из цикла for
    case gotoOpcFor_infinity: str = "goto_for_I";       break;              //goto метка для перехода в while(1)

    case loop_begin:          str = "loop_begin";       break;
    case loop_short_init:     str = "loop_short_init";  break;
    case callFunc_Begin:      str = "call_f_begin";     break;
    case callFunc_End:        str = "call_f_end";       break;
    case param:               str = "param";            break;
    case returnOpc:           str ="return";            break;
    case arg_call_func:       str ="arg_call_func";     break;
    case call_func:           str = "call_func";        break;
    case gotoOpc_return:      str = "goto_return";      break;
    case gotoOpcFor_return:   str = "goto_for_return";  break;

    case var_statement_one:   str = "var_statement_one";break;
    default: break;
    }

    return str;
}

void Polish_notation::set_goto_label(for_or_if typ)
{

    if(stack_goto_labels.empty())
    {
        switch (typ)
        {
        case it_is_for_infiniti:
        case it_is_for:
        {

            //Если мы нашли break
            if(label_begin_for.empty())
            {
                break;
            }

            //Установить оператор goto (по достижению закрывающейся скобки)
            switch (typ)
            {
            case it_is_for_infiniti:
                push_operation(gotoOpcFor_infinity);
                break;
            default:
                push_operation(gotoOpcFor);
                break;
            }



            stack_goto_labels.top()->arg1->val.label->position = label_begin_for.top();


            //if(!stack_breaks.empty())
            //{

            //В стеке есть break
            if(!stack_breaks.top()->empty())
            {
                auto stack_break_for = stack_breaks.top();

                while(!stack_break_for->empty())
                {
                    stack_break_for->top()->arg1->val.label->position = vector_polish.size();
                    stack_break_for->pop();
                }
                delete stack_break_for;
                stack_breaks.pop();
            }
            //}

            label_begin_for.pop();
            stack_goto_labels.pop();


            break;
        }
        case it_is_for_break:
        {
            //Установить оператор goto (по достижению закрывающейся скобки)
            push_operation(gotoOpcFor_break);
            //Сохраняем break в стек;
            stack_breaks.top()->push(stack_goto_labels.top());
            //Убираем break из массива так как он хранится в стеке break;
            stack_goto_labels.pop();

            break;
        }
        case it_is_if_break:
        {
            //Установить оператор goto (по достижению закрывающейся скобки)
            push_operation(gotoOpc);
            break;
        }

        case func_return:
        {
            //Установить оператор goto (по достижению закрывающейся скобки функции)
            push_operation(gotoOpc_return);
            //Сохраняем return в стек;
            stack_return.push(stack_goto_labels.top());
            //Убираем break из массива так как он хранится в стеке return;
            stack_goto_labels.pop();
            break;
        }
        case for_return: {
            //Установить оператор goto (по достижению закрывающейся скобки функции)
            push_operation(gotoOpcFor_return);
            //Сохраняем return в стек;
            stack_return.push(stack_goto_labels.top());
            //Убираем break из массива так как он хранится в стеке return;
            stack_goto_labels.pop();
            break;
        }
        case if_return_t:
        {
            //Установить оператор goto (по достижению закрывающейся скобки функции)
            push_operation(gotoOpcIf_trueReturn);
            //Сохраняем return в стек;
            stack_return.push(stack_goto_labels.top());
            //Убираем break из массива так как он хранится в стеке return;
            stack_goto_labels.pop();
        }
        case if_return_f:
        {
            //Установить оператор goto (по достижению закрывающейся скобки функции)
            push_operation(gotoOpcIf_falseReturn);
            //Сохраняем return в стек;
            stack_return.push(stack_goto_labels.top());
            //Убираем break из массива так как он хранится в стеке return;
            stack_goto_labels.pop();
        }

        case if_break_f:
        {
            //Брейк в ложном условии
            //Установить оператор goto (по достижению закрывающейся скобки)
            push_operation(gotoOpcIf_falseBreak);
            //Сохраняем break в стек;
            stack_breaks.top()->push(stack_goto_labels.top());
            //Убираем break из массива так как он хранится в стеке break;
            stack_goto_labels.pop();
            break;
        }
        case if_true:
        {
            //Установить оператор goto (по выходу из положительного условия)
            push_operation(gotoOpcIfZ_true);

            //если не встретили условие Else то оставляем метку до данного действия
            stack_goto_labels.top()->arg1->val.label->position = vector_polish.size();

            switch (stack_goto_labels.top()->opc)
            {
            case ifZOpc:
                stack_goto_labels.top()->arg2->val.label->position = vector_polish.size()+1; //+1  потому что я потом вставляю еще один оператор goto
                stack_goto_labels.pop();
                //Установить оператор goto (по выходу из положительного условия)
                push_operation(gotoOpcIfZ_true);

                //если не встретили условие Else то оставляем метку до данного действия
                stack_goto_labels.top()->arg1->val.label->position = vector_polish.size();
                break;
            default:
                break;
            }
            break;
        }
        case if_false:
        {
            //Установить оператор goto (по выходу из положительного условия)
            push_operation(gotoOpcIfZ_false);

            //если не встретили условие Else то оставляем метку до данного действия
            stack_goto_labels.top()->arg1->val.label->position = vector_polish.size();

            switch (stack_goto_labels.top()->opc)
            {
            case gotoOpcIfZ_true:
                //если не встретили условие Else то оставляем метку до данного действия
                stack_goto_labels.top()->arg1->val.label->position = vector_polish.size()+1;
                stack_goto_labels.pop();

                push_operation(gotoOpcIfZ_false);

                stack_goto_labels.top()->arg1->val.label->position = vector_polish.size();
                stack_goto_labels.pop();
                break;
            default:
                break;
            }
            break;
        }


        default: break;
        }
        return;
    }

    if (typ == if_return_t)
    {
        //Установить оператор goto (по достижению закрывающейся скобки функции)
        push_operation(gotoOpcIf_trueReturn);
        //Сохраняем return в стек;
        stack_return.push(stack_goto_labels.top());
        //Убираем break из массива так как он хранится в стеке return;
        stack_goto_labels.pop();
    }

    if (typ == if_return_f)
    {
        //Установить оператор goto (по достижению закрывающейся скобки функции)
        push_operation(gotoOpcIf_falseReturn);
        //Сохраняем return в стек;
        stack_return.push(stack_goto_labels.top());
        //Убираем break из массива так как он хранится в стеке return;
        stack_goto_labels.pop();
    }

    if(typ == func_return)
    {
        //Установить оператор goto (по достижению закрывающейся скобки цикла)
        push_operation(gotoOpc_return);
        //Сохраняем return в стек;
        stack_return.push(stack_goto_labels.top());
        //Убираем break из массива так как он хранится в стеке return;
        stack_goto_labels.pop();
        return;
    }

    if(typ == for_return)
    {
        //Установить оператор goto (по достижению закрывающейся скобки цикла)
        push_operation(gotoOpcFor_return);
        //Сохраняем return в стек;
        stack_return.push(stack_goto_labels.top());
        //Убираем break из массива так как он хранится в стеке return;
        stack_goto_labels.pop();
        return;
    }

    if(typ == it_is_for_continue)
    {
        //continue в цикле
        //Установить оператор goto (по достижению закрывающейся скобки)
        push_operation(gotoOpcFor_continue);
        //Сохраняем Continue в стек;
        stack_continue.top()->push(stack_goto_labels.top());
        //Убираем break из массива так как он хранится в стеке break;
        stack_goto_labels.pop();
        return;
    }

    if(typ == if_continue_t)
    {
        //continue в истенном условии
        //Установить оператор goto (по достижению закрывающейся скобки)
        push_operation(gotoOpcIf_trueContinue);
        //Сохраняем Continue в стек;
        stack_continue.top()->push(stack_goto_labels.top());
        //Убираем break из массива так как он хранится в стеке break;
        stack_goto_labels.pop();
        return;
    }

    if(typ == if_continue_f)
    {
        //continue в ложном условии
        //Установить оператор goto (по достижению закрывающейся скобки)
        push_operation(gotoOpcIf_falseContinue);
        //Сохраняем Continue в стек;
        stack_continue.top()->push(stack_goto_labels.top());
        //Убираем break из массива так как он хранится в стеке break;
        stack_goto_labels.pop();
        return;
    }

    if(typ == if_break_t)
    {
        //Брейк в истенном условии
        //Установить оператор goto (по достижению закрывающейся скобки)
        push_operation(gotoOpcIf_trueBreak);
        //Сохраняем break в стек;
        stack_breaks.top()->push(stack_goto_labels.top());
        //Убираем break из массива так как он хранится в стеке break;
        stack_goto_labels.pop();
        return;
    }

    if(typ == if_break_f)
    {
        //Брейк в ложном условии
        //Установить оператор goto (по достижению закрывающейся скобки)
        push_operation(gotoOpcIf_falseBreak);
        //Сохраняем break в стек;
        stack_breaks.top()->push(stack_goto_labels.top());
        //Убираем break из массива так как он хранится в стеке break;
        stack_goto_labels.pop();
        return;
    }

    if(typ == it_is_for_break)
    {
        //Брейк в ложном условии

        //Установить оператор goto (по достижению закрывающейся скобки)
        push_operation(gotoOpcFor_break);
        //Сохраняем break в стек;
        stack_breaks.top()->push(stack_goto_labels.top());
        //Убираем break из массива так как он хранится в стеке break;
        stack_goto_labels.pop();
        return;
    }

    if(typ == it_is_for_infiniti)
    {

        while (!stack_goto_labels.empty())
        {
            if(stack_goto_labels.top()->opc == ifZOpc_while)
            {
                break;
            }
            else
            {
                stack_goto_labels.top()->arg1->val.label->position = vector_polish.size();
                stack_goto_labels.pop();
            }
        }


        if(!stack_expression_for.empty())
        {
            //В цикле есть логика по окончанию тела цикла
            auto expression_for = stack_expression_for.top();

            for(unsigned int i=0; i < expression_for->size(); i++)
            {
                vector_polish.push_back(expression_for->at(i));
            }

            expression_for->clear();
            delete expression_for;

            //stack_breaks.pop();
        }


        //Установить оператор goto (по достижению закрывающейся скобки)
        // Конец цикла
        push_operation(gotoOpcFor_infinity);

        stack_goto_labels.top()->arg1->val.label->position = label_begin_for.top();


        if(!stack_breaks.empty())
        {
            //В стеке есть break
            auto stack_break_for = stack_breaks.top();

            while(!stack_break_for->empty())
            {
                stack_break_for->top()->arg1->val.label->position = vector_polish.size();
                stack_break_for->pop();
            }
            delete stack_break_for;
            stack_breaks.pop();
        }

        //В стеке есть continue
        auto stack_continue_for = stack_continue.top();
        if(!stack_continue.empty())
        {

            while(!stack_continue_for->empty())
            {
                stack_continue_for->top()->arg1->val.label->position = label_begin_for.top();
                stack_continue_for->pop();
            }
            delete stack_continue_for;
            stack_continue.pop();
        }


        label_begin_for.pop();
        stack_goto_labels.pop();

        return;
    }

    if(typ == it_is_for_while)
    {

        while (!stack_goto_labels.empty())
        {

            if(stack_goto_labels.top()->opc == ifZOpc_while)
            {
                stack_goto_labels.top()->arg2->val.label->position = vector_polish.size()+1;
                stack_goto_labels.pop();
                break;
            }
            else
            {
                stack_goto_labels.top()->arg1->val.label->position = vector_polish.size();
                stack_goto_labels.pop();
            }
        }

        if(!stack_expression_for.empty())
        {
            //В цикле есть логика по окончанию тела цикла
            auto expression_for = stack_expression_for.top();

            for(unsigned int i=0; i < expression_for->size(); i++)
            {
                vector_polish.push_back(expression_for->at(i));
            }

            expression_for->clear();
            delete expression_for;

            stack_breaks.pop();
        }

        //Установить оператор goto (по достижению закрывающейся скобки)
        // Конец цикла
        push_operation(gotoOpcFor);

        stack_goto_labels.top()->arg1->val.label->position = label_begin_for.top();



        if(!stack_breaks.empty())
        {
            //В стеке есть break
            auto stack_break_for = stack_breaks.top();

            while(!stack_break_for->empty())
            {
                stack_break_for->top()->arg1->val.label->position = vector_polish.size();
                stack_break_for->pop();
            }

            delete stack_break_for;
            stack_breaks.pop();
        }


        if(!stack_continue.empty())
        {
            //В стеке есть continue
            auto stack_continue_for = stack_continue.top();

            while(!stack_continue_for->empty())
            {
                stack_continue_for->top()->arg1->val.label->position = label_begin_for.top();
                stack_continue_for->pop();
            }
            delete stack_continue_for;
            stack_continue.pop();
        }

        label_begin_for.pop();
        stack_goto_labels.pop();

        return;

    }

    if(typ == it_is_for)
    {
        while (!stack_goto_labels.empty())
        {

            if(stack_goto_labels.top()->opc == ifZOpc_while || stack_goto_labels.top()->opc == ifZOpc_for)
            {
                stack_goto_labels.top()->arg2->val.label->position = vector_polish.size()+1;
                stack_goto_labels.pop();
                break;
            }
            else
            {
                stack_goto_labels.top()->arg1->val.label->position = vector_polish.size();
                stack_goto_labels.pop();
            }
        }

        if(!stack_expression_for.empty())
        {
            //В цикле есть логика по окончанию тела цикла
            auto expression_for = stack_expression_for.top();

            for(unsigned int i=0; i < expression_for->size(); i++)
            {
                vector_polish.push_back(expression_for->at(i));
            }

            expression_for->clear();

            if(expression_for != nullptr)
            {
                delete expression_for;
            }

            if(!stack_breaks.empty())
            {
                stack_breaks.pop();
            }
            stack_expression_for.pop();
        }

        //Установить оператор goto (по достижению закрывающейся скобки)
        // Конец цикла
        push_operation(gotoOpcFor);

        stack_goto_labels.top()->arg1->val.label->position = label_begin_for.top();



        if(!stack_breaks.empty())
        {
            //В стеке есть break
            auto stack_break_for = stack_breaks.top();

            while(!stack_break_for->empty())
            {
                stack_break_for->top()->arg1->val.label->position = vector_polish.size();
                stack_break_for->pop();
            }

            delete stack_break_for;
            stack_breaks.pop();
        }


        if(!stack_continue.empty())
        {
            //В стеке есть continue
            auto stack_continue_for = stack_continue.top();

            while(!stack_continue_for->empty())
            {
                stack_continue_for->top()->arg1->val.label->position = label_begin_for.top();
                stack_continue_for->pop();
            }
            delete stack_continue_for;
            stack_continue.pop();
        }

        label_begin_for.pop();
        stack_goto_labels.pop();

        return;
    }
    if(typ == if_true)
    {

        switch (stack_goto_labels.top()->opc)
        {
        case ifZOpc:
            stack_goto_labels.top()->arg2->val.label->position = vector_polish.size()+1; //+1  потому что я потом вставляю еще один оператор goto
            stack_goto_labels.pop();
            //Установить оператор goto (по выходу из положительного условия)
            push_operation(gotoOpcIfZ_true);

            //если не встретили условие Else то оставляем метку до данного действия
            stack_goto_labels.top()->arg1->val.label->position = vector_polish.size();
            break;
        default:
            break;
        }


        return;
    }


    if(typ == if_false)
    {

        switch (stack_goto_labels.top()->opc)
        {
        case gotoOpcIfZ_true:
            //если не встретили условие Else то оставляем метку до данного действия
            stack_goto_labels.top()->arg1->val.label->position = vector_polish.size()+1;
            stack_goto_labels.pop();

            push_operation(gotoOpcIfZ_false);

            stack_goto_labels.top()->arg1->val.label->position = vector_polish.size();
            stack_goto_labels.pop();
            break;
        default:
            break;
        }


        return;
    }

    /*
    switch (stack_goto_labels.top()->opc)
    {

    //в стеке лежит уже условие истенного перехода из предыдущего условия if
    case gotoOpcIfZ_false:
    {
        //если не встретили условие Else то оставляем метку до данного действия
        if(typ == if_false)
        {
            stack_goto_labels.top()->arg1->val.label->position = vector_polish.size();
            stack_goto_labels.pop();

            push_operation(gotoOpcIfZ_false);
        }
        else
        {
            stack_goto_labels.top()->arg1->val.label->position = vector_polish.size();
            stack_goto_labels.pop();

            if(!stack_goto_labels.empty())
            {
                stack_goto_labels.top()->arg2->val.label->position = vector_polish.size()+1;
                stack_goto_labels.pop();
                push_operation(gotoOpcIfZ_true);
            }
        }
        break;
    }
    case gotoOpcIfZ_true:
    {
        //если не встретили условие Else то оставляем метку до данного действия
        if(typ == if_false)
        {
            stack_goto_labels.top()->arg1->val.label->position = vector_polish.size()+1;
            stack_goto_labels.pop();

            push_operation(gotoOpcIfZ_false);
        }
        else
        {
            stack_goto_labels.top()->arg1->val.label->position = vector_polish.size();
            stack_goto_labels.pop();

            if(!stack_goto_labels.empty())
            {
                stack_goto_labels.top()->arg2->val.label->position = vector_polish.size()+1;
                stack_goto_labels.pop();
                push_operation(gotoOpcIfZ_true);
            }

        }



        switch (typ)
        {
        case it_is_for_infiniti:
        case it_is_for:
        {
            //Установить оператор goto (по достижению закрывающейся скобки)
            // Конец цикла

            if(typ == it_is_for_infiniti)
            {
                push_operation(gotoOpcFor_infinity);
            }
            else
            {
                push_operation(gotoOpcFor);
            }

            stack_goto_labels.top()->arg1->val.label->position = label_begin_for.top();

            //В стеке есть break
            auto stack_break_for = stack_breaks.top();

            if(!stack_breaks.empty())
            {

                while(!stack_break_for->empty())
                {
                    stack_break_for->top()->arg1->val.label->position = vector_polish.size();
                    stack_break_for->pop();
                }
                delete stack_break_for;
                stack_breaks.pop();
            }

            label_begin_for.pop();
            stack_goto_labels.pop();
            break;
        }
        default:break;
        }

        break;

    }
    case gotoOpc:
    {
        stack_goto_labels.top()->arg1->val.label->position = vector_polish.size()+1;
        stack_goto_labels.pop();

        switch (typ)
        {
        case it_is_for_infiniti:
        case it_is_for:
        {
            //Установить оператор goto (по достижению закрывающейся скобки)
            // Конец цикла

            if(typ == it_is_for_infiniti)
            {
                push_operation(gotoOpcFor_infinity);
            }
            else
            {
                push_operation(gotoOpcFor);
            }

            stack_goto_labels.top()->arg1->val.label->position = label_begin_for.top();

            //В стеке есть break
            auto stack_break_for = stack_breaks.top();

            if(!stack_breaks.empty())
            {
                while(!stack_break_for->empty())
                {
                    stack_break_for->top()->arg1->val.label->position = vector_polish.size();
                    stack_break_for->pop();
                }
                delete stack_break_for;
                stack_breaks.pop();
            }

            label_begin_for.pop();
            stack_goto_labels.pop();
            break;
        }
        default:break;
        }

        break;
    }

    case gotoOpcIf_falseBreak:
    case gotoOpcIf_trueBreak:
    {
        switch (typ)
        {

        case if_true:
        {

            stack_goto_labels.top()->arg2->val.label->position = vector_polish.size()+1; //+1  потому что я потом вставляю еще один оператор goto
            stack_goto_labels.pop();
            //Установить оператор goto (по выходу из положительного условия)
            push_operation(gotoOpcIfZ_true);
            break;
        }
        case if_false:
        {
            stack_goto_labels.top()->arg2->val.label->position = vector_polish.size()+1; //+1  потому что я потом вставляю еще один оператор goto
            stack_goto_labels.pop();
            //Установить оператор goto (по выходу из положительного условия)
            push_operation(gotoOpcIfZ_false);
            break;
        }

        case it_is_for_infiniti:
        case it_is_for:
        {
            stack_goto_labels.top()->arg2->val.label->position = vector_polish.size()+1;
            stack_goto_labels.pop();

            //Установить оператор goto (по достижению закрывающейся скобки)
            push_operation(gotoOpc);
            stack_goto_labels.top()->arg1->val.label->position = label_begin_for.top();
            label_begin_for.pop();
            stack_goto_labels.pop();
            break;
        }
        default:break;
        }
    }
        ////////////////////////
    case ifZOpc:
    {
        switch (typ)
        {
        case if_true:
        {

            stack_goto_labels.top()->arg2->val.label->position = vector_polish.size()+1; //+1  потому что я потом вставляю еще один оператор goto
            stack_goto_labels.pop();
            //Установить оператор goto (по выходу из положительного условия)
            push_operation(gotoOpcIfZ_true);
            break;
        }
        case if_false:
        {
            stack_goto_labels.top()->arg2->val.label->position = vector_polish.size()+1; //+1  потому что я потом вставляю еще один оператор goto
            stack_goto_labels.pop();
            //Установить оператор goto (по выходу из положительного условия)
            push_operation(gotoOpcIfZ_false);
            break;
        }
        case it_is_for_infiniti:
        case it_is_for:
        {
            stack_goto_labels.top()->arg2->val.label->position = vector_polish.size()+1;
            stack_goto_labels.pop();

            //Установить оператор goto (по достижению закрывающейся скобки)
            push_operation(gotoOpc);
            stack_goto_labels.top()->arg1->val.label->position = label_begin_for.top();
            label_begin_for.pop();
            stack_goto_labels.pop();
            break;
        }
        default:break;
        }
        break;
    }

    default:
    {
        stack_goto_labels.pop();
        break;
    }
    }

    */
}

void Polish_notation::label_pop()
{

    stack_goto_labels.pop();
}

void Polish_notation::save_label_begin_for(for_or_if typ)
{
    if(typ == it_is_for)
    {
        //Удаляем предыдущее состояние так как появлилось объявление переменной
        label_begin_for.pop();
        //Создаем новую точку прыжка цикла
        label_begin_for.push(vector_polish.size()); // сохранненная позиция  метки на начало цикла forƒ

        return;
    }

    label_begin_for.push(vector_polish.size()); // сохранненная позиция  метки на начало цикла forƒ
    stack_breaks.push(new stack<INSTRUCTION*>); // создаем стек breaks на данный цикл
    stack_continue.push(new stack<INSTRUCTION*>); // создаем стек continue на данный цикл
    stack_expression_for.push(new vector<INSTRUCTION*>); // создаем стек для сохранения действий которые должны выполниться в конце цикла
}

int Polish_notation::begin_position_expression()
{
    return vector_polish.size();
}

int Polish_notation::end_position_expression()
{
    return vector_polish.size();
}

void Polish_notation::save_expression_for(unsigned int begin, unsigned int end)
{

    for(unsigned int i=begin;i < end;i++)
    {
        stack_expression_for.top()->push_back(vector_polish[i]);
    }

    vector_polish.erase(vector_polish.begin()+begin,vector_polish.begin()+end);

}

void Polish_notation::add_function(FuncType *func)
{
    vector_func.push_back(func);
}


