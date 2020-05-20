#include "scanner.h"


Scanner::Scanner(string fileData):
    tlt_(new Tlt(fileData))
{
    debug_out_lexValue_ = "";

    // инициализация таблицы ключевых слов, используемых в языке

    table_keyLexValue_["local"]       = keyLocal;
    table_keyLexValue_["union"]       = keyUnion;
    table_keyLexValue_["break"]	    = keyBreak;
    table_keyLexValue_["case"]	    = keyCase;
    table_keyLexValue_["const"]	    = keyConst;
    table_keyLexValue_["continue"]    = keyContinue;
    table_keyLexValue_["default"]     = keyDefault;
    table_keyLexValue_["else"]	    = keyElse;
    table_keyLexValue_["fallthrough"] = keyFallthrough;
    table_keyLexValue_["for"]         = keyFor;
    table_keyLexValue_["func"]	    = keyFunc;
    table_keyLexValue_["goto"]	    = keyGoto;
    table_keyLexValue_["if"]          = keyIf;
    table_keyLexValue_["import"]	    = keyImport;
    table_keyLexValue_["map"]         = keyMap;
    table_keyLexValue_["return"]	    = keyReturn;
    table_keyLexValue_["select"]	    = keySelect;
    table_keyLexValue_["struct"]	    = keyStruct;
    table_keyLexValue_["switch"]	    = keySwitch;
    table_keyLexValue_["type"]	    = keyType;
    table_keyLexValue_["var"]         = keyVar;


    //тип данных
    //Целые числа
    table_keyLexValue_["int"]         = keyInt;
    table_keyLexValue_["int8"]        = keyInt8;
    table_keyLexValue_["int16"]       = keyInt16;
    table_keyLexValue_["int32"]       = keyInt32;
    table_keyLexValue_["int64"]       = keyInt64;

    //Целые без знаковые числа
    table_keyLexValue_["uint"]         = keyUInt;
    table_keyLexValue_["uint8"]        = keyUInt8;
    table_keyLexValue_["uint16"]       = keyUInt16;
    table_keyLexValue_["uint32"]       = keyUInt32;
    table_keyLexValue_["uint64"]       = keyUInt64;

    //Дробные числа
    table_keyLexValue_["float32"]      = keyFloat32;
    table_keyLexValue_["float64"]      = keyFloat64;

    table_keyLexValue_["string"]      = keyString;
    table_keyLexValue_["byte"]      = keyByte;
}

