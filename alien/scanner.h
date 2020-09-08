#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
using namespace std;

#include "alien/tlt.h"
#include <map>
//------------------------------------------------------------------------------

const unsigned int decMax = (0x7FFFFFFF / 10);		// максимальное десятичное
const unsigned int hexMax = 0x07FFFFFF;			// максимальное шестнадцатеричное

//------------------------------------------------------------------------------
// Список лексем разрабатываемого языка программирования
//------------------------------------------------------------------------------
enum LexClass
{
    keyLocal,
    keyUnion,
    keyBreak,
    keyCase,
    //keyChan,
    keyConst,
    keyContinue,
    keyDefault,
    keyDefer,
    keyElse,
    keyFallthrough,
    keyFor,
    keyFunc,
    keyGo,
    keyGoto,
    keyIf,
    keyImport,
    //keyInterface,
    keyMap,
    //keyPackage,
    //keyRange,
    keyReturn,
    keySelect,
    keyStruct,
    keySwitch,
    keyType,
    keyVar,

    lcEnter,


    //тип данных
    //Целые числа
    keyInt,
    keyInt8,
    keyInt16,
    keyInt32,
    keyInt64,

    //Целые без знаковые числа
    keyUInt,
    keyUInt8,
    keyUInt16,
    keyUInt32,
    keyUInt64,

    //Дробные числа
    keyFloat,
    keyFloat32,
    keyFloat64,

    keyString,
    keyByte,

    lcLCircle,			// круглые скобки '('
    lcRCircle,			// круглые скобки ')'
    lcLSquare,			// квадратные скобки '['
    lcRSquare,			// квадратные скобки ']'
    lcLFigure,			// фигурные скобки '{'
    lcRFigure,			// фигурные скобки '}'

    lcPoint,			// точка '.'
    lcComma,			// запятая ','
    lcSemicolon,		// точка с запятой ';'
    lcColon,			// двоеточие ':'

    lc2Points,			// две точки подряд '..'

    lcImpersant,		// имперсант '&'
    lcRoof,				// крышка '^'
    lcStick,			// палка '|'
    lcWave,				// ~

    lcStar,				// звездочка '*'
    lcSlash,			// деление '/'
    lcPlus,				// плюс '+'
    lcMinus,			// минус '-'
    lcMod,              // остаток от деления '%'

    lcAssign,			// авто присваивание ':='
    lcEqual,			// присвоение '='
    lcNotEqual,			// не равно '#'
    lcSmaller,			// меньше '<'
    lcLarger,			// больше '>'
    lcSmallerEQ,		// меньше или равно '<='
    lcLargerEQ,			// больше или равно '>='
    lcEQ,               // равенство '=='

    lcNoValue,			// нет значения ''

    lcError,			// ошибка

    lcComment,			// комментарий
    lcSpace,			// пустой
    lcIgnore,			// игнорируемый

    lcIntNum,			// целое число
    lcRealNum,			// действительное число
    lcHardType,         // сложный тип данных (структура)
    lcRecordType,       // сложный тип данных (обобщение)
    lcAuto,             // Автоматический тип

    lcCharConst,		// символ
    lcString,			// строка

    lcId,				// идентификатор
    lcLabel,			// метка
    //lcQualident,        // квалидент (двойной идентификатор)

    lcEof				// конец файла

};

// Разновидности переменной
typedef enum {
    varGlobal,   // глобальная переменная
    varLocal,    // локальные переменные
    varHardType, // переменная из структуры
    varRecordType,  // переменная из обобщения
} varType;


//==============================================================================
// синтаксический анализатор
//==============================================================================
class Scanner
{
public:
    Scanner(string fileData);
    virtual ~Scanner() { delete tlt_; }


    // формирование класса и значения очередной лексемы
    void Next();

    // текущая лексема
    LexClass GetLex();
    // текстовое значение лексемы
    std::string GetTxtValue() { return lex_value_; }
    void setValue(string value);
    // целочисленное значение лексемы
    unsigned int GetIntValue() { return uInt_; }
    // действительное значение лексемы
    double GetRealValue() { return _dNum; }

    // текущая позиция относительно файла
    int Get_position_file_str() { return tlt_->Get_position_file_str(); }
    // текущая строка
    int Get_position_row_str() { return tlt_->Get_position_row_str(); }
    // текущий столбец
    int Get_position_col_char_str_() { return tlt_->Get_position_col_char_str_(); }

    // предыдущая строка
    int GetPrevStr() { return _prevStr; }
    // предыдущий столбец
    int GetPrevCol() { return _prevCol; }

    InformPosition getPosition()
    {
        //debug_out_lexValue_ = debugOutLex;
        return tlt_->getPosition();
    }

    bool setPosition(InformPosition oldInfoPosition)
    {
       // debugOutLex = debugOutLexSave;
        return tlt_->setPosition(oldInfoPosition);
    }

    bool setSavePosition()
    {
        //debugOutLex = debugOutLexSave;
        tlt_->setSavePosition();
        return true;
    }

    //debug
    void DebugOut();

    string Out_str_lex(LexClass lex);

protected:
    void CompileError(unsigned int n);
    void FillFailedLexem(unsigned int errorCode); // "проматываю" последующие буквы и цифры, чтобы избежать появления лишних лексем

    void IdOrKw();				// опознание идентификатора или ключевого слова
    void CommentOrDiv();        // распознавание комментариев
    void Number();				// общая проверка на число
    void DecNumber();           // проверка на десятичное число
    void HexNumber();				// проверка на шестнадцатиричное число
    void ForbiddenSymbol();			// проверка на запрещенный символ, который может следовать

    // действительные числа
    void RealContinue();			// перед точкой были цифры
    void Exp();					// проверка на порядок
    char EscapeSymbol(char ch);
    void String();
    void CharConst();

    int DecToInt(const char* Str);
    int HexToInt(const char* Str);

private:
    Tlt * tlt_;

    LexClass lex_class_;			// класс лексемы
    string lex_value_;				// значение лексемы

    std::map<string,LexClass,less<std::string>> table_keyLexValue_;		// таблица ключевых слов


    // временные переменные для хранения значения лексемы когда она определяется как число
    unsigned int uInt_;			// целое
    double _dNum;			// действительное

    //переменные хранящие в себе преведущую позицию
    int _prevStr; // позиция символа в строке
    int _prevCol; // позиция в строке (абзаце)


     string debug_out_lexValue_;
};

#endif // SCANNER_H
