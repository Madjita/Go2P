#include "tlt.h"

Tlt::Tlt()
{

}


Tlt::Tlt(string fileData):
    value_{'\0'},             // значение текущего символа
    previous_value_{'\0'}     // значение текущего символа
{
    streem_in_ = fileData;
    end_of_file = false;

    position_file_str_ = 0;         //Позиция относительно  массива файла
    position_row_str_ = 0;          // текущая строка
    position_col_char_str_ = 0;		//  столбец (позиция в строке по символьно)

    save_position_file_str_ = position_file_str_;           // запомненная позиция Позиция относительно  массива файла
    save_position_row_str_ = position_row_str_;             // запомненная позиция текущей строки
    save_position_col_char_str_ = position_col_char_str_;	// запомненная позиция столбца (позиция в строке по символьно)


    previous_position_file_str_ = position_file_str_;           //предыдущая запомненная позиция Позиция относительно  массива файла
    previous_position_row_str_ = position_row_str_;             //предыдущая запомненная позиция текущей строки
    previous_position_col_char_str_ = position_col_char_str_;	//предыдущая запомненная позиция столбца (позиция в строке по символьно)

    value_ = streem_in_[position_file_str_];
    previous_value_ = value_;

}

//Получить объект сохраненной позиции
InformPosition Tlt::getPosition() {
    InformPosition info;
    info.position_file_str = Get_position_file_str();
    info.position_row_str = Get_position_row_str();
    info.position_col_char_str = Get_position_col_char_str_();
    info.value = Get_value();

    if(info.value == 's')
    {
        cout << "SUM"<<endl;
    }
    return  info;
}

//Установить сохранненную позицию в буфер сохренной позиции
bool Tlt::setPosition(InformPosition oldInfoPosition) {
    save_position_file_str_ = oldInfoPosition.position_file_str;
    save_position_row_str_ = oldInfoPosition.position_row_str;
    save_position_col_char_str_ = oldInfoPosition.position_col_char_str;
    save_value_ = oldInfoPosition.value;

    setSavePosition();

    return  true;
}

//Установить сохранненную позицию
bool Tlt::setSavePosition()
{
    position_file_str_      = save_position_file_str_;
    position_row_str_       = save_position_row_str_;
    position_col_char_str_   = save_position_col_char_str_;
    value_ = save_value_;

    flagEnter = false;
    return true;
}

//--------------------------------------------------------------------------
// берет из файла очередной символ, фиксирует его значение
//--------------------------------------------------------------------------


void Tlt::Next() noexcept {

    DebugOut(cout);

    Save_previous_position_file_str();
    Save_previous_value();


    position_file_str_++;
    value_ = streem_in_[position_file_str_];

    Save_previous_position_col_char_str();

    if(flagEnter || previous_value_ == '\n')
    {
        Save_previous_position_row_str_();
        position_row_str_++;
        position_col_char_str_ = 0;
        flagEnter = false;
        return;
    }

    if(value_ == '\n') {
            flagEnter = true;
    }


    position_col_char_str_++;
}

//--------------------------------------------------------------------------
// возвращается назад на один символ
//--------------------------------------------------------------------------
bool Tlt::Prev()
{

    if(previous_position_file_str_ == 0 && end_of_file)
    {
         cout << "ERORO: previous_position_file_str_  == 0;"<<endl;
         exit(-1);
    }

    if(previous_position_file_str_ == 0)
    {
        end_of_file = true;
    }
    else
    {
        end_of_file = false;
    }

    if(value_ == '\n')
    {
        position_row_str_ = previous_position_row_str_;
    }
    position_col_char_str_ = previous_position_col_char_str_;
    value_ = streem_in_[previous_position_file_str_];

    position_file_str_ = previous_position_file_str_;

    return true;
}

bool Tlt::PrevRow()
{
    position_row_str_--;

    return true;
}


//--------------------------------------------------------------------------
// вывод отладочной информации о текущих параметрах транслитератора
//--------------------------------------------------------------------------
void Tlt::DebugOut(ostream &out)
{
    switch (value_)
    {
    case '\n':
        out << "["<<Get_position_file_str()<<"](" << Get_position_row_str() << ", " << Get_position_col_char_str_() << ") = '\\n'";
        break;
    case '\t':
        out << "["<<Get_position_file_str()<<"](" << Get_position_row_str() << ", " << Get_position_col_char_str_() << ") = '\\t'";
        break;
    case '\r':
        out << "["<<Get_position_file_str()<<"](" << Get_position_row_str() << ", " << Get_position_col_char_str_() << ") = '\\r'";
        break;
    default:
        out << "["<<Get_position_file_str()<<"](" << Get_position_row_str() << ", " << Get_position_col_char_str_() << ") = '" << Get_value()<<"'";
    }

    out << endl;
}

void Tlt::Save_position_file_str() noexcept {
    save_position_file_str_ = position_file_str_;
}

void Tlt::Save_position_row_str() noexcept {
    save_position_row_str_ = position_row_str_;
}

void Tlt::Save_position_col_char_str() noexcept {
    save_position_row_str_ = position_row_str_;
}

void Tlt::Save_value() noexcept {
    save_value_ = value_;
}

void Tlt::Save_previous_position_file_str() noexcept {
    previous_position_file_str_  = position_file_str_;
}

void Tlt::Save_previous_position_row_str_() noexcept {
    previous_position_row_str_ = position_row_str_;
}

void Tlt::Save_previous_position_col_char_str() noexcept {
    previous_position_col_char_str_ = position_col_char_str_;
}

void Tlt::Save_previous_value() noexcept {
    previous_value_ = value_;
}

//--------------------------------------------------------------------------
// проверка на конец файла
//--------------------------------------------------------------------------
bool Tlt::End_of_file() noexcept
{
    auto size_file = (streem_in_.size() <= position_file_str_);
    return size_file;
}
