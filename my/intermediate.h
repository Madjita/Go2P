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
    stack<MashineSteck*> next;
    stack<MashineSteck*> next_tmp;
    MashineSteck* parent;
};



//Для вывода в файл
enum countSteck{
    st1, // Стек с приоритетом
    st2 //  Стек без приоритета
};



class Intermediate
{
public:
    Intermediate();


    //Мои функции

    void set_auto_rang(opcType typ);

    string opc(opcType typ);

    bool out_list();
    //string out_stek(string& str,stack<MYINST *> *steck);

    string out_stek_2(string& str, MashineSteck* item,int tab,countSteck st);
    void create_tmpNumber(MashineSteck*);

    void out_stek_file(string fileName,countSteck st);
    void file_clear(string fileName);




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
    int priority_rang; //ранг приоритета операции

    MashineSteck* steck_begin_;
    MashineSteck* steck_select_;
    MashineSteck* steck_selectPrev_;
};

#endif // INTERMEDIATE_H
