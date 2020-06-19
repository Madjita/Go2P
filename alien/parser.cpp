#include "parser.h"

Parser::Parser(std::string fileData, CompileErrors &errors)
{
    scanAliend = new Scanner(fileData); //,errors
}

//bool Parser::TypeInteger()
//{
//    auto position = scanAliend->getPosition();

//    if(nextLex() == keyInt)
//    {
//        goto _end;
//    }
//    scanAliend->setPosition(position);

//    if(nextLex() == keyInt8)
//    {
//        goto _end;
//    }
//    scanAliend->setPosition(position);

//    if(nextLex() == keyInt16)
//    {
//        goto _end;
//    }
//    scanAliend->setPosition(position);

//    if(nextLex() == keyInt32)
//    {
//        goto _end;
//    }
//    scanAliend->setPosition(position);

//    if(nextLex() == keyInt64)
//    {
//        goto _end;
//    }
//    scanAliend->setPosition(position);

//    return false;

//_end:
//    return true;
//}

bool Parser::TypeInteger()
{
    //auto position = scanAliend->getPosition();
    if(lex == keyInt)
    {
        goto _end;
    }
    //scanAliend->setPosition(position);

    if(lex == keyInt8)
    {
        goto _end;
    }
    //scanAliend->setPosition(position);

    if(lex == keyInt16)
    {
        goto _end;
    }
    //scanAliend->setPosition(position);

    if(lex == keyInt32)
    {
        goto _end;
    }
    // scanAliend->setPosition(position);

    if(lex == keyInt64)
    {
        goto _end;
    }
    //scanAliend->setPosition(position);

    return false;

_end:
    return true;
}

bool Parser::TypeUInteger()
{
    if(lex == keyUInt)
    {
        goto _end;
    }

    if(lex == keyUInt8)
    {
        goto _end;
    }

    if(lex == keyUInt16)
    {
        goto _end;
    }

    if(lex == keyUInt32)
    {
        goto _end;
    }

    if(lex == keyUInt64)
    {
        goto _end;
    }

    return false;

_end:
    return true;
}

bool Parser::TypeFloat()
{


    if(lex == keyFloat)
    {
        goto _end;
    }

    if(lex == keyFloat32)
    {
        goto _end;
    }

    if(lex == keyFloat64)
    {
        goto _end;
    }

    return false;

_end:
    return true;
}

bool Parser::Type()
{

    if(lex == lcStar)
    {
        if(newFuncItem != nullptr)
        {
            newFuncItem->addNameFuncReturnType(scanAliend->GetTxtValue());
        }

        nextLex();
    }

    if(lex == lcImpersant)
    {
        if(newFuncItem != nullptr)
        {
            newFuncItem->addNameFuncReturnType(scanAliend->GetTxtValue());
        }

        nextLex();
    }


    if(TypeInteger())
    {
        goto _end;
    }

    if(TypeUInteger())
    {
        goto _end;
    }

    if(TypeFloat())
    {
        goto _end;
    }

    if(lex == keyString)
    {
        goto _end;
    }

    if(lex == keyByte)
    {
        goto _end;
    }

    if(lex == lcId)
    {
        goto _end;
    }

    return false;

_end:
    return true;
}

bool Parser::NewType()
{
    if(lex == keyType)
    {
        nextLex();
    }
    else
    {
        return false;
    }

    if(lex == lcId)
    {
        //запомнить имя нового типа
        //nameThisType = scanAliend->GetTxtValue();
        nextLex();
    }
    else
    {

        return false;
    }

    if(Type())
    {
        nextLex();
    }
    else
    {
        return false;
    }

    if(End())
    {
        goto _end;
    }

_end:
    return true;

}

bool Parser::newFunc()
{
    InformPosition position;
    LexClass saveLex;
    string saveValue;
    vector<map<LexClass,string>> saveItem;

    //Позиция откуда записывать для сохранения информации о написанном коде в объекте
    int start_position_vvMap_KeyData = distance(vvMap_KeyData.begin(),vvMap_KeyData.end()-1);

    if(lex == keyFunc)
    {
        nextLex();
        goto _1;
    }
    else
    {
        return false;
    }

_1:
    if(lex == lcId)
    {
        //Запомнить имя новой функции
        newFuncItem->addNameFuncType(scanAliend->GetTxtValue());
        nextLex();
        goto _2;
    }
    else
    {
        return false;
    }

_2:
    if(lex == lcLCircle)
    {
        nextLex();
        goto _3;
    }
    else
    {
        return false;
    }

_3:

    if(funcArgsStatement())
    {
        nextLex();
    }

    if(lex == lcComma)
    {
        nextLex();
        goto _3;
    }


_4:
    if(lex == lcRCircle)
    {
        nextLex();
        goto _5;
    }
    else
    {
        return false;
    }

_5:

    if(Type())
    {
        newFuncItem->addNameFuncReturnType(scanAliend->GetTxtValue());
        nextLex();
        goto _6;
    }

_6:

    if(lex == lcEnter)
    {
        nextLex();
        goto _6;
    }


    if(lex == lcLFigure)
    {
        nextLex();
        goto _7;
    }
    else
    {
        return false;
    }

    position = scanAliend->getPosition();

_7:

    if(lex == lcEnter)
    {
        nextLex();
        position = scanAliend->getPosition();
        saveLex = lex;
        saveValue = scanAliend->GetTxtValue();
        //saveValue = scanAliend->GetTxtValue();

        //        string value = scanAliend->GetTxtValue();
        //        if(lex == lcId && scanAliend->GetLex() != lcId)
        //        {
        //            position.value = *value.begin();
        //            position.position_file_str = position.position_file_str - (value.size()-1);
        //            position.position_col_char_str = position.position_col_char_str - (value.size()-1);
        //            scanAliend->setPosition(position);
        //        }

        //Ищим имя в функциях
        if(lex == lcId)
        {
            string func_name = scanAliend->GetTxtValue();

            auto func = table.find_name_func(func_name);

            if(func != nullptr)
            {
                scanAliend->setValue(saveValue);
                goto _var;
            }
        }


        saveItem = *vvMap_KeyData.rbegin();

        goto _7;
    }

    if(funcReturn())
    {
        nextLex();
        goto _8;
    }



    //инициализация атоматическая
    if(StatementShortInit())
    {
        auto item = *vvMap_KeyData.rbegin();
        item.erase(item.begin(),item.begin()+2);
        newFuncItem->setExpression(item);
        if(newFuncItem != nullptr)
        {
            newFuncItem->addNewVariables();
        }

        goto _7;
    }
    scanAliend->setPosition(position);
    lex = saveLex;
    (*vvMap_KeyData.rbegin()) = saveItem;


    if(Statement())
    {
        auto item = *vvMap_KeyData.rbegin();
        item.clear();
        newFuncItem->setExpression(item);
        if(newFuncItem != nullptr)
        {
            newFuncItem->addNewVariables();
        }

        goto _7;
    }
    scanAliend->setPosition(position);
    lex = saveLex;
    (*vvMap_KeyData.rbegin()) = saveItem;

    if(StatementInit())
    {
        auto item = *vvMap_KeyData.rbegin();

        string variable_label;

        for(auto it = item.begin(); it != item.end(); it++)
        {
            if(it->rbegin()->first == lcEqual )
            {

                for(auto it_variable = item.begin(); it_variable != it;it_variable++)
                {
                    variable_label += it_variable->begin()->second;
                }


                item.erase(item.begin(),it);
                break;
            }
        }

        newFuncItem->setExpression(item);
        if(newFuncItem != nullptr)
        {
            newFuncItem->initVariables(variable_label);
        }

        goto _7;
    }
    scanAliend->setPosition(position);
    lex = saveLex;
    (*vvMap_KeyData.rbegin()) = saveItem;

    if(addRecordNewRow())
    {
        goto _7;
    }
    scanAliend->setPosition(position);
    lex = saveLex;
    (*vvMap_KeyData.rbegin()) = saveItem;



    //if/else
    newFuncItem->add_new_if_else();
    point_if_else = newFuncItem->get_if_else(newFuncItem->get_if_else_size());
    if(IfElse())
    {
        goto _7;
    }
    point_if_else = nullptr;
    newFuncItem->remove_new_if_else();
    scanAliend->setPosition(position);
    lex = saveLex;
    (*vvMap_KeyData.rbegin()) = saveItem;

    if(loop())
    {
        goto _7;
    }
    scanAliend->setPosition(position);
    lex = saveLex;
    (*vvMap_KeyData.rbegin()) = saveItem;

    if(switchCase())
    {
        goto _7;
    }
    scanAliend->setPosition(position);
    lex = saveLex;
    (*vvMap_KeyData.rbegin()) = saveItem;


_var:
    if(Variable())
    {
        nextLex();
        goto _7;
    }
    scanAliend->setPosition(position);
    lex = saveLex;
    (*vvMap_KeyData.rbegin()) = saveItem;




_8:
    if(lex == lcRFigure)
    {
        goto _end;
    }
    else
    {
        return false;
    }

_end:

    //Сохранение информации о написанном коде в объекте
    newFuncItem->add_vvMap_KeyData(vvMap_KeyData,start_position_vvMap_KeyData);

    return true;


}

