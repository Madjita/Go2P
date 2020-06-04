#include "intermediate.h"

static int opdNum = 0; // Номер операнда (в порядке создания)
static int tmpVarOpdNum = 0; // Номер временной переменной (в порядке создания)

Intermediate::Intermediate()
{


    priority_rang = 0;
    myinstruction = create_instruction();


    steck_begin_ = new MashineSteck();
    steck_select_ = steck_begin_;


}









//мои функции

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

            steck_select_->next_tmp.push(steck_select_->next.top());
            steck_select_->next.pop();


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

    while(!steck_select_->next_tmp.empty())
    {
        steck_select_->next.push(steck_select_->next_tmp.top());
        steck_select_->next_tmp.pop();
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


//        steck_select_->steck_tmp.push(steck_select_->steck_no_priority.top());

//        myinstruction = steck_select_->steck_no_priority.top();

//        if(myinstruction->opc != assignOpc)
//        {

//        }


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
        delete steck_select_->steck_no_priority.top()->arg2;
        steck_select_->steck_no_priority.top()->arg2 = myinstruction->arg1;
        delete  myinstruction;
    }
    else
    {
        auto item = steck_select_->steck_no_priority.top();
        steck_select_->steck_no_priority.pop();
        if(item->arg2 != nullptr && item->arg1 == nullptr)
        {

            if(steck_select_->steck_no_priority.top()->opc == LCircleOpc)
            {
                //Надо подумать
            }

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

                        auto item = steck_select_->steck_no_priority.top();

                        steck_select_->steck_tmp.push(steck_select_->steck_no_priority.top());
                        steck_select_->steck_no_priority.pop();

                        auto left = steck_select_->steck_no_priority.top();

                        //возвращаем скобку обратно
                        steck_select_->steck_no_priority.push(steck_select_->steck_tmp.top());
                        steck_select_->steck_tmp.pop();

                        if(left->opc == assignOpc)
                        {
                            if(steck_select_->steck_tmp.empty())
                            {
                                add_left_operand(steck_select_->steck_no_priority.top()->rez);
                            }
                            else
                            {
                                add_left_operand(left->arg1);
                            }

                            add_right_operand(create_myoperand(tmpVarOpd,nullptr));
                            left->arg1 = create_myoperand(tmpVarOpd,nullptr);
                            add_rezult(left->arg1);

                        }
                        else
                        {
                            if(left->priority_rang == 6)
                            {
                                add_left_operand(left->arg2);
                                left->arg2 = create_myoperand(tmpVarOpd,nullptr);
                                add_rezult(left->arg2);
                            }
                            else
                            {

                                add_rezult(left->rez);
                                left->rez = create_myoperand(tmpVarOpd,nullptr);
                                add_left_operand(left->rez);
                                add_right_operand(create_myoperand(tmpVarOpd,nullptr));

                            }
                        }




                        // if(!steck_select_->steck_tmp.empty())
                        // {

                        //auto item = steck_select_->steck_no_priority.top();

                        //steck_select_->steck_tmp.push(steck_select_->steck_no_priority.top());
                        //steck_select_->steck_no_priority.pop();

                        //auto left = steck_select_->steck_no_priority.top();

                        //Если за скобкой следует равно то арг1 установить как результат
                        //if(left->opc == assignOpc)
                        //{

                        //
                        //Думаю
                        //Берем результат скобки и записываем в 1 аргумент
                        /*if(steck_select_->steck_tmp.empty())
                            {
                                add_left_operand(steck_select_->steck_no_priority.top()->rez);

                            }
                            else
                            {
                                //add_left_operand(steck_select_->steck_tmp.top()->rez);
                                add_left_operand(left->arg1);
                                left->arg1 = create_myoperand(tmpVarOpd,nullptr);
                                add_rezult(left->arg1);
                            }*/
                        //

                        //                            left->arg1 = create_myoperand(tmpVarOpd,nullptr);
                        //                            add_rezult(left->arg1);

                        //                            //возвращаем скобку обратно
                        //                            steck_select_->steck_no_priority.push(steck_select_->steck_tmp.top());
                        //                            steck_select_->steck_tmp.pop();

                        //                            //Берем результат скобки и записываем в 1 аргумент
                        //                            if(steck_select_->steck_tmp.empty())
                        //                            {
                        //                                add_left_operand(steck_select_->steck_no_priority.top()->rez);
                        //                            }
                        //                            else
                        //                            {
                        //                                add_left_operand(steck_select_->steck_tmp.top()->rez);
                        //                            }


                        /*myinstruction->arg2 = create_myoperand(tmpVarOpd,nullptr);
                        }
                        else
                        {

                            steck_select_->steck_tmp.push(steck_select_->steck_no_priority.top());
                            steck_select_->steck_no_priority.pop();
                            continue;
                        }*/

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
                    add_left_operand(steck_select_->steck_no_priority.top()->arg2);
                    steck_select_->steck_no_priority.top()->arg2 = create_myoperand(tmpVarOpd,nullptr);
                    add_rezult(steck_select_->steck_no_priority.top()->arg2);
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

                        if(myinstruction->opc == LCircleOpc)
                        {
                            if(myinstruction->arg1 == nullptr)
                            {
                                myinstruction->arg1 = steck_select_->steck_no_priority.top()->arg2;
                            }
                        }




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

