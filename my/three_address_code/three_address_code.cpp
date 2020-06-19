#include "three_address_code.h"

int tmpVarOpdNum = 0; // Номер временной переменной (в порядке создания)

Three_address_code::Three_address_code()
{

}

INSTRUCTION *Three_address_code::Create_new_inctruction(opcType var_ocp)
{
    INSTRUCTION* iPtr = new INSTRUCTION;

    iPtr->opc = var_ocp;
    iPtr->arg1 = nullptr;
    iPtr->arg2 = nullptr;
    iPtr->rez = nullptr;
    iPtr->priority_rang = get_auto_rang(var_ocp); //устанавливаем приоритет

    return iPtr;

}

OPERAND *Three_address_code::Create_new_operand(opdType typ, void *val)
{
    OPERAND *opdPtr = new OPERAND;
    memset(opdPtr,0,sizeof (opdPtr));

    opdPtr->typ = typ;
    switch(typ)
    {
    case labelOpd:
        opdPtr->val.label = new LABEL;
        opdPtr->val.label->position = 0;
        break;
    case constOpd:
        opdPtr->val.cons = (struct CONST *)val;
        break;
    case nameVarOpd:
        opdPtr->val.var = (MyVariable*)val;
        break;
    case tmpVarOpd:
        tmpVarOpdNum++; // увеличиваем счетчик временных переменных
        opdPtr->val.varTmp = new application; // организация и содержимое определятся позднее
        opdPtr->val.varTmp->name ="t"+to_string(tmpVarOpdNum);
        break;
    default:
        opdPtr->val.var = nullptr; //
    }
    return opdPtr;
}

void Three_address_code::add_left_operand(INSTRUCTION *instruction_ptr, OPERAND *operand_ptr)
{
    instruction_ptr->arg1 = operand_ptr;
}

void Three_address_code::add_right_operand(INSTRUCTION *instruction_ptr, OPERAND *operand_ptr)
{
    instruction_ptr->arg2 = operand_ptr;
}

void Three_address_code::add_rezult_operand(INSTRUCTION *instruction_ptr, OPERAND *operand_ptr)
{
    instruction_ptr->rez = operand_ptr;
}

int Three_address_code::get_auto_rang(opcType typ)
{
    int priority_rang = 0;

    switch (typ)
    {
        case LCircleOpc:			// круглые скобки '('
        case RCircleOpc:			// круглые скобки ')'
        {
            priority_rang = 1;
            break;
        }

        //унарный минус
        case dicrimentOpc:
        case incrimentOpc:
        case minusUnOpc:
        {
            priority_rang = 3;
            break;
        }

        case starOpc:
        case slashOpc:
        case modOpc:
        {
            priority_rang = 5;
            break;
        }

        case  plusOpc:           //плюс '+'
        case  minusOpc:          //минус '-'
        {
            priority_rang = 6;
            break;
        }

        case smallerOpc:        //меньше '<'
        case smallerEQOpc:      //меньше или равно '<='
        case largerOpc:         //больше '>'
        case largerEQOpc:       //больше или равно '>='
        {
            priority_rang = 8;
            break;
        }

        case eqOpc:         // равенство '=='
        {
            priority_rang = 9;
            break;
        }


        case assignOpc:         // авто присваивание ':='
        {
            priority_rang = 15;
            break;
        }
    }

    return priority_rang;
}