//------------------------------------------------------------------------------
// формирование класса и значения очередной лексемы
//------------------------------------------------------------------------------
void Scanner::Next()
{
    do
    {

        lex_value_ = "";			// значение лексемы еще не сформировано
        lex_class_ = lcNoValue;		// класс тоже еще не известен



        if(tlt_->End_of_file())
        {
            lex_class_ = lcEof;
            //return;
            break;
        }

        if(tlt_->IsSpace())
        {

            lex_class_ = lcSpace;
            lex_value_ += tlt_->Get_value();
            // сохраняем предыдущую позицию для вывода ошибки
            _prevStr = tlt_->Get_position_row_str();
            _prevCol = tlt_->Get_position_col_char_str_();
            tlt_->Next();

            if(lex_value_.size() > 0)
            {
                if(lex_value_[lex_value_.size()-1] == '\n')
                {
                    lex_class_ = lcEnter;
                }
            }

            debug_out_lexValue_ +=lex_value_;  //добавил сбор пропусков в отладачную переменную



        }
        else
            if(tlt_->IsLetter() || tlt_->Get_value() == '_')
            {
                IdOrKw();
                //LexClass tmpLc = lex_class_;
                if(tlt_->Get_value() == ':') {
                    // возможно метка
                    tlt_->Next();
                    if(tlt_->End_of_file())
                    {
                        // откат назад
                        tlt_->Prev();
                    }
                    if(tlt_->Get_value() == ':') // это метка
                    {
                        lex_class_ = lcLabel;
                        tlt_->Next();
                    }
                    else
                    {
                        // откат на один символ назад
                        tlt_->Prev();
                    }
                }
                /*
            // Имеет смысл сделать qualident отдельной лексемой
            // Иначе возможны пробелы при разборе в правилах парсера
            else if(tlt_->Get_value()=='.') {
        if(tlt_->IsLetter() || tlt_->Get_value() == '_') {
            IdOrKw();
            if((lex_class_ == lcId) && (tmpLc == lcId)) {
            lex_class_ = lcQualident;

            }
        }
            }
            */
            }
            else if(tlt_->IsDec())
                Number();
            else if(tlt_->IsIgnore()) { lex_class_ = lcIgnore; tlt_->Next(); }
            else if(tlt_->Get_value() == '(') { lex_class_ = lcLCircle; lex_value_ += "("; tlt_->Next(); }
            else if(tlt_->Get_value() == ')') { lex_class_ = lcRCircle; lex_value_ += ")"; tlt_->Next(); }
            else if(tlt_->Get_value() == '[') { lex_class_ = lcLSquare; lex_value_ += "["; tlt_->Next(); }
            else if(tlt_->Get_value() == ']') { lex_class_ = lcRSquare; lex_value_ += "]"; tlt_->Next(); }
            else if(tlt_->Get_value() == '{') { lex_class_ = lcLFigure; lex_value_ += "{"; tlt_->Next(); }
            else if(tlt_->Get_value() == '}') { lex_class_ = lcRFigure; lex_value_ += "}"; tlt_->Next(); }
            else if(tlt_->Get_value() == ',') { lex_class_ = lcComma; lex_value_ += ","; tlt_->Next(); }
            else if(tlt_->Get_value() == ';') { lex_class_ = lcSemicolon; lex_value_ += ";"; tlt_->Next(); }
            else if(tlt_->Get_value() == '&') { lex_class_ = lcImpersant; lex_value_ += "&"; tlt_->Next(); }
            else if(tlt_->Get_value() == '^') { lex_class_ = lcRoof; lex_value_ += "^"; tlt_->Next(); }
            else if(tlt_->Get_value() == '|') { lex_class_ = lcStick; lex_value_ += "|"; tlt_->Next(); }
            else if(tlt_->Get_value() == '~') { lex_class_ = lcWave; lex_value_ += "~"; tlt_->Next(); }
            else if(tlt_->Get_value() == '*') { lex_class_ = lcStar; lex_value_ += "*"; tlt_->Next(); }
            else if(tlt_->Get_value() == '+') { lex_class_ = lcPlus; lex_value_ += "+"; tlt_->Next(); }
            else if(tlt_->Get_value() == '-') { lex_class_ = lcMinus; lex_value_ += "-"; tlt_->Next(); }
            else if(tlt_->Get_value() == '%') {lex_class_ = lcMod; lex_value_ += "%"; tlt_->Next();  }
            else if(tlt_->Get_value() == '/')
                CommentOrDiv();
            else if(tlt_->Get_value() == '=') {

                //
                lex_class_ = lcEqual;
                lex_value_ += "=";
                tlt_->Next();

                // ==
                if(tlt_->Get_value() == '=')
                {
                    lex_class_ = lcEQ;
                    lex_value_ += "=";
                    tlt_->Next();
                }

            }
            else if(tlt_->Get_value() == '#') { lex_class_ = lcNotEqual; lex_value_ += "#"; tlt_->Next(); }
            else if(tlt_->Get_value() == ':')
            {
                // :
                lex_class_ = lcColon;
                lex_value_ += ":";

                tlt_->Next();
                if(tlt_->End_of_file())
                    //return;
                    break;

                // :=
                if(tlt_->Get_value() == '=')
                {
                    lex_class_ = lcAssign;
                    lex_value_ += "=";
                    tlt_->Next();
                }
            }
            else if(tlt_->Get_value() == '.')
            {
                // .
                lex_class_ = lcPoint;
                lex_value_ += ".";

                tlt_->Next();
                if(tlt_->End_of_file())
                    //return;
                    break;

                // ..
                if(tlt_->Get_value() == '.')
                {
                    lex_class_ = lc2Points;
                    lex_value_ += ".";
                    tlt_->Next();
                }
            }
            else if(tlt_->Get_value() == '<')
            {
                // <
                lex_class_ = lcSmaller;
                lex_value_ += "<";

                tlt_->Next();
                if(tlt_->End_of_file())
                    //return;
                    break;

                // <=
                if(tlt_->Get_value() == '=')
                {
                    lex_class_ = lcSmallerEQ;
                    lex_value_ += "=";
                    tlt_->Next();
                }
            }
            else if(tlt_->Get_value() == '>')
            {
                // >
                lex_class_ = lcLarger;
                lex_value_ += ">";

                tlt_->Next();
                if(tlt_->End_of_file())
                    //return;
                    break;

                // >=
                if(tlt_->Get_value() == '=')
                {
                    lex_class_ = lcLargerEQ;
                    lex_value_ += "=";
                    tlt_->Next();
                }
            }
            else if(tlt_->Get_value() == '\"')
            {
                String();
            }
            else if(tlt_->Get_value() == '\'')
                CharConst();
            else
            {
                CompileError(1);
                lex_class_ = lcError;
                tlt_->Next();
            }



    }
    while(lex_class_ == lcIgnore || lex_class_ == lcComment || lex_class_ == lcSpace);
    DebugOut();

//    if(lex_class_ != lcEnter && tlt_->Get_value() == '\n')
//    {
//        tlt_->PrevRow();
//    }
}