//var <name> <type>
bool Parser::Statement()
{
    //Позиция откуда записывать для сохранения информации о написанном коде в объекте
    int start_position_vvMap_KeyData = distance(vvMap_KeyData.begin(),vvMap_KeyData.end()-1);

    string nameId = "";
    string type = "";
    if(lex == keyVar)
    {
        nextLex();
    }
    else
    {
        return false;
    }

_1:
    if(lex == lcId)
    {
        nameId = scanAliend->GetTxtValue();

        if(newFuncItem != nullptr)
        {
            //новое
            if(!newFuncItem->add_variables_to_vector(nameId))
            {
                cout << "Error: Find repeat label in Func body Statment."<<endl;
                return false;
            }

        }



        nextLex();
        goto _2;
    }
    else
    {
        //Ошибка нет индификатора
        return false;
    }

_2:
    if(lex == lcComma)
    {
        nextLex();
        goto _1;
    }

    if(Type())
    {
        type = scanAliend->GetTxtValue();

        if(newFuncItem != nullptr)
        {
            HardType* newObject = table.create_object_hardType(type);
            if(newObject != nullptr)
            {
                newFuncItem->set_variable_struct(newObject);
            }

            newFuncItem->add_variables_type(type);
        }

        nextLex();
        goto _3;
    }
    else
    {
        //Ошибка нет типа данных
        return false;
    }

_3:
    if(End())
    {
        if(newFuncItem == nullptr)
        {
            HardType* newObject = table.create_object_hardType(type);
            if(newObject != nullptr)
            {
                //Добавить глобальную переменную в таблицу, так как находимся не в функции и данный тип существует как структра (сложный тип данных)
                table.add_gloabals(lcId,nameId,lcHardType,type,newObject);
            }
        }
        goto _end;
    }

    if(lex == lcEqual)
    {
        nextLex();
    }
    else
    {
        return false;
    }

_5:
    if(Expression())
    {
        goto _4;
    }
    else
    {
        return false;
    }

_4:
    if(lex == lcComma)
    {
        nextLex();
        goto _5;
    }

    if(lex == lcEof)
    {
        goto _end;
    }


    if(End())
    {
        goto _end;
    }
    else
    {
        //Ошибка нет типа данных
        return false;
    }

_end:
    if(newFuncItem != nullptr)
    {
        newFuncItem->get_variable_las()->add_vvMap_KeyData(vvMap_KeyData,start_position_vvMap_KeyData);
    }
    return true;

}

bool Parser::StatementArray()
{
    if(lex == keyVar)
    {
        nextLex();
    }
    else
    {
        return false;
    }

_1:
    if(lex == lcId)
    {
        nextLex();
        goto _2;
    }
    else
    {
        //Ошибка нет индификатора
        return false;
    }

_2:
    if(lex == lcLSquare)
    {
        nextLex();
    }
    else
    {
        return false;
    }

    if(lex == lcIntNum)
    {
        nextLex();
    }

    if(lex == lcRSquare)
    {
        nextLex();
    }
    else
    {
        return false;
    }


    if(Type())
    {
        nextLex();
        goto _3;
    }
    else
    {
        //Ошибка нет типа данных
        return false;
    }

_3:
    if(End())
    {
        goto _end;
    }


    if(lex == lcEqual)
    {
        nextLex();
    }
    else
    {
        //Ошибка нет '='
        return false;
    }

    if(StatementArrayInit())
    {
        nextLex();
    }
    else
    {
        //Ошибка нет инициализации
        return false;
    }

    if(End())
    {
        goto _end;
    }
    else
    {
        return false;
    }



_end:
    return true;
}

bool Parser::StatementArrayInit()
{
    if(lex == lcLSquare)
    {
        nextLex();
    }
    else
    {
        return false;
    }

    if(lex == lcIntNum)
    {
        nextLex();
    }

    if(lex == lcRSquare)
    {
        nextLex();
    }
    else
    {
        return false;
    }

    if(Type())
    {
        nextLex();
    }
    else
    {
        return false;
    }

    if(lex == lcLFigure)
    {
        nextLex();
    }
    else
    {
        return false;
    }

_2:

    if(lex == lcEnter)
    {
        nextLex();
        goto _2;
    }

    if(lex == lcIntNum)
    {
        nextLex();
    }
    else
    {
        return false;
    }

    if(lex == lcComma)
    {
        nextLex();
        goto _2;
    }

    //_3:
    //    if(lex == lcEnter)
    //    {
    //        nextLex();
    //        goto _3;
    //    }

    if(lex == lcRFigure)
    {
        goto _end;
    }
    else
    {
        return false;
    }

_end:
    return true;

}



//инициализация атоматическая
bool Parser::StatementShortInit()
{
    //Позиция откуда записывать для сохранения информации о написанном коде в объекте
    int start_position_vvMap_KeyData = distance(vvMap_KeyData.begin(),vvMap_KeyData.end()-1);

    string nameId;
    if(lex == lcId)
    {
        nameId = scanAliend->GetTxtValue();
        nextLex();
    }
    else {
        return false;
    }

    if(lex == lcAssign)
    {
        if(newFuncItem != nullptr)
        {

            if(point_if_else != nullptr)
            {
                point_if_else->add_variables_to_vector(nameId);

                //Новый код Польской записи (проверяю)
                polish.push_operand(nameVarOpd,newFuncItem->get_variable_at(nameId));
                polish.push_operation(assignOpc);
            }
            else
            {
                if(!newFuncItem->addVectorNameThisLabel(nameId))
                {
                    cout << "Error: Find repeat label in Func body Statment."<<endl;
                    exit(-1);
                }

                //новое пробую
                if(!newFuncItem->add_variables_to_vector(nameId))
                {
                    cout << "Error: Find repeat label in Func body Statment."<<endl;
                    exit(-1);
                }


                //Новый код Польской записи (проверяю)
                polish.push_operand(nameVarOpd,newFuncItem->get_variable_at(nameId));
                polish.push_operation(assignOpc);

            }


        }

        //Если это не функция то значит глобальная переменная

        nextLex();

    }
    else {
        return false;
    }

    if(Expression())
    {
        //inter.add_last_oper();
        if(newFuncItem != nullptr)
        {
            newFuncItem->addNameThisType("auto");
        }
    }
    else
    {
        return false;
    }

    if(End())
    {
        //inter.out_list();

        //Новый код Польской записи (проверяю)
        polish.End();
        ///

        auto item = *vvMap_KeyData.rbegin();
        string variable_label = "";


        if(newFuncItem == nullptr)
        {
            variable_label = find_variable_name(item);
            table.expression.setExpression(item);
        }
        else
        {
            item.erase(item.begin(),item.begin()+2);
            newFuncItem->setExpression(item);
            newFuncItem->addNewVariables();

            table.expression.setExpression(item);

        }

        string type = "auto";
        auto result_item = table.expression.getResult().rbegin();
        auto result_item_type = result_item->first;
        switch (result_item_type) {
        case lcIntNum: {
            type = "int";
            break;
        }
        case lcRealNum: {
            type = "float";
            break;
        }
        }

        auto result_expression = table.expression.getResult().rbegin()->second;

        if(newFuncItem == nullptr)
        {
            //Если это не функция то значит глобальная переменная
            table.add_gloabals(lcId,variable_label,result_item_type,type,result_expression);
        }
        else
        {
            newFuncItem->add_variables_type(type);
            newFuncItem->add_variables_expression(result_expression);
        }


        goto _end;
    }
    else {
        return false;
    }

_end:
    if(newFuncItem != nullptr)
    {
        newFuncItem->get_variable_las()->add_vvMap_KeyData(vvMap_KeyData,start_position_vvMap_KeyData);
    }
    return true;
}

