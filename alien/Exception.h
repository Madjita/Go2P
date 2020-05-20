#ifndef __Exception__
#define __Exception__

#include <string>
using namespace std;

enum Errors
{
        // отсутствие ошибок
        sExit = 0,
        // лексические ошибки
        // Error 001: forbidden symbol
        leForbiddenSymbol,
        // Error 002: unexpected end of file, string not closed
        leEOFStringNotClosed,
        // Error 003: unexpected end of file, missed symbol after '\', string not closed
        leEOFMissedSymbolStringNotClosed,
        // Error 004: unexpected end of file, symbol expected, character costant not closed
        leEOFSymbolExprectedCharConstNotClosed,
        // Error 005: unexpected end of file, missed symbol after '\', character costant not closed
        leEOFMissedSymbolCharConstNotClosed,
        // Error 006: unexpected end of file, character costant not closed
        leEOFCharConstNotClosed,
        // Error 007: forbidden symbol, character costant not closed
        leForbiddenSymbolConstNotClosed,
        // Error 008: unexpected end of file, comment not closed
        leEOFCommentNotClosed,
        // Error 009: dec number too big
        leDecNumTooBig,
        // Error 010: hex number too big
        leHexNumTooBig,
        // Error 011: forbidden symbol, hex number merges with id
        leForbiddenSymbolHexNumMergesWithId,
        // Error 012: unexpected end of file, hex number not finished
        leEOFHexNumNotFinished,
        // Error 013: missed 'X' at the center of hex number
        leHexNumMissedX,
        // Error 014: wrong letter at the end of number
        leWrongLetterAtNumEnd,
        // Error 015: wrong letter, expected 'E'
        leWrongLetterExprectedE,
        // Error 016: unexpected end of file, real number exp value not completed
        leEOFRealNumENotCompleted,
        // Error 017: unexpected end of file, real number exp value not complited, missed number after '+/-'
        leEOFRealNumNotCompletedMissedNumAfterSign,
        // Error 018: missed real number exp value
        leRealNumMissedExp,
        // Error 019: wrong letter at the end of exp number
        leRealNumWrongLetterAtExpEnd,

        // синтаксические ошибки
        // Error 020: missed id
        peMissedId,
        // Error 021: ')' or '*' or '-' expected
        peRCircleStarMinusExpected,
        // Error 022: missed ';'
        peMissedSemicolon,
        // Error 023: missed 'END'
        peMissedEND,
        // Error 024: missed '.'
        peMissedDot,
        // Error 025: ',' or ';' expected
        peCommaSemicolonExpected,
        // Error 026: missed ')'
        peMissedRCircle,
        // Error 027: ';' or ':=' expected
        peSemicolonAssignExpected,
        // Error 028: missed 'BEGIN'
        peMissedBEGIN,
        // Error 029: id or '0' expected
        peIdZeroExpected,
        // Error 030: missed '='
        peMissedEqual,
        // Error 031: constant must have some value
        peConstMustHaveValue,
        // Error 032: missed type
        peMissedType,
        // Error 033: ',' or ':' expected
        peCommaColonExpected,
        // Error 034: ';' or 'END' expected
        peSemicolonENDexpected,
        // Error 035: missed formal parameter
        peMissedFormalParam,
        // Error 036: must be at least one common parameter
        peMustBeAtLeastOneCommonParam,
        // Error 037: ';' or '}' expected
        peSemicolonRFigureExpected,
        //Error 038: missed 'OF'
        peMissedOF,
        //Error 039: missed array length
        peMissedArrayLength,
        //Error 040: '{' or '(' expected
        peLFigureLCircleExpected,
        //Error 041: missed 'TO'
        peMissedTO,
        //Error 042: missed specialization
        peMissedSpecialization,
        //Error 043: ',' or ':' or '.' expected
        peCommaColonDotExpected,
        //Error 044: id or 'NIL' expected
        peIdNILExpected,
        //Error 045: 'OF' or id expected
        peIdOFExpected,
        //Error 046: missed ':='
        peMissedAssign,
        //Error 047: missed expression
        peMissedExpression,
        //Error 048: missed designator
        peMissedDesignator,
        //Error 049: ',' or '}' expected
        peCommaRFigureExpected,
        //Error 050: ',' or ')' expected
        peCommaRCircleExpected,
        //Error 051: missed 'THEN'
        peMissedTHEN,
        //Error 052: missed variant
        peMissedVariant,
        //Error 053: missed 'DO'
        peMissedDO,
        //Error 054: ';' or 'UNTIL' expected
        peSemicolonUNTILExpected,
        //Error 055: missed type guard
        peMissedTypeGuard,
        //Error 056: missed ':'
        peMissedColon,
        //Error 057: missed tag
        peMissedTag,
        //Error 058: missed label
        peMissedLabel,
        //Error 059: missed addition argument
        peMissedAdditionArgument,
        //Error 060: missed argument
        peMissedArgument,
        //Error 061: ';' or ']' expected
        peSemicolonRSquareExpected,
        //Error 062: missed operator
        peMissedOperator,
        //Error 063: missed '+'
        peMissedAdd,
        //Error 064: missed '^' or id
        peMissedIdRoof,
        //Error 065: missed '+' or '='
        peMissedAddEqual,
        //Error 066: missed MODULE
        peMissedMODULE,
        //Error 067: missed DEFINITION
        peMissedDEFINITION,

