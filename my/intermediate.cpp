#include "intermediate.h"

OPD_LIST opdList;

Intermediate::Intermediate()
{

    instruction = new INSTRUCTION;
    instruction->opc = noValueOpc;
    instruction->rez = nullptr;
    instruction->arg1 = nullptr;
    instruction->arg2 = nullptr;
    instruction->next = nullptr;


    list = new INSTR_LIST;
    list->lastOpd = instruction->rez;
    list->lastInstr = instruction;
    list->firstInstr = instruction;

    priority_rang = 0;
    myinstruction = create_instruction();


    steck_begin_ = new MashineSteck();
    steck_select_ = steck_begin_;


}


static int opdNum = 0; // Номер операнда (в порядке создания)
static int tmpVarOpdNum = 0; // Номер временной переменной (в порядке создания)

// Создание и заполнение структуры константы
struct CONST *Intermediate::CONST_constructor(scalType typ, void *val)
{
    struct CONST *constPtr;
    if((constPtr = (struct CONST*)malloc(sizeof(struct CONST)))==NULL)
    {
        //er(23);
        exit(1);
    }
    constPtr->typ = typ;
    if(typ == INTTYP) constPtr->val.unum = *((int *)val);
    else if(typ == FLOATTYP) constPtr->val.fnum = *((float *)val);
    return constPtr;
}

// Создание и заполнение структуры операнда
OPERAND *Intermediate::OPERAND_constructor(opdType typ, void *val)
{
    struct OPERAND *opdPtr;

    int OPERAND_size = sizeof(struct OPERAND);
    if((opdPtr = (struct OPERAND*)malloc(OPERAND_size))==NULL)
    {
        //er(23);
        exit(1);
    }
    opdPtr->ident = ++opdNum;
    opdPtr->typ = typ;
    opdPtr->next = NULL;
    switch(typ)
    {
    case constOpd:
        opdPtr->val.cons = (struct CONST *)val;
        break;
    case nameVarOpd:
        opdPtr->val.var = (struct application *)val;
        break;
    case tmpVarOpd:
        opdPtr->val.var = (struct application *)val; // организация и содержимое определятся позднее
        break;
    default:
        opdPtr->val.var = NULL; //
    }
    OPD_LIST_append(&opdList, opdPtr);
    return opdPtr;
}

// Вывод списка операндов в файл
void Intermediate::OPD_LIST_out(struct OPD_LIST* list, FILE* outfil)
{
    struct OPERAND* p_tmp;
    fprintf(outfil, "\n    Список операндов:\n");
    if(list->last == NULL) {
        fprintf(outfil, "Операнды отсутствуют!\n");
        return;
    }
    p_tmp = list->last;
    do {
        char* typ;
        char* vec_scal;
        switch(p_tmp->next->typ) {
        case nameVarOpd:
            if(p_tmp->next->val.var->typ == INTTYP) typ = (char*)"int";
            else typ = (char*)"float";
            if(p_tmp->next->val.var->len == 0) vec_scal = (char*)"скаляр";
            else vec_scal = (char*)"вектор";
            fprintf(outfil, "%d: Объявленная переменная, %s, %s, адрес = %d\n",
                    p_tmp->next->ident, typ, vec_scal, p_tmp->next->val.var->addr
                    );
            break;
        case tmpVarOpd:
            fprintf(outfil, "%d: Промежуточная переменная (не определяется)\n",
                    p_tmp->next->ident
                    );
            /*
    if(p_tmp->next->val.var->typ == INTTYP) typ = (char*)"int";
    else typ = (char*)"float";
    if(p_tmp->next->val.var->len == 0) vec_scal = (char*)"скаляр";
    else vec_scal = (char*)"вектор";
    fprintf(outfil, "%d: Промежуточная переменная, %s, %s\n",
      p_tmp->next->ident, typ, vec_scal
    );
      */
            break;
        case constOpd:
            if(p_tmp->next->val.cons->typ == INTTYP) {
                typ = (char*)"int";
                fprintf(outfil, "%d: Константа, %s, %d\n",
                        p_tmp->next->ident, typ, p_tmp->next->val.cons->val.unum
                        );
            } else {
                typ = (char*)"float";
                fprintf(outfil, "%d: Константа, %s, %e\n",
                        p_tmp->next->ident, typ, p_tmp->next->val.cons->val.fnum
                        );
            }
            break;
        case labelOpd:
            fprintf(outfil, "%d: Метка\n",
                    p_tmp->next->ident
                    );
            break;
        default:
            fprintf(outfil, "Структура операнда непонятна\n");
        }
        p_tmp = p_tmp->next;
    } while(p_tmp != list->last);
}

// Добавление операнда к списку операндов
void Intermediate::OPD_LIST_append(OPD_LIST* list,OPERAND*  opdPtr)
{
    if(list->last == NULL)
        opdPtr->next = opdPtr;
    else {
        opdPtr->next = list->last->next;
        list->last->next = opdPtr;
    }
    list->last = opdPtr;
}