bool Parser::StatementInit()
{
    vector<string> save_name_hard_type;
    vector<string> save_type;

    vector<MyVariable*> save_object_hard_type;
    FuncType* find_func = nullptr;

_0:
    string name_id;
    MyVariable* Object_variable = nullptr;

    if(lex == lcId)
    {
        name_id = (*vvMap_KeyData.rbegin()).rbegin()->rbegin()->second;

        if(newFuncItem != nullptr)
        {
            if(save_object_hard_type.empty())
            {
                Object_variable = table.get_variable_in_globals(name_id);

                if(Object_variable == nullptr)
                {
                    //не нашли объявленную переменную в глобальной таблице значит ищем в локальной
                    Object_variable = newFuncItem->get_variable_at(name_id);
                    if(Object_variable == nullptr)
                    {

                        //не нашли объявленную переменную в глобальной и локальной таблице, значит ищем в функции

                        //Не является переменной
                        // Может это имя вызов функции ?
                        // Ищем является ли данное имя объявленной функцией
                        find_func = table.find_name_func(name_id);
                        if(find_func != nullptr)
                        {
                            // мы нашли функцию
                            //Значит данное имя явлется вызовом функции
                        }
                        else
                        {
                            cout << "Error_1: Find label in Func body Statment."<<endl;
                            cout << "Error_2: Variable or func call don't Init"<<endl;
                            exit(-1);
                        }
                    }
                }

            }
            else
            {
                auto prev_object = save_object_hard_type[save_object_hard_type.size()-1];
                Object_variable = prev_object->get_variable(name_id);
            }
        }
        else
        {
            if(save_object_hard_type.empty())
            {
                Object_variable = table.get_variable_in_globals(name_id);
                if(Object_variable == nullptr)
                {
                    //не нашли объявленную переменную в глобальной таблице значит ищем в локальной
                    cout << "Error: Find repeat label in Func body Statment."<<endl;
                    exit(-1);
                }
            }
            else
            {
                auto prev_object = save_object_hard_type[save_object_hard_type.size()-1];
                Object_variable = prev_object->get_variable(name_id);
            }
        }





        //        if(newFuncItem != nullptr)
        //        {



        //            if(save_name_hard_type.empty())
        //            {
        //                if(!newFuncItem->findInitStatment(name_id))
        //                {
        //                    cout << "Error: Find repeat label in Func body Statment."<<endl;
        //                    exit(-1);
        //                }

        //                save_type.push_back(newFuncItem->getInitStatment_type(name_id));
        //            }
        //            else
        //            {
        //                //                bool flagExit = false;
        //                //                for(auto vector_struct: vector_structType_nameType)
        //                //                {
        //                //                    if(vector_struct.getNameHardType() == save_type[0])
        //                //                    {
        //                //                        for(auto item_struct: vector_struct.getVariables())
        //                //                        {
        //                //                            for(auto item: item_struct)
        //                //                            {
        //                //                                if(item.first == name_id)
        //                //                                {
        //                //                                    //нашли переменную
        //                //                                    flagExit = true;
        //                //                                    break;
        //                //                                }
        //                //                            }

        //                //                            if(flagExit)
        //                //                                break;
        //                //                        }

        //                //                        if(flagExit)
        //                //                            break;
        //                //                    }
        //                //                }

        //                //////
        //                bool flagExit = false;
        //                for(auto vector_struct: table.types_)
        //                {
        //                    if(vector_struct->getNameHardType() == save_type[0])
        //                    {
        //                        if(vector_struct->find_init_variables(name_id))
        //                        {
        //                            //нашли переменную
        //                            flagExit = true;
        //                            break;
        //                        }
        //                    }
        //                }
        //                //////////////////////////
        //                if(!flagExit)
        //                {
        //                    //не нашли переменную значит ошибка
        //                    cout << "Error: Find repeat label in Func body Statment."<<endl;
        //                    exit(-1);
        //                }

        //            }
        //        }
        //        else
        //        {
        //            if(save_object_hard_type.empty())
        //            {
        //               Object_variable = table.get_variable_in_globals(name_id);
        //            }
        //            else
        //            {
        //               auto prev_object = save_object_hard_type[save_object_hard_type.size()-1];
        //               Object_variable = prev_object->get_variable(name_id);
        //            }
        //        }



        nextLex();
        goto _1;
    }
    else
    {
        return false;
    }

_1:
    if(lex == lcLSquare)
    {
        nextLex();
        goto _2;

    }

    //    if(Type())
    //    {
    //        if(newFuncItem != nullptr)
    //        {
    //            if(!newFuncItem->addVectorNameThisLabel(nameId))
    //            {
    //                cout << "Error: Find repeat label in Func body Statment."<<endl;
    //                exit(-1);
    //            }
    //        }
    //        newFuncItem->addNameThisType(scanAliend->GetTxtValue());


    //        nextLex();
    //        goto _2;
    //    }

_3:
    if(lex == lcPoint)
    {
        save_name_hard_type.push_back(name_id);

        //новое
        save_object_hard_type.push_back(Object_variable);

        nextLex();
        goto _0;

    }

    if(lex == lcEqual)
    {
        nextLex();
    }
    else
    {
        return false;
    }

    if(Expression())
    {

        auto item = *vvMap_KeyData.rbegin();
        string variable_label = "";
        variable_label = find_variable_name(item);


        //Зацикливаем пока не появиться false
        //теоритически можно зациклить внутри функции рекурсией чтоб не проходить по массиву лишний раз
        //но так наглядней,потом может подправлю
        while(table.find_name_variable(newFuncItem,item));

        table.expression.setExpression(item);


        string type = "auto";
        auto result_item = table.expression.getResult().rbegin();
        auto result_item_type = result_item->first;
        switch (result_item_type) {
        case lcIntNum: {
            type = "int";
            break;
        }
        case lcRealNum: {
            type = "float";
            break;
        }
        }

        if(type == Object_variable->get_type())
        {
            //Типы совпали
        }

        auto result_expression = result_item->second;
        Object_variable->set_expression(result_expression);
        Object_variable->set_lex_type(result_item_type);

        goto _end;
    }
    else {
        return false;
    }

_2:
    if(Expression())
    {
        goto _end;
    }

    if(lex == lcRSquare)
    {
        nextLex();
        goto _3;
    }



_end:
    return true;



}

bool Parser::funcReturn()
{
    if(lex == keyReturn)
    {
        nextLex();
    }
    else
    {
        return false;
    }


    //    if(lex == keyString)
    //    {
    //        nextLex();
    //    }

    if(lex == lcCharConst)
    {
        nextLex();
    }

    if(Expression())
    {
        //nextLex();

        //Нужно добавить код который вычисляет пример если он есть и запимсывает в результат фукнции
    }


    if(End())
    {
        goto _end;
    }
    else {
        return false;
    }

_end:
    return true;

}

bool Parser::rowStructStatement()
{

_0:
    //Нашли индификатор поля структуры
    if(lex == lcId)
    {
        string nameId = scanAliend->GetTxtValue();
        if(newHardTypeItem != nullptr)
        {
            //новое
            if(!newHardTypeItem->add_variables_to_vector(nameId))
            {
                cout << "Error: Find repeat label statment."<<endl;
                return false;
            }

        }

        if(newRecordHardTypeItem != nullptr)
        {
            //новое
            if(!newRecordHardTypeItem->add_variables_to_vector(nameId))
            {
                cout << "Error: Find repeat label statment."<<endl;
                return false;
            }
        }


        nextLex();
        goto _1;
    }
    else
    {
        //Ошибка нет индификатора
        return false;
    }

_1:
    if(lex == lcComma)
    {
        nextLex();
        goto _0;
    }

    if(Type())
    {
        string type = scanAliend->GetTxtValue();
        if(newHardTypeItem != nullptr)
        {
            HardType* newObject = table.create_object_hardType(type);
            if(newObject != nullptr)
            {
                newHardTypeItem->set_variable_struct(newObject);
            }

            newHardTypeItem->add_variables_type(type);
        }

        if(newRecordHardTypeItem != nullptr)
        {
            newRecordHardTypeItem->add_variables_type(type);
        }

        nextLex();
        goto _2;
    }
    else
    {
        //Ошибка нет типа данных
        return false;
    }

_2:
    if(End())
    {
        goto _end;
    }
    else
    {
        //Ошибка нет типа данных
        return false;
    }

_end:

    return true;
}

bool Parser::funcArgsStatement()
{

_0:
    if(lex == lcId)
    {
        string nameId = scanAliend->GetTxtValue();
        if(!newFuncItem->addVectorNameThisLabel(nameId))
        {
            cout <<"Error: Find repeat label in func Args."<<endl;
            return false;
        }

        //новое пробую
        if(!newFuncItem->add_variables_to_vector(nameId))
        {
            cout << "Error: Find repeat label in Func body Statment."<<endl;
            exit(-1);
        }
        nextLex();
        goto _1;
    }
    else
    {
        //Ошибка нет индификатора
        return false;
    }

_1:
    if(lex == lcComma)
    {
        nextLex();
        goto _0;
    }

    if(Type())
    {
        newFuncItem->addNameThisType(scanAliend->GetTxtValue());

        //новое
        newFuncItem->add_variables_type(scanAliend->GetTxtValue());
        goto _end;
    }
    else
    {
        //Ошибка нет типа данных
        return false;
    }

_end:
    newFuncItem->addNewVariables();
    return true;
}

bool Parser::funcArgsCall()
{
_0:
    if(Expression())
    {

        if(lex == lcRCircle)
        {
            goto _end;
        }

    }
    else
    {
        return false;
    }

    if(lex == lcComma)
    {
        nextLex();
        goto _0;
    }

_end:
    return true;
}