LexClass Scanner::GetLex() {
    return lex_class_;
}

void Scanner::setValue(string value) {
    lex_value_ = value;
}

void Scanner::DebugOut() {


    switch (lex_value_[0])
    {
    case '\n':
       cout << "Scanner find : ["<< Out_str_lex(lex_class_)<<"] = '\\n'"<<endl;
        break;
    case '\t':
       cout << "Scanner find : ["<< Out_str_lex(lex_class_)<<"] = '\\t'"<<endl;
        break;
    case '\r':
        cout << "Scanner find : ["<< Out_str_lex(lex_class_)<<"] = '\\r'"<<endl;
        break;
    default:
        cout << "Scanner find : ["<< Out_str_lex(lex_class_)<<"] = '"<< lex_value_ <<"'"<<endl;
    }

    cout << "----------------------------"<<endl;
}

string Scanner::Out_str_lex(LexClass lex)
{
    string data;

    switch(lex)
    {
    case lcLCircle:   data = "lcLCircle";   break;
    case lcRCircle:   data = "lcRCircle";   break;
    case lcLSquare:   data = "lcLSquare";   break;
    case lcRSquare:   data = "lcRSquare";   break;
    case lcLFigure:   data = "lcLFigure";   break;
    case lcRFigure:   data = "lcRFigure";   break;

    case lcPoint:     data = "lcPoint";     break;
    case lcComma:     data = "lcComma";     break;
    case lcSemicolon: data = "lcSemicolon"; break;
    case lcColon:     data = "lcColon";     break;

    case lc2Points:   data = "lc2Points";   break;
    case lcImpersant: data = "lcImpersant"; break;
    case lcRoof:      data = "lcRoof";      break;
    case lcStick:     data = "lcStick";     break;
    case lcWave:      data = "lcWave";      break;

    case lcStar:      data = "lcStar";      break;
    case lcSlash:     data = "lcSlash";     break;
    case lcPlus:      data = "lcPlus";      break;
    case lcMinus:     data = "lcMinus";     break;
    case lcMod:       data = "lcMod";       break;

    case lcAssign:    data = "lcAssign";    break;
    case lcEqual:     data = "lcEqual";     break;
    case lcEQ:        data = "lcEQ";        break;
    case lcNotEqual:  data = "lcNotEqual";  break;
    case lcSmaller:   data = "lcSmaller";   break;
    case lcLarger:    data = "lcLarger";    break;
    case lcSmallerEQ: data = "lcSmallerEQ"; break;
    case lcLargerEQ:  data = "lcLargerEQ";  break;

    case lcNoValue:   data = "lcNoValue";   break;

    case lcError:     data = "lcError";     break;

    case lcComment:   data = "lcComment";   break;
    case lcSpace:     data = "lcSpace";     break;
    case lcIgnore:    data = "lcIgnore";    break;

    case lcIntNum:
    {
        data = "lcIntNum";
        break;
    }
    case lcRealNum:
    {
        data = "lcRealNum";
        break;
    }

    case lcCharConst: data = "lcCharConst";	break;
    case lcString:    data = "lcString";    break;

    case lcId:        data = "lcId";        break;
    case lcLabel:     data = "lcLabel";     break;

    case lcEof:
    {
       // auto size = debugOutLex.find("lcEof");

      //  if(size > debugOutLex.size())
      //  {
            data = "lcEof";
      //  }

        break;
    }
    case lcEnter:
    {
        data = "lcEnter";
        break;
    }


    case keyBreak:       data = "keyBreak";       break;
    case keyCase:        data = "keyCase";        break;
    case keyConst:       data = "keyConst";       break;
    case keyContinue:    data = "keyContinue";    break;
    case keyDefault:     data = "keyDefault";     break;
    case keyElse:        data = "keyElse";        break;
    case keyFallthrough: data = "keyFallthrough"; break;
    case keyFor:         data = "keyFor";         break;
    case keyFunc:        data = "keyFunc";        break;
    case keyGoto:        data = "keyGoto";        break;
    case keyIf:          data = "keyIf";          break;
    case keyImport:      data = "keyImport";      break;
    case keyMap:         data = "keyMap";         break;
    case keyReturn:
    {
        data = "keyReturn";
        break;
    }
    case keySelect:      data = "keySelect";      break;
    case keyStruct:      data = "keyStruct";      break;
    case keySwitch:      data = "keySwitch";      break;

    case keyType:        data = "keyType";        break;
    case keyVar:         data = "keyVar";         break;

    case keyInt:         data = "keyInt";         break;
    case keyInt8:        data = "keyInt8";        break;
    case keyInt16:       data = "keyInt16";       break;
    case keyInt32:       data = "keyInt32";       break;
    case keyInt64:       data = "keyInt64";       break;

    case keyUInt:        data = "keyUInt";        break;
    case keyUInt8:       data = "keyUInt8";       break;
    case keyUInt16:      data = "keyUInt16";      break;
    case keyUInt32:      data = "keyUInt32";      break;
    case keyUInt64:      data = "keyUInt64";      break;

    case keyFloat32:     data = "keyFloat32";     break;
    case keyFloat64:     data = "keyFloat64";     break;
    case keyUnion:       data = "keyUnion";       break;
    case keyLocal:       data = "keyLocal";       break;

    case keyString:      data = "keyString";      break;
    case keyByte:        data = "keyByte";        break;

    default:
        data = "Unknown class!";
    }

    return data;
}

