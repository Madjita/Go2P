#include <iostream>



#include "alien/scanner.h"
#include "alien/CompileErrors.h"
#include "alien/parser.h"

#include "my/myvariable.h"
#include "my/code_generator/codegenerator.h"

using namespace std;
#include <fstream>
#include <vector>

bool ReadAllBytes(string filename,string& data)
{
    ifstream file(filename, ios::binary);

    if (file.fail()) {
        perror(filename.data());
        return false;
    }

    // copies all data into buffer
    vector<char> prov(
            (istreambuf_iterator<char>(file)),
            (istreambuf_iterator<char>()));

    if(prov[prov.size()-1] == '\n' && prov[prov.size()-2] == '\r')
        prov.erase(prov.end()-2,prov.end());


    data.resize(prov.size());
    copy(prov.begin(),prov.end(),data.begin());
    file.close();

    return true;
}

bool Start_program(Parser* pars)
{
    auto main = pars->table.find_name_func("main");

    return main;
}


int main(int argc, char *argv[])
{
    string data ="";
    string fileName ="";
    string fileNameOut ="";

    bool flagIn = false;
    bool flagOut = false;

    if(argc > 2)
    {
        for(int i=0; i < argc; i++)
        {
            if(string(argv[i]) == "-in")
            {
                flagIn = true;
                if(string(argv[i+1]).size() > 0)
                {
                    i++;
                    fileName = argv[i];
                }
                else
                {
                    cout << "Error!\n";
                    cout <<"You need write: -in <dir path> -o <dir path>\n";
                    cout <<"Example: -in go2p.txt -o go2p_result.txt\n";
                    return -1;
                }

            }

            if(string(argv[i]) == "-o")
            {
                flagIn = false;
                flagOut = true;

                if(string(argv[i+1]).size() > 0)
                {
                    i++;
                    fileNameOut = argv[i];
                }
                else
                {
                    cout << "Error!\n";
                    cout <<"You need write: -in <dir path> -o <dir path>\n";
                    cout <<"Example: -in go2p.txt -o go2p_result.txt\n";
                    return -1;
                }
            }

            if(flagIn && i != argc-1)
            {
                fileName += argv[i];
            }

            if(flagOut && i != argc-1)
            {
                fileNameOut += argv[i];
            }
        }
    }

    flagIn = false;
    flagOut = false;

    if(fileName == "")
    {
        cout << "Error!\n";
        cout <<"You need write: -in <dir path> -o <dir path>\n";
        cout <<"Example: -in go2p.txt -o go2p_result.txt\n";
        return -1;
    }

    if(fileNameOut == "")
    {
        fileNameOut = "go2p_result.txt";
    }


    ReadAllBytes(fileName,data);

    //-----Парсер alien------------
    CompileErrors err;
    Parser pars(data,&err);

    //1 деййствие : Собрать объект table в котором хрнаится вся информация о коде по массивам
    pars.Do();



    //2 действие : Начать выполнение программы с функции main
    Start_program(&pars);

    ofstream fileOutScaner_keyStr("scaner_out_keyStr.txt", ios::binary | ios::trunc);

    for(auto row: pars.get_vvMap_KeyData())
    {
        for(auto col: row)
        {
            for(auto item: col)
            {

                auto lex = pars.scanAliend->Out_str_lex(item.first);
                auto str = item.second;
                string it;

                if(str == "\n")
                {
                    str.resize(2);
                    str[0] = '\\';
                    str[1] = 'n';
                    it = "('"+lex+"','"+str+"')\n";
                }
                else
                {
                    it = "('"+lex+"','"+str+"')";
                }


                fileOutScaner_keyStr << it;
            }

        }
    }
    fileOutScaner_keyStr.close();


// //----Вывести разобранный код в файл----------
     ofstream fileOut(fileNameOut, ios::binary | ios::trunc);

     fileOut << pars.getWorkData();

     fileOut.close();


     //Код генератор
     CodeGenerator generator;
     generator.generate(pars.polish.get_vector_polish(),all);


    system("pause");
    return 0;
}



