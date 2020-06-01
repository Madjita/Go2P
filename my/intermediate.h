#ifndef INTERMEDIATE_H
#define INTERMEDIATE_H
#include <fstream>

#include <iostream>

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



struct element {
    string name;
    struct application appl;
};

struct node {
    struct element val;
    struct node *next;
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

//// �������� � ���������� ��������� ����⠭��
//struct CONST *CONST_constructor(scalType, void*);

// Разновидности операнда
typedef enum {
    nameVarOpd, // именованная переменная из таблицы имен
    tmpVarOpd,  // промежуточная переменная в выражениях
    constOpd,   // константный операнд
    labelOpd    // операнд - метка
} opdType;

// Организация операнда
struct OPERAND {
    opdType typ; // тип операнда
    union {
        // ссылка на именованную или промежуточную переменную
        application *var;
        // ссылка на константу
        CONST *cons;
        // ссылка на метку
        struct INSTRUCTION *label;
    } val; // значение операнда
    // Дополнительные параметры, обеспечивающие тестирование
    int ident; // уникальный идентификатор операнда
    OPERAND *next; // для организации списка операндов
};

//// �������� � ���������� ��������� ���࠭��
//struct OPERAND* OPERAND_constructor(opdType, void*);

// Организация списка операндов
struct OPD_LIST {
    struct OPERAND *last;
};

//// Добавление операнда к списку операндов
//void OPD_LIST_append(struct OPD_LIST*, struct OPERAND*);

//// Вывод списка операндов в файл
//void OPD_LIST_out(struct OPD_LIST *, char *, char *);

// Структуры данных и функции, обеспечивающие хранение и обработку
// команд промежуточного представления

// Разновидности команд промежуточного представления
typedef enum {
    starOpc,            // умножение '*'
    plusOpc,            //плюс '+'
    minusOpc,           //минус '-'
    minusUnOpc,         //унарный минус '-'
    plusUnOpc,         //унарный плюс '+'
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
    ifOpc,              //if
    forOpc,             //for
    whileOpc,           //while
    doOpc,              //do {} while()
    switchOpc,          //switch
    funcOpc,            //call func
} opcType;

//typedef enum {
//  addOpc,	assOpc,		divOpc,		eqOpc,
//  emptyOpc,     exitOpc,	geOpc,		gotoOpc,
//  gtOpc,
//  ifOpc,	inOpc,		indexOpc,	labelOpc,
//  leOpc,	ltOpc,		minOpc,         modOpc,
//  multOpc,	neOpc,          outOpc,		skipOutOpc,
//  spaceOutOpc,  subOpc,         tabOutOpc
//} opcType;

// Организация промежуточной команды
struct INSTRUCTION {
    opcType opc; //код операции
    struct INSTRUCTION *next; // отношение расположения/управления

    // Операндная часть инструкции
    struct OPERAND *arg1;
    struct OPERAND *arg2; // промежуточное представление ( может arg2 = rez - тогда это означает о промежуточном результате)
    struct OPERAND *rez;
    // Место встречи в тексте программы
    int line;
    int column;
    //Добавил
    INSTRUCTION* parent;
    int priority_rang; //ранг приоритета операции
};




// Организация списка промежуточных команд
struct INSTR_LIST {
    INSTRUCTION *firstInstr;
    INSTRUCTION *lastInstr;
    OPERAND* lastOpd;
};


#include <stack>



//у каждой функции есть своя стековая машина (может вставлятьс на вызове функции)


struct MYINST{
    opcType opc; //код операции
    struct MYINST *next; // отношение расположения/управления