//------------------------------------------------------------------------------
// вывод сообщения об ошибке компиляции
//------------------------------------------------------------------------------
void Scanner::CompileError(unsigned int n)
{
    //_errors.Message(GetStr(), GetCol(), n);

    cout << "!!!!!!! ERROR ["<< n <<"] !!!!!"<<endl;
}



void Scanner::FillFailedLexem(unsigned int errorCode)
{
    // "проматываю" последующие буквы и цифры, чтобы избежать появления лишних лексем
    do {
        lex_value_ += tlt_->Get_value();
        tlt_->Next();
    } while((tlt_->IsDec() || tlt_->IsLetter()) && !tlt_->End_of_file());

    CompileError(errorCode);
    lex_class_ = lcError;
}

//------------------------------------------------------------------------------
// распознавание идентификатора или ключевого слова
// (буква | "_") {буква | цифра | "_"}
//------------------------------------------------------------------------------
void Scanner::IdOrKw()
{
    // собираю слово по буквам
    do
    {
        lex_value_ += tlt_->Get_value();
        tlt_->Next();
        if(tlt_->End_of_file())
            break;
    }
    while(tlt_->IsLetter() || tlt_->IsDec() || tlt_->Get_value() == '_');

    std::map< std::string, LexClass, std::less<std::string> >::iterator it = table_keyLexValue_.find(lex_value_);
    if(it != table_keyLexValue_.end())
        lex_class_ = (*it).second;
    else
        lex_class_ = lcId;
}