// Создание и начальная установка операнда метки
OPERAND* Intermediate::CreateLabel()
{
    struct INSTRUCTION* tmpInstrPtr; // указатель на команду - метку
    // создается операнд - метка
    struct OPERAND* tmpOpdPtr =
            OPERAND_constructor(labelOpd, NULL);
    // команда - метка с обратной связью на операнд
    tmpInstrPtr = INSTRUCTION_constructor(labelOpc, tmpOpdPtr, NULL, NULL);
    tmpOpdPtr->val.label = tmpInstrPtr;
    return tmpOpdPtr;
}


// Создание и заполнение структуры команды
struct INSTRUCTION* Intermediate::INSTRUCTION_constructor(opcType opc, struct OPERAND* arg1, OPERAND* arg2, OPERAND* rez)
{
    struct INSTRUCTION* iPtr;
    if((iPtr = (struct INSTRUCTION*)malloc(sizeof(struct INSTRUCTION)))==NULL)
    {
        //er(23);
        exit(1);
    }
    iPtr->opc = opc;
    iPtr->arg1 = arg1;
    iPtr->arg2 = arg2;
    iPtr->rez = rez;
    iPtr->next = NULL;
    //iPtr->line = line; // строка
    //iPtr->column = column; // колонка по симпольно
    return iPtr;
}

// Создание и заполнение структуры списка команд промежуточного представления
struct INSTR_LIST *Intermediate::INSTR_LIST_constructor(struct INSTRUCTION *fInstrPtr, struct INSTRUCTION *eInstrPtr, struct OPERAND *opdPtr)

{
    struct INSTR_LIST *iListPtr;
    if((iListPtr = (struct INSTR_LIST*)malloc(sizeof(struct INSTR_LIST)))==NULL)
    {
        //er(23);
        exit(1);
    }
    iListPtr->firstInstr = fInstrPtr;
    iListPtr->lastInstr = eInstrPtr;
    iListPtr->lastOpd = opdPtr;
    return iListPtr;
}

// Конкатенация двух списков команд в первый
// Замена или установка операнда списка не производится.
// Она должна осуществляться явно в программе анализатора
void Intermediate::INSTR_LIST_cat(struct INSTR_LIST **iListRecPtrPtr, struct INSTR_LIST *iListSrcPtr)
{
    // Проверка  на пустоту списков команд
    if(*iListRecPtrPtr == NULL) // Список приемника пуст
        if(iListSrcPtr == NULL) // Список источника пуст
            return; // - Получили пустой результат
        else { // а источник есть
            *iListRecPtrPtr = iListSrcPtr;
            return;
        }
    else // Список приемника есть
        if(iListSrcPtr == NULL) // Список источника пуст
            return; // - Получили результат, равный приемнику
    // Конкатенация реально происходит при непустом источнике
    if(iListSrcPtr->firstInstr != NULL) { // Источник не является пустым списком
        if((*iListRecPtrPtr)->firstInstr != NULL) // Приемник - не пустой список
            (*iListRecPtrPtr)->lastInstr->next = iListSrcPtr->firstInstr;
        else
            (*iListRecPtrPtr)->firstInstr = iListSrcPtr->firstInstr;
        (*iListRecPtrPtr)->lastInstr  = iListSrcPtr->lastInstr;
    }
}

// Добавление к списку команд отдельной команды
void Intermediate::INSTR_LIST_append(INSTR_LIST **iListRecPtrPtr, INSTRUCTION *instrPtr)
{
    // Проверка на отсутствие списка команд и его формирование в этом случае
    if(*iListRecPtrPtr == NULL) {// список команд отсутствует
        *iListRecPtrPtr = INSTR_LIST_constructor(NULL, NULL, NULL);
        (*iListRecPtrPtr)->firstInstr = instrPtr;
        (*iListRecPtrPtr)->lastInstr = instrPtr;
        return;
    }
    if((*iListRecPtrPtr)->firstInstr != NULL)
        (*iListRecPtrPtr)->lastInstr->next = instrPtr;
    else
        (*iListRecPtrPtr)->firstInstr = instrPtr;
    (*iListRecPtrPtr)->lastInstr = instrPtr;
}