        // семантические ошибки
        //Error 068: id's doesn't coincide
        seIdsNotCoincide,
        //Error 069: id already exist
        seIdExist,
        //Error 070: object doesn't exist
        seObjectNotExist,
        //Error 071: not type
        seNotType,
        //Error 072: label was declared twice
        seLabelDeclaredTwice,
        //Error 073: label wasn't declared
        seLabelNotDeclared,
        //Error 074: incorrect type
        seIncorrectType,
        //Error 075: step can't be zero
        seStepCanNotBeZero,
        //Error 076: expression result not constant
        seExpressionResultNotConst,
        //Error 077: incorrect operation with constant
        seIncorrectOperationWithConst,
        //Error 078: object is not a pointer
        seObjectNotPointer,
        //Error 079: size of array is not unsigned
        seArraySizeNotUnsigned,
        //Error 080: not array
        seNotArray,
        //Error 081: not variable
        seNotVariable,
        //Error 082: different types of arguments
        seDifferentTypesOfArgs,
        //Error 083: parameter-variable can't be array
        seParameterVariableCanNotBeArray,
        //Error 084: incorrect number of array indices
        seIncorrectNumOfArrayIndices,
        //Error 085: not all parts of procedure return value
        seNotAllPartsOfProcedureReturnValue,
        //Error 086: impossible to return outside of procedure
        seImpossibleToReturnOutsideOfProc,
        //Error 087: procedure can't return value
        seProcCanNotReturnValue,
        //Error 088 procedure must return value
        seProcMustReturnValue,
        //Error 089: incorrect object
        seIncorrectObject,
        //Error 090: not field
        seNotField,
        //Error 091: type is not type record
        seTypeNotTypeRecord,
        //Error 092: incorrect field
        seIncorrectField,
        //Error 093: not label
        seNotLabel,
        //Error 094: parameters lists not coincide
        seParametersNotCoincide,
        //Error 095: length must be > 0
        seLengthMustBePositive,
        //Error 096: forward pointer is possible only for records and arrays
        seForwardPtrIsPossibleForRecordArray,
        //Error 097: wrong return type, only scalars allowed
        seWrongReturnTypeScalarsAllowed,
        //Error 098: pointer to pointer not allowed
        sePtrToPtrNotAllowed,
        //Error 099: formal parameter is parameter-variable, only variables acceptable as arguments
        seFParamIsParamVariableVariablesAcceptable,
        //Error 100: module not found
        seModuleNotFound,
        //Error 101: incorrect number of parameters
        seParamsIncorrectNum,
        //Error 102: specialization already exist
        seSpecExist,
        //Error 103: can be only one default specialization
        seCanBeOneDefaultSpec,
        //Error 104: specialization id missed
        seSpecIdMissed,
        //Error 105: unnecessary id of specialization
        seUnneccessarySpecId,
        //Error 106: DFN module not found
        seDfnNotFound,
        //Error 107: not a generic
        seNotGeneric,
        //Error 108: missed NIL or QualId
        seMissedNILQualId,
        //Error 109: extension restricted
        seExtensionRestricted,
    //Error 110: '.' or ':' expected",
    sePointOrCommaExpected,
        //Error 111: new type can't be based on generic
        seNewTypeCanNotBeBasedOnGeneric,
        //Error 112: not a specialization
        seNotSpec,
        //Error 113: variable type can't be pure generic
        seVariableTypeCanNotBePureGeneric,
        //Error 114: generic proc missed
        seMissedGenProc,
        //Error 115: can't bind specialized proc to generic proc
        seCanNotBindSpecProcToGenProc,
        //Error 116: generic param can be only pointer or var param
        seGParamCanBePtrVarParam,
        //Error 117: missed generic or specialization
        seMissedGenericSpec,
        //Error 118: only generic proc can be abstract
        seOnlyGenProcCanBeAbstract,
        //Error 119: object is already defined
        seObjectIsDefined,
        //Error 120: recursive import is not allowed
        seRecursiveImportNotAllowed,
        //Error 121: couldn't load attachment module
        seCouldNotLoadAttachmentModule,
        //Error 122: generic or generic record expected
        seGenericGenRecordExpected,
        //Error 123: inadmissible type of case label
        seCaseLabelInadmissibleType,
        //Error 124: case label value occured more than once
        seCaseLabelValueOccuredMoreThanOnce,
        //Error 125: the actual parameter must be variable
        seTheActualParameterMustBeVariable,
    //"Error 126: incorrect the name of MODULE",
    seIncorrectNameOfModule,
    //"Error 127: incorrect the name of TYPE",
    seIncorrectNameOfType,
    //"Error 128: PROCEDURE or MODULE expected",
    seProcOrModuleExpected,
    //"Error 129: incorrect the name of VARIABLE",
    seIncorrectNameOfVar,