//------------------------------------------------------------------------------
// распознаание комментария или знака деления
// "/*" {знак} "*/"
// или
// "//" {знак} конец_строки
//------------------------------------------------------------------------------
void Scanner::CommentOrDiv()
{
    tlt_->Next();
    if(tlt_->End_of_file())
    {
        lex_class_ = lcSlash;
        lex_value_ += "/";
        return;
    }

    if(tlt_->Get_value() == '/')			// однострочный комментарий
    {
        tlt_->Next();
        while(tlt_->Get_value() != '\n' && !tlt_->End_of_file())
        {
            lex_value_ += tlt_->Get_value();
            tlt_->Next();
        }
        lex_class_ = lcComment;
    }
    else
        if(tlt_->Get_value() == '*')		// многострочный комментарий
        {
            tlt_->Next();
            while(1)
            {
                if(tlt_->End_of_file())
                {
                    // неожиданный конец файла, комментарий не закрыт
                    CompileError(8);
                    lex_class_ = lcError;
                    return;
                }

                if(tlt_->Get_value() == '/')
                    CommentOrDiv();
                else if(tlt_->Get_value() == '\"')
                    String();
                else if(tlt_->Get_value() == '*')
                {
                    tlt_->Next();
                    if(tlt_->Get_value() == '/')
                    {
                        lex_class_ = lcComment;
                        tlt_->Next();
                        return;
                    }
                    lex_value_ += '*';
                }
                else
                {
                    lex_value_ += tlt_->Get_value();
                    tlt_->Next();
                }
            }
        }
        else
        {
            lex_class_ = lcSlash;
            lex_value_ += "/";
        }
}

//------------------------------------------------------------------------------
// общая проверка чисел
//------------------------------------------------------------------------------
void Scanner::Number()
{
    // возможно шестнадцатеричное число
    if (tlt_->Get_value() == '0') {
        lex_value_ += tlt_->Get_value();

        tlt_->Next();

        if(tlt_->Get_value() == 'X')			// шестнадцатеричное число
        {
            HexNumber();
        }
        else if (tlt_->Get_value() == '.')	// дробное число начинается с 0 или это ".."
        {
            // Попытка проверить следующую точку
            tlt_->Next();   // Для этого берется еще один символ
            if(tlt_->Get_value() == '.') {
                // Две точки после нуля
                // Откат назад с порождением целого числа
                lex_class_ = lcIntNum;
                uInt_ = DecToInt(lex_value_.c_str());
                //ForbiddenSymbol();
                // Откат на начало предыдущей лексемы (..)
                tlt_->Prev();
                //tlt_->Prev();
                return;
            }
            tlt_->Prev();   // Откат назад, чтобы продолжить разбор действительного
            RealContinue();
        }
        else
        {
            // это число "0"
            lex_class_ = lcIntNum;
            uInt_ = DecToInt(lex_value_.c_str());
            ForbiddenSymbol();
        }
    }
    else
    {
        DecNumber();
    }
}

//------------------------------------------------------------------------------
// проверка на десятичное число
// цифра {цифра}
//------------------------------------------------------------------------------
void Scanner::DecNumber()
{
    // собираю число
    do
    {
        lex_value_ += tlt_->Get_value();
        tlt_->Next();
        if(tlt_->End_of_file())
        {
            lex_class_ = lcIntNum;
            uInt_ = DecToInt(lex_value_.c_str());
            ForbiddenSymbol();
            return;
        }
    }
    while(tlt_->IsDec());




    if(tlt_->Get_value() == '.')	{	// действительное число
        tlt_->Next();
        if(tlt_->Get_value() == '.') {
            // Две точки после нуля
            // Откат назад с порождением целого числа
            lex_class_ = lcIntNum;
            uInt_ = DecToInt(lex_value_.c_str());
            //ForbiddenSymbol();
            // Откат на начало предыдущей лексемы (..)
            tlt_->Prev();
            //tlt_->Prev();
            return;
        }
        tlt_->Prev();   // Откат назад, чтобы продолжить разбор действительного
        RealContinue();
    }
    else
    {
        lex_class_ = lcIntNum;
        uInt_ = DecToInt(lex_value_.c_str());
        ForbiddenSymbol();

        //кастыль
//        if(tlt_->Get_value() == '\n')
//        {
//            tlt_->Prev();
//        }
    }
}

