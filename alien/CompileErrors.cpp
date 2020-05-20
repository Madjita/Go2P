///==============================================================================
/// @file CompileErrors.cpp
/// Содержит методы класса CompileErrors,
/// а также данные с сообщениями об ошибках
/// Позволяет избавиться от внешнего файла ошибок, что убирает из бинарника
/// лишние зависимости.
///==============================================================================


#include "CompileErrors.h"

namespace
{
    char* erMes[] = {
        (char*)"Error 000: no errors",
        (char*)"Error 001: forbidden symbol",
        (char*)"Error 002: unexpected end of file, string not closed",
        (char*)"Error 003: unexpected end of file, missed symbol after '\', string not closed",
        (char*)"Error 004: unexpected end of file, symbol expected, character costant not closed",
        (char*)"Error 005: unexpected end of file, missed symbol after '\', character costant not closed",
        (char*)"Error 006: unexpected end of file, character costant not closed",
        (char*)"Error 007: forbidden symbol, character costant not closed",
        (char*)"Error 008: unexpected end of file, comment not closed",
        (char*)"Error 009: dec number too big",
        (char*)"Error 010: hex number too big",
        (char*)"Error 011: forbidden symbol, hex number merges with id",
        (char*)"Error 012: unexpected end of file, hex number not finished",
        (char*)"Error 013: missed 'X' at the center of hex number",
        (char*)"Error 014: wrong letter at the end of number",
        (char*)"Error 015: wrong letter, expected 'E'",
        (char*)"Error 016: unexpected end of file, real number exp value not complited",
        (char*)"Error 017: unexpected end of file, real number exp value not complited, missed number after '+/-'",
        (char*)"Error 018: missed real number exp value",
        (char*)"Error 019: wrong letter at the end of exp number",
        (char*)"Error 020: missed id",
        (char*)"Error 021: ')' or '*' or '-' expected",
        (char*)"Error 022: missed ';'",
        (char*)"Error 023: missed 'END'",
        (char*)"Error 024: missed '.'",
        (char*)"Error 025: ',' or ';' expected",
        (char*)"Error 026: missed ')'",
        (char*)"Error 027: ';' or ':=' expected",
        (char*)"Error 028: missed 'BEGIN'",
        (char*)"Error 029: id or '0' expected",
        (char*)"Error 030: missed '='",
        (char*)"Error 031: constant must have some value",
        (char*)"Error 032: missed type",
        (char*)"Error 033: ',' or ':' expected",
        (char*)"Error 034: ';' or 'END' expected",
        (char*)"Error 035: missed formal parameter",
        (char*)"Error 036: must be at least one common parameter",
        (char*)"Error 037: ';' or '}' expected",
        (char*)"Error 038: missed 'OF'",
        (char*)"Error 039: missed array length",
        (char*)"Error 040: '{' or '(' expected",
        (char*)"Error 041: missed 'TO'",
        (char*)"Error 042: missed specialization",
        (char*)"Error 043: ',' or ':' or '.' expected",
        (char*)"Error 044: id or 'NIL' expected",
        (char*)"Error 045: 'OF' or id expected",
        (char*)"Error 046: missed ':='",
        (char*)"Error 047: missed expression",
        (char*)"Error 048: missed LValue",
        (char*)"Error 049: ',' or '}' expected",
        (char*)"Error 050: ',' or ')' expected",
        (char*)"Error 051: missed 'THEN'",
        (char*)"Error 052: missed variant",
        (char*)"Error 053: missed 'DO'",
        (char*)"Error 054: ';' or 'UNTIL' expected",
        (char*)"Error 055: missed type guard",
        (char*)"Error 056: missed ':'",
        (char*)"Error 057: missed tag",
        (char*)"Error 058: missed label",
        (char*)"Error 059: missed addition argument",
        (char*)"Error 060: missed argument",
        (char*)"Error 061: ';' or ']' expected",
        (char*)"Error 062: missed operator",
        (char*)"Error 063: missed '+'",
        (char*)"Error 064: missed '^' or id",
        (char*)"Error 065: missed '+' or '='",
        (char*)"Error 066: missed MODULE",
        (char*)"Error 067: missed DEFINITION",

        (char*)"Error 068: id's doesn't coincide",
        (char*)"Error 069: id already exist",
        (char*)"Error 070: object doesn't exist",
        (char*)"Error 071: not type",
        (char*)"Error 072: label was declared twice",
        (char*)"Error 073: label wasn't declared",
        (char*)"Error 074: incorrect type",
        (char*)"Error 075: step can't be zero",
        (char*)"Error 076: expression result not constant",
        (char*)"Error 077: incorrect operation with constant",
        (char*)"Error 078: object is not a pointer",
        (char*)"Error 079: size of array is not unsigned",
        (char*)"Error 080: not array",
        (char*)"Error 081: not variable",
        (char*)"Error 082: different types of arguments",
        (char*)"Error 083: parameter-variable can't be array",
        (char*)"Error 084: incorrect number of array indices",
        (char*)"Error 085: not all parts of procedure return value",
        (char*)"Error 086: impossible to return outside of procedure",
        (char*)"Error 087: procedure can't return value",
        (char*)"Error 088: procedure must return value",
        (char*)"Error 089: incorrect object",
        (char*)"Error 090: not field",
        (char*)"Error 091: type is not type record",
        (char*)"Error 093: incorrect field",
        (char*)"Error 093: not label",
        (char*)"Error 094: parameters lists not coincide",
        (char*)"Error 095: length must be > 0",
        //(char*)"Error 094: missed MODULE",
        (char*)"Error 096: forward pointer is possible only for records and arrays",
        (char*)"Error 097: wrong return type, only scalars allowed",
        (char*)"Error 098: pointer to pointer not allowed",
        (char*)"Error 099: formal parameter is parameter-variable, only variables acceptable as arguments",
        //(char*)"Error 100: arrays to arrays restricted (temporary)",
        (char*)"Error 100: module not found",
        (char*)"Error 101: incorrect number of parameters",
        (char*)"Error 102: specialization already exist",
        (char*)"Error 103: can be only one default specialization",
        (char*)"Error 104: specialization id missed",
        (char*)"Error 105: unnecessary id of specialization",
        (char*)"Error 106: missed '+' or '='",
        (char*)"Error 107: not a generic",
        (char*)"Error 108: missed NIL or QualId",
        (char*)"Error 109: extension restricted",
        (char*)"Error 110: '.' or ':' expected",
        (char*)"Error 111: new type can't be based on generic",
        (char*)"Error 112: not a specialization",
        (char*)"Error 113: variable type can't be pure generic",
        (char*)"Error 114: generic proc missed",
        (char*)"Error 115: can't bind specialized proc to generic proc",
        (char*)"Error 116: generic param can be only pointer or var param",
        (char*)"Error 117: missed generic or specialization",
        (char*)"Error 118: only generic proc can be abstract",
        (char*)"Error 119: object is already defined",
        (char*)"Error 120: recursive import is not allowed",
        (char*)"Error 121: couldn't load attachment module",
        (char*)"Error 122: generic or generic record expected",
        (char*)"Error 123: inadmissible type of case label",
        (char*)"Error 124: case label value occured more than once",
        (char*)"Error 125: the actual parameter must be variable",
        (char*)"Error 126: incorrect the name of MODULE",
        (char*)"Error 127: incorrect the name of TYPE",
        (char*)"Error 128: PROCEDURE or MODULE expected",
        (char*)"Error 129: incorrect the name of VARIABLE",

        (char*)"Error 10000: ..."
    };

    int erMesNumber = sizeof(erMes) / sizeof(char);


}


CompileErrors::CompileErrors():	num(0), out(cout), size(erMesNumber)
{
    list = erMes;
}

//==========================================================================
// выводит сообщение об ошибке
//--------------------------------------------------------------------------
// Str - [in] строка с ошибкой
// Col - [in] столбец
// n   - [in] номер сообщения об ошибке
//==========================================================================
void CompileErrors::Message(unsigned int Str, unsigned int Col, int n)
{
	out << "[" << Str << ", " << Col << "] ";
	if(n > 0 && n <= size)
		out << list[n] << endl;
	++num;
}

/*
CompileErrors::CompileErrors(string filename):	num(0), out(cout)
{
    // считываю текст сообщений об ошибках
    ifstream in(filename.c_str());
    if(!in)
        throw Exception(__FILE__, __LINE__, string("Can't open ") + filename);

    while(!in.eof()) {
        string Str;
        getline(in, Str);
        list.push_back(Str);
    }
}
*/