// Вывод списка команд
void Intermediate::INSTR_LIST_out(struct INSTR_LIST *iListRec, FILE* outfil)
{
    int arg1Ident, arg2Ident, rezIdent;
    struct INSTRUCTION* tmpPtr;
    fprintf(outfil, "\n           Список команд:\n");
    if(iListRec == NULL) {
        fprintf(outfil, "Список команд отсутствует!\n");
        return;
    }
    if(iListRec->firstInstr == NULL) {
        fprintf(outfil, "Список команд пуст!\n");
        return;
    }
    for(tmpPtr = iListRec->firstInstr; tmpPtr != NULL; tmpPtr = tmpPtr->next) {
        char *opcName;
        switch(tmpPtr->opc) {
        //      case addOpc: 	 opcName = (char*)"ADD";	break;
        //      case assOpc: 	 opcName = (char*)"ASSIGN";	break;
        //      case divOpc: 	 opcName = (char*)"DIV";	break;
        //      case eqOpc: 	 opcName = (char*)"EQ";	break;
        //      case emptyOpc: 	 opcName = (char*)"EMPTY";	break;
        //      case exitOpc: 	 opcName = (char*)"EXIT";	break;
        //      case geOpc: 	 opcName = (char*)"GE";	break;
        //      case gotoOpc: 	 opcName = (char*)"GOTO";	break;
        //      case gtOpc: 	 opcName = (char*)"GT";	break;
        //      case ifOpc: 	 opcName = (char*)"IF";	break;
        //      case inOpc: 	 opcName = (char*)"IN";	break;
        //      case indexOpc: 	 opcName = (char*)"INDEX";	break;
        //      case labelOpc: 	 opcName = (char*)"LABEL";	break;
        //      case leOpc: 	 opcName = (char*)"LE";	break;
        //      case ltOpc: 	 opcName = (char*)"LT";	break;
        //      case minOpc: 	 opcName = (char*)"MIN";	break;
        //      case modOpc: 	 opcName = (char*)"MOD";	break;
        //      case multOpc: 	 opcName = (char*)"MULT";	break;
        //      case neOpc: 	 opcName = (char*)"NE";	break;
        //      case outOpc: 	 opcName = (char*)"OUT";	break;
        //      case subOpc: 	 opcName = (char*)"SUB";	break;
        //      case skipOutOpc: 	 opcName = (char*)"SKIPOUT";	break;
        //      case spaceOutOpc:  opcName = (char*)"SPACEOUT";	break;
        //      case tabOutOpc:    opcName = (char*)"TABOUT";	break;
        default: opcName = (char*)"NODEF";
        }
        arg1Ident = (tmpPtr->arg1 == NULL) ? 0 : tmpPtr->arg1->ident;
        arg2Ident = (tmpPtr->arg2 == NULL) ? 0 : tmpPtr->arg2->ident;
        rezIdent = (tmpPtr->rez == NULL) ? 0 : tmpPtr->rez->ident;

        fprintf(outfil, "%s\t%d,\t%d,\t%d\t// Строка = %d, столбец = %d\n",
                opcName, arg1Ident, arg2Ident, rezIdent, tmpPtr->line, tmpPtr->column
                );
    }
}


//мои функции

