#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H


#include "my/three_address_code/polish_notation/polish_notation.h"
#include <iostream>

#include <fstream>


typedef enum
{
    all,
    c_plus_plus,
    c_sharp,
    javascript,
    python,
    llvm
} to_generate;

class CodeGenerator
{
public:
    explicit CodeGenerator();

    virtual bool generate(vector<INSTRUCTION*> vector_polish,to_generate typ);

};

#endif // CODEGENERATOR_H
