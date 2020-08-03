#include "c_plusplus.h"

C_PlusPlus::C_PlusPlus(CodeGenerator* parent)
{
    fileOut2.open("C_PlusPlus.txt", ios::binary | ios::trunc);
    fileOut2.close();

    fileOut2.open("C_PlusPlus.txt", ios::binary | ios::app); //| ios::trunc
    str = "";
}

bool C_PlusPlus::generate(vector<INSTRUCTION *> vector_polish)
{

    vector<INSTRUCTION*> vector_func_args;
    vector<INSTRUCTION*> vector_return;

    for(INSTRUCTION* item: vector_polish)
    {
        switch (item->opc)
        {
        case param:{
            vector_func_args.push_back(item);
            break;
        }
        case callFunc_Begin:{
            //Берем возвращяемое значение функции
            auto return_type = item->rez->val.fanc->getReturnFuncType();

            if(return_type != ""){
                str += return_type+" ";
            }
            else {
               str += "void ";
            }

             //Вставляем имя функции
             str += item->rez->val.fanc->getNameFuncType()+" ";
             str += "(";

             if(!vector_func_args.empty())
             {
                str += " ";

                for(int i=0; i < vector_func_args.size();i++)
                {
                    auto item_arg = vector_func_args[i]->rez->val.var;
                    str += item_arg->get_type();
                    str += " ";
                    str += item_arg->get_name();

                    if(i+1 != vector_func_args.size())
                        str += ", ";

                    //Находим локальную переменную
                   //auto item_local = item->rez->val.fanc->get_variable_at();
                }

                vector_func_args.clear();
             }

             str+= ")";
             str+= "\n";
             str+= "{";
             str+= "\n";
             str+= "\t";

            break;
        }
        case returnOpc:
        {
            str+= "return ";

            switch (item->rez->typ)
            {
            case constOpd:
                switch (item->rez->val.cons->typ)
                {
                case INTTYP:
                    str+= to_string(item->rez->val.cons->val.unum);
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }

            str += "\n";

            break;
        }

        case callFunc_End:{
            str+= "}\n";
            str+= "\n";
            break;
        }
        default:
            break;
        }
    }

    fileOut2 << str;

    fileOut2.close();
}
