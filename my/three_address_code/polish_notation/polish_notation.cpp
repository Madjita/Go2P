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
        if(instruction_ptr->opc == LCircleOpc && stack_operations.top()->opc != RCircleOpc  || priority_top == 1)
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



    while(positionIn_vvector_polish < vector_polish.size())
    {
        top = vector_polish[positionIn_vvector_polish];

        //        for(int i=0 ; i < tab;i++)
        //        {
        //            str +="\t";
        //        }

        str+=to_string(positionIn_vvector_polish)+":\t";
        if(top->arg1 != nullptr)
        {
            switch (top->arg1->typ)
            {
            case constOpd:
            {
                str += to_string(top->arg1->val.cons->val.unum)+"\t";
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

                str += top->arg1->val.var->get_name()+"\t";
                break;
            }
            case tmpVarOpd:
            {
                str += top->arg1->val.varTmp->name+"\t";
                break;
            }
            case labelOpd:
            {
                str += "_L"+to_string(top->arg1->val.label)+"\t";
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

        str +=opc(top->opc)+"\t";

        if(top->arg2 != nullptr)
        {
            switch (top->arg2->typ)
            {
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
                str += "_L"+to_string(top->arg2->val.label)+"\t";
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

string Polish_notation::opc(opcType typ)
{
    string str = "";
    switch (typ)
    {
        case LCircleOpc:    str = "(";break;
        case RCircleOpc:    str = ")";break;
        case starOpc:       str = "*";break;
        case slashOpc:      str = "/";break;
        case  plusOpc:      str = "+";break;
        case minusUnOpc:
        case  minusOpc:     str = "-";break;
        case assignOpc:     str = ":=";break;
        case modOpc:        str = "%";break;
        case smallerOpc:    str = "<";break;
        case smallerEQOpc:  str = "<=";break;
        case largerOpc:     str = ">";break;
        case largerEQOpc:   str = ">=";break;
        case eqOpc:         str = "==";break;
        case ifZOpc:        str = "ifz";break;
        case gotoOpc:       str = "goto";break;
    }

    return str;
}

void Polish_notation::set_goto_label()
{
    if(stack_goto_labels.empty())
    {
        return;
    }

    switch (stack_goto_labels.top()->opc)
    {
        case gotoOpc:
        {
            stack_goto_labels.top()->arg1->val.label = vector_polish.size();
            stack_goto_labels.pop();
            break;
        }
        case ifZOpc:
        {
            stack_goto_labels.top()->arg2->val.label = vector_polish.size()+1; //+1  потому что я потом вставляю еще один оператор goto
            stack_goto_labels.pop();

            //Установить оператор goto (по выходу из положительного условия)
            push_operation(gotoOpc);
            break;
        }
        default:
        {
            stack_goto_labels.pop();
            break;
        }
    }

}