bool Parser::structNewType()
{

    //Позиция откуда записывать для сохранения информации о написанном коде в объекте
    int start_position_vvMap_KeyData = distance(vvMap_KeyData.begin(),vvMap_KeyData.end()-1);

    if(lex == keyType)
    {
        nextLex();
    }
    else
    {
        return false;
    }

    if(lex == lcId)
    {
        nameStructType = scanAliend->GetTxtValue(); //взяли имя структуры
        newHardTypeItem->addNameHardType(nameStructType);
        nextLex();
    }
    else
    {
        return false;
    }

    if(lex == keyStruct)
    {
        nextLex();
        goto _1;
    }
    else
    {
        return false;
    }

_1:
    if(lex == lcEnter)
    {
        nextLex();
        goto _1;
    }

    if(lex == lcLFigure)
    {
        nextLex();
        goto _3; // проверить на фигурную скобку
    }

_2:
    if(rowStructStatement())
    {
        nextLex();
        goto _2;
    }

    if(lex == lcEnter)
    {
        nextLex();
        goto _3;
    }


    if(lex == lcRFigure)
    {
        goto _end;
    }
    else
    {
        return false;
    }


_3:
    if(lex == lcRFigure)
    {
        //nextLex();
        goto _end;
    }
    else
    {

        goto _2;
    }

_end:

    //Проверяем есть ли данная структура
    if(!addVector_NameType())
    {
        //Ошибка структура с данным именем уже объявленна
        cout << "Error: Add new struct type."<<endl;
        exit(-1);
    }

    //добавили в массив новых типов данных
    vector_nameType.push_back(nameStructType);

    //Сохранение информации о написанном коде в объекте
    newHardTypeItem->add_vvMap_KeyData(vvMap_KeyData,start_position_vvMap_KeyData);


    return true;
}

bool Parser::VariableInit()
{
_0:
    if(lex == lcId)
    {
        nextLex();
        goto _1;
    }
    else
    {
        return false;
    }

_1:
    if(lex == lcLSquare)
    {
        nextLex();
        goto _2;

    }

_3:
    if(lex == lcPoint)
    {
        nextLex();
        goto _0;

    }

_2:
    if(Expression())
    {

    }

    if(lex == lcRSquare)
    {
        nextLex();
        goto _3;
    }


    if(lex == lcRCircle || lex == lcLCircle || lex == lcPlus || lex == lcMinus || lex == lcSlash || lex == lcStar || lex == lcMod)
    {
        goto _end;
    }

    return false;

_end:
    return true;
}

bool FlagNew = false;
//Нашли переменную в выражении
bool Parser::Variable()
{
    FuncType* find_func = nullptr;
    MyVariable* Object_variable = nullptr;

    vector<string> save_name_hard_type;
    vector<string> save_type;
    vector<MyVariable*> save_object_hard_type;

_0:
    if(lex == lcId)
    {
        if(newFuncItem != nullptr)
        {
            if(FlagNew)
            {
                newFuncItem->addNameThisType(scanAliend->GetTxtValue());
                FlagNew = false;
                newFuncItem->addNewVariables();
            }
            else
            {
                string name = scanAliend->GetTxtValue(); //nameCallFunc

                if(name == "new")
                {
                    FlagNew = true;
                }
                else
                {

                    if(save_object_hard_type.empty())
                    {
                        if(Object_variable == nullptr)
                        {
                            //Если это перменная то нужно найти ее в массиве переменных к функции
                            Object_variable = newFuncItem->get_variable_at(name);

                            if(Object_variable != nullptr)
                            {
                                //мы нашли переменную, значит она объявленна и существует (все хорошо)

                            }
                            else
                            {
                                //Не является переменной
                                // Может это имя вызов функции ?
                                // Ищем является ли данное имя объявленной функцией
                                find_func = table.find_name_func(name);
                                if(find_func != nullptr)
                                {
                                    // мы нашли функцию
                                    //Значит данное имя явлется вызовом функции
                                }
                                else
                                {

                                    //Если это не функция то наверно глобальная перменная
                                    Object_variable = table.get_variable_in_globals(name);

                                    if(Object_variable != nullptr)
                                    {
                                        //Переменная является глобальной

                                        //пробую промежуточное представление
                                        //inter.add_operand(inter.create_myoperand(nameVarOpd,Object_variable));
                                    }
                                    else
                                    {
                                        cout << "Error: Variable or func call don't Init"<<endl;
                                        cout << "Error: Global variable don't Init"<<endl;
                                        exit(-1);
                                    }
                                }
                            }

                        }
                    }
                    else
                    {
                        auto prev_object = save_object_hard_type[save_object_hard_type.size()-1];
                        auto parent = Object_variable;
                        Object_variable = prev_object->get_variable(name);

                        if(Object_variable != nullptr)
                        {
                            Object_variable->set_parent_var(parent);
                        }
                        else
                        {
                            //Ошибка не инициализированная переменная
                            cout << "Error: Variable or func call don't Init"<<endl;
                            cout << "Error: Global variable don't Init"<<endl;
                            exit(-1);
                        }
                    }


                    ////


                    //                for(auto item: vector_FuncType_nameFuncType)
                    //                {
                    //                    if(item.getNameFuncType() == nameCallFunc)
                    //                    {
                    //                        //функция найдена
                    //                        newFuncItem->addNameThisType(item.getReturnFuncType());
                    //                        break;
                    //                    }
                    //                }

                    //cout << "Error: Func don't Init"<<endl;
                    //return false;
                }

            }
        }




        nextLex();
    }
    else
    {
        return false;
    }

    if(lex == lcLSquare)
    {
        nextLex();
        goto _1;
    }

    if(lex == lcLCircle)
    {
        nextLex();

        if(lex == lcRCircle)
        {
            nextLex();
            goto _end;
        }
        goto _3;
    }
    else
    {
        if(find_func != nullptr)
        {
            //Был вызов функции
            //а открывающейся скобочки следом не было
            // Ошибка (не правильный вызов функции)
            cout << "Error: Function call don't  open '('"<<endl;
            return false;
        }
    }

_2:
    if(lex == lcPoint)
    {
        nextLex();

        //новое
        save_object_hard_type.push_back(Object_variable);

        goto _0;
    }

    //Дописать чтоб было имя структуры (на данный момент есть лишь указатель на конкретную переменную в нутри стурктуры)
    if(Object_variable != nullptr)
    {
        //пробую промежуточное представление
        //inter.add_operand(inter.create_myoperand(nameVarOpd,Object_variable));

        //Новый код Польской записи (проверяю)
        polish.push_operand(nameVarOpd,Object_variable);
    }


    goto _end;

_1:
    if(Expression())
    {

    }

    if(lex == lcRSquare)
    {
        nextLex();
        goto _2;
    }
    else
    {
        return false;
    }


_3:
    if(funcArgsCall())
    {
        if(lex == lcRCircle)
        {
            nextLex();
            goto _end;
        }
    }
    else
    {
        return false;
    }


    if(lex == lcComma)
    {
        nextLex();
        goto _3;
    }
    else
    {
        return false;
    }

_end:
    return true;
    //    _0:
    //        if(lex == lcId)
    //        {
    //            nextLex();
    //        }
    //        else
    //        {
    //            return false;
    //        }

    //        if(lex == lcPoint)
    //        {
    //            nextLex();
    //            goto _0;
    //        }

    //        if(lex == lcLSquare)
    //        {
    //            nextLex();
    //            goto _1;
    //        }

    //        if(lex == lcLCircle)
    //        {
    //            nextLex();
    //            goto _2;
    //        }

    //        if(End())
    //        {
    //            goto _end;
    //        }

    //        return false;

    //    _1:
    //        if(Expression())
    //        {

    //        }

    //        if(lex == lcRSquare)
    //        {
    //            auto  position = scanAliend->getPosition();
    //            nextLex();
    //            if(lex == lcPoint)
    //            {
    //                nextLex();
    //                goto _0;
    //            }

    //            scanAliend->setPosition(position);
    //            goto _end;
    //        }

    //        return false;

    //    _2:
    //        if(funcArgsCall())
    //        {

    //        }

    //        if(lex == lcComma)
    //        {
    //            goto _1;
    //        }

    //        if(lex == lcRCircle)
    //        {
    //            auto  position = scanAliend->getPosition();
    //            nextLex();
    //            if(lex == lcPoint)
    //            {
    //                nextLex();
    //                goto _0;
    //            }

    //            scanAliend->setPosition(position);
    //            goto _end;
    //        }

    //        return false;

    //    _end:
    //        return true;
}

bool Parser::VariableFuncCall()
{
_0:
    if(lex == lcId)
    {
        nextLex();
    }
    else
    {
        return false;
    }

    if(lex == lcLCircle)
    {
        nextLex();
        goto _2;
    }

    if(End())
    {
        goto _end;
    }

    return false;



_2:
    if(funcArgsCall())
    {

    }

    if(lex == lcComma)
    {
        goto _2;
    }

    if(lex == lcRCircle)
    {
        auto  position = scanAliend->getPosition();
        nextLex();
        if(lex == lcPoint)
        {
            nextLex();
            goto _0;
        }

        scanAliend->setPosition(position);
        goto _end;
    }

    return false;

_end:
    return true;
}