bool Intermediate::add_operand(opdType typ, void *val)
{
    OPERAND *opdPtr;
    opdPtr = OPERAND_constructor(typ,val);

    if(instruction->arg1 == nullptr)
    {
        instruction->arg1 = opdPtr;

        if(priority_rang != 0)
        {
            instruction->priority_rang = priority_rang; //устанавливаем данному вырожению приоритет
            //set_next_priori(priority_rang);
        }
    }
    else
    {
        if(instruction->opc != noValueOpc)
        {
            if(instruction->arg2 == nullptr)
            {
                instruction->arg2 = opdPtr;
                //instruction->rez  = opdPtr;

                // Создаем новую инструкцию
                INSTRUCTION* iPtr;
                if((iPtr = (struct INSTRUCTION*)malloc(sizeof(struct INSTRUCTION)))==NULL)
                {
                    //er(23);
                    exit(1);
                }
                iPtr->opc = noValueOpc;
                iPtr->arg1 = nullptr;
                iPtr->arg2 = nullptr;
                iPtr->rez = instruction->arg2;
                iPtr->next = nullptr;
                //iPtr->line = line; // строка
                //iPtr->column = column; // колонка по симпольно



                INSTR_LIST_append(&list,iPtr);
                instruction->next = iPtr;

                instruction->priority_rang = priority_rang; //устанавливаем данному вырожению приоритет

                instruction = instruction->next;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    return true;
}

bool Intermediate::add_opcType(opcType typ)
{
    switch (instruction->opc)
    {
    case noValueOpc:
    {
        instruction->opc = typ;

        tmpVarOpdNum++;

        //пробую промежуточное представление
        application* item = new application;
        item->typ =  INTTYP;
        item->len = 0;
        item->addr = 0;
        item->name = "tmp_"+to_string(tmpVarOpdNum);
        item->val = "tmp_"+to_string(tmpVarOpdNum);
        item->name = "tmp_"+to_string(tmpVarOpdNum);

        set_auto_rang(typ); //устанавливаем приоритет операции

        add_operand(tmpVarOpd,item);
        break;
    }
    case minusUnOpc:
    {
        break;
    }
    }

    return true;
}

bool Intermediate::set_next_priori(int rang)
{
    auto copy_first = list->firstInstr;
    auto copy_last = list->lastInstr;

    switch (rang)
    {
    case 1:
    {
        break;
    }

    case 4:
    {
        if(list)

            list->firstInstr = list->lastInstr;
        list->lastInstr = copy_first;

        list->firstInstr->next = copy_first->next; //скопировали указатель на следующий элимент от 1


        break;
    }


    case 5:
    {
        break;
    }
    }

    return true;
}

// устанавливает приоритет
void Intermediate::set_auto_rang(opcType typ)
{
    priority_rang = 0;

    switch (typ)
    {
    case LCircleOpc:			// круглые скобки '('
    case RCircleOpc:			// круглые скобки ')'
    {
        priority_rang = 1;
        break;
    }

    case starOpc:
    case slashOpc:
    case modOpc:
    {
        priority_rang = 4;
        break;
    }

    case  plusOpc:           //плюс '+'
    case  minusOpc:          //минус '-'
    {
        priority_rang = 5;
        break;
    }

    case assignOpc:         // авто присваивание ':='
    {
        priority_rang = 15;
        break;
    }
    }
}

string Intermediate::opc(opcType typ)
{
    string str = "";
    switch (typ)
    {
    case LCircleOpc: str = "(";break;
    case RCircleOpc: str = ")";break;
    case starOpc:    str = "*";break;
    case slashOpc:   str = "/";break;
    case  plusOpc:   str = "+";break;
    case minusUnOpc:
    case  minusOpc:  str = "-";break;
    case assignOpc:  str = ":=";break;
    case modOpc:     str = "%";break;
    }

    return str;
}


void Intermediate::file_clear(string fileName)
{
    ofstream fileOut2(fileName, ios::binary | ios::trunc); //| ios::trunc
    fileOut2.close();
}
/*
string Intermediate::out_stek(string& str2,stack<MYINST*>* steck)
{
    MYINST* top;
    while(!steck->empty())
    {
        top = steck->top();
        if(top->arg1 != nullptr)
        {
            switch (top->arg1->typ)
            {
            case constOpd:
            {
                str2 += to_string(steck->top()->arg1->val.cons->val.unum)+"\t";
                break;
            }
            case nameVarOpd:
            {
                auto tmpItem = steck->top()->arg1->val.var;

                while(!tmpItem->parent_empty())
                {
                    str2 += tmpItem->parent_get()->get_name()+".";
                    tmpItem = steck->top()->arg1->val.var->parent_get();
                }

                str2 += steck->top()->arg1->val.var->get_name()+"\t";
                break;
            }
            case tmpVarOpd:
            {
                str2 += steck->top()->arg1->val.varTmp->name+"\t";
                break;
            }
            default:
                str2 += "null\t";
                break;
            }

        }
        else
        {
            str2 += "null\t";
        }

        str2 +=opc(top->opc)+"\t";

        if(top->arg2 != nullptr)
        {
            switch (top->arg2->typ)
            {
            case constOpd:
            {
                str2 += to_string(steck->top()->arg2->val.cons->val.unum)+"\t";
                break;
            }
            case nameVarOpd:
            {
                auto tmpItem = steck->top()->arg2->val.var;

                while(!tmpItem->parent_empty())
                {
                    str2 += tmpItem->parent_get()->get_name()+".";
                    tmpItem = steck->top()->arg2->val.var->parent_get();
                }

                str2 += steck->top()->arg2->val.var->get_name()+"\t";
                break;
            }
            case tmpVarOpd:
            {

                str2 += steck->top()->arg2->val.varTmp->name+"\t";
                break;
            }
            default:
                str2 += "null\t";
                break;
            }
        }
        else
        {
            str2 += "null\t";
        }

        str2 +="\t=\t";

        if(top->rez != nullptr)
        {
            switch (top->rez->typ)
            {
            case constOpd:
            {
                str2 += to_string(steck->top()->rez->val.cons->val.unum)+"\t";
                break;
            }
            case nameVarOpd:
            {
                str2 += steck->top()->rez->val.var->get_name()+"\t";
                break;
            }
            case tmpVarOpd:
            {
                str2 += steck->top()->rez->val.varTmp->name+"\t";
                break;
            }
            default:
                str2 += "null\t";
                break;
            }
        }
        else
        {
            str2 += "null\t";
        }

        str2 += "priority_rang: "+to_string(steck->top()->priority_rang)+"\t number: "+to_string(steck->top()->number)+"\t\n";
        steck_begin_->steck_tmp.push(steck->top());
        steck->pop();
    }

    while(!steck_begin_->steck_tmp.empty())
    {
        steck->push(steck_begin_->steck_tmp.top());
        steck_begin_->steck_tmp.pop();
    }



}
*/

string Intermediate::out_stek_2(string &str, MashineSteck *item, int tab,countSteck st)
{
    MYINST* top;
    stack<MYINST*>* steck;

    switch (st)
    {
    case st1:
        steck = &item->steck;
        break;
    case st2:
        steck = &item->steck_no_priority;
        break;
    }

    while(!steck->empty())
    {
        top = steck->top();

        //        for(int i=0 ; i < tab;i++)
        //        {
        //            str +="\t";
        //        }

        if(top->opc == LCircleOpc)
        {
            out_stek_2(str,item->next.top(),tab++,st);
            item->steck_tmp.push(steck->top());
            steck->pop();

            item->next_tmp.push(item->next.top());
            item->next.pop();

            continue;
        }

        if(top->arg1 != nullptr)
        {
            switch (top->arg1->typ)
            {
            case constOpd:
            {
                str += to_string(steck->top()->arg1->val.cons->val.unum)+"\t";
                break;
            }
            case nameVarOpd:
            {
                auto tmpItem = steck->top()->arg1->val.var;

                while(!tmpItem->parent_empty())
                {
                    str += tmpItem->parent_get()->get_name()+".";
                    tmpItem = steck->top()->arg1->val.var->parent_get();
                }

                str += steck->top()->arg1->val.var->get_name()+"\t";
                break;
            }
            case tmpVarOpd:
            {
                str += steck->top()->arg1->val.varTmp->name+"\t";
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
                str += to_string(steck->top()->arg2->val.cons->val.unum)+"\t";
                break;
            }
            case nameVarOpd:
            {
                auto tmpItem = steck->top()->arg2->val.var;

                while(!tmpItem->parent_empty())
                {
                    str += tmpItem->parent_get()->get_name()+".";
                    tmpItem = steck->top()->arg2->val.var->parent_get();
                }

                str += steck->top()->arg2->val.var->get_name()+"\t";
                break;
            }
            case tmpVarOpd:
            {

                str += steck->top()->arg2->val.varTmp->name+"\t";
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
                str += to_string(steck->top()->rez->val.cons->val.unum)+"\t";
                break;
            }
            case nameVarOpd:
            {
                str += steck->top()->rez->val.var->get_name()+"\t";
                break;
            }
            case tmpVarOpd:
            {
                str += steck->top()->rez->val.varTmp->name+"\t";
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

        str += "priority_rang: "+to_string(steck->top()->priority_rang)+"\t number: "+to_string(steck->top()->number)+"\t\n";
        item->steck_tmp.push(steck->top());
        steck->pop();
    }

    while(!item->steck_tmp.empty())
    {
        steck->push(item->steck_tmp.top());
        item->steck_tmp.pop();
    }

    while(!item->next_tmp.empty())
    {
        item->next.push(item->next_tmp.top());
        item->next_tmp.pop();
    }

    return str;
}

void Intermediate::out_stek_file(string fileName,countSteck st)
{
    ofstream fileOut2(fileName, ios::binary | ios::app); //| ios::trunc
    string str2 = "\n\n";


    out_stek_2(str2,steck_begin_,0,st);

    fileOut2 << str2;
    fileOut2.close();
}


int tmpVariable = 0;
void Intermediate::create_tmpNumber(MashineSteck* steck_select_)
{

    while(!steck_select_->steck.empty())
    {

        if(steck_select_->steck.top()->opc == LCircleOpc)
        {
            create_tmpNumber(steck_select_->next.top());
            steck_select_->steck_tmp.push(steck_select_->steck.top());
            steck_select_->steck.pop();
            continue;
        }

        if(steck_select_->steck.top()->rez->typ == tmpVarOpd)
        {
            if(steck_select_->steck.top()->rez->val.varTmp->name == "")
            {
                tmpVariable++;
                steck_select_->steck.top()->rez->val.varTmp->name = "@t"+to_string(tmpVariable);
            }
        }

        if(steck_select_->steck.top()->arg1 != nullptr)
        {
            if(steck_select_->steck.top()->arg1->typ == tmpVarOpd)
            {
                if(steck_select_->steck.top()->arg1->val.varTmp->name == "")
                {
                    tmpVariable++;
                    steck_select_->steck.top()->arg1->val.varTmp->name = "@t"+to_string(tmpVariable);
                }
            }
        }


        steck_select_->steck_tmp.push(steck_select_->steck.top());
        steck_select_->steck.pop();
    }

    while(!steck_select_->steck_tmp.empty())
    {
        steck_select_->steck.push(steck_select_->steck_tmp.top());
        steck_select_->steck_tmp.pop();
    }

}

int tabs = 0;

bool Intermediate::out_list()
{

    create_tmpNumber(steck_begin_);
    out_stek_file("out_list2.txt",st1);
    out_stek_file("out_list3.txt",st2);

    steck_begin_ = new MashineSteck;
    steck_select_ = steck_begin_;
    steck_select_->parent = nullptr;

    myinstruction = create_instruction();

    return true;

}


MYOPER* Intermediate::create_myoperand(opdType typ, void *val)
{
    MYOPER *opdPtr;

    int OPERAND_size = sizeof(struct MYOPER);
    if((opdPtr = (struct MYOPER*)malloc(OPERAND_size))==NULL)
    {
        //er(23);
        exit(1);
    }
    opdPtr->ident = ++opdNum;
    opdPtr->typ = typ;
    opdPtr->next = NULL;
    switch(typ)
    {
    case constOpd:
        opdPtr->val.cons = (struct CONST *)val;
        break;
    case nameVarOpd:
        opdPtr->val.var = (MyVariable*) val;
        break;
    case tmpVarOpd:
        opdPtr->val.varTmp = new application; // организация и содержимое определятся позднее
        break;
    default:
        opdPtr->val.var = NULL; //
    }
    //OPD_LIST_append(&opdList, opdPtr);
    return opdPtr;
}

// Создаем новую инструкцию
MYINST *Intermediate::create_instruction()
{
    MYINST* iPtr = new MYINST;

    iPtr->opc = noValueOpc;
    iPtr->arg1 = nullptr;
    iPtr->arg2 = nullptr;
    iPtr->rez = nullptr;
    iPtr->next = nullptr;
    iPtr->line = 0; // строка
    iPtr->column = 0; // колонка по симпольно
    iPtr->priority_rang = 0;

    return iPtr;

}

bool Intermediate::add_operand(MYOPER *opdPtr)
{

    switch (myinstruction->opc)
    {
    case minusUnOpc:
    {

        add_left_operand(opdPtr);
        add_rezult(create_myoperand(tmpVarOpd,nullptr));
        steck_select_->steck.top(); // подумать о указателе на предыдущую иснтрукцию


        auto inPtr = create_instruction();
        myinstruction->next = inPtr;
        myinstruction = myinstruction->next;

        break;
    }
    case noValueOpc:
    {
        add_left_operand(opdPtr);
        break;
    }
    default:
    {
        add_right_operand(opdPtr);

        if(myinstruction->rez == nullptr)
        {

            auto item = steck_select_->steck_no_priority.top();
            steck_select_->steck_no_priority.pop();

            myinstruction->rez =  steck_select_->steck_no_priority.top()->arg2;
            steck_select_->steck_no_priority.push(item);


        }

        auto inPtr = create_instruction();
        myinstruction->next = inPtr;
        myinstruction = myinstruction->next;

        break;
    }
    }


    return true;
}

int command= 0;
bool Intermediate::add_left_operand(MYOPER* opdPtr)
{
    if(myinstruction->arg1 != nullptr)
    {
        return false;
    }

    myinstruction->arg1 = opdPtr;

    return true;
}

bool Intermediate::add_right_operand(MYOPER *opdPtr)
{
    if(myinstruction->arg2 != nullptr)
    {
        return false;
    }

    myinstruction->arg2 = opdPtr;
    return true;
}

bool Intermediate::add_rezult(MYOPER *opdPtr)
{
    if(myinstruction->rez != nullptr)
    {
        return false;
    }

    myinstruction->rez = opdPtr;
    return true;
}

bool Intermediate::add_opc(opcType typ)
{

    switch (typ)
    {
    case LCircleOpc:
    {

        if(myinstruction->arg1 != nullptr && myinstruction->rez != nullptr)
        {
            myinstruction->arg2 = create_myoperand(tmpVarOpd,nullptr);

            auto inPtr = create_instruction();
            myinstruction->next = inPtr;
            myinstruction = myinstruction->next;
        }


        command++;
        myinstruction->number = command;
        myinstruction->priority_rang = get_auto_rang(typ); // выявляем приоритет
        myinstruction->opc = typ;

        add_inSteck();



        steck_select_->next.push(new MashineSteck); //создаем новый стек (для скобок)
        steck_selectPrev_ = steck_select_; // запоминаем родителя
        steck_select_ = steck_select_->next.top(); // переходим к дочернему объекту
        steck_select_->parent = steck_selectPrev_;


        auto inPtr = create_instruction();
        myinstruction->next = inPtr;
        myinstruction = myinstruction->next;

        break;
    }
    case RCircleOpc:
    {
        add_last_oper();
        steck_select_ = steck_select_->parent; // возвращаемся на стек назад

        auto inPtr = create_instruction();
        myinstruction->next = inPtr;
        myinstruction = myinstruction->next;

        break;
    }

    default:
    {
        if(!steck_select_->steck.empty())
        {
            if(steck_select_->steck_no_priority.top()->opc == minusUnOpc && myinstruction == nullptr)
            {
                auto inPtr = create_instruction();
                myinstruction->next = inPtr;
                myinstruction = myinstruction->next;
            }

            if(typ == minusUnOpc || typ == plusUnOpc )
            {
                steck_select_->steck_no_priority.top()->arg2 = create_myoperand(tmpVarOpd,nullptr);
                auto inPtr = create_instruction();
                myinstruction->next = inPtr;
                myinstruction = myinstruction->next;
            }

        }



        command++;
        myinstruction->number = command;
        myinstruction->priority_rang = get_auto_rang(typ); // выявляем приоритет
        myinstruction->opc = typ;

        add_inSteck();

        break;
    }
    }


    return true;
}

bool Intermediate::add_last_oper()
{
    // if(steck_begin_->steck_no_priority.size() < 2)
    // {
    if(myinstruction->opc == noValueOpc && myinstruction->arg1 != nullptr)
    {
        steck_select_->steck_no_priority.top()->arg2 = myinstruction->arg1;
        delete  myinstruction;
    }
    else
    {
        auto item = steck_select_->steck_no_priority.top();
        steck_select_->steck_no_priority.pop();
        if(item->arg2 != nullptr && item->arg1 == nullptr)
        {
            item->arg1 = steck_select_->steck_no_priority.top()->arg2;
            item->rez = create_myoperand(tmpVarOpd,nullptr);
            steck_select_->steck_no_priority.top()->arg2 = item->rez;

        }
        steck_select_->steck_no_priority.push(item);
    }
    //  }


    return true;
}

//Создать инструкцию присвоения
bool Intermediate::create_statement(opcType typ, MYOPER *opdPtr)
{
    add_rezult(opdPtr);

    //создаем временную переменную в левом оперенде
    add_left_operand(create_myoperand(tmpVarOpd,nullptr));

    add_opc(typ);
}

bool Intermediate::add_inSteck()
{
    if(!steck_select_->steck_no_priority.empty())
    {

        //Создаем временные переменные
        switch (myinstruction->opc)
        {
        case assignOpc:
        {
            break;
        }
        case minusOpc:
        case plusOpc:
        {
            // проверить есть ли предыдущие операторы?


            if(steck_select_->steck_no_priority.top()->arg1 == nullptr && steck_select_->steck_no_priority.top()->arg2 != nullptr)
            {
                add_last_oper();
            }

            while(steck_select_->steck_no_priority.top()->opc != assignOpc || steck_select_->steck_no_priority.top()->opc != plusOpc || steck_select_->steck_no_priority.top()->opc != minusOpc)
            {

                if(steck_select_->steck_no_priority.top()->opc == LCircleOpc || steck_select_->steck_no_priority.top()->opc == assignOpc || steck_select_->steck_no_priority.top()->opc == plusOpc || steck_select_->steck_no_priority.top()->opc == minusOpc)
                {

                    //Если наткнулись на скобку то  посомтреть что с лева от скобки и справа (записать в аргумент и продолжить поиск)
                    if(steck_select_->steck_no_priority.top()->opc == LCircleOpc && myinstruction->arg1 == nullptr)
                    {
                        if(!steck_select_->steck_tmp.empty())
                        {

                            auto item = steck_select_->steck_no_priority.top();

                            steck_select_->steck_tmp.push(steck_select_->steck_no_priority.top());
                            steck_select_->steck_no_priority.pop();

                            auto left = steck_select_->steck_no_priority.top();

                             //Если за скобкой следует равно то арг1 установить как результат
                            if(left->opc == assignOpc)
                            {
                                left->arg1 = create_myoperand(tmpVarOpd,nullptr);
                                add_rezult(left->arg1);

                                //возвращаем скобку обратно
                                steck_select_->steck_no_priority.push(steck_select_->steck_tmp.top());
                                steck_select_->steck_tmp.pop();

                                //Берем результат скобки и записываем в 1 аргумент
                                add_left_operand(steck_select_->steck_tmp.top()->rez);
                                myinstruction->arg2 = create_myoperand(tmpVarOpd,nullptr);
                            }
                            else
                            {
                                steck_select_->steck_tmp.push(steck_select_->steck_no_priority.top());
                                steck_select_->steck_no_priority.pop();
                                continue;
                            }
                        }
                    }



                    break;
                }

                steck_select_->steck_tmp.push(steck_select_->steck_no_priority.top());
                steck_select_->steck_no_priority.pop();
            }


            switch (steck_select_->steck_no_priority.top()->opc)
            {
            case assignOpc:
            {
                // возвращаем в основной стек следующую операцию и берем от нее результат
                if(!steck_select_->steck_tmp.empty())
                {
                    steck_select_->steck_no_priority.push(steck_select_->steck_tmp.top());
                    steck_select_->steck_tmp.pop();

                    add_rezult(steck_select_->steck_no_priority.top()->rez);
                    steck_select_->steck_no_priority.top()->rez = create_myoperand(tmpVarOpd,nullptr);
                    add_left_operand(steck_select_->steck_no_priority.top()->rez);
                    add_right_operand(create_myoperand(tmpVarOpd,nullptr));

                }
                else
                {
                    add_rezult(steck_select_->steck_no_priority.top()->arg1);
                    add_right_operand(create_myoperand(tmpVarOpd,nullptr));
                }


                while(!steck_select_->steck_tmp.empty())
                {
                    steck_select_->steck_no_priority.push(steck_select_->steck_tmp.top());
                    steck_select_->steck_tmp.pop();
                }

                break;
            }
            default:
            {

                if(myinstruction->arg1 == nullptr && myinstruction->arg2 == nullptr && myinstruction->rez == nullptr)
                {
                    //Если наткнулись на скобку то записать в аргумент и продолжить поиск
                    if(steck_select_->steck_no_priority.top()->opc == LCircleOpc)
                    {
                        if(!steck_select_->steck_tmp.empty())
                        {
                            add_left_operand(steck_select_->steck_tmp.top()->rez);
                            myinstruction->arg2 = create_myoperand(tmpVarOpd,nullptr);

                            steck_select_->steck_tmp.push(steck_select_->steck_no_priority.top());
                            steck_select_->steck_no_priority.pop();

                            //Если за скобкой следует равно то арг1 установить как результат
                            if(steck_select_->steck_no_priority.top()->opc == assignOpc)
                            {
                                steck_select_->steck_no_priority.top()->arg1 = create_myoperand(tmpVarOpd,nullptr);
                                add_rezult(steck_select_->steck_no_priority.top()->arg1);
                            }

                            //add_rezult(create_myoperand(tmpVarOpd,nullptr));
                        }
                    }
                    else
                    {
                        add_left_operand(steck_select_->steck_no_priority.top()->arg2);
                        steck_select_->steck_no_priority.top()->arg2 = create_myoperand(tmpVarOpd,nullptr);
                        add_rezult(steck_select_->steck_no_priority.top()->arg2);
                    }
                }
                else
                {
                    add_rezult(steck_select_->steck_no_priority.top()->arg2);
                    add_right_operand(create_myoperand(tmpVarOpd,nullptr));
                }

                while(!steck_select_->steck_tmp.empty())
                {
                    steck_select_->steck_no_priority.push(steck_select_->steck_tmp.top());
                    steck_select_->steck_tmp.pop();
                }

                break;



                break;
            }
            }

            break;
        }

        case plusUnOpc:
        case minusUnOpc:
        {

            if(steck_select_->steck_no_priority.top()->opc == assignOpc)
            {
                add_rezult(steck_select_->steck_no_priority.top()->arg1);
            }
            else
            {
                add_rezult(steck_select_->steck_no_priority.top()->arg2);
            }


            break;
        }

        default:
        {
            if(steck_select_->steck_no_priority.top()->opc == assignOpc)
            {
                add_rezult(steck_select_->steck_no_priority.top()->arg1);

                add_left_operand(create_myoperand(tmpVarOpd,nullptr));
                //add_right_operand(create_myoperand(tmpVarOpd,nullptr));
            }
            else
            {
                //унарный плюс и унарный минус
                if(steck_select_->steck_no_priority.top()->priority_rang == 3)
                {
                    auto item = steck_select_->steck_no_priority.top();
                    steck_select_->steck_no_priority.pop();
                    add_left_operand(steck_select_->steck_no_priority.top()->arg2);
                    add_rezult(create_myoperand(tmpVarOpd,nullptr));
                    steck_select_->steck_no_priority.top()->arg2 = myinstruction->rez;
                    steck_select_->steck_no_priority.push(item);
                }
                else
                {

                    if(steck_select_->steck_no_priority.top()->opc == LCircleOpc)
                    {
                        if(myinstruction->arg1 == nullptr)
                        {
                            //add_left_operand(steck_select_->steck_no_priority.top()->rez);
                            //add_rezult(create_myoperand(tmpVarOpd,nullptr));

                            auto item = steck_select_->steck_no_priority.top();
                            add_left_operand(item->rez);
                            add_rezult(create_myoperand(tmpVarOpd,nullptr));

                            steck_select_->steck_no_priority.pop();
                            if(steck_select_->steck_no_priority.top()->opc == assignOpc)
                            {
                                steck_select_->steck_no_priority.top()->arg1 = myinstruction->rez;
                            }
                            else
                            {
                                steck_select_->steck_no_priority.top()->arg2 = myinstruction->rez;
                            }
                            steck_select_->steck_no_priority.push(item);
                        }
                        else
                        {

                        }
                    }
                    else
                    {

                        //                        if(myinstruction->opc == LCircleOpc)
                        //                        {
                        //                            if(myinstruction->arg1 == nullptr)
                        //                            {
                        //                                myinstruction->arg1 = steck_select_->steck_no_priority.top()->arg2;
                        //                            }
                        //                        }


                        add_rezult(steck_select_->steck_no_priority.top()->arg2);
                        //add_right_operand(create_myoperand(tmpVarOpd,nullptr));
                    }

                }



            }
            break;
        }
        }


        int rang = myinstruction->priority_rang; // приоритет иснтрукции которую хотят положить
        int rang_steck = steck_select_->steck.top()->priority_rang; // приоритет иснтрукции которая лежит на верху стека

        //Условие по приоритету
        while(rang > rang_steck)
        {
            steck_select_->steck_tmp.push(steck_select_->steck.top());
            steck_select_->steck.pop();
            rang_steck = steck_select_->steck.top()->priority_rang;
        }

    }
    else
    {
        if(steck_select_->parent != nullptr)
        {
            if(myinstruction->arg1 ==nullptr)
            {
                add_rezult(steck_select_->parent->steck_no_priority.top()->rez);
                add_right_operand(create_myoperand(tmpVarOpd,nullptr));
            }
            else
            {

                if(steck_select_->steck_no_priority.empty())
                {
                    add_rezult(steck_select_->parent->steck_no_priority.top()->rez);
                    add_right_operand(create_myoperand(tmpVarOpd,nullptr));
                }
                else
                {
                    add_rezult(create_myoperand(tmpVarOpd,nullptr));
                    add_right_operand(create_myoperand(tmpVarOpd,nullptr));
                }



            }
        }
    }


    steck_select_->steck.push(myinstruction);
    steck_select_->steck_no_priority.push(myinstruction);

    while(!steck_select_->steck_tmp.empty())
    {
        steck_select_->steck.push(steck_select_->steck_tmp.top());
        steck_select_->steck_tmp.pop();
    }


    if((myinstruction->arg1 != nullptr && myinstruction->arg2 != nullptr) || myinstruction->opc == assignOpc)
    {
        auto inPtr = create_instruction();
        myinstruction->next = inPtr;
        myinstruction = myinstruction->next;
    }


    switch (myinstruction->opc)
    {

    case starOpc:
    case minusUnOpc:
    {
        break;
    }
        //    default:
        //    {
        //        auto inPtr = create_instruction();
        //        myinstruction->next = inPtr;
        //        myinstruction = myinstruction->next;
        //        break;
        //    }
    }

    return true;
}


int Intermediate::get_auto_rang(opcType typ)
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

    case assignOpc:         // авто присваивание ':='
    {
        priority_rang = 15;
        break;
    }
    }

    return priority_rang;
}

bool Intermediate::create_tmpVar()
{
    //    while(!steck_begin_->steck.empty())
    //    {
    //        switch(steck_begin_->steck.top()->opc)
    //        {
    //            case assignOpc:
    //            {
    //                leftOpc
    //                break;
    //            }
    //        }
    //    }
}

//bool Intermediate::remove_last_tmpVarOpd()
//{
//    auto parent = instruction->parent;
//    if(parent->opc == assignOpc || parent->opc != noValueOpc)
//    {
//        parent->arg2 = instruction->arg1;
//    }

//    delete  instruction;

//    instruction = parent;
//}

