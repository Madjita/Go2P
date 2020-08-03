#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <string>


using namespace std;

#include "alien/scanner.h"

#include <map>
#include "my/hardtype.h"
#include "my/functype.h"
#include "my/recordhardtype.h"
#include "my/ifelse.h"
#include "my/funccall.h"

#include "alien/CompileErrors.h"

#include "my/table.h"


#include "my/three_address_code/polish_notation/polish_notation.h"



class Parser
{
public:
    Parser(string fileData, CompileErrors *errors = nullptr);

    opcType type;

    Polish_notation polish;



    Scanner* scanAliend;
    LexClass lex;
    LexClass prevLex;

    bool TypeInteger();
    bool TypeUInteger();
    bool TypeFloat();
    bool Type();
    bool NewType();

    //определение новой функции
    bool newFunc();
    bool Statement();
    bool StatementArray();
    bool StatementArrayInit();
    //инициализация атоматическая
    bool StatementShortInit();
    bool StatementInit();
    bool funcReturn();
    bool rowStructStatement();
    bool funcArgsStatement();
    bool funcArgsCall();
    bool structNewType();

    bool VariableInit();
    bool Variable();
    bool VariableFuncCall();
    bool Number();
    bool Term();
    bool Expression();
    bool Product();
    bool Addition();

    //Условия if else
    bool IfElse();
    //цикл
    bool loop();
    bool switchCase();

    bool Do();

    bool Spaces();

    bool End();

    bool import();

    //Обобщение
    bool recordNewType();
    bool rowRecordStatementType();
    bool rowRecordStatementKey();

    //Расширить обобщение
    bool addRecordNewRow();

    //Пробую функцию со скобками как в с++ {}
    bool lcLFigure_lcRFigure(label_typ typ = label_not);

    vector<vector<map<LexClass,string>>> get_vvMap_KeyData();


    //Функция вывода данных для файла
    string getWorkData();


    //MY
    Table table;

    string find_variable_name(vector<map<LexClass,string>> &item);
    bool expression_between_ifelse();

private:
    LexClass nextLex(bool flagSetSavePosition = false);

    void add_vvMap_KeyData();

    int savePosition;


    //
    FuncType* newFuncItem;
    HardType* newHardTypeItem;
    RecordHardType* newRecordHardTypeItem;

    FuncCall* call_function;

    //Не знаю нужен ли тут указать на условия но его сложно передавать по функциям
    class IfElse* point_if_else; //временный указатель на актуальное условие

    //Вектор уже объявленных типов данных
    vector<string> vector_nameType;
    bool addVector_NameType();
    bool addVector_RecordNameType();



    vector<map<string,string>> vectorMap_nameType;
    vector<string> vectorNameThisLabel;
    string nameThisType;
    string nameStructType;
    string nameRecordType;
    string nameFuncStr;


    //пробую создать массив ключ значение
    vector<vector<map<LexClass,string>>> vvMap_KeyData; //вектор строк хранящий в себе вектор лексем (ключ значение)
};

#endif // PARSER_H
