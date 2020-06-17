#ifndef POLISH_NOTATION_H
#define POLISH_NOTATION_H

#include <iostream>
using namespace std;

#include <stack>
#include <vector>
#include <map>

#include <my/three_address_code/three_address_code.h>



//RPN - обратная польская запись
//Формирует дерево, где листья - это оперенды, а корни - это операции
class Polish_notation : public Three_address_code
{
public:
    Polish_notation();
    // добавить объект в стек операций
    void push_operation(opcType var_opc);
    // добавить объект в стек операндов
    OPERAND *push_operand(opdType typ,void* var_operand);
    // Строка закончилась вытащить из стека оставшиеся операции
    void End();
    //проверка приоритета
    bool check_priopity(INSTRUCTION* instruction_ptr);

    //вытащить из стека операций верхушку стека
    INSTRUCTION* pop_operation();
    //вытащить из стека оперендов верхушку стека
    OPERAND* pop_operand();

    //Записать в файл результат
    void out_stek_file(string fileName);
    //Преобразование операции в строку операции
    string opc(opcType typ);

    //Установить метку
    void set_goto_label();



private:
    stack<INSTRUCTION*> stack_operations; // стек операций для обратной польской записи
    stack<OPERAND*> stack_operand; // стек операндов для обратной польской записи

    vector<INSTRUCTION*> vector_polish;  // стек польской записи
    stack<INSTRUCTION*> stack_goto_labels; // стек хранит инструкции где ожидается определение метки

};

#endif // POLISH_NOTATION_H
