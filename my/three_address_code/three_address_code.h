#ifndef THREE_ADDRESS_CODE_H
#define THREE_ADDRESS_CODE_H

#include <iostream>
using namespace std;
#include <stack>
#include "my/myvariable.h"
#include "my/functype.h"

#include "my/three_address_code/Type_data.h"




struct INSTRUCTION
{
    opcType opc; //код операции

    // Операндная часть инструкции
    struct OPERAND *arg1;
    struct OPERAND *arg2;
    struct OPERAND *rez;

    int priority_rang; //ранг приоритета операции

    enum{
        none,
        leftCircle,
        rightCircle
    } circle;
};


// Организация операнда
struct OPERAND
{
    opdType typ; // тип операнда
    union{
        //ссылка на функцию
        FuncType* fanc;
        //ссылка на именованную переменную
        MyVariable* var;
        // ссылка на константу
        CONST *cons;
        // ссылка на промежуточную переменную
        application* varTmp;
        // ссылка на метку (позиция в основном векторе трехадресных команд)
        LABEL* label;
    }val;

    // Дополнительные параметры, обеспечивающие тестирование
    //int ident; // уникальный идентификатор операнда
    //OPERAND *next; // для организации списка операндов
};


class Three_address_code
{
public:
    Three_address_code();

    //1) создать новую инструкцию
    INSTRUCTION* Create_new_inctruction(opcType var_ocp);
    //2) создаем операнд
    OPERAND* Create_new_operand(opdType typ,void* val);

    //Добавить левый оеранд к инструкции
    void add_left_operand(INSTRUCTION* instruction_ptr, OPERAND* operand_ptr);
    //Добавить правый оеранд к инструкции
    void add_right_operand(INSTRUCTION* instruction_ptr, OPERAND* operand_ptr);
    //Добавить результат оеранд к инструкции
    void add_rezult_operand(INSTRUCTION* instruction_ptr, OPERAND* operand_ptr);


    //Приоритет инструкции
    int get_auto_rang(opcType typ);


};

#endif // THREE_ADDRESS_CODE_H