bool Parser::Number()
{
    if(lex == lcIntNum)
    {


        //пробую промежуточное представление
        CONST* item = new CONST;
        item->typ =  INTTYP;
        item->val.unum = scanAliend->GetIntValue();


        //inter.add_operand(inter.create_myoperand(constOpd,item));

        //Новый код Польской записи (проверяю)
        polish.push_operand(constOpd,item);


        nextLex();
        goto _end;
    }

    if(lex == lcRealNum)
    {
        nextLex();
        goto _end;
    }

    return false;

_end:
    return true;
}


bool Parser::Term()
{
    InformPosition position = scanAliend->getPosition();


//    if(lex == lcMinus)
//    {
//        find_minusUnOpc = true;
//        nextLex();
//    }

//_1:
    if(Number())
    {
        goto _end;
    }

    if(lex == lcString)
    {
        nextLex();
        goto _end;
    }

    if(Variable())
    {
        goto _end;
    }


    //    position = scanAliend->getPosition();
    //    nextLex();
    //    if(lex == lcComma)
    //    {
    //        goto _end;
    //    }
    //    scanAliend->setPosition(position);
    //    nextLex();

   // scanAliend->setPosition(position);


_2:
    if(lex == lcLCircle)
    {
        //inter.add_opc(LCircleOpc);

        //Новый код Польской записи (проверяю)
        polish.push_operation(LCircleOpc);

        nextLex();
    }
    else
    {
        return false;
    }

    if(Expression())
    {
        goto _3;
    }

_3:
    if(lex == lcRCircle)
    {
        //inter.add_opc(RCircleOpc);

        //Новый код Польской записи (проверяю)
        polish.push_operation(RCircleOpc);

        nextLex();
        goto _end;
    }
    else
    {
        return false;
    }

_end:
    return true;
}

bool Parser::Expression()
{

_0:
    //    if(lex == lcAssign)
    //    {
    //        nextLex();
    //        goto _1;
    //    }

    if(lex == lcEqual)
    {
        nextLex();
        goto _1;
    }

    if(lex == lcLarger)
    {
        //Новый код Польской записи (проверяю)
        polish.push_operation(largerOpc);

        nextLex();
        goto _1;
    }

    if(lex == lcSmaller)
    {
        //Новый код Польской записи (проверяю)
        polish.push_operation(smallerOpc);

        nextLex();
        goto _1;
    }
    if(lex == lcSmallerEQ)
    {
        //Новый код Польской записи (проверяю)
        polish.push_operation(smallerEQOpc);

        nextLex();
        goto _1;
    }
    if(lex == lcLargerEQ)
    {
        //Новый код Польской записи (проверяю)
        polish.push_operation(largerEQOpc);

        nextLex();
        goto _1;
    }

    if(lex == lcEQ)
    {
        //Новый код Польской записи (проверяю)
        polish.push_operation(eqOpc);

        nextLex();
        goto _1;
    }


    if(Addition())
    {
        goto _1;
    }
    else
    {
        return false;
    }

_1:
    if(lex == lcComma)
    {
        goto _end;
    }
    if(lex == lcRCircle)
    {
        goto _end;
    }

    if(lex == lcEof)
    {
        goto _end;
    }

    //END
    if(lex == lcEnter)
    {
        goto _end;
    }

    if(lex  == lcSemicolon)
    {
        goto _end;
    }

    if(lex  == lcLFigure ) //&& vvMap_KeyData.rbegin()->begin()->begin()->first == keyIf
    {
        goto _end;
    }

    goto _0;

_end:
    return true;
}

bool Parser::Product()
{
    if(Term())
    {
        goto _end;
    }

    if(lex == lcStar)
    {
        //пробую промежуточное представление
        //inter.add_opc(starOpc);

        //Новый код Польской записи (проверяю)
        polish.push_operation(starOpc);

        nextLex();
        goto _end;
    }

    if(lex == lcSlash)
    {
        //пробую промежуточное представление
        //inter.add_opc(slashOpc);

        //Новый код Польской записи (проверяю)
        polish.push_operation(slashOpc);

        nextLex();
        goto _end;
    }

    if(lex == lcMod)
    {
        //пробую промежуточное представление
        //inter.add_opc(modOpc);

        //Новый код Польской записи (проверяю)
        polish.push_operation(modOpc);

        nextLex();
        goto _end;
    }

    if(End())
    {
        goto _end;
    }

    return false;

_end:
    return true;
}

bool Parser::Addition()
{
    InformPosition position;
    LexClass saveLex;


    if(Product())
    {
        goto _end;
    }

    position = scanAliend->getPosition();
    saveLex = lex;
    if(lex == lcPlus)
    {
        //Написать код на проверку инкримента
        nextLex();
        //Если следом идет + то это инкримент
        if(lex == lcPlus)
        {
            //Новый код Польской записи (проверяю)
            polish.push_operation(incrimentOpc);
        }
        else
        {
            scanAliend->setPosition(position);
            lex = saveLex;


            if(vvMap_KeyData.rbegin()->size() > 1)
            {
                auto item = (vvMap_KeyData.rbegin()->rbegin()+1)->rbegin()->first;
                if(item == lcId || item == lcIntNum || item == lcRealNum || item == lcRCircle)
                {
                    //пробую промежуточное представление
                    //inter.add_opc(plusOpc);

                    //Новый код Польской записи (проверяю)
                    polish.push_operation(plusOpc);

                }
                else
                {
                    //inter.add_opc(plusUnOpc);

                    //Новый код Польской записи (проверяю)
                    polish.push_operation(plusUnOpc);
                }
            }
            else
            {
                //пробую промежуточное представление
                //inter.add_opc(plusOpc);

                //Новый код Польской записи (проверяю)
                polish.push_operation(plusOpc);
            }
        }


        nextLex();
        goto _end;
    }

    position = scanAliend->getPosition();
    saveLex = lex;
    if(lex == lcMinus)
    {

        //Написать код на проверку дикримент
        nextLex();
        //Если следом идет + то это дикримент
        if(lex == lcMinus)
        {
            //Новый код Польской записи (проверяю)
            polish.push_operation(dicrimentOpc);
        }
        else
        {
            scanAliend->setPosition(position);
            lex = saveLex;

            if(vvMap_KeyData.rbegin()->size() > 1)
            {
                auto item = (vvMap_KeyData.rbegin()->rbegin()+1)->rbegin()->first;
                if(item == lcId || item == lcIntNum || item == lcRealNum || item == lcRCircle)
                {
                    //пробую промежуточное представление
                    //inter.add_opc(minusOpc);

                    //Новый код Польской записи (проверяю)
                    polish.push_operation(minusOpc);

                }
                else
                {
                   //inter.add_opc(minusUnOpc);

                   //Новый код Польской записи (проверяю)
                   polish.push_operation(minusUnOpc);
                }
            }
            else
            {
                //пробую промежуточное представление
                //inter.add_opc(minusOpc);

                //Новый код Польской записи (проверяю)
                polish.push_operation(minusOpc);
            }

        }


        nextLex();
        goto _end;
    }



    if(End())
    {
        goto _end;
    }

    return false;
_end:
    return true;
}

bool Parser::IfElse()
{
    InformPosition position;
    LexClass saveLex;
    class IfElse* local_if_else = point_if_else; //сохраним указатель (чтоб не переназначать глобальный указатель)

    //Позиция откуда записывать для сохранения информации о написанном коде в объекте
    int start_position_vvMap_KeyData = distance(vvMap_KeyData.begin(),vvMap_KeyData.end()-1);

    if(lex == keyIf)
    {
        nextLex();
        goto _1;
    }
    else
    {
        return false;
    }

_1:
    if(Expression())
    {
        //Новый код Польской записи (проверяю)
        polish.End();
        //Как только закончится условное вырожение добавить иснтрукцию Отрицательного условия ifZ
        polish.push_operation(ifZOpc,label_if_false);


        //Написать код который обрабатывает условие между if и {
        //записывает результат в объект
        //local_if_else->set_result_if(expression_between_ifelse(*vvMap_KeyData.rbegin()));
        goto _2;
    }

_2:

    if(lex == lcEnter)
    {
        nextLex();
        goto _2;
    }

    if(lcLFigure_lcRFigure())
    {
        //Новый код Польской записи (проверяю)
        polish.End();

    }
    else
    {
        return false;
    }

_5:
    if(lex == lcEnter)
    {
        nextLex();
        goto _5;
    }

    if(lex == keyElse)
    {
        nextLex();

        //подумать может быть надо убрать чтоб вложеннасть была
        if(lex == keyIf)
        {
            //            nextLex();
            //            goto _1;

            //Подумать над тем что когда я захожу в 2 условие по if во время выхода из последнего нужно указать возвращать на 1
            local_if_else->add_new_if_else();
            point_if_else = local_if_else->get_if_else(local_if_else->get_if_else_size());
            if(IfElse())
            {

            }
            else
            {
                return false;
            }
        }

        //Код если есть ELSE
_6:
        if(lex == lcEnter)
        {
            nextLex();
            goto _6;
        }

        if(lcLFigure_lcRFigure())
        {

        }
        else
        {
            return false;
        }

        position = scanAliend->getPosition();
        scanAliend->setPosition(position);
        //nextLex();

        goto _end;
    }
    else
    {
        position = scanAliend->getPosition();
        scanAliend->setPosition(position);
        nextLex();

        goto _end;
    }

_end:

    if(local_if_else != nullptr)
    {
        local_if_else->add_vvMap_KeyData(vvMap_KeyData,start_position_vvMap_KeyData);
    }
    return true;

}