    // Операндная часть инструкции
    struct MYOPER *arg1;
    struct MYOPER *arg2; // промежуточное представление ( может arg2 = rez - тогда это означает о промежуточном результате)
    struct MYOPER *rez;
    // Место встречи в тексте программы
    int line;
    int column;
//    //Добавил
//    INSTRUCTION* parent;
    int priority_rang; //ранг приоритета операции
    int number; // номер операции
};

#include "my/myvariable.h"

// Организация операнда
struct MYOPER {
    opdType typ; // тип операнда
    union{
        //ссылка на именованную или промежуточную переменную
        MyVariable* var;
        // ссылка на константу
        CONST *cons;
        // ссылка на именованную или промежуточную переменную
        application* varTmp;
    }val;


    // Дополнительные параметры, обеспечивающие тестирование
    int ident; // уникальный идентификатор операнда
    MYOPER *next; // для организации списка операндов
};

//Стековая машина
struct MashineSteck
{
    stack<MYINST*> steck_no_priority;
    stack<MYINST*> steck;
    stack<MYINST*> steck_tmp; //промежуточный стек
    MashineSteck* next;
};



class Intermediate
{
public:
    Intermediate();


    // Создание и заполнение структуры константы
    struct CONST *CONST_constructor(scalType, void*);


    // Создание и заполнение структуры операнда
    OPERAND* OPERAND_constructor(opdType, void*);

    // Добавление операнда к списку операндов
    void OPD_LIST_append(OPD_LIST*,OPERAND*);


    // Вывод списка операндов в файл
    void OPD_LIST_out(OPD_LIST*, FILE*);

    // Вывод списка команд
    void INSTR_LIST_out(INSTR_LIST *, FILE*);

    // Добавление к списку команд отдельной команды
    void INSTR_LIST_append( INSTR_LIST**,  INSTRUCTION*);

    // Конкатенация двух списков команд в первый
    // Замена или установка операнда списка не производится.
    // Она должна осуществляться явно в программе анализатора
    void INSTR_LIST_cat( INSTR_LIST**,  INSTR_LIST*);

    // Создание и заполнение структуры списка команд промежуточного представления
    INSTR_LIST *INSTR_LIST_constructor( INSTRUCTION*,
                                        INSTRUCTION*,  OPERAND*);

    // Создание и заполнение структуры команды
    INSTRUCTION* INSTRUCTION_constructor
    (opcType,  OPERAND*,  OPERAND*,  OPERAND*);

    // Создание операнда - метки
    OPERAND* CreateLabel();


    //Мои функции
    bool add_operand(opdType typ,void*); // добавить операнд
    bool add_opcType(opcType typ);
    bool set_next_priori(int rang);


    bool remove_last_tmpVarOpd();

    void set_auto_rang(opcType typ);

    string opc(opcType typ);
    bool out_list();
    void out_stek(string fileName, stack<MYINST *> *steck);
    void create_tmpNumber();



    //Пробую со стековой машиной
    MYOPER* create_myoperand(opdType typ, void* val); // добавить операнд
    MYINST *create_instruction(); //создать пустую инструкцию

    bool add_operand(MYOPER* opdPtr);
    bool add_left_operand(MYOPER* opdPtr);  // добавить левый операнд
    bool add_right_operand(MYOPER* opdPtr); // добавить правый операнд
    bool add_rezult(MYOPER* opdPtr);        // добавить результат
    bool add_opc(opcType typ);              // добавить операцию

    bool add_last_oper(); // удаление последней созданной инструкции и перенос переменной в правый оперенд предпоследней иснтрукции

    bool create_statement(opcType typ,MYOPER* opdPtr); //Создать инструкцию присвоения

    bool add_inSteck();                     //добавить иснтрукцию в стековую машину
    int  get_auto_rang(opcType typ);

    bool create_tmpVar(); // создаем временные переменне по стеку;

    MYINST* myinstruction;
    MYOPER* leftOpc;
    MYOPER* rightopc;
    MYOPER* rezult;

private:
    INSTRUCTION* instruction;
    INSTR_LIST* list;
    int priority_rang; //ранг приоритета операции

    MashineSteck* steck_begin_;
    MashineSteck* steck_select_;
    MashineSteck* steck_selectPrev_;
};

#endif // INTERMEDIATE_H
