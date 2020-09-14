#ifndef C_PLUSPLUS_H
#define C_PLUSPLUS_H

#include "my/code_generator/codegenerator.h"



class C_PlusPlus : public CodeGenerator
{
public:
    C_PlusPlus(CodeGenerator* parent = nullptr);

    virtual bool generate(); //vector<INSTRUCTION *> vector_polish

    void expression(string &str, stack<INSTRUCTION *> &stack_expression,string tmp);

    string expression(INSTRUCTION * item);

    string reverse(stack<char> stroka);

    string worker(stack<INSTRUCTION*>* stack_expression,int n);

    //обработка аргументов в функции
    string processing_func_arg(int n);

    //обработка цикла
    string loop(stack<INSTRUCTION*>* stack_expression, int n, int &position_str_add, string &code);

    //Обработка выражения
    string expression(stack<INSTRUCTION*>* stack_expression,int n);


    //Вспомогательная функция указывающая на принадлежность к Операции
    bool get_opcType(INSTRUCTION * item);



private:
    ofstream fileOut2;
    string str;

    int tabs = 0;

    unsigned int position;

};

#endif // C_PLUSPLUS_H