//------------------------------------------------------------------------------
// проверка на шестнадцатиричное число
// "0""X"{/шестнЦифра/}
//------------------------------------------------------------------------------
void Scanner::HexNumber()
{
    if (tlt_->End_of_file())
    {
        // ошибка: шестнадцатеричное число не завершено
        FillFailedLexem(static_cast<unsigned int>(12));
    }

    do
    {
        lex_value_ += tlt_->Get_value();
        tlt_->Next();
        if(tlt_->End_of_file())
        {
            lex_class_ = lcIntNum;
            uInt_ = HexToInt(lex_value_.c_str());
            tlt_->Next();
            ForbiddenSymbol();
            return;
        }
    }
    while(tlt_->IsHex());

    lex_class_ = lcIntNum;
    uInt_ = HexToInt(lex_value_.c_str());
    ForbiddenSymbol();
}

//------------------------------------------------------------------------------
// проверка на запрещенный символ, который может следовать
// после числа (цифра или буква)
//------------------------------------------------------------------------------
void Scanner::ForbiddenSymbol()
{
    if((tlt_->IsDec() || tlt_->IsLetter()) && !tlt_->End_of_file())
    {
        FillFailedLexem(static_cast<unsigned int>(11));
    }



}

//------------------------------------------------------------------------------
// перед точкой были цифры
// "." {цифра} [Порядок]
//------------------------------------------------------------------------------
void Scanner::RealContinue()
{
    lex_value_ += '.';
    tlt_->Next();
    if(tlt_->End_of_file())
    {
        lex_class_ = lcRealNum;
        _dNum = atof(lex_value_.c_str());
        return;
    }
    while(1)
    {
        if(tlt_->IsDec())
        {
            lex_value_ += tlt_->Get_value();
            tlt_->Next();
            if(tlt_->End_of_file())
            {
                lex_class_ = lcRealNum;
                _dNum = atof(lex_value_.c_str());
                return;
            }
        }
        else
            if(tlt_->Get_value() == 'E')
            {
                Exp();
                return;
            }
            else
                if(tlt_->IsLetter())
                {
                    FillFailedLexem(static_cast<unsigned int>(15));
                    return;
                }
                else
                {
                    lex_class_ = lcRealNum;
                    _dNum = atof(lex_value_.c_str());
                    return;
                }
    }

    lex_class_ = lcRealNum;
    _dNum = atof(lex_value_.c_str());
}

//------------------------------------------------------------------------------
// проверка на порядок числа
// "E" ["+" | "-"] цифра {цифра}
//------------------------------------------------------------------------------
void Scanner::Exp()
{
    lex_value_ += tlt_->Get_value();
    tlt_->Next();
    if(tlt_->End_of_file())
    {
        CompileError(16);
        lex_class_ = lcError;
        return;
    }

    if(tlt_->Get_value() == '+' || tlt_->Get_value() == '-')
    {
        lex_value_ += tlt_->Get_value();
        tlt_->Next();
        if(tlt_->End_of_file())
        {
            CompileError(17);
            lex_class_ = lcError;
            return;
        }
    }
    else
        if(tlt_->IsDec())
        {
            lex_value_ += tlt_->Get_value();
            tlt_->Next();
            if(tlt_->End_of_file())
            {
                lex_class_ = lcRealNum;
                _dNum = atof(lex_value_.c_str());
                return;
            }
        }
        else
        {
            // ошибка: после символа степени должны идти '+', '-' или число
            FillFailedLexem(static_cast<unsigned int>(18));
            return;
        }

    while(tlt_->IsDec())
    {
        lex_value_ += tlt_->Get_value();
        tlt_->Next();
        if(tlt_->End_of_file())
        {
            lex_class_ = lcRealNum;
            _dNum = atof(lex_value_.c_str());
            return;
        }
    }

    if(tlt_->IsLetter())
    {
        FillFailedLexem(static_cast<unsigned int>(19));
        return;
    }

    lex_class_ = lcRealNum;
    _dNum = atof(lex_value_.c_str());
    tlt_->Next();
}