bool Parser::loop()
{
    InformPosition position;
    LexClass saveLex = lex;

    position = scanAliend->getPosition();
    if(lex == keyFor)
    {
        polish.save_label_begin_for();
        nextLex();
    }
    else
    {
        return false;
    }


    /////////////
    if(StatementShortInit())
    {
        polish.save_label_begin_for();
    }
//    else
//    {
//        scanAliend->setPosition(position);
//        lex = saveLex;
//    }


    if(lex == lcSemicolon)
    {
        nextLex();
    }
    else
    {
        scanAliend->setPosition(position);
        nextLex();
        goto _5;
    }

_5:
    if(Expression())
    {
        //Новый код Польской записи (проверяю)
        polish.End();
        //Как только закончится условное вырожение добавить иснтрукцию Отрицательного условия ifZ
        polish.push_operation(ifZOpc,label_for);
    }


    if(lex == lcSemicolon)
    {
        nextLex();

        if(Expression())
        {
            //Новый код Польской записи (проверяю)
            polish.End();
        }
    }

_12:
    if(lex == lcEnter)
    {
        // position = scanAliend->getPosition();
        nextLex();
        position = scanAliend->getPosition();
        saveLex = lex;

        goto _12;
    }

    if(lex == lcLFigure)
    {

        nextLex();
        goto _4;
    }
    else
    {
        scanAliend->setPosition(position);
        nextLex();
        goto _6;
    }



    //////////

_6:
    //инициализация атоматическая
    if(StatementShortInit())
    {
        goto _1;
    }

_1:
    if(lex == lcSemicolon)
    {
        nextLex();
    }
    else
    {
        return false;
    }

    if(Expression())
    {
        goto _3;
    }


//_2:
//    if(lex == lcSemicolon)
//    {
//        nextLex();
//    }
//    else
//    {
//        return false;
//    }

//    if(Expression())
//    {

//        goto _3;
//    }


_3:
    if(lex == lcEnter)
    {
        nextLex();
        goto _3;
    }


    if(lex == lcLFigure)
    {
        nextLex();
        goto _4;
    }
    else
    {
        return false;
    }

_4:
    if(lex == lcEnter)
    {
        // position = scanAliend->getPosition();
        nextLex();
        position = scanAliend->getPosition();
        saveLex = lex;

        goto _4;
    }

//    if(lex == keyBreak)
//    {
//        polish.set_goto_label(it_is_for);
//        nextLex();
//        goto _4;
//    }

    //инициализация атоматическая
    if(StatementShortInit())
    {
        goto _4;
    }
    scanAliend->setPosition(position);
    lex = saveLex;


    if(Statement())
    {
        goto _4;
    }
    scanAliend->setPosition(position);
    lex = saveLex;

    if(StatementInit())
    {
        goto _4;
    }
    scanAliend->setPosition(position);
    lex = saveLex;


    if(Variable())
    {
        goto _4;
    }
    scanAliend->setPosition(position);
    lex = saveLex;

    //исправить
    if(IfElse())
    {
        goto _4;
    }
    else
    {
//        if(lex == keyBreak)
//        {
//            polish.set_goto_label(it_is_for);
//            nextLex();
//            goto _4;
//        }
    }
    scanAliend->setPosition(position);
    lex = saveLex;

    if(loop())
    {
        goto _4;
    }
    else
    {
//        if(lex == keyBreak)
//        {
//            polish.set_goto_label(it_is_for);
//            nextLex();
//            goto _4;
//        }
    }
    scanAliend->setPosition(position);
    lex = saveLex;

    if(switchCase())
    {
        goto _4;
    }
    else
    {
//        if(lex == keyBreak)
//        {
//            polish.set_goto_label(it_is_for);
//            nextLex();
//            goto _4;
//        }
    }
    scanAliend->setPosition(position);
    lex = saveLex;



    if(lex == lcRFigure)
    {
        //Новый код Польской записи (проверяю)
        //Устанавливаем метку
        polish.set_goto_label(it_is_for);

        nextLex();
        goto _end;
    }
    else
    {
        return false;
    }

_end:
    return true;

}

bool Parser::switchCase()
{
    InformPosition position;
    LexClass saveLex;

    if(lex == keySwitch)
    {
        nextLex();
    }
    else
    {
        return false;
    }

_1:
    if(lex == lcEnter)
    {
        nextLex();
        goto _1;
    }

    if(lex == lcLFigure)
    {
        nextLex();
        goto _2;
    }
    else
    {
        return false;
    }

_2:
    if(lex == lcEnter)
    {
        nextLex();
        goto _2;
    }

    if(lex == keyCase)
    {
        nextLex();
        goto _3;
    }

    if(lex == lcRFigure)
    {
        nextLex();
        goto _end;
    }
    else
    {
        return false;
    }

_3:
    if(Expression())
    {

    }

    if(lex == lcComma)
    {
        nextLex();
        goto _3;
    }

    if(lex == lcColon)
    {
        nextLex();
        goto _4;
    }
    else
    {
        return false;
    }


_4:
    if(lex == lcEnter)
    {
        position = scanAliend->getPosition();
        nextLex();

        goto _4;
    }

    if(lex == lcLFigure)
    {
        nextLex();
        goto _5;
    }
    else {
        return false;
    }

_5:
    if(lex == lcEnter)
    {
        // position = scanAliend->getPosition();
        nextLex();
        position = scanAliend->getPosition();
        saveLex = lex;

        goto _5;
    }

    //инициализация атоматическая
    if(StatementShortInit())
    {
        goto _5;
    }
    scanAliend->setPosition(position);
    lex = saveLex;


    if(Statement())
    {
        goto _5;
    }
    scanAliend->setPosition(position);
    lex = saveLex;

    if(StatementInit())
    {
        goto _5;
    }
    scanAliend->setPosition(position);
    lex = saveLex;


    if(Variable())
    {
        goto _5;
    }
    scanAliend->setPosition(position);
    lex = saveLex;

    //исправить
    if(IfElse())
    {
        goto _5;
    }
    scanAliend->setPosition(position);
    lex = saveLex;

    if(loop())
    {
        goto _5;
    }
    scanAliend->setPosition(position);
    lex = saveLex;

    if(funcReturn())
    {
        nextLex();
    }




    if(lex == lcRFigure)
    {
        nextLex();
        goto _6;
    }
    else
    {
        return false;
    }


_6:
    if(lex == lcRFigure)
    {
        nextLex();
        goto _end;
    }
    else
    {
        goto _2;
    }

_end:
    return true;


}


bool Parser::Do()
{

    lex = lcNoValue;
    nextLex(); //перейти на первую лексему

    // Процесс пошел!
    if(Spaces())
    {
        //Запись в файл трехадресеных команд
        polish.out_stek_file("polish.txt");

        cout <<"\\\\------------------------------\n";
        cout << "Good\n";
        return true; // Все прошло нормально
    }
    else {
        cout << "Don't good\n";
        return false; // Есть ошибки
    }

}

bool Parser::Spaces()
{
    InformPosition position;
    LexClass saveLex;
    vector<vector<map<LexClass,string>>> saveItem;

_0:
    newRecordHardTypeItem = nullptr;
    newHardTypeItem = nullptr;
    newFuncItem = nullptr;
    point_if_else = nullptr;

    position = scanAliend->getPosition();
    saveLex = lex;

    saveItem = vvMap_KeyData;


    if(lex == lcEof)
    {
        nextLex();
        goto _end;
    }
    else
    {
        if(lex == lcEnter)
        {
            nextLex();
            goto _0;
        }
        goto _1;
    }

_1:
    if(NewType())
    {
        nextLex();
        goto _0;
    }
    scanAliend->setPosition(position);
    lex = saveLex;
    vvMap_KeyData = saveItem;

    if(StatementArray())
    {
        goto _0;
    }
    scanAliend->setPosition(position);
    lex = saveLex;
    vvMap_KeyData = saveItem;

    //инициализация атоматическая
    if(StatementShortInit())
    {
        goto _0;
    }
    scanAliend->setPosition(position);
    lex = saveLex;
    vvMap_KeyData = saveItem;


    //инициализация
    if(Statement())
    {
        goto _0;
    }
    scanAliend->setPosition(position);
    lex = saveLex;
    vvMap_KeyData = saveItem;

    //инициализация
    if(StatementInit())
    {
        goto _0;
    }
    scanAliend->setPosition(position);
    lex = saveLex;
    vvMap_KeyData = saveItem;

    //создание нового обобщения
    newRecordHardTypeItem = new RecordHardType;
    if(recordNewType())
    {
        nextLex();
        table.add_record_hard_type_types(newRecordHardTypeItem);
        goto _0;
    }
    delete newRecordHardTypeItem;
    newRecordHardTypeItem = nullptr;


    scanAliend->setPosition(position);
    lex = saveLex;
    vvMap_KeyData = saveItem;

    //создание новго типа данных
    newHardTypeItem = new HardType;
    if(structNewType())
    {
        nextLex();

        table.add_hard_types(newHardTypeItem);
        goto _0;
    }
    delete newHardTypeItem;
    newHardTypeItem = nullptr;

    scanAliend->setPosition(position);
    lex = saveLex;
    vvMap_KeyData = saveItem;

    //Создание новой функции
    newFuncItem = new FuncType;
    if(newFunc())
    {
        nextLex();
        table.add_func_types(newFuncItem);
        goto _0;
    }
    delete newFuncItem;
    newFuncItem = nullptr;

    //    if(import())
    //    {
    //        nextLex();
    //        goto _0;
    //    }


    if(lex == lcEnter)
    {
        nextLex();
        goto _0;
    }

    if(nextLex() == lcEof)
    {
        goto _end;
    }

    return false;

_end:
    return true;
}

