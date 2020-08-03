#ifndef C_PLUSPLUS_H
#define C_PLUSPLUS_H

#include "my/code_generator/codegenerator.h"



class C_PlusPlus : public CodeGenerator
{
public:
    C_PlusPlus(CodeGenerator* parent = nullptr);

    virtual bool generate(vector<INSTRUCTION *> vector_polish);

private:
    ofstream fileOut2;

    string str;
};

#endif // C_PLUSPLUS_H