//------------------------------------------------------------------------------
// служебная функция, возвращает escape symbol соответствующий букве
//------------------------------------------------------------------------------
char Scanner::EscapeSymbol(char ch)
{
    switch(ch)
    {
    case 'n': return '\n';
    case 't': return '\t';
    case 'v': return '\v';
    case 'b': return '\b';
    case 'r': return '\r';
    case 'f': return '\f';
    case 'a': return '\a';
    case '\\': return '\\';
    case '\'': return '\'';
    case '\"': return '\"';
    case '0': return '\0';
    default:
        // warning: неизвестный escape symbol
        return ch;
    }
}

//------------------------------------------------------------------------------
// строка символов
// {/ “ " ” {символ} “ " “ /}
//------------------------------------------------------------------------------
void Scanner::String()
{
    while(1)
    {
        tlt_->Next();
        if(tlt_->End_of_file())
        {
            // error: строка не закрыта
            CompileError(2);
            lex_class_ = lcError;
            return;
        }
        if(tlt_->Get_value() == '\"')
        {
            do
            {
                tlt_->Next();
            }
            while((tlt_->IsSpace() || tlt_->IsIgnore()) && !tlt_->End_of_file());

            if(tlt_->End_of_file() || tlt_->Get_value() != '\"')
            {
                lex_class_ = lcString;
                return;
            }
        }
        else if(tlt_->Get_value() == '\\')
        {
            tlt_->Next();
            if(tlt_->End_of_file())
            {
                // error: неожиданный конец файла, ожидался символ
                CompileError(3);
                lex_class_ = lcError;
                return;
            }
            lex_value_ += EscapeSymbol(tlt_->Get_value());
        }
        else
        {
            lex_value_ += tlt_->Get_value();
        }
    }
}

//------------------------------------------------------------------------------
// символьная константа
//------------------------------------------------------------------------------
void Scanner::CharConst()
{
    tlt_->Next();
    if(tlt_->End_of_file())
    {
        // error: неожиданный конец файла, ожидался символ
        CompileError(4);
        lex_class_ = lcError;
        return;
    }
    if(tlt_->Get_value() == '\\')
    {
        tlt_->Next();
        if(tlt_->End_of_file())
        {
            // error: неожиданный конец файла, ожидался символ
            CompileError(5);
            lex_class_ = lcError;
            return;
        }
        lex_value_ += EscapeSymbol(tlt_->Get_value());
    }
    else
    {
        lex_value_ += tlt_->Get_value();
    }

    tlt_->Next();
    if(tlt_->End_of_file())
    {
        // error: неожиданный конец файла, ожидалась одинарная кавычка (')
        CompileError(6);
        lex_class_ = lcError;
        return;
    }
    if(tlt_->Get_value() != '\'')
    {
        // error: отсутствует закрывающая одинарная кавычка (')
        CompileError(7);
        lex_class_ = lcError;
        return;
    }
    lex_class_ = lcCharConst;
    tlt_->Next();
}

//------------------------------------------------------------------------------
// преобразование строки десятичных цифр в целое число
//------------------------------------------------------------------------------
int Scanner::DecToInt(const char* Str)
{
    unsigned int num = 0;

    while(*Str != '\0')
    {
        unsigned int digit = static_cast<unsigned int>(*Str);
        if(num > decMax)
        {
            // возникнет переполнение при добавлении очередной цифры
            CompileError(9);
            lex_class_ = lcError;
            return 0;
        }
        num = (num * 10) + (digit - static_cast<unsigned int>('0'));
        ++Str;
    }

    return num;
}

//------------------------------------------------------------------------------
// преобразование строки шестнадцатиричных цифр в целое число
//------------------------------------------------------------------------------
int Scanner::HexToInt(const char* Str)
{
    unsigned int num = 0;

    while(*Str != '\0')
    {
        if (*Str == 'X')
        {
            ++Str;
            continue;
        }

        if(hexMax < num)
        {
            // возникнет переполнение при добавлении очередной цифры
            CompileError(10);
            lex_class_ = lcError;
            return 0;
        }

        unsigned int digit = static_cast<unsigned int>(*Str);

        num = num << 4;
        if(*Str >= '0' && *Str <= '9')
            num += (digit - static_cast<unsigned int>('0'));
        else if(*Str >= 'A' && *Str <= 'F')
            num += (digit - static_cast<unsigned int>('A')) + 10;
        else
            num += (digit - static_cast<unsigned int>('a')) + 10;

        ++Str;
    }

    return num;
}