bool Parser::End()
{

    if(lex == lcEnter)
    {
        goto _end;
    }

    if(lex  == lcSemicolon)
    {
        goto _end;
    }

    //    if(nextLex() == lcEof)
    //    {
    //        goto _end;
    //    }

    //    scanAliend->setPosition(position);
    return false;

_end:
    return true;
}

bool Parser::import()
{
    if(lex == keyImport)
    {
        nextLex();
    }
    else
    {
        return false;
    }

    if(lex == lcLCircle)
    {
        nextLex();
        goto _1;
    }

    if(lex == lcString)
    {
        nextLex();
    }
    else
    {
        return false;
    }

    if(End())
    {
        goto _end;
    }
    else
    {
        return false;
    }

_1:
    if(lex == lcEnter)
    {
        nextLex();
        goto _1;
    }

    if(lex == lcString)
    {
        nextLex();
    }
    else
    {
        return false;
    }

    if(End())
    {
        goto _2;
    }
    else
    {
        return false;
    }

_2:
    if(lex == lcRCircle)
    {
        goto _end;
    }
    else
    {
        goto _1;
    }

_end:
    return true;
}

bool Parser::recordNewType()
{

    if(lex == keyType)
    {
        nextLex();
    }
    else
    {
        return false;
    }

    if(lex == lcId)
    {
        nameRecordType = scanAliend->GetTxtValue();

        newRecordHardTypeItem->addNameHardType(nameRecordType);
        nextLex();
    }
    else
    {
        return false;
    }

    if(lex == keyStruct)
    {
        nextLex();
        goto _1;
    }
    else
    {
        return false;
    }

_1:
    if(lex == lcEnter)
    {
        nextLex();
        goto _1;
    }

    if(lex == lcLFigure)
    {
        nextLex();
        goto _3; // проверить на фигурную скобку
    }

_2:
    if(rowStructStatement())
    {
        nextLex();
        goto _2;
    }

    if(lex == lcEnter)
    {
        nextLex();
        goto _3;
    }


    if(lex == lcRFigure)
    {
        nextLex();
        goto _4;
    }
    else
    {
        return false;
    }


_3:
    if(lex == lcRFigure)
    {
        nextLex();
        goto _4;
    }
    else
    {

        goto _2;
    }

_4:
    if(lex == keyUnion)
    {
        nextLex();
    }
    else
    {
        return false;
    }

    if(lex == keyLocal)
    {
        newRecordHardTypeItem->setLocal(scanAliend->GetTxtValue());
        nextLex();
    }

    if(lex == keyType)
    {
        nextLex();
        goto _5;
    }

_6:
    if(lex == lcEnter)
    {
        nextLex();
        goto _6;
    }

    if(lex == lcLFigure)
    {
        nextLex();
    }
    else
    {
        return false;
    }

    //_7:
    //    if(lex == lcEnter)
    //    {
    //        nextLex();
    //        goto _7;
    //    }

    //    if(rowRecordStatementKey())
    //    {
    //        goto _7;
    //    }

    //    if(lex == lcRFigure)
    //    {
    //        nextLex();
    //        goto _end;
    //    }
    //    else
    //    {
    //        return false;
    //    }



_5:
    if(lex == lcEnter)
    {
        nextLex();
        goto _5;
    }

    if(lex == lcLFigure)
    {
        nextLex();
    }
    else
    {
        return false;
    }




_7:
    if(lex == lcEnter)
    {
        nextLex();
        goto _7;
    }

    if(rowRecordStatementKey())
    {
        goto _7;
    }

    if(lex == lcRFigure)
    {
        nextLex();
        goto _end;
    }
    else
    {
        return false;
    }



    //_8:
    //    if(lex == lcEnter)
    //    {
    //        nextLex();
    //        goto _8;
    //    }

    //    if(rowRecordStatementType())
    //    {
    //        goto _8;
    //    }

    //    if(lex == lcRFigure)
    //    {
    //        nextLex();
    //        goto _end;
    //    }
    //    else
    //    {
    //        return false;
    //    }
_end:

    //Проверяем есть ли данное обобщение
    if(!addVector_RecordNameType())
    {
        //Ошибка обобщение с данным именем уже объявленна
        cout << "Error: Add new struct type."<<endl;
        exit(-1);
    }

    //добавили в массив новых типов данных
    vector_nameType.push_back(nameRecordType);

    return true;
}

bool Parser::rowRecordStatementType()
{
    if(lex == lcId)
    {
        nextLex();
        goto _end;
    }

    return false;
_end:
    return true;
}

bool Parser::rowRecordStatementKey()
{

_0:
    if(lex == lcId)
    {
        string nameId = scanAliend->GetTxtValue();

        if(newRecordHardTypeItem != nullptr)
        {
            //новое
            if(!newRecordHardTypeItem->add_variables_union_to_vector(nameId))
            {
                cout << "Error: Find repeat label statment."<<endl;
                return false;
            }
        }

        nextLex();
        goto _1;
    }
    else
    {
        //Ошибка нет индификатора
        return false;
    }

_1:
    if(lex == lcComma)
    {
        nextLex();
        goto _0;
    }

    if(Type())
    {

        if(newRecordHardTypeItem != nullptr)
        {
            newRecordHardTypeItem->add_variables_union_type(scanAliend->GetTxtValue());
        }

        nextLex();
        goto _2;
    }
    else
    {
        //Ошибка нет типа данных
        return false;
    }

_2:
    if(End())
    {
        goto _end;
    }
    else
    {
        //Ошибка нет типа данных
        return false;
    }

_end:
    return true;
}

bool Parser::addRecordNewRow()
{
    InformPosition position;
    LexClass lexSave;

    if(lex == lcId)
    {
        nextLex();
    }
    else
    {
        return false;
    }

    if(lex == lcPlus)
    {
        nextLex();
    }
    else
    {
        return false;
    }

    if(lex == lcEqual)
    {
        nextLex();
    }
    else
    {
        return false;
    }

    position = scanAliend->getPosition();
    lexSave = lex;
    if(rowRecordStatementType())
    {
        if(lex == lcEnter)
        {
            goto _1;
        }
    }
    scanAliend->setPosition(position);
    lex = lexSave;

_2:
    if(rowRecordStatementKey())
    {
        goto _1;
    }



    return false;

_1:

    if(End())
    {
        goto _end;
    }
    else
    {
        return false;
    }

_end:
    return true;
}

bool Parser::lcLFigure_lcRFigure()
{
    InformPosition position;
    LexClass saveLex;

    //Позиция откуда записывать для сохранения информации о написанном коде в объекте
    int start_position_vvMap_KeyData = distance(vvMap_KeyData.begin(),vvMap_KeyData.end()-1);

    //вспомогательные переменные
    class IfElse* local_if_else = point_if_else; //сохраним указатель (чтоб не переназначать глобальный указатель)


    if(lex == lcLFigure)
    {
        nextLex();
        goto _3;
    }
    else
    {
        return false;
    }

_3:
    if(lex == lcEnter)
    {
        nextLex();

        position = scanAliend->getPosition();
        saveLex = lex;

        goto _3;
    }


    //инициализация автоматическая
    if(StatementShortInit())
    {
        goto _3;
    }
    scanAliend->setPosition(position);
    lex = saveLex;


    if(Statement())
    {
        goto _3;
    }
    scanAliend->setPosition(position);
    lex = saveLex;

    if(StatementInit())
    {
        goto _3;
    }
    scanAliend->setPosition(position);
    lex = saveLex;


    if(Variable())
    {
        goto _3;
    }
    scanAliend->setPosition(position);
    lex = saveLex;


    if(local_if_else != nullptr)
    {
        //Подумать над тем что когда я захожу в 2 условие по if во время выхода из последнего нужно указать возвращать на 1
        local_if_else->add_new_if_else();
        point_if_else = local_if_else->get_if_else(local_if_else->get_if_else_size());


        if(IfElse())
        {
            goto _4;
        }
        local_if_else->remove_new_if_else();
        scanAliend->setPosition(position);
        lex = saveLex;

     }

    if(loop())
    {
        goto _4;
    }
    scanAliend->setPosition(position);
    lex = saveLex;

    if(switchCase())
    {
        goto _4;
    }
    scanAliend->setPosition(position);
    lex = saveLex;




_4:

    if(lex == lcEnter)
    {
        goto _3;
    }

    if(lex == lcRFigure)
    {

        //Новый код Польской записи (проверяю)
        //Устанавливаем метку
        polish.set_goto_label(it_is_if);


        //Удалить временные переменные
        if(newFuncItem != nullptr)
        {
            if(local_if_else != nullptr)
            {
                local_if_else->variable_clear();
            }
        }


        position = scanAliend->getPosition();
        nextLex();
        goto _end;
    }
    else
    {
        return false;
    }

_end:
    return  true;
}

