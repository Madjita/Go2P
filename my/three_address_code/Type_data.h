#ifndef TYPE_DATA_H
#define TYPE_DATA_H

#include <istream>
using namespace std;

typedef enum {
    LETTER = 1, DIG, ERS, BTFN, IGN, NO_SIC
} sic_type;

typedef enum {
    NO_LC = 0,
    ADD, ASS, CL,  COM, DIV, DVT, EQ,  ENF, ERL, FLT,
    GE,  GT,  ID,  IGNORE,   INT,
    LE,  LRB, LSB, LT,  MOD, MUL, NE,  PCL, PNT, RRB, RSB,
    SPACE,    STRELA,   SUB,
    KWABORT, KWBEGIN, KWCASE,  KWEND, KWFLOAT, KWINT,   KWLOOP,
    KWOR,    KWREAD,  KWSKIP,  KWSPACE, KWTAB, KWVAR,   KWWRITE
} lc_type;

// Типы данных, необходимые для работы с таблицей имен
typedef enum {INTTYP, FLOATTYP,UNDEF,AUTO} scalType;

struct application {
    scalType typ; // скалярный тип элемента таблицы
    int  len; // длина вектора ( = 0, если скаляр)
    int addr; // адрес в физической памяти (только статической)

    string name; // имя перемеенной
    string val; // значение
};


// Структуры данных и функции, обеспечивающие хранение и обработку операндов

// Организация константы
struct CONST {
    scalType typ; // тип константы
    union {
        int   unum;  // целая константа
        float fnum;  // действительная константа
    } val; // значение константы
};

typedef enum
{
    label_not,      // не выбран
    label_if_false, // изначальное условие if
    label_if_true,
    label_for,
    label_break,
    label_continue
} label_typ;

struct LABEL{
    label_typ typ; // тип метки
    int position; // позиция в трех-адресной ленте
};

// Разновидности операнда
typedef enum {
    noOpd,      // не выбранна
    typeNameOpd,// тип данных
    nameVarOpd, // именованная переменная из таблицы имен
    tmpVarOpd,  // промежуточная переменная в выражениях
    funcVar,    // аргументы функции
    call,       // функция
    funcVar_count, // количество аргументов переданных в функцию
    constOpd,   // константный операнд
    labelOpd    // операнд - метка
} opdType;


// Структуры данных и функции, обеспечивающие хранение и обработку
// команд промежуточного представления

// Разновидности команд промежуточного представления
typedef enum {
    starOpc,            // умножение '*'
    plusOpc,            //плюс '+'
    minusOpc,           //минус '-'
    minusUnOpc,         //унарный минус '-'
    plusUnOpc,         //унарный плюс '+'
    dicrimentOpc,       // декремент '−−'
    incrimentOpc,       //инкремент '++'
    modOpc,             //остаток от деления '%'
    slashOpc,           // деление '/'
    assignOpc,			// авто присваивание ':='
    equalOpc,			// присвоение '='
    notEqualOpc,		// не равно '#'
    smallerOpc,			// меньше '<'
    largerOpc,			// больше '>'
    smallerEQOpc,		// меньше или равно '<='
    largerEQOpc,		// больше или равно '>='
    eqOpc,              // равенство '=='
    noValueOpc,			// нет значения ''
    impersantOpc,		// имперсант '&'

    LCircleOpc,			// круглые скобки '('
    RCircleOpc,			// круглые скобки ')'


    labelOpc,           // метка

    gotoOpc,            //goto метка
    gotoOpcIfZ_true, //goto метка для перехода if после истенного условия
    gotoOpcIfZ_false,//goto метка для перехода if после ложного условия
    gotoOpcIf_trueBreak,//goto метка для выхода из цикла (for) в истенном условии
    gotoOpcIf_falseBreak,//goto метка для выхода из цикла (for) в ложном условии
    gotoOpcIf_trueContinue,
    gotoOpcIf_falseContinue,
    gotoOpcIf_trueReturn,
    gotoOpcIf_falseReturn,

    elseOpc,

    loop_begin,
    loop_short_init,
    lopp_end,
    gotoOpcFor,         //goto метка для перехода в цикле
    gotoOpcFor_infinity, //goto метка для перехода в while(1)
    gotoOpcFor_break,   //goto метка для перехода из цикла for
    gotoOpcFor_continue,
    gotoOpc_return,
    gotoOpcFor_return,
    gotoOpcIf_true_return,
    gotoOpcIf_false_return,

    ifZOpc_for,           //ifZ (если условие отрицательное то перейти по метке выйти из цикла)
    ifZOpc_while,         //ifZ (если условие отрицательное то перейти по метке выйти из цикла)
    ifZOpc,              //ifZ (если условие отрицательное то перейти по метке на else) (goto метка для перехода if после отрицательного действия)
    forOpc,             //for
    whileOpc,           //while
    doOpc,              //do {} while()
    switchOpc,          //switch
    param,              // param Xn
    callFunc_Begin,     // call func
    callFunc_End,
    returnOpc,           //return
    arg_call_func,       //Передача аргументов в функцию
    call_func,           //Вызов функции

    var_statement_one,   // объявление одной переменной с одним типом данных
    var_statement_more,   // объявление нескольких переменных через запятую с одним типом данных
} opcType;


#endif // TYPE_DATA_H
