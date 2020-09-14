#include "codegenerator.h"

#include "my/code_generator/c_plusplus.h"

CodeGenerator::CodeGenerator()
{

}

bool CodeGenerator::generate(vector<INSTRUCTION *> _vector_polish, to_generate typ)
{
        C_PlusPlus c_plus_plus_ = new C_PlusPlus();
        switch (typ)
        {
            default:
                c_plus_plus_.vector_polish = _vector_polish;
                c_plus_plus_.generate();
            break;
        }
}