vector<vector<map<LexClass, string> > > Parser::get_vvMap_KeyData()
{
    return vvMap_KeyData;
}



LexClass Parser::nextLex(bool flagSetSavePosition)
{

    prevLex = lex;

    if(flagSetSavePosition)
    {
        scanAliend->setSavePosition();
    }

    scanAliend->Next();
    lex = scanAliend->GetLex();


    //Собираем вектор ключ-значение всех лексем
    add_vvMap_KeyData();

    //    //Собираем таблицу связанности на каждой новой лексеме
    //    int row = scanAliend->GetStr();
    //    if(vvMap_KeyData.at(row-1)[0].begin()->first == keyFunc)
    //    {
    //            auto nameFunc = vvMap_KeyData.at(row-1)[1];
    //            //Мы нашли объявление функции
    //           if(nameFunc.begin()->first == lcId)
    //           {
    //               //Запомнить имя новой функции
    //               //newFuncItem->addNameFuncType(nameFunc.begin()->second);
    //           }
    //    }


    return lex;
}

void Parser::add_vvMap_KeyData()
{
    //пробую добавить в вектор лексем
    cout << "Example : row = " << scanAliend->Get_position_row_str() <<endl;

    vector<map<LexClass,string>> col_vvMap_keyData;
    map<LexClass,string> map_vvMap_keyData;
    int row = scanAliend->Get_position_row_str();

    if(lex == lcEnter)
    {
        row--;
    }

    if(vvMap_KeyData.empty())
    {
        vvMap_KeyData.push_back(col_vvMap_keyData);
    }
    else
    {
        if(vvMap_KeyData.size() == row)
        {
            vvMap_KeyData.push_back(col_vvMap_keyData);
        }
    }

    map_vvMap_keyData.insert(make_pair(lex,scanAliend->GetTxtValue()));
    vvMap_KeyData[row].push_back(map_vvMap_keyData);
}



bool Parser::addVector_NameType()
{
    bool flag = true;
    for(auto itemType: vector_nameType)
    {
        if(itemType == nameStructType)
        {
            flag = false;
            return false;
        }
    }
    return true;
}


bool Parser::addVector_RecordNameType()
{
    bool flag = true;
    for(auto itemType: vector_nameType)
    {
        if(itemType == nameRecordType)
        {
            flag = false;
            return false;
        }
    }
    return true;
}

string Parser::getWorkData()
{
    string result = "";
    result += "//-------------------\r\n\r\n";

    result += "All new types: [ "+to_string(vector_nameType.size())+" ]";
    result += "\r\n";

    if(vector_nameType.size() > 0)
    {
        for (auto item: vector_nameType)
        {
            result += "\t"+item;
            result += "\r\n";
        }
    }
    else
    {
        result += "Don't find new types.";
        result += "\r\n";
    }
    result += "//-------------------\r\n\r\n";

    //    result += "All predefined types: [ "+to_string(vectorMap_predefined_dataType.size())+" ]";
    //    result += "\r\n";

    //    if(vectorMap_predefined_dataType.size() > 0)
    //    {
    //        for (auto itemVector: vectorMap_predefined_dataType)
    //        {
    //            for (auto itemMap: itemVector)
    //            {
    //                result += "\t"+itemMap.first + " -> " + itemMap.second;
    //                result += "\r\n";
    //            }
    //        }
    //    }
    //    else {
    //        result += "Don't find predefined types.";
    //        result += "\r\n";
    //    }
    //    result += "//-------------------\r\n\r\n";



    result += "All struct hard types: [ "+to_string(table.types_.size())+" ]";
    result += "\r\n";
    if(table.types_.size() > 0)
    {
        for (auto item: table.types_)
        {
            result += "\t"+item->getNameHardType()+":";
            result += "\r\n";

            for(int i=0; i < item->get_count();i++)
            {
                auto structure = item->get_variable_at(i);

                result += "\t\t"+structure->get_name() + " " + structure->get_type();
                result += "\r\n";

            }
        }
    }
    else
    {
        result += "Don't find struct hard type.";
        result += "\r\n";
    }
    result += "//-------------------\r\n\r\n";

    result += "All record hard types: [ "+to_string(table.records_.size())+" ]";
    result += "\r\n";

    if(table.records_.size() > 0)
    {
        for (auto item: table.records_)
        {
            result += "\t"+item->getNameHardType()+":";
            result += "\r\n";

            for(int i=0; i < item->get_count();i++)
            {
                auto record = item->get_variable_at(i);

                result += "\t\t"+record->get_name() + " " + record->get_type();
                result += "\r\n";

            }

            result += "\t union "+item->getLocal()+":";
            result += "\r\n";

            for(int i=0; i < item->get_count_union();i++)
            {
                auto record_union = item->get_variable_union_at(i);

                result += "\t\t"+record_union->get_name() + " " + record_union->get_type();
                result += "\r\n";

            }
        }
    }
    else
    {
        result += "Don't find record hard type.";
        result += "\r\n";
    }
    result += "//-------------------\r\n\r\n";

    result += "All func: [ "+to_string(table.functions_.size())+" ]";
    result += "\r\n";

    if(table.functions_.size() > 0)
    {
        for (auto item: table.functions_)
        {
            result += "\t{\r\n";
            result += "\t\t"+item->getNameFuncType()+":";
            result += "\r\n";

            for(auto itemVariables: item->getVariables2())
            {
                for (auto item: itemVariables)
                {
                    for (auto itemArg: item.first)
                    {
                        result += "\t\t\t"+itemArg.first + " " + itemArg.second;

                        if(item.second.getExpression().size() > 0)
                        {

                            result += " = ";
                            result += item.second.getResult().rbegin()->second;

                            result +=" | ";
                            for (auto itemExpression: item.second.getExpression())
                            {
                                result += itemExpression.begin()->second;
                            }

                        }

                        result += "\r\n";
                    }
                }
            }

            string returnName = item->getReturnFuncType();
            if(returnName == "")
            {
                result += "\t\tReturn type: void";
            }
            else
            {
                result += "\t\tReturn type: "+item->getReturnFuncType();
            }
            result += "\r\n\t}";
            result += "\r\n";
            result += "\r\n";
        }
    }
    else
    {
        result += "Don't find func.";
        result += "\r\n";
    }
    result += "//-------------------\r\n\r\n";


    return result;
}

string Parser::find_variable_name(vector<map<LexClass, string> > &item)
{
    string variable_label;
    for(auto it = item.begin(); it != item.end(); it++)
    {
        if(it->rbegin()->first == lcEqual || it->rbegin()->first == lcAssign )
        {
            for(auto it_variable = item.begin(); it_variable != it;it_variable++)
            {
                variable_label += it_variable->begin()->second;
            }
            item.erase(item.begin(),it+1);
            break;
        }
    }

    return variable_label;
}

bool Parser::expression_between_ifelse(vector<map<LexClass, string> > &item)
{
    //Написать код который обрабатывает условие между if и {
    //примерный код:

    auto item_copy = *vvMap_KeyData.rbegin();

    item_copy.erase(item_copy.begin());
    item_copy.erase(item_copy.end()-1);

    //Зацикливаем пока не появиться false
    //теоритически можно зациклить внутри функции рекурсией чтоб не проходить по массиву лишний раз
    //но так наглядней,потом может подправлю
    while(table.find_name_variable(newFuncItem,item_copy));

    table.expression.setExpression(item_copy);


    string type = "auto";
    auto result_item = table.expression.getResult().rbegin();
    auto result_item_type = result_item->first;
    switch (result_item_type) {
    case lcIntNum: {
        type = "int";
        break;
    }
    case lcRealNum: {
        type = "float";
        break;
    }
    }

    auto result_expression = result_item->second;

    map<LexClass, string> newItem;
    newItem.insert(make_pair(result_item_type,result_expression));

    if(result_expression == "0")
    {
        return false;
    }

    return true;

    //item.erase(item.begin()+1,item.end()-1);
    //item.insert(item.begin()+1,newItem);

}