    //"Error 10000: ..."
    qqLastErrorTeg
};

const string ErrorMessages[] =
{
        // отсутствие ошибок
        "Internal error: wrong error code",
        // лексические ошибки
        // Error 001: forbidden symbol
        "Forbidden symbol",
        // Error 002: unexpected end of file, string not closed
        "Unexpected end of file, string not closed",
        // Error 003: unexpected end of file, missed symbol after '\', string not closed
        "Unexpected end of file, missed symbol after '\', string not closed",
        // Error 004: unexpected end of file, symbol expected, character costant not closed
        "Unexpected end of file, symbol expected, character costant not closed",
        // Error 005: unexpected end of file, missed symbol after '\', character costant not closed
        "Unexpected end of file, missed symbol after '\', character costant not closed",
        // Error 006: unexpected end of file, character costant not closed
        "Unexpected end of file, character costant not closed",
        // Error 007: forbidden symbol, character costant not closed
        "Forbidden symbol, character costant not closed",
        // Error 008: unexpected end of file, comment not closed
        "Unexpected end of file, comment not closed",
        // Error 009: dec number too big
        "Decimal number too big",
        // Error 010: hex number too big
        "Hex number too big",
        // Error 011: forbidden symbol, hex number merges with id
        "Forbidden symbol, hex number merges with id",
        // Error 012: unexpected end of file, hex number not finished
        "Unexpected end of file, hex number not finished",
        // Error 013: missed 'X' at the center of hex number
        "Missed 'X' at the center of hex number",
        // Error 014: wrong letter at the end of number
        "Wrong letter at the end of number",
        // Error 015: wrong letter, expected 'E'
        "Wrong letter, expected 'E'",
        // Error 016: unexpected end of file, real number exp value not completed
        "Unexpected end of file, real number exp value not completed",
        // Error 017: unexpected end of file, real number exp value not complited, missed number after '+/-'
        "Unexpected end of file, real number exp value not complited, missed number after '+/-'",
        // Error 018: missed real number exp value
        "Missed real number exp value",
        // Error 019: wrong letter at the end of exp number
        "Wrong letter at the end of exp number",

        // синтаксические ошибки
        // Error 020: missed id
        "Missed id",
        // Error 021: ')' or '*' or '-' expected
        "')' or '*' or '-' expected",
        // Error 022: missed ';'
        "Missed ';'",
        // Error 023: missed 'END'
        "Missed 'END'",
        // Error 024: missed '.'
        "Missed '.'",
        // Error 025: ',' or ';' expected
        "',' or ';' expected",
        // Error 026: missed ')'
        "Missed ')'",
        // Error 027: ';' or ':=' expected
        "';' or ':=' expected",
        // Error 028: missed 'BEGIN'
        "Missed 'BEGIN'",
        // Error 029: id or '0' expected
        "Id or '0' expected",
        // Error 030: missed '='
        "Missed '='",
        // Error 031: constant must have some value
        "Constant must have some value",
        // Error 032: missed type
        "Missed type",
        // Error 033: ',' or ':' expected
        "',' or ':' expected",
        // Error 034: ';' or 'END' expected
        "';' or 'END' expected",
        // Error 035: missed formal parameter
        "Missed formal parameter",
        // Error 036: must be at least one common parameter
        "Must be at least one common parameter",
        // Error 037: ';' or '}' expected
        "';' or '}' expected",
        //Error 038: missed 'OF'
        "Missed 'OF'",
        //Error 039: missed array length
        "Missed array length",
        //Error 040: '{' or '(' expected
        "'{' or '(' expected",
        //Error 041: missed 'TO'
        "Missed 'TO'",
        //Error 042: missed specialization
        "Missed specialization",
        //Error 043: ',' or ':' or '.' expected
        "',' or ':' or '.' expected",
        //Error 044: id or 'NIL' expected
        "Id or 'NIL' expected",
        //Error 045: 'OF' or id expected
        "'OF' or id expected",
        //Error 046: missed ':='
        "Missed ':='",
        //Error 047: missed expression
        "Missed expression",
        //Error 048: missed designator
        "Missed designator",
        //Error 049: ',' or '}' expected
        "',' or '}' expected",
        //Error 050: ',' or ')' expected
        "',' or ')' expected",
        //Error 051: missed 'THEN'
        "Missed 'THEN'",
        //Error 052: missed variant
        "Missed variant",
        //Error 053: missed 'DO'
        "Missed 'DO'",
        //Error 054: ';' or 'UNTIL' expected
        "';' or 'UNTIL' expected",
        //Error 055: missed type guard
        "Missed type guard",
        //Error 056: missed ':'
        "Missed ':'",
        //Error 057: missed tag
        "Missed tag",
        //Error 058: missed label
        "Missed label",
        //Error 059: missed addition argument
        "Missed addition argument",
        //Error 060: missed argument
        "Missed argument",
        //Error 061: ';' or ']' expected
        "';' or ']' expected",
        //Error 062: missed operator
        "Missed operator",
        //Error 063: missed '+'
        "Missed '+'",
        //Error 064: missed '^' or id
        "Missed '^' or id",
        //Error 065: missed '+' or '='
        "Missed '+' or '='",
        //Error 066: missed MODULE
        "Missed MODULE",
        //Error 067: missed DEFINITION
        "Missed DEFINITION",

        // семантические ошибки
        //Error 068: id's doesn't coincide
        "Ids doesn't coincide",
        //Error 069: id already exist
        "Id already exist",
        //Error 070: object doesn't exist
        "Object doesn't exist",
        //Error 071: not type
        "Object not type",
        //Error 072: label was declared twice
        "Label was declared twice",
        //Error 073: label wasn't declared
        "Label was not declared",
        //Error 074: incorrect type
        "Incorrect type",
        //Error 075: step can't be zero
        "Step can not be zero",
        //Error 076: expression result not constant
        "Expression result not constant",
        //Error 077: incorrect operation with constant
        "Incorrect operation with constant",
        //Error 078: object is not a pointer
        "Object is not a pointer",
        //Error 079: size of array is not unsigned integer
        "Size of array is not unsigned integer",
        //Error 080: not array
        "Object not array",
        //Error 081: not variable
        "Object not variable",
        //Error 082: different types of arguments
        "Different types of arguments",
        //Error 083: parameter-variable can't be array
        "Parameter-variable can not be array",
        //Error 084: incorrect number of array indices
        "Incorrect number of array indices",
        //Error 085: not all parts of procedure return value
        "Not all parts of procedure return value",
        //Error 086: impossible to return outside of procedure
        "Impossible to return outside of procedure",
        //Error 087: procedure can't return value
        "Procedure can not return value",
        //Error 088: procedure must return value
        "Procedure must return value",
        //Error 089: incorrect object
        "Incorrect object",
        //Error 090: not field
        "Object not field",
        //Error 091: type is not type record
        "Type is not type record",
        //Error 092: incorrect field
        "Incorrect field",
        //Error 093: not label
        "Object not label",
        //Error 094: parameters lists not coincide
        "Parameters lists not coincide",
        //Error 095: length must be > 0
        "Length must be > 0",
        //Error 096: forward pointer is possible only for records and arrays
        "Forward pointer is possible only for records and arrays",
        //Error 097: wrong return type, only scalars allowed
        "Wrong return type, only scalars allowed",
        //Error 098: pointer to pointer not allowed
        "Pointer to pointer not allowed",
        //Error 099: formal parameter is parameter-variable, only variables acceptable as arguments
        "Formal parameter is parameter-variable, only variables acceptable as arguments",
        //Error 100: module not found
        "Module not found",
        //Error 101: incorrect number of parameters
        "Incorrect number of parameters",
        //Error 102: specialization already exist
        "Specialization already exist",
        //Error 103: can be only one default specialization
        "Can be only one default specialization",
        //Error 104: specialization id missed
        "Specialization id missed",
        //Error 105: unnecessary id of specialization
        "Unnecessary id of specialization",
        //Error 106: DFN module not found
        "DFN module not found",
        //Error 107: not generic
        "Object not generic",
        //Error 108: missed NIL or QualId
        "Missed NIL or QualId",
        //Error 109: extension restricted
        "Extension restricted",
        //Error 110: new type can't be based on generic
        "New type can not be based on generic",
        //Error 111: not specialization
        "Not specialization",
        //Error 112: variable type can't be pure generic
        "Variable type can not be pure generic",
        //Error 113: generic proc missed
        "Generic procedure missed",
        //Error 114: can't bind specialized proc to generic proc
        "Could not bind specialized proc to generic proc",
        //Error 115: generic param can be only pointer or var param
        "Generic parameter can be only pointer or var parameter",
        //Error 116: missed generic or specialization
        "Missed generic or specialization",
        //Error 117: only generic proc can be abstract
        "Only generic proc can be abstract",
        //Error 118: object is already defined
        "Object is already defined",
        //Error 119: recursive import is not allowed
        "Recursive import is not allowed",
        //Error 120: couldn't load attachment module
        "Could not load attachment module",
        //Error 121: generic or generic record expected
        "Generic or generic record expected",
        //Error 122: inadmissible type of case label
        "Inadmissible type of case label",
        //Error 123: case label value occured more than once
        "case label value occured more than once"
};

//==============================================================================
// класс для исключений
//==============================================================================
class Exception
{
public:
    Exception(string& _file, int _line, string& _error)
        :	file(_file),
                line(_line),
                error(_error)
        {
        }

    int line;			// строка
    string &file;		// файл
    string &error;		// описание ошибки
};

/**
  Убрал макросы, так как использовались только в одном месте.
  Заменил на прямой вызов исключения после проверки на создание потока
//------------------------------------------------------------------------------

#define EXCEPTION(errmsg)														\
{																				\
	throw Exception(__FILE__, __LINE__, errmsg);								\
}

#define CODE_EXCEPTION(code, errmsg)											\
{																				\
	throw Exception(__FILE__, __LINE__, string(#code) + "\n" + errmsg);			\
}

#define CHECK_FALSE(code, errmsg)												\
{																				\
	if(!(code))																	\
	{																			\
		CODE_EXCEPTION(code, errmsg);											\
	}																			\
}
*/

#endif
