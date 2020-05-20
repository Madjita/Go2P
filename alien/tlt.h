#ifndef TLT_H
#define TLT_H

#include <iostream>

using namespace std;

// объект хранящий позицию
struct InformPosition{
    int position_row_str;
    int position_col_char_str;
    int position_file_str;
    char value;
};

//------------------------------------------------------------------------------
// транслитератор
//------------------------------------------------------------------------------
class Tlt
{
public:
    Tlt();
    Tlt(string fileData);

public:
    // текущая позиция относительно всего файла
    int Get_position_file_str() { return position_file_str_; }
    // текущая строка
    int Get_position_row_str() { return position_row_str_; }
    // текущий столбец
    int Get_position_col_char_str_() { return position_col_char_str_; }
    // текущий символ
    char Get_value() { return value_; }


    //Получить объект сохраненной позиции
    InformPosition getPosition() {
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


    bool flagEnter = false;

    //Установить сохранненную позицию в буфер сохренной позиции
    bool setPosition(InformPosition oldInfoPosition) {
        save_position_file_str_ = oldInfoPosition.position_file_str;
        save_position_row_str_ = oldInfoPosition.position_row_str;
        save_position_col_char_str_ = oldInfoPosition.position_col_char_str;
        save_value_ = oldInfoPosition.value;

        setSavePosition();

        return  true;
    }

    //Установить сохранненную позицию
    bool setSavePosition()
    {
        position_file_str_      = save_position_file_str_;
        position_row_str_       = save_position_row_str_;
        position_col_char_str_   = save_position_col_char_str_;
        value_ = save_value_;

        flagEnter = false;
        return true;
    }

    //--------------------------------------------------------------------------
    // проверка на конец файла
    //--------------------------------------------------------------------------
    bool End_of_file() noexcept;
    //--------------------------------------------------------------------------
    // берет из файла очередной символ, фиксирует его значение
    //--------------------------------------------------------------------------
    void Next() noexcept;

    //--------------------------------------------------------------------------
    // возвращается назад на один символ
    //--------------------------------------------------------------------------
    bool Prev();

    bool PrevRow();

    //--------------------------------------------------------------------------
    // вывод отладочной информации о текущих параметрах транслитератора
    //--------------------------------------------------------------------------
    void DebugOut(std::ostream &out);

    //--------------------------------------------------------------------------
    // функции, обеспечивающие проверку классов литер

    // буква
    bool IsLetter() { return (value_ >= 'A' && value_ <= 'Z') || (value_ >= 'a' && value_ <= 'z'); }
    // пропуск
    bool IsSpace()
    {
        bool ok = (value_ == ' ' || value_ == '\t'  || value_ == '\n' || value_ == '\012' || value_ == '\015');
        return ok;
    }
    // игнорируемый символ
    bool IsIgnore()
    {
        return (value_ > 0 && value_ < ' ' &&
                value_ != '\t' && value_ != '\n' && value_ != '\f' &&
                value_ != '\v' && value_ != '\b' && value_ != '\r' &&
                value_ != '\\' && value_ != '\0' && value_ != '\'' &&
                (value_ != '\"' || value_ != '"'));
    }
    // двоичное
    //bool IsBin() { return (value_ == '0' || value_ == '1'); }
    // восьмеричное
    //bool IsOct() { return (value_ >= '0' && value_ <= '7'); }
    // десятичное
    bool IsDec() { return (value_ >= '0' && value_ <= '9'); }
    // шестнадцатеричное
    bool IsHex() { return (value_ >= '0' && value_ <= '9') || (value_ >= 'A' && value_ <= 'F'); }

private:

    void Save_position_file_str() noexcept;
    void Save_position_row_str() noexcept;
    void Save_position_col_char_str() noexcept;
    void Save_value() noexcept;

    void Save_previous_position_file_str() noexcept;
    void Save_previous_position_row_str_() noexcept;
    void Save_previous_position_col_char_str() noexcept;
    void Save_previous_value() noexcept;


private:
    string streem_in_;		// строка данных Файла из которого читаются символы

    unsigned int position_file_str_;	//Позиция относительно  массива файла
    unsigned int position_row_str_;		// текущая строка
    unsigned int position_col_char_str_;		//  столбец (позиция в строке по символьно)

    unsigned int save_position_file_str_;	// запомненная позиция Позиция относительно  массива файла
    unsigned int save_position_row_str_;      // запомненная позиция текущей строки
    unsigned int save_position_col_char_str_;	// запомненная позиция столбца (позиция в строке по символьно)


    unsigned int previous_position_file_str_;	//предыдущая запомненная позиция Позиция относительно  массива файла
    unsigned int previous_position_row_str_;      //предыдущая запомненная позиция текущей строки
    unsigned int previous_position_col_char_str_;	//предыдущая запомненная позиция столбца (позиция в строке по символьно)


    char value_;            // значение текущего символа
    char save_value_;       // значение сохранненного символа
    char previous_value_;	// значение предыдущего символа до сохранения

    bool end_of_file;
};

#endif // TLT_H
