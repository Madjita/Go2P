#ifndef POLISH_NOTATION_H
#define POLISH_NOTATION_H

#include <iostream>
using namespace std;

#include <stack>
#include <vector>
#include <map>

#include <my/three_address_code/three_address_code.h>


typedef enum
{
  if_true,
  if_false,
  if_break_t,
  if_break_f,
  if_continue_t,
  if_continue_f,

  it_is_for,
  it_is_for_while,    //  цикол с пост условием
  it_is_for_infiniti, //  бесконечный цикл
  it_is_for_break,
  it_is_for_continue,
  it_is_if_break
} for_or_if;

//RPN - обратная польская запись
//Формирует дерево, где листья - это оперенды, а корни - это операции
class Polish_notation : public Three_address_code
{
public:
    Polish_notation();
    // добавить объект в стек операций
    void push_operation(opcType var_opc);
    // добавить объект в стек операций в виде определенной метки
    void push_operation(opcType var_opc,label_typ typ);
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
    void set_goto_label(for_or_if typ);

    //Убрать из стека метку
    void label_pop();

    // сохранненная позиция  метки на начало цикла forƒ
    void save_label_begin_for(for_or_if typ = it_is_for_infiniti);

    //Возвращает позицию выполнения действия по окончания тела цикла
    int begin_position_expression();
    int end_position_expression();

    void save_expression_for(unsigned int begin, unsigned int end);

private:
    stack<INSTRUCTION*> stack_operations; // стек операций для обратной польской записи
    stack<OPERAND*> stack_operand; // стек операндов для обратной польской записи

    vector<INSTRUCTION*> vector_polish;  // стек польской записи
    stack<INSTRUCTION*> stack_goto_labels; // стек хранит инструкции где ожидается определение метки

    stack<int> label_begin_for; // сохранненная позиция  метки на начало цикла for
    stack<stack<INSTRUCTION*>*> stack_breaks; // сохраняем в стек циклы из которых нужно выйти
    stack<stack<INSTRUCTION*>*> stack_continue; // сохраняем в стек циклы в которых нужно прыгнуть на следующий шаг

    stack<vector<INSTRUCTION*>*> stack_expression_for; //сохранить действия которые должны быть выполненны в конце цикла
};

#endif // POLISH_NOTATION_H